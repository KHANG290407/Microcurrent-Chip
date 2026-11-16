# Chắt Lọc Kiến Thức Trọng Tâm Từ Cuộc Trò Chuyện & Phương Án Tích Hợp STM32
> **Tham chiếu nguồn:** Cuộc trao đổi kỹ thuật về nguồn dòng, trở kháng da tay và vi điều khiển STM32.

---

## PHẦN 1: CHẮT LỌC CÁC Ý QUAN TRỌNG VỀ TẠO NGUỒN DÒNG & ĐIỆN SINH LÝ

### 1. Bản Chất Mạch Nguồn Dòng Không Đổi (VCCS) & Điện Áp Tuân Thủ (Compliance Voltage)
- **Vấn đề cốt lõi của da người:** Trở kháng da lòng bàn tay dao động cực kỳ lớn:
  - Da khô / lạnh: $50\,\text{k}\Omega - 100\,\text{k}\Omega+$ (do lớp sừng dày $400 - 600\,\mu\text{m}$, vân tay làm giảm diện tích tiếp xúc thực tế còn $20 - 30\%$).
  - Da ẩm / bình thường: $20\,\text{k}\Omega - 50\,\text{k}\Omega$.
  - Da ra mồ hôi: $5\,\text{k}\Omega - 15\,\text{k}\Omega$.
- **Định luật Ohm cho nguồn dòng vi mô:** Để đẩy dòng $I = 200\,\mu\text{A}$ qua tải da khô $100\,\text{k}\Omega$:
  $$V_{\text{tải}} = 200\,\mu\text{A} \times 100\,\text{k}\Omega = 20\,\text{V}$$
- **Quy tắc thiết kế phần cứng sống còn:**
  - Nguồn cấp cao áp $V_{\text{BOOST}}$ phải đạt tối thiểu **$20\,\text{V} - 24\,\text{V}$**.
  - Sụt áp trên điện trở đo dòng $R_{\text{sense}}$ phải **càng nhỏ càng tốt** ($\Delta V \approx 0.2\,\text{V} - 1.0\,\text{V}$) để dành trọn vẹn dải điện áp $20\,\text{V}$ cho tải $R_{\text{skin}}$. Nếu sụt áp trên $R_{\text{sense}}$ lên tới $10\,\text{V}$, mạch sẽ bị bão hòa ngay khi da hơi khô!

---

### 2. So Sánh Mạch Howland Current Pump vs. High-Side VCCS (Op-Amp + P-MOSFET)

| Tiêu chí | Mạch Howland Current Pump (Option 1) | Mạch High-Side VCCS (Option 2) |
| :--- | :--- | :--- |
| **Cấu trúc** | Cầu 4 điện trở vi sai hồi tiếp âm & dương qua 1 Op-Amp | 1 Op-Amp + 1 P-MOSFET + **1 điện trở $R_{\text{sense}}$ duy nhất** |
| **Điều kiện cân bằng** | Bắt buộc $\frac{R_2}{R_1} = \frac{R_4}{R_3}$ với sai số $< 0.1\%$ | **Không cần cân bằng cầu trở**. Ổn định tuyệt đối |
| **Rủi ro tự kích** | Dễ bị tự kích dao động nếu sai số điện trở $> 0.1\%$ hoặc tải dung tính | Không có nguy cơ tự kích vì chỉ dùng hồi tiếp âm |
| **Giao tiếp STM32 (3.3V)** | Khó dịch mức tín hiệu vì Op-Amp phải treo ở dải áp cao hoặc nguồn đối xứng $\pm 15\,\text{V}$ | **Cực kỳ dễ ghép nối** qua tầng dịch mức dòng (Current Mirror) |
| **Đánh giá lựa chọn** | Phù hợp báo cáo học thuật, dùng mạng điện trở LT5400 | **Khuyên dùng thực tế cho mạch tích hợp cùng STM32** |

---

### 3. Vấn Đề An Toàn Y Sinh & Chống Sốc Hở Mạch
1. **Chống phóng điện khi chạm tay lại (Anti-Shock):**
   - Khi nhấc tay khỏi chuột ($R_{\text{skin}} = \infty$), Op-Amp bị bão hòa đẩy ngõ ra lên kịch $20\,\text{V}$.
   - Nếu chạm tay trở lại đột ngột, điện tích trên tụ ký sinh sẽ phóng thẳng vào da gây cảm giác giật nhói.
   - **Giải pháp phần cứng:** Mắc điện trở xả $1\,\text{M}\Omega - 2\,\text{M}\Omega$ song song 2 bản cực và trở hạn dòng $1\,\text{k}\Omega - 2\,\text{k}\Omega$ nối tiếp đường ra.
2. **Cảm biến mồ hôi tự thích ứng (GSR):**
   - Không cần cảm biến độ ẩm rời. Đo trực tiếp $V_{\text{skin}}$ bằng ADC của STM32 để tính $R_{\text{skin}} = \frac{V_{\text{skin}}}{I_{\text{out}}}$.
   - Khi mồ hôi ra nhiều ($R_{\text{skin}} < 20\,\text{k}\Omega$), mật độ dòng tại tuyến mồ hôi tăng cao dễ gây châm chích $\implies$ STM32 tự động giảm dòng mục tiêu từ $200\,\mu\text{A}$ xuống $60\,\mu\text{A} - 80\,\mu\text{A}$.
3. **Cách ly nguồn an toàn (Galvanic Isolation):**
   - Nguồn USB $5\,\text{V}$ máy tính $\to$ Module DC-DC cách ly (B0505S-1WR3) $\to$ Tầng Boost $20\,\text{V\_ISO}$.
   - Tín hiệu từ STM32 sang tầng cao áp qua Optocoupler để chống rò điện vào máy tính.

---

## PHẦN 2: THIẾT KẾ MẠCH TÍCH HỢP STM32 VỚI MẠCH NGUỒN DÒNG

### 1. Bài Toán Giao Tiếp Mức Điện Áp: STM32 (3.3V) $\to$ Nguồn Dòng (20V)
Chân STM32 chỉ xuất được tối đa $3.3\,\text{V}$, trong khi ngõ vào điều khiển $V_{\text{CTRL}}$ của mạch High-Side nằm sát mức $20\,\text{V}$ ($19.8\,\text{V} - 19.0\,\text{V}$).

**Giải pháp: Tầng dịch mức gương dòng (Active Level Shifter):**
```
      Nguồn Boost V_BOOST (+20V)
           │                  │
       [ R_pull (10k) ]   [ R_sense (10k) ]
           │                  │
           ├── V_CTRL ────────┤ Chân (+) Op-Amp U1
           │                  │
        Collector             │ Chân (-) Op-Amp U1 bám theo V_CTRL
      ┌────┴────┐             │
      │   BJT   │         ┌───┴───┐
      │  NPN    │         │ PMOS  │ ──► Dòng ra tải I_out
      └────┬────┘         │  M1   │
           │ Base         └───┬───┘
           │                  │
       [ R_base ]         [ R_skin ]
           │                  │
      Chân DAC/PWM           GND
      của STM32 (0 - 3.3V)
```

**Nguyên lý vận hành hoàn hảo:**
1. STM32 xuất điện áp $V_{\text{DAC}}$ ($0 - 3.3\,\text{V}$) điều khiển dòng tầng dưới: $I_{\text{ctrl}} = \frac{V_{\text{DAC}}}{R_{\text{emitter}}}$.
2. Dòng này kéo sụt áp qua $R_{\text{pull}}$ ở phía trên: $V_{\text{CTRL}} = V_{\text{BOOST}} - (I_{\text{ctrl}} \times R_{\text{pull}})$.
3. Mạch High-Side Op-Amp duy trì sụt áp trên $R_{\text{sense}}$ bằng đúng sụt áp trên $R_{\text{pull}}$:
   $$I_{\text{out}} = \frac{V_{\text{BOOST}} - V_{\text{CTRL}}}{R_{\text{sense}}} = \frac{I_{\text{ctrl}} \times R_{\text{pull}}}{R_{\text{sense}}}$$
4. Chọn $R_{\text{pull}} = R_{\text{sense}} = 10\,\text{k}\Omega$:
   $$I_{\text{out}} = I_{\text{ctrl}} = \frac{V_{\text{DAC}}}{R_{\text{emitter}}}$$
   **Dòng điện bơm vào cơ thể người tỉ lệ tuyến tính 1:1 với điện áp DAC của STM32 và hoàn toàn miễn nhiễm với sự trồi sụt của nguồn Boost 20V!**

---

### 2. Phân Công Ngoại Vi STM32 (Firmware Architecture)

1. **Timer 1 (Hardware PWM / DAC DMA):**
   - Tạo tín hiệu biên độ dòng điện $10\,\mu\text{A} - 500\,\mu\text{A}$.
2. **Timer 2 (Biphasic Polarity Switching):**
   - Tạo ngắt định thời $20\,\text{Hz}$ (chu kỳ $50\,\text{ms}$, mỗi pha $25\,\text{ms}$) điều khiển IC cầu H tín hiệu nhỏ (TS5A3159) để đảo chiều dòng điện (Zero Net DC).
3. **ADC1 (Chân PA0 - Đo điện áp tải):**
   - Đọc điện áp rơi $V_{\text{skin}}$ qua cầu phân áp tỉ lệ $1:10$.
   - Tính $R_{\text{skin}}$: Nếu $V_{\text{skin}} > 18.5\,\text{V}$ $\to$ Nhận diện hở mạch (nhấc tay) $\to$ Ngắt dòng về $0$.
   - Nếu $R_{\text{skin}} < 20\,\text{k}\Omega$ (tay ra mồ hôi) $\to$ Tự động hạ dòng từ $200\,\mu\text{A}$ xuống $80\,\mu\text{A}$.
4. **Timer 3 (Ngắt chống dội nút bấm Non-blocking):**
   - Áp dụng đúng thuật toán EXTI + One-Pulse Timer 20ms chắt lọc từ cuộc trò chuyện để chuyển đổi các chế độ điều trị mà không làm đơ hệ thống.
