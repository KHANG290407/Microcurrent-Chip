# Hướng Dẫn Tương Thích & Tinh Chỉnh Tham Số Cho Từng Ngoại Vi
## (Peripheral Adaptation Guide: Mouse vs. Smartwatch / Wristband)

Khi sử dụng chung một lõi phần cứng **Universal Microcurrent Driver Module (UMDM)** cho nhiều thiết bị ngoại vi khác nhau, mạch nguồn dòng cốt lõi không cần thay đổi. Tuy nhiên, **cấu hình thanh ghi Firmware** và **diện tích điện cực vật lý** phải được hiệu chỉnh dựa trên đặc điểm giải phẫu học của từng vùng da tiếp xúc.

---

## 1. Bảng Đối Chiếu Đặc Điểm Giải Phẫu & Điện Sinh Lý

| Thông số | 🖱️ Chuột Máy Tính (Lòng bàn tay & Ngón tay) | ⌚ Đồng Hồ / Vòng Đeo Tay (Mặt trong cổ tay) | 墊 Đệm Kê Tay Kết Hợp Chuột (Bàn tay $\to$ Cổ tay) |
| :--- | :--- | :--- | :--- |
| **Độ dày lớp sừng (Stratum Corneum)** | **Rất dày ($400 - 600\,\mu\text{m}$)** | **Rất mỏng ($10 - 20\,\mu\text{m}$)** | Trung bình – Dày |
| **Trở kháng da điển hình ($R_{\text{skin}}$)** | $20\,\text{k}\Omega$ (ẩm) $\to 80\,\text{k}\Omega$ (khô/lạnh) | $2\,\text{k}\Omega$ (ẩm) $\to 15\,\text{k}\Omega$ (khô) | $15\,\text{k}\Omega \to 50\,\text{k}\Omega$ |
| **Dây thần kinh mục tiêu** | Nhánh bì Thần kinh Giữa & Thần kinh Trụ, cơ nội tại bàn tay | Trực tiếp **Dây thần kinh Giữa (Median Nerve)** trong ống cổ tay | Xuyên suốt toàn bộ **Ống cổ tay (Carpal Tunnel)** |
| **Diện tích mỗi điện cực ($A$)** | $4\,\text{cm}^2 - 8\,\text{cm}^2$ | $1\,\text{cm}^2 - 2.5\,\text{cm}^2$ | $5\,\text{cm}^2 - 10\,\text{cm}^2$ |
| **Áp lực tiếp xúc** | Biến thiên liên tục (tì tay, nhấc tay gõ phím) | Cố định, ôm sát cổ tay nhờ dây đeo | Ổn định khi đặt tay làm việc |

---

## 2. Bảng Cấu Hình Tham Số Chuẩn Trên Chip (Firmware Preset Profiles)

Để đảm bảo mật độ dòng điện nằm trong vùng hiệu quả sinh học tối ưu ($J = \frac{I}{A} \in [20, 60]\,\mu\text{A/cm}^2$) và luôn dưới ngưỡng cảm giác (Sub-sensory):

| Thanh ghi / Tham số | Profile 1: `MOUSE_MODE` | Profile 2: `WRISTBAND_MODE` | Profile 3: `CARPAL_TUNNEL_PAD` |
| :--- | :--- | :--- | :--- |
| **Cường độ dòng (`CURRENT_SET`)** | **$200\,\mu\text{A} - 350\,\mu\text{A}$** | **$60\,\mu\text{A} - 120\,\mu\text{A}$** | **$250\,\mu\text{A} - 400\,\mu\text{A}$** |
| **Tần số xung (`FREQ_SET`)** | $20\,\text{Hz}$ (Sóng Alpha & tăng ATP) | $10\,\text{Hz}$ (Thư giãn thần kinh ngoại vi) | $15\,\text{Hz} - 30\,\text{Hz}$ (Giảm viêm gân) |
| **Áp Boost tối đa (`V_BOOST_MAX`)** | $20\,\text{V} - 24\,\text{V}$ | $6\,\text{V} - 9\,\text{V}$ (Tiết kiệm pin nhỏ) | $20\,\text{V} - 24\,\text{V}$ |
| **Thời gian Soft-Start (`RAMP_TIME`)** | $1.5\,\text{s}$ (Do tay nhấc lên/đặt xuống nhiều) | $3.0\,\text{s}$ (Êm dịu cho da cổ tay nhạy cảm) | $2.0\,\text{s}$ |
| **Ngưỡng nhận diện hở mạch (`Z_OPEN`)** | $> 100\,\text{k}\Omega$ | $> 40\,\text{k}\Omega$ | $> 80\,\text{k}\Omega$ |
| **Chu kỳ hoạt động (Pomodoro)** | 20 phút BẬT / 40 phút NGHỈ | 15 phút BẬT / 45 phút NGHỈ | 20 phút BẬT / 40 phút NGHỈ |
