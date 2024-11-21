#include <avr/io.h>
#include "thuvienrieng.h" // Thay bằng tên file thư viện của bạn
#include <util/delay.h>

#define ADC_CHANNEL 0 // Kết nối LM35 vào chân ADC0
#define ADC_REF_VOLTAGE 5.0 // Điện áp tham chiếu (VD: 5V)

// Hàm khởi tạo ADC
void ADC_INIT() {
    ADC_AVCC();           // Chọn điện áp tham chiếu AVCC
    ADC_ALIGN_R();        // Căn phải kết quả ADC
    ADC_PRES(64);         // Đặt prescaler = 64 (với f_CPU = 16MHz, tần số ADC sẽ là 250kHz)
}

// Hàm đọc giá trị ADC
uint16_t ADC_READ(uint8_t channel) {
    ADC_IN(channel);      // Chọn kênh ADC
    ADC_STA_CONVERT();    // Bắt đầu chuyển đổi
    while (ADCSRA & (1 << ADSC)); // Chờ chuyển đổi hoàn thành
    return ADC;           // Trả về giá trị ADC
}

// Hàm chuyển đổi giá trị ADC sang nhiệt độ (LM35)
float ADC_TO_TEMPERATURE(uint16_t adc_value) {
    float voltage = (adc_value * ADC_REF_VOLTAGE) / 1024.0; // Điện áp = ADC_value * (Vref / 1024)
    return voltage * 100.0; // LM35: 10mV tương ứng với 1°C => Nhân với 100
}

int main() {
    uint16_t adc_value;
    float temperature;

    // Khởi tạo LCD và ADC
    LCD4_INIT(1, 0);      // Khởi tạo LCD, con trỏ gạch dưới, không nhấp nháy
    ADC_INIT();           // Khởi tạo ADC

    LCD4_CLR();           // Xóa LCD
    LCD4_OUT_STR("Temp:"); // Hiển thị "Temp:" ở dòng đầu

    while (1) {
        // Đọc giá trị ADC từ cảm biến LM35
        adc_value = ADC_READ(ADC_CHANNEL);

        // Chuyển đổi giá trị ADC sang nhiệt độ
        temperature = ADC_TO_TEMPERATURE(adc_value);

        // Hiển thị nhiệt độ lên LCD
        LCD4_CUR_GOTO(2, 0); // Di chuyển con trỏ đến dòng 2, cột 0
        LCD4_OUT_STR("Temp: "); // In "Temp: "
        LCD4_OUT_DEC((unsigned long)temperature, 3); // In giá trị nhiệt độ
        LCD4_OUT_STR(" C"); // Thêm ký tự "C"

        _delay_ms(500); // Đợi 500ms trước khi đọc lại
    }

    return 0;
}
