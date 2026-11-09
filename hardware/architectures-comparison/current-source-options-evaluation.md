# So Sánh Các Kiến Trúc Nguồn Vi Dòng Cho Microcurrent Chip (UMDM)
> **Mục tiêu:** Đánh giá ưu/nhược điểm, độ phức tạp, độ ổn định và chi phí của 3 giải pháp phần cứng tạo nguồn dòng $10\,\mu\text{A} - 500\,\mu\text{A}$ cho ứng dụng y sinh.

---

## 1. Bảng So Sánh Tổng Quan 3 Option

| Tiêu chí kỹ thuật | Option 1: Howland Pump + Matched Resistor Network | Option 2: High-Side VCCS (Op-Amp + MOSFET/BJT + 1 R_sense) | Option 3: IC Kích Thích Dòng Tích Hợp (Dedicated Stimulator AFE) |
| :--- | :--- | :--- | :--- |
| **Bản chất nguyên lý** | Cầu 4 điện trở vi sai hồi tiếp âm + dương qua 1 Op-Amp | Nguồn dòng 1 chiều khép kín qua 1 điện trở shunt duy nhất $R_{\text{sense}}$ + Cầu H chuyển mạch | IC chuyên dụng tích hợp sẵn DAC, Current Source & H-Bridge bên trong 1 con chip |
| **Yêu cầu khớp điện trở** | **Bắt buộc cực cao ($0.01\% - 0.05\%$)**. Nếu lệch dễ bị tự kích dao động | **Không cần khớp cầu**. Chỉ cần 1 con trở $R_{\text{sense}}$ duy nhất có độ chính xác $0.1\%$ | Không cần điện trở ngoài hoặc chỉ cần 1 điện trở định chuẩn (Reference Resistor) |
| **Rủi ro tự kích dao động** | Cao nếu tải dung tính hoặc tỷ số cầu bị lệch ($Z_{\text{out}} < 0$) | **Cực thấp**. Ổn định tuyệt đối vì chỉ có hồi tiếp âm | Rất thấp (hãng đã tích hợp sẵn mạch bù tần số) |
| **Linh kiện đại diện** | Op-Amp Rail-to-Rail + **LT5400** / **Vishay MPM** | **OPA320 / MCP6001** + MOSFET P-ch + Cầu H **TS5A3159 / ADG884** | **TI LM13700**, **MAX1739**, **ADI AD5748** hoặc AFE Y Sinh |
| **Kích thước PCB** | Trung bình ($15 \times 20\,\text{mm}$) | Nhỏ gọn ($12 \times 16\,\text{mm}$) | Siêu nhỏ ($8 \times 10\,\text{mm}$) |
| **Độ sẵn có & Giá thành** | Matched Network giá cao ($3 - \$5/chip$), khó mua hơn | **Rất phổ biến, giá cực rẻ ($< \$1)**, linh kiện dễ tìm | Giá cao, khó mua mẫu nhỏ lẻ tại Việt Nam |
| **Mức độ phù hợp Đồ án KS Tự Động Hóa** | **Rất tốt** (thể hiện xuất sắc kiến thức lý thuyết mạch vi sai) | **Khuyên dùng nhất (Thực tế & Ổn định nhất)** | Ít đất diễn cho thuật toán phần cứng vì chip đã làm hết |

---

## 2. Chi Tiết Từng Option

### 🔹 OPTION 1: Howland Current Pump Dùng Matched Resistor Network
- **Sơ đồ nguyên lý:**
  1 Op-Amp kết hợp mạng 4 điện trở màng mỏng (Thin Film) tích hợp trong vỏ SOIC-8 (như **LT5400-1** hoặc **Vishay MPM**).
- **Cơ chế hoạt động:**
  Dòng điện ra: $I_{\text{out}} = \frac{V_{\text{ctrl}}}{R}$. Do 4 điện trở nằm trên cùng một die silicon, tỷ số $\frac{R_2}{R_1} = \frac{R_4}{R_3}$ được đảm bảo với sai số $< 0.01\%$, triệt tiêu hoàn toàn hiện tượng trôi nhiệt (TCR Tracking $< 0.2\,\text{ppm}/^\circ\text{C}$).
- **Ưu điểm:**
  - Có thể tạo dòng 2 chiều trực tiếp nếu cấp nguồn đối xứng ($\pm 15\,\text{V}$).
  - Cấu trúc kinh điển, được các hội đồng học thuật và giáo sư đánh giá rất cao về tính hàn lâm.
- **Nhược điểm:**
  - Con chip mạng điện trở chuyên dụng (như LT5400) khá đắt tiền và thường phải đặt hàng quốc tế (Mouser/Digikey).

---

### 🔹 OPTION 2: High-Side VCCS (Op-Amp + P-MOSFET) + Cầu H Chuyển Mạch Tín Hiệu Nhỏ (Khuyên Dùng Cho Dự Án)
- **Sơ đồ nguyên lý:**
  ```
     V_BOOST (+15V đến +24V)
         │
         ├───[ R_sense (1k 0.1%) ]───┐
         │                           │
        ┌┴┐ (Không đảo +)            │
        │+│                          │
  V_ctrl│ ├────── Source (P-MOSFET) ─┘
        │-│            │ Gate
        └┬┘ (Đảo -)    │
         ▲             ▼ Drain (Dòng không đổi: I = (V_boost - V_ctrl)/R_sense)
         └─────────────┤
                       │
             ┌─────────┴─────────┐
             │ Cầu H (Dual SPDT) │ ◄── Chân GPIO đảo chiều xung Biphasic
             │     TS5A3159      │
             └────┬─────────┬────┘
                  │         │
               Cực A     Cực B
                  │         │
                  └─[Da]────┘
  ```
- **Cơ chế hoạt động:**
  1. Op-Amp điều khiển cực Gate của P-MOSFET để giữ cho điện áp rơi trên điện trở $R_{\text{sense}}$ luôn bằng $V_{\text{boost}} - V_{\text{ctrl}}$.
  2. Dòng cực Drain là dòng không đổi: $I = \frac{\Delta V}{R_{\text{sense}}}$.
  3. Dòng điện này đi qua một IC chuyển mạch analog 2 kênh (Cầu H tín hiệu nhỏ như TS5A3159 hoặc ADG884) để đảo cực thành xung 2 pha đối xứng (Biphasic).
- **Ưu điểm vượt trội:**
  - **Chỉ cần đúng 1 con điện trở chuẩn $R_{\text{sense}}$ duy nhất** $\to$ Không có bài toán lệch cầu, không sợ sai số tỷ lệ!
  - Ổn định $100\%$, không bao giờ bị hiện tượng trở kháng ngõ ra âm ($Z_{\text{out}} < 0$).
  - Linh kiện cực kỳ phổ thông, giá thành bo mạch chỉ vài chục nghìn VND.

---

### 🔹 OPTION 3: Sử Dụng IC Chuyên Dụng Tích Hợp (Dedicated AFE / Current DAC)
- **Sơ đồ nguyên lý:**
  Sử dụng các dòng IC Current Output DAC (như AD5748, AD5422) hoặc IC tạo dòng vi sai chuyên dụng cho thiết bị y tế.
- **Cơ chế hoạt động:**
  Vi điều khiển giao tiếp với chip qua SPI/I2C, chỉ cần gửi byte giá trị dòng (VD: `write(200uA)`), chip tự sinh dòng chính xác với độ phân giải 16-bit.
- **Ưu điểm:**
  - Bo mạch siêu nhỏ, linh kiện tối giản, tích hợp sẵn mạch chẩn đoán hở mạch/quá nhiệt bên trong.
- **Nhược điểm:**
  - Hầu hết IC công nghiệp hướng tới dải dòng $4 - 20\,\text{mA}$, dải vi dòng ($10 - 500\,\mu\text{A}$) thường yêu cầu dòng chip y tế chuyên sâu (Bio-impedance / Stimulation AFE), khó mua lẻ và tài liệu thường yêu cầu NDA với hãng.

---

## 3. Đề Xuất Chiến Lược Cho Nhóm Dự Án
1. **Thiết kế Schematic & PCB theo cấu hình linh hoạt (Hybrid Layout):**
   - Thiết kế footprint chính trên bo mạch theo **Option 2 (Op-Amp + P-MOSFET + Analog Switch)** vì tính khả thi thi công tại Việt Nam là $100\%$, giá rẻ và cực kỳ bền bỉ.
   - Bố trí thêm footprint dự phòng cho **Option 1 (Mạng LT5400)** để khi cần làm báo cáo học thuật hoặc nâng cấp phiên bản cao cấp có thể hàn vào ngay.
2. Cả 2 mô hình Option 1 và Option 2 đã được chuẩn bị file mô phỏng LTspice trong thư mục `hardware/simulation/` để nhóm đối chiếu kết quả.
