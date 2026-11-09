# Phân Tích Mạch Bơm Dòng Howland (Enhanced Howland Current Pump)
## Ứng Dụng Trong Kích Thích Vi Dòng Sinh Học

---

## 1. Nguyên lý hoạt động

Mạch Howland Current Pump là mạch chuyển đổi điện áp thành dòng điện (VCCS - Voltage Controlled Current Source) kinh điển, có khả năng cung cấp dòng điện không đổi vào tải hai đầu (Bilateral / Floating load hoặc Grounded load) bất chấp trở kháng tải thay đổi.

```
                    R1
        Vin ───/\/\/\/\────┐
                           │
                          ┌┴┐ (Đảo -)
                          │-│
                          │ ├────────────── Vout
                          │+│
                          └┬┘ (Không đảo +)
                           │
                           ├─────/\/\/\/\──── Vout
                           │        R4
                           │
                           ├───/\/\/\/\──── Load (R_skin) ─── GND
                           │        R3
                          [R2]
                           │
                          GND
```

### Điều kiện cân bằng mạch cầu:
Để trở kháng ra của nguồn dòng đạt vô cùng ($Z_{\text{out}} \to \infty$), tỷ số các điện trở phải thỏa mãn:
$$
\frac{R_4}{R_2} = \frac{R_3}{R_1}
$$

Khi điều kiện này được đáp ứng, dòng điện qua tải $I_{\text{load}}$ hoàn toàn không phụ thuộc vào giá trị điện trở tải $R_{\text{load}}$ (trở kháng da):
$$
I_{\text{load}} = - \frac{V_{\text{in}}}{R_1} \cdot \frac{R_4}{R_3}
$$

Nếu chọn $R_1 = R_2 = R_4 = R_3 = R$:
$$
I_{\text{load}} = - \frac{V_{\text{in}}}{R}
$$

---

## 2. Thách thức kỹ thuật và giải pháp trong môi trường y sinh

1. **Sai số dung sai điện trở (Resistor Matching):**
   - Nếu tỷ số điện trở lệch dù chỉ $1\%$, trở kháng ngõ ra $Z_{\text{out}}$ sẽ tụt giảm mạnh, khiến dòng điện $I_{\text{load}}$ bị sụt khi trở kháng da tăng.
   - **Giải pháp:** Sử dụng mạng điện trở tích hợp chính xác cao (Resistor Network sai số $0.1\%$ hoặc $0.05\%$) hoặc mạch Op-Amp + BJT khép kín hồi tiếp dòng qua điện trở Shunt độ chính xác cao.

2. **Dải điện áp tuân thủ (Compliance Voltage):**
   - Khi trở kháng da khô $R_{\text{skin}} = 20\,\text{k}\Omega$ và dòng cần bơm $I_{\text{load}} = 500\,\mu\text{A}$:
     $$V_{\text{load}} = 500\,\mu\text{A} \times 20\,\text{k}\Omega = 10\,\text{V}$$
   - Nếu dùng nguồn nuôi $3.3\,\text{V}$ thông thường của MCU, Op-Amp sẽ bị bão hòa (Saturate) và không thể đẩy đủ dòng.
   - **Giải pháp:** Thiết kế tầng Boost Converter cấp nguồn kép $\pm 12\,\text{V}$ hoặc $+15\,\text{V}$ chuyên dụng cho tầng công suất Op-Amp.
