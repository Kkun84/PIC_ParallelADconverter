#include <18f23k22.h>
#fuses INTRC, PUT, NOBROWNOUT, NOWDT, NOPROTECT, NOLVP, NOMCLR
#device ADC=10
#use delay(CLOCK = 64M)
#use RS232(BAUD=19200,XMIT=PIN_C6,RCV=PIN_C7)
#use fast_io(a)
#use fast_io(b)
#use fast_io(c)

const int analog_num = 2;

void main(void)
{
	int i;
	unsigned long analog_value[2] = {0};
	
	set_tris_a(0b10000111);
	set_tris_b(0b10000000);
	set_tris_c(0b10000000);
	
	setup_adc(ADC_CLOCK_INTERNAL);
	// sAN0 :A0, sAN1 :A1
	setup_adc_ports(sAN0 | sAN1);
	setup_adc_ports(VSS_VDD);
	
	setup_timer_0(T0_INTERNAL | T0_DIV_16);
	setup_timer_1(T1_INTERNAL | T1_DIV_BY_8);
	enable_interrupts(INT_TIMER0);
	enable_interrupts(INT_TIMER1);
	enable_interrupts(GLOBAL);
	
	while(1)
	{
		for(i = 0; i < analog_num; i++)
		{
			set_adc_channel(i);
			analog_value[i] = read_adc();
		}
		if(input(PIN_A2))
			printf("%ld,%ld\n", analog_value[0], analog_value[1]);
	}
}
