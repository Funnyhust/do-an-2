#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "thuvienrieng.h"

#define DC DDRC
#define DD DDRD
#define PC PORTC
#define PD PORTD

/////LCD
void setup(unsigned char x){
	PC = x;
	PD &=~(1<<RS);
	PD &=~(1<<RW) ;
	PD |= (1<<E);
	_delay_ms(1);
	PD &=~(1<<E);
	_delay_ms(2);
	
}
void dischar(unsigned char x){
	PC =x;
	PD |=(1<<RS);
	PD &=~(1<<RW);
	PD |= (1<<E);
	_delay_ms(1);
	PD &=~(1<<E);
	_delay_ms(3);
	
}
void khoitao(){
	DC=0xFF; //output du lieu
	DD=0xFF;
	_delay_ms(20);
	setup(0x38);
	setup(0x0C);
	setup(0x06);
	setup(0x01);
	setup(0x80);	
}
void disstr(char *str){
	for(int i=0; str[i]!=0;i++){
		dischar(str[i]);
	}
	
}
void display(char row, char pos, char *str){
	if (row == 0 && pos < 16){
		setup((pos&0x0F)|0x80);
	}
	else if(row ==1 && pos <16){
		setup((pos&0x0F)|0xC0);
	}	
	disstr(str);
}
void clear(){
	setup(0x01);
	setup(0x80);
	_delay_ms(2);
}
/////

////Cac ham doc nhiet do
void adc_init() {
	ADC_PRES(128); // Prescaler 128
	ADC_AVCC();    // ?i?n áp tham chi?u AVCC (5V)
	ADC_ALIGN_R(); // C?n ph?i k?t qu? ADC
}

// Hàm ??c giá tr? ADC
uint16_t adc_read(uint8_t channel) {
	ADC_IN(channel);          // Chon kênh ADC
	ADC_STA_CONVERT();        // bat dau chuyen doi
	while (ADCSRA & (1<<ADSC)); // Chuyen doi hoàn thành
	return ADC;               // Tra ve giá tri ADC (10-bit)
}

// Hàm chuy?n giá tr? adc thành nhi?t ??
float lm35_read_temp(uint16_t adc_value) {
	// dien áp (mV) = (ADC_value / 1024) * 5000 (Vref = 5V)
	float voltage = (adc_value) / 1023.0; // dien áp (V)
	return voltage * 100; // Tra ve nhiet do (°C)
}
/////

int main(void) {
    char buffer[16]; // Chuoi luu gia tri hien thi LCD

    // Khoi tao LCD và ADC
    khoitao();
    adc_init();

    clear();
    disstr("Do am:");

    while (1) {
	    uint16_t adc_value = adc_read(0);      // doc gia ADC tai kênh 0 (LM35)
	    float temperature = lm35_read_temp(adc_value); // Chuyen sang nhi?t ??

	    // hi?n th? nhi?t ?? lên LCD
	    dtostrf(temperature, 6, 2, buffer);
		_delay_ms(20);
	    display(1, 1,buffer ); // ??a con tr? xu?ng dòng 2
		disstr("%");
		_delay_ms(200);
    }

    return 0;
}




/*
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#define LED_Direction DDRC		//* define LED Direction
#define LED_PORT PORTC			//* define LED port 

int main(void)
{
	LED_Direction |= 0xff;		//* define LED port direction is output 
	LED_PORT = 0xff;
	
	DDRD |= 0xff;
	PORTD = 0xff;
	
char array[]={0x80, 0xE3, 0x44, 0x41, 0x23, 0x11, 0x10, 0xC3, 0x00, 0x01};
char led[]={0x01,0x02,0x04,0x08,0X10,0x20,0x40,0x80}	;
//*g f a b dp c d e

while(1)
{
	for(int i=0;i<10;i++)
	{
		LED_PORT = array[i]; //* write data on to the LED port 
		_delay_ms(1000); //* wait for 1 second 
	}
}

		for(int i=0;i<9;i++)
	{
		i%=9;
		PORTD = ~(1<<i); //* write data on to the LED port 
		_delay_ms(100); //* wait for 1 second 
	}
}
}
*/