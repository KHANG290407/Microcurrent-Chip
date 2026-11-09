/**
 * @file umdm_control_loop.c
 * @brief Closed-Loop Current Regulation, Skin Impedance Sensing, Soft-Start & Safety Failsafe
 * @project MICROCURRENT (Universal Microcurrent Chip)
 */

#include "umdm_driver.h"

/* Hardware Abstraction Layer (HAL) Hooks - Implemented per target MCU (STM32 / ESP32 / CH32V) */
extern void hal_dac_set_current_ua(uint16_t current_ua);
extern void hal_hbridge_set_polarity(bool phase_positive);
extern void hal_hbridge_disable_all(void);
extern void hal_boost_set_voltage_mv(uint16_t boost_mv);

static bool s_current_phase_positive = true;

void umdm_init(umdm_state_t *state, umdm_profile_t profile) {
    state->actual_current_ua = 0;
    state->duty_cycle_pct = 50; /* Strict 50% biphasic symmetry for Zero Net DC */
    state->enabled = false;
    state->skin_contact_detected = false;
    state->overcurrent_fault = false;

    switch (profile) {
        case UMDM_PROFILE_MOUSE:
            state->target_current_ua = 250;
            state->frequency_hz = 20;
            hal_boost_set_voltage_mv(20000); /* 20V compliance for palm stratum corneum */
            break;

        case UMDM_PROFILE_SMARTWATCH:
            state->target_current_ua = 80;
            state->frequency_hz = 10;
            hal_boost_set_voltage_mv(9000);  /* 9V compliance for thin wrist skin */
            break;

        case UMDM_PROFILE_CARPAL_PAD:
        default:
            state->target_current_ua = 300;
            state->frequency_hz = 15;
            hal_boost_set_voltage_mv(24000);
            break;
    }

    hal_dac_set_current_ua(0);
    hal_hbridge_disable_all();
}

/**
 * @brief Periodic control loop (called every 10 ms)
 * - Measures skin impedance R_skin = V_load / I_actual
 * - Detects hand contact / open-circuit condition
 * - Executes smooth Soft-Start ramp-up to prevent micro-shock
 * - Enforces IEC 60601-1 < 1 mA safety shutdown
 */
void umdm_update_control_loop(umdm_state_t *state, uint16_t adc_load_mv, uint32_t dt_ms) {
    (void)dt_ms;

    if (!state->enabled || state->overcurrent_fault) {
        state->actual_current_ua = 0;
        hal_dac_set_current_ua(0);
        hal_hbridge_disable_all();
        return;
    }

    /* 1. Clamp target current to safe therapeutic bounds */
    if (state->target_current_ua > UMDM_MAX_CURRENT_UA) {
        state->target_current_ua = UMDM_MAX_CURRENT_UA;
    }

    /* 2. Contact Detection: If load voltage saturates near compliance rail (> 18.5V), electrodes are open */
    if (adc_load_mv > 18500U) {
        state->skin_contact_detected = false;
        state->actual_current_ua = 0;
        hal_dac_set_current_ua(0);
        return;
    } else {
        state->skin_contact_detected = true;
    }

    /* 3. Soft-Start Ramp: Increment by 5 uA per 10ms step (reaches 250 uA in 500 ms) */
    if (state->actual_current_ua < state->target_current_ua) {
        uint16_t next_ua = state->actual_current_ua + 5U;
        state->actual_current_ua = (next_ua > state->target_current_ua) ? state->target_current_ua : next_ua;
    } else if (state->actual_current_ua > state->target_current_ua) {
        state->actual_current_ua = state->target_current_ua;
    }

    /* 4. Calculate real-time skin impedance: R (ohm) = (V_mv * 1000) / I_ua */
    if (state->actual_current_ua >= UMDM_MIN_CURRENT_UA) {
        uint32_t r_ohm = ((uint32_t)adc_load_mv * 1000UL) / state->actual_current_ua;
        state->skin_impedance_ohm_x10 = (uint16_t)(r_ohm / 10UL);
    }

    /* 5. Apply regulated current command to DAC / VCCS stage */
    hal_dac_set_current_ua(state->actual_current_ua);
}

/**
 * @brief Hardware Timer ISR for Biphasic Polarity Alternation (Zero Net DC)
 */
void umdm_biphasic_timer_isr(const umdm_state_t *state) {
    if (!state->enabled || !state->skin_contact_detected || state->overcurrent_fault) {
        hal_hbridge_disable_all();
        return;
    }

    s_current_phase_positive = !s_current_phase_positive;
    hal_hbridge_set_polarity(s_current_phase_positive);
}
