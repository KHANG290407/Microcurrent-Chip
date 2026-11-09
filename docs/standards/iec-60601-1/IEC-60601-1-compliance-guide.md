# Tiêu Chuẩn IEC 60601-1 & Áp Dụng Cho Microcurrent Chip (UMDM)
> **Tài liệu tham chiếu chuẩn y tế:** [Intertek Medical - IEC 60601-1 Regulatory Requirements](https://www.intertek.com/medical/regulatory-requirements/iec-60601-1/)  
> **Áp dụng cho:** Universal Microcurrent Driver Module (UMDM) và Thiết bị ngoại vi y sinh (Chuột máy tính, Đồng hồ/Vòng đeo tay).

---

## 1. Tổng quan về bộ tiêu chuẩn IEC 60601-1

**IEC 60601-1** là tiêu chuẩn quốc tế được công nhận toàn cầu về **An toàn cơ bản (Basic Safety)** và **Hiệu năng thiết yếu (Essential Performance)** của thiết bị điện y tế (Medical Electrical Equipment - ME Equipment).

Cấu trúc tiêu chuẩn bao gồm:
1. **IEC 60601-1 (Base Standard):** Các yêu cầu chung về an toàn điện, an toàn cơ khí, bức xạ, quá nhiệt và quản lý rủi ro (Risk Management theo ISO 14971).
2. **IEC 60601-1-XX (Collateral Standards):** Tiêu chuẩn phụ trợ áp dụng theo phân loại:
   - **IEC 60601-1-2:** Tương thích điện từ (EMC - Electromagnetic Compatibility).
   - **IEC 60601-1-10:** Yêu cầu an toàn đối với bộ điều khiển vòng kín sinh lý (Physiologic Closed-Loop Controllers - PCLC).
   - **IEC 60601-1-11:** Thiết bị sử dụng trong môi trường y tế gia đình và văn phòng (Home Healthcare Environment - áp dụng trực tiếp cho chuột và vòng đeo tay).
3. **IEC 60601-2-10 (Particular Standard):** Tiêu chuẩn chuyên biệt cho **Thiết bị kích thích thần kinh và cơ (Nerve and Muscle Stimulators)**.

---

## 2. Phân loại thiết bị áp dụng cho dự án (Classification)

| Tiêu chí | Phân loại IEC 60601-1 | Giải thích & Thiết kế phần cứng tương ứng |
| :--- | :--- | :--- |
| **Nguồn cấp điện** | **Internally Powered ME Equipment** | Thiết bị chạy pin sạc (Li-ion/Li-Po), sạc qua cổng Type-C ngắt khỏi lưới điện AC trong khi sử dụng. |
| **Bộ phận tiếp xúc trực tiếp (Applied Part)** | **Type BF (Body Floating)** | Điện cực tiếp xúc trực tiếp vào da người, phải được cách ly điện hoàn toàn khỏi đất và nguồn sạc lưới. |
| **Môi trường sử dụng** | **IEC 60601-1-11** | Sử dụng cá nhân ngoài phòng khám (văn phòng, gia đình), đòi hỏi khả năng chống ẩm, chống sốc và giao diện an toàn cao. |

---

## 3. Các yêu cầu an toàn cốt lõi đối với Module Vi Dòng (UMDM)

### 3.1. Giới hạn dòng rò qua người (Patient Leakage Current)
Theo IEC 60601-1 đối với bộ phận tiếp xúc **Type BF**:
- **Trạng thái bình thường (Normal Condition - NC):** Dòng rò một chiều qua người $\le 10\,\mu\text{A}$, dòng xoay chiều $\le 100\,\mu\text{A}$.
- **Trạng thái lỗi đơn (Single Fault Condition - SFC):** Dòng rò $\le 50\,\mu\text{A}$ (DC) và $\le 500\,\mu\text{A}$ (AC).
- 👉 **Áp dụng thiết kế:** Mặc dù dòng phát có chủ đích của vi dòng là $10 - 500\,\mu\text{A}$, nhưng **thành phần DC rò rỉ bắt buộc phải bằng 0** để tránh tổn thương màng tế bào.

### 3.2. Triệt tiêu thành phần một chiều (Zero Net DC Offset)
- Dòng kích thích bắt buộc phải là **dòng xoay chiều hoặc xung 2 pha đối xứng (Biphasic Pulse)** với tổng tích phân điện tích chu kỳ dương bằng chu kỳ âm:
  $$\int_{0}^{T} I(t)\,dt = 0$$
- Tránh hiện tượng điện phân (electrolysis), tích tụ axit/kiềm làm bỏng hóa học bề mặt da.
- Bổ sung tụ điện ngăn DC (DC-blocking capacitor) trên đường phát điện cực như một tầng bảo vệ phần cứng dự phòng.

### 3.3. Bảo vệ an toàn khi xảy ra lỗi đơn (Single Fault Safety)
Theo triết lý IEC 60601-1: *Không một lỗi đơn lẻ nào của linh kiện được phép gây nguy hiểm cho người dùng.*
- **Lỗi vi điều khiển (MCU Crash / Hang):** Nếu MCU bị đơ ở trạng thái HIGH, chân điều khiển không được phép tiếp tục bơm dòng liên tục.
  - $\implies$ Sử dụng mạch dập xung AC-coupling và mạch Watchdog Timer (WDT) phần cứng tự động ngắt relay/analog switch sau $500\,\text{ms}$.
- **Lỗi bão hòa điện áp Op-Amp:** Mạch nguồn tăng áp (Boost Converter) phải có diode Zener hoặc mạch kẹp dòng (Current Limiting Circuit) khống chế dòng đỉnh tuyệt đối $< 1\,\text{mA}$.

### 3.4. Khoảng cách cách điện & Đường rò bề mặt (Creepage & Clearance)
- Vì module có mạch boost converter ($18\,\text{V} - 24\,\text{V}$), khoảng cách giữa đường điện áp cao và đường tín hiệu số I2C/USB phải đạt tối thiểu $1.5\,\text{mm} - 2.5\,\text{mm}$ trên PCB để chống phóng điện bề mặt.

---

## 4. Tương thích điện từ (EMC - IEC 60601-1-2)

Khi module vi dòng được đặt cạnh các linh kiện ngoại vi nhạy cảm:
- **Cảm biến quang học chuột (PAW3395/PMW3360):** Tần số quét hình ảnh vài nghìn FPS rất nhạy với xung switching của nguồn Boost.
- **Ăng-ten không dây 2.4GHz / BLE:**
- 👉 **Yêu cầu thiết kế:**
  1. Tầng Boost Converter phải chọn tần số đóng cắt cao ($> 1\,\text{MHz}$) nằm xa băng thông tín hiệu ngoại vi và được lọc bằng cuộn cảm bọc kim (Shielded Inductor).
  2. Phủ mass liền mạch (Solid Ground Plane), tách riêng đường Ground Tương tự (AGND) và Ground Số (DGND).
  3. Bố trí linh kiện vi dòng trong khung che chắn kim loại (Shielding Can) nếu cần.

---

## 5. Check-list thẩm tra chuẩn IEC 60601-1 cho dự án

- [ ] **Mức dòng giới hạn:** Có mạch ngắt cứng ngắt dòng tại $\le 1\,\text{mA}$?
- [ ] **Cách ly sạc:** Xe/Chuột/Vòng tay có tự ngắt kích thích vi dòng khi đang cắm dây sạc USB Type-C không?
- [ ] **Dạng sóng:** Xung vuông 2 pha (Biphasic) có độ lệch DC $\le 1\,\mu\text{A}$?
- [ ] **Tự động ngắt (Failsafe):** Tự ngắt dòng khi nhấc tay khỏi thiết bị trong vòng $\le 100\,\text{ms}$?
- [ ] **Vật liệu điện cực:** Sử dụng thép không gỉ y tế 316L hoặc silicone dẫn điện sinh học, không gây kích ứng da (tương thích ISO 10993 về tương thích sinh học)?
