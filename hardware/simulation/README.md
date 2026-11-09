# Hướng Dẫn Mô Phỏng Mạch Bơm Dòng Howland Trên LTspice
> **Dành cho dự án:** Microcurrent Chip / Universal Microcurrent Driver Module (UMDM)

Tài liệu này hướng dẫn bạn cách mở, chạy và phân tích mạch **Howland Current Pump** (mạch nguồn dòng không đổi) bằng phần mềm **LTspice**.

---

## 1. Phân tích ảnh mạch mẫu bạn cung cấp

Trong ảnh bạn gửi (*Howland Current Pump Circuit - Circuit Digest*):
- Mạch sử dụng IC Op-Amp **uA741** (hoặc Op-Amp thông dụng).
- Các điện trở cầu Howland:
  - Cặp điện trở nhánh đảo: $R_1 = 3.9\,\text{k}\Omega$, $R_2 = 3.9\,\text{k}\Omega$
  - Cặp điện trở nhánh không đảo: $R_3 = 1\,\text{k}\Omega$, $R_4 = 1\,\text{k}\Omega$
  - Điện trở tải: $R_5 = 3\,\text{k}\Omega$ (trong bài toán của chúng ta, $R_5$ chính là **trở kháng da người** $R_{\text{skin}}$).
  - Tỷ số cân bằng: $\frac{R_2}{R_1} = \frac{3.9\,\text{k}}{3.9\,\text{k}} = 1$ và $\frac{R_4}{R_3} = \frac{1\,\text{k}}{1\,\text{k}} = 1 \implies$ Cầu cân bằng hoàn hảo!
- Khi cầu cân bằng, dòng điện qua tải $R_5$ được tính theo công thức:
  $$I_{\text{load}} = \frac{V_{\text{in}}}{R_3} \cdot \frac{R_2}{R_1} = \frac{V_{\text{in}}}{R_3}$$
  *(Dòng qua tải hoàn toàn không phụ thuộc vào giá trị điện trở tải $R_5$!)*

---

## 2. Các tệp mô phỏng sẵn có trong thư mục

Thư mục [`hardware/simulation/`](./) đã được tạo sẵn 3 file mô phỏng:

| Tên tệp | Mô tả | Ứng dụng |
| :--- | :--- | :--- |
| **`howland_pump_basic.asc`** | File Schematic đồ họa chuẩn của **LTspice** (mở trực tiếp bằng chuột) | Kiểm chứng cơ bản với nguồn sin / DC, quét giá trị tải từ $1\,\text{k}\Omega \to 50\,\text{k}\Omega$ |
| **`howland_pump_biphasic.cir`** | Netlist SPICE mô phỏng xung vuông 2 pha (Biphasic Pulse $20\,\text{Hz}$, đúng chuẩn y sinh MENS) | Đánh giá độ cân bằng DC và khả năng giữ dòng $100\,\mu\text{A}$ |
| **`skin_model_simulation.cir`** | Mô phỏng kèm **mô hình tế bào da người (Randles Model)** gồm $R_{\text{sc}} \parallel C_{\text{sc}} + R_{\text{deep}}$ | Đánh giá đáp ứng quá độ khi da bị khô hoặc ra mồ hôi |

---

## 3. Các bước chạy mô phỏng trên LTspice

### Bước 1: Mở file trên LTspice
1. Khởi động phần mềm **LTspice** trên máy tính.
2. Chọn `File` $\to$ `Open` $\to$ Trỏ tới đường dẫn:
   `C:\Users\TBC\.gemini\antigravity\scratch\microcurrent-chip\hardware\simulation\howland_pump_basic.asc`

### Bước 2: Chạy mô phỏng (Run)
- Nhấn vào biểu tượng **hình người đang chạy (Run)** trên thanh công cụ (hoặc phím tắt `Alt + R`).
- LTspice sẽ tự động giải hệ phương trình vi phân và hiện ra cửa sổ đồ thị màu đen.

### Bước 3: Đặt que đo kiểm tra kết quả
1. **Đo dòng điện qua tải:**
   - Di chuột lên thân điện trở `Rload` (con trỏ chuột sẽ biến thành hình kẹp đo dòng điện **Current Probe** hình vòng tròn có mũi tên).
   - Click chuột trái $\to$ Đồ thị dòng điện `I(Rload)` sẽ xuất hiện.
2. **Quan sát hiện tượng:**
   - Bạn sẽ thấy nhiều đường cong tương ứng với các giá trị tải khác nhau (`Rload` = $1\,\text{k}\Omega$, $5\,\text{k}\Omega$, $10\,\text{k}\Omega$, $20\,\text{k}\Omega$, $50\,\text{k}\Omega$).
   - **Tất cả các đường đều chồng khít lên nhau ở biên độ đỉnh $100\,\mu\text{A}$!**
   - Điều này chứng minh: Dù trở kháng da có thay đổi thế nào, dòng điện kích thích bơm vào người dùng vẫn giữ nguyên ở mức vi dòng định trước.

---

## 4. Những điểm cần lưu ý khi thiết kế thực tế

1. **Điện áp tuân thủ (Compliance Voltage):**
   - Trong mô phỏng, nguồn nuôi Op-Amp được đặt $\pm 15\,\text{V}$ hoặc $\pm 20\,\text{V}$.
   - Nếu bạn giảm nguồn nuôi xuống $+3.3\,\text{V}$ hoặc $+5\,\text{V}$ (nguồn USB/Pin), bạn sẽ thấy khi `Rload` $= 20\,\text{k}\Omega$, đỉnh dòng điện bị xén phẳng (bị méo/bão hòa). Đây chính là bằng chứng vì sao chip/module bắt buộc phải có tầng Boost Converter nâng áp!
2. **Dung sai điện trở (Resistor Tolerance):**
   - Để nguồn dòng đạt độ chính xác cao nhất, 4 điện trở $R_1, R_2, R_3, R_4$ trên mạch in thực tế nên chọn loại sai số nhỏ ($0.1\%$ hoặc $0.5\%$).
