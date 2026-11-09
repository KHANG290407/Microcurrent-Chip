# Register Map & Communication Protocol
## Microcurrent Driver Module (UMDM) - I2C Slave Interface

- **I2C Address (Mặc định):** `0x38` (7-bit address)
- **Tốc độ xung nhịp:** Hỗ trợ Standard mode (100 kHz) và Fast mode (400 kHz)

---

### Danh sách thanh ghi (Register Table)

| Địa chỉ (Hex) | Tên thanh ghi | Quyền | Giá trị mặc định | Mô tả chức năng |
| :---: | :---: | :---: | :---: | :--- |
| `0x00` | `WHO_AM_I` | R | `0x55` | Mã nhận diện module (ID cố định) |
| `0x01` | `CTRL_REG` | R/W | `0x00` | Điều khiển hoạt động: <br> - Bit 0: `ENABLE` (1: Bật phát vi dòng, 0: Tắt)<br> - Bit 1: `MODE` (0: Xung 2 pha đối xứng, 1: Xung đơn)<br> - Bit 2: `SOFT_START` (1: Kích hoạt tăng dòng êm) |
| `0x02` | `CURRENT_SET_L` | R/W | `0x64` (100) | Byte thấp của cường độ dòng điện đặt ($\mu\text{A}$), dải $10 - 500\,\mu\text{A}$ |
| `0x03` | `CURRENT_SET_H` | R/W | `0x00` | Byte cao của cường độ dòng điện đặt |
| `0x04` | `FREQ_SET` | R/W | `0x0A` (10) | Tần số xung kích thích ($\text{Hz}$), dải $1 - 100\,\text{Hz}$ |
| `0x05` | `DUTY_CYCLE` | R/W | `0x32` (50) | Chu kỳ làm việc Duty Cycle ($10\% - 90\%$, mặc định $50\%$) |
| `0x06` | `STATUS_REG` | R | `0x00` | Trạng thái hệ thống: <br> - Bit 0: `RUNNING` (Đang phát dòng)<br> - Bit 1: `SKIN_CONTACT` (1: Đang có tiếp xúc da, 0: Hở mạch)<br> - Bit 2: `OVER_CURRENT_FAULT` (Báo động lỗi quá dòng)<br> - Bit 3: `COMPLIANCE_LIMIT` (Áp nguồn kịch trần) |
| `0x07` | `IMPEDANCE_L` | R | `0x00` | Byte thấp của trở kháng da ước lượng (Đơn vị: $\times 10\,\Omega$) |
| `0x08` | `IMPEDANCE_H` | R | `0x00` | Byte cao của trở kháng da ước lượng |

---

### Ví dụ luồng điều khiển từ Host MCU (ESP32 / Chuột):
1. Ghi `WHO_AM_I` (`0x00`) kiểm tra kết nối với module.
2. Ghi `CURRENT_SET_L` = `0xC8`, `CURRENT_SET_H` = `0x00` (Đặt dòng kích thích $= 200\,\mu\text{A}$).
3. Ghi `FREQ_SET` = `0x14` (Đặt tần số $= 20\,\text{Hz}$).
4. Ghi `CTRL_REG` = `0x05` (Bật `ENABLE` và `SOFT_START`).
5. Định kỳ đọc `STATUS_REG` (`0x06`) để biết người dùng có đang chạm tay vào chuột/vòng đeo hay không.
