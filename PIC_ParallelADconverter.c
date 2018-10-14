#include <18f23k22.h>
#device ADC=10
#fuses INTRC, PUT, NOBROWNOUT, NOWDT, NOPROTECT, NOLVP, NOMCLR
#use delay(CLOCK = 64M)
#use fast_io(a)
#use fast_io(b)
#use fast_io(c)

// アナログ入力ピン
const int analog_num = 4;
// highになるとアナログ値を10bit出力する
const long input_flag  = PIN_A7;
// 出力完了するとhigh
const long output_flag = PIN_A6;
// 要素番号のbit数
const int element_pin_num = 2;
// 要素番号を指定するピン
const long element_pin[2] = {PIN_A4, PIN_A5};

void main(void)
{
	// 立ち上がり・立ち下がり判定
	short flag = true;
	int element = 0;
	// ループカウンタ
	int i, pin;
	// アナログ値格納
	unsigned long analog_value[analog_num] = {0};
	
	set_tris_a(0b10111111);
	set_tris_b(0b00000000);
	set_tris_c(0b00000000);
	
	setup_adc(ADC_CLOCK_INTERNAL);
	// sAN0 :A0, sAN1 :A1, sAN2 :A2, sAN3 :A3
	setup_adc_ports(sAN0 | sAN1 | sAN2 | sAN3);
	setup_adc_ports(VSS_VDD);
	
	setup_timer_0(T0_INTERNAL | T0_DIV_16);
	setup_timer_1(T1_INTERNAL | T1_DIV_BY_8);
	enable_interrupts(INT_TIMER0);
	enable_interrupts(INT_TIMER1);
	enable_interrupts(GLOBAL);
	
	delay_ms(2000);
	
	while(1)
	{
		for(i = 0; i < analog_num; i++)
		{
			set_adc_channel(i);
			analog_value[i] = read_adc();
			
			if(input(input_flag) && flag)
			{
				// 要素番号の取得
				for(pin = 0, element = 0; pin < element_pin_num; pin++)
					element |= input(element_pin[pin]) << pin;
				// 10bitで出力
				output_b(0xff & (analog_value[element] >> 2));
				output_c(analog_value[element] << (8-2));
				// 出力完了フラグ
				output_high(output_flag);
				// 立ち下がるまでfalse
				flag = false;
			}
			else if(!input(input_flag) && !flag)
			{
				// 出力完了フラグのリセット
				output_low(output_flag);
				// 立ち上がるまでtrue
				flag = true;
			}
		}
	}
}
