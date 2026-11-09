/**
 * @file umdm_driver.h
 * @brief Universal Microcurrent Driver Module (UMDM) Firmware Interface & Register Definitions
 * @project MICROCURRENT (Universal Microcurrent Chip)
 */

#ifndef UMDM_DRIVER_H
#define UMDM_DRIVER_H

#include <stdint.h>
#include <stdbool.h>

/* ==========================================================================
 * 1. I2C SLAVE ADDRESS & REGISTER MAP
 * ========================================================================== */
#define UMDM_I2C_ADDR               0x38
#define UMDM_DEVICE_ID              0x55

#define UMDM_REG_WHO_AM_I           0x00
#define UMDM_REG_CTRL               0x01
#define UMDM_REG_CURRENT_SET_L      0x02
#define UMDM_REG_CURRENT_SET_H      0x03
#define UMDM_REG_FREQ_SET           0x04
#define UMDM_REG_DUTY_CYCLE         0x05
#define UMDM_REG_STATUS             0x06
#define UMDM_REG_IMPEDANCE_L        0x07
#define UMDM_REG_IMPEDANCE_H        0x08

/* ==========================================================================
 * 2. SAFETY & BIOMEDICAL LIMITS (IEC 60601-1 Compliance)
 * ========================================================================== */
#define UMDM_MIN_CURRENT_UA         10U     /* Minimum biological current (10 uA) */
#define UMDM_MAX_CURRENT_UA         500U    /* Maximum therapeutic MENS current (500 uA) */
#define UMDM_HARDWARE_CLAMP_UA      950U    /* Absolute ceiling < 1 mA */

#define UMDM_MIN_FREQ_HZ            1U
#define UMDM_MAX_FREQ_HZ            100U

#define UMDM_SESSION_ACTIVE_MS      (20UL * 60UL * 1000UL) /* 20 min Pomodoro active */
#define UMDM_SESSION_REST_MS        (40UL * 60UL * 1000UL) /* 40 min receptor rest */

/* ==========================================================================
 * 3. PERIPHERAL PRESET PROFILES
 * ========================================================================== */
typedef enum {
    UMDM_PROFILE_MOUSE = 0,         /* Thick stratum corneum (palm): 250 uA, 20 Hz, 20V boost */
    UMDM_PROFILE_SMARTWATCH = 1,    /* Thin wrist skin: 80 uA, 10 Hz, 9V boost */
    UMDM_PROFILE_CARPAL_PAD = 2     /* Deep trans-carpal path: 300 uA, 15 Hz, 24V boost */
} umdm_profile_t;

typedef struct {
    uint16_t target_current_ua;     /* Target current in microamps (10 - 500 uA) */
    uint16_t actual_current_ua;     /* Instantaneous ramped current (for Soft-Start) */
    uint8_t  frequency_hz;          /* Biphasic switching frequency (1 - 100 Hz) */
    uint8_t  duty_cycle_pct;        /* Symmetry duty cycle (default 50% for zero net DC) */
    uint16_t skin_impedance_ohm_x10;/* Estimated skin impedance (in 10-ohm units) */
    bool     enabled;
    bool     skin_contact_detected;
    bool     overcurrent_fault;
} umdm_state_t;

void umdm_init(umdm_state_t *state, umdm_profile_t profile);
void umdm_update_control_loop(umdm_state_t *state, uint16_t adc_load_mv, uint32_t dt_ms);
void umdm_biphasic_timer_isr(const umdm_state_t *state);

#endif /* UMDM_DRIVER_H */
