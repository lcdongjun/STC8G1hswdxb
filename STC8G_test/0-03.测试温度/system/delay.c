#include "delay.h"

void Delay_us(unsigned int us) 
{
    while (us--) {
        _nop_(); _nop_(); _nop_(); _nop_(); // 适用于 24MHz
        _nop_(); _nop_(); _nop_(); _nop_();
        _nop_(); _nop_(); _nop_(); _nop_();
				_nop_();
    }
}

void Delay_ms(unsigned int ms) 
{
    while (ms--) {
        Delay_us(1000);
    }
}




//void Delay_us(unsigned int us) 
//{
//	//使用定时器2延时
//    unsigned int reload = 65536 - (FOSC / 1000000 * us);
//    T2H = reload >> 8;
//    T2L = reload & 0xFF;
//    AUXR |= 0x04;
//    AUXR &= ~0x08;
//    AUXR |= 0x10;
//    while (!(AUXINTIF & 0x01));
//    AUXINTIF &= ~0x01;
//    AUXR &= ~0x10;
//}

//void Delay_ms(unsigned int ms) 
//{
//    while (ms--) {
//        Delay_us(1000);
//    }
//}