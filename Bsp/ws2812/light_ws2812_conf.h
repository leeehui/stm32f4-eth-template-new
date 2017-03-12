/*
 * light weight WS2812 lib - ARM Cortex M0/M0+ version
 *
 * Created: 07.07.2013
 *  Author: Tim (cpldcpu@gmail.com)
 */

#ifndef LIGHT_WS2812_CONF_H_
#define LIGHT_WS2812_CONF_H_

/* Ensure stdint is only used by the compiler, and not the assembler. */
#if defined(__ICCARM__) || defined(__CC_ARM) || defined(__GNUC__)
    #include <stdint.h>
    #include "stm32f4xx.h"

void ws2812_send(uint8_t *data, uint8_t *data1,uint8_t *data2,uint8_t *data3,uint8_t *data4,uint8_t *data5,int datlen);

#endif
///////////////////////////////////////////////////////////////////////
// User defined area: CPU specific CMSIS include
///////////////////////////////////////////////////////////////////////

//#include "LPC8XX.h"			// Change this to the include for your MCU


//delay 1 nanosecond needs (ws2812_cpuclk/1000)/1000000 cpu clcle
#define ws2812_ctot	(((ws2812_cpuclk/1000)*1250)/1000000) //1.25¦Ìs = 1250ns  ws2812_ctot = 168 * 1.25 = 210    
#define ws2812_t1	(((ws2812_cpuclk/1000)*375 )/1000000)	  // floor       ws2812_t1 = 168 * 0.375 = 63
#define ws2812_t2	(((ws2812_cpuclk/1000)*625)/1000000)    // ceil          ws2812_t2 = 168 * 0.625 = 105

//ZERO
//  T0______T1       T2        T3
//  |        |                 |
//   |        |_________________|

//ONE
//  T0______T1_______T2        T3
//  |                 |        |
//  |                 |________|

//¡¡T0 : set high, wait "w1" cpu cycles
//  T1 : set low if sending zero, keep unchanged if sending one, wait "w2" cycles
//  T2 : set low unconditionally, wait "w3" cycles
//  T3 : T0

//considering the time that occupied by instructions between T0 and T1(T1 and T2, T2 and T3(T0))
//Note: we assume that the performance is 1 cpu cycle per instruction.
//      according to spec, this cpu has 1.25 DMIPS/MHz (Dhrystone 2.1) performance at 0 wait state memory access.
//      so theoretically less than 1 cpu cycle per instruction.
//      when cpu runs faster, there will be less affect to the timing, and the assumption is acceptable for timing
#define w1_single (ws2812_t1-2)
#define w2_single (ws2812_t2-ws2812_t1-2)
#define w3_single (ws2812_ctot-ws2812_t2-5)

#define w1_multi 9 //10//(ws2812_t1-10) //23
#define w2_multi 7 //8//(ws2812_t2-ws2812_t1-10)//52
#define w3_multi 4 //5//(ws2812_ctot-ws2812_t2-85) //63


#define ws2812_DEL1 nop
#define ws2812_DEL2 nop nop
#define ws2812_DEL4 ws2812_DEL2 ws2812_DEL2
#define ws2812_DEL8 ws2812_DEL4 ws2812_DEL4
#define ws2812_DEL16 ws2812_DEL8 ws2812_DEL8
#define ws2812_DEL32 ws2812_DEL16 ws2812_DEL16
#define ws2812_DEL64 ws2812_DEL32 ws2812_DEL32
#define ws2812_DEL128 ws2812_DEL64 ws2812_DEL64


///////////////////////////////////////////////////////////////////////
// User defined area: Define I/O pin
///////////////////////////////////////////////////////////////////////

// Data port and pin definition for your CPU. Depending on the way
// port access is implemented in your Cortex CPU, the set and clr
// mask and addresses may be the same. This example is for the
// NXP LPC81X
#ifdef  USE_HAL_DRIVER
#define PORTB_SET ((uint32_t*)&GPIOB->BSRR)
#define PORTB_CLR ((uint32_t*)(&GPIOB->BSRR + 2))
#else
#define PORTB_SET ((uint32_t*)&GPIOB->BSRRL)
#define PORTB_CLR ((uint32_t*)&GPIOB->BSRRH)
#endif

#define ws2812_port_set0 PORTB_SET	// Address of the data port register to set the pin
#define ws2812_port_clr0 PORTB_CLR	// Address of the data port register to clear the pin

#define ws2812_mask_set0  (1<<0)		// Bitmask to set the data out pin
#define ws2812_mask_clr0  (1<<0)		// Bitmask to clear the data out pin

#define ws2812_port_set1 PORTB_SET	// Address of the data port register to set the pin
#define ws2812_port_clr1 PORTB_CLR	// Address of the data port register to clear the pin

#define ws2812_mask_set1  (1<<1)		// Bitmask to set the data out pin
#define ws2812_mask_clr1  (1<<1)		// Bitmask to clear the data out pin

#define ws2812_port_set2 PORTB_SET	// Address of the data port register to set the pin
#define ws2812_port_clr2 PORTB_CLR	// Address of the data port register to clear the pin

#define ws2812_mask_set2  (1<<2)		// Bitmask to set the data out pin
#define ws2812_mask_clr2  (1<<2)		// Bitmask to clear the data out pin

#define ws2812_port_set3 PORTB_SET	// Address of the data port register to set the pin
#define ws2812_port_clr3 PORTB_CLR	// Address of the data port register to clear the pin

#define ws2812_mask_set3  (1<<3)		// Bitmask to set the data out pin
#define ws2812_mask_clr3  (1<<3)		// Bitmask to clear the data out pin

#define ws2812_port_set4 PORTB_SET	// Address of the data port register to set the pin
#define ws2812_port_clr4 PORTB_CLR	// Address of the data port register to clear the pin

#define ws2812_mask_set4  (1<<4)		// Bitmask to set the data out pin
#define ws2812_mask_clr4  (1<<4)		// Bitmask to clear the data out pin

#define ws2812_port_set5 PORTB_SET	// Address of the data port register to set the pin
#define ws2812_port_clr5 PORTB_CLR	// Address of the data port register to clear the pin

#define ws2812_mask_set5  (1<<5)		// Bitmask to set the data out pin
#define ws2812_mask_clr5  (1<<5)		// Bitmask to clear the data out pin

///////////////////////////////////////////////////////////////////////
// User defined area: Define CPU clock speed
//
// The current implementation of the sendarray routine uses cycle accurate
// active waiting. The routine is automatically adjusted according to
// the clockspeed defined below. Only values between 8 Mhz and 60 Mhz
// are allowable.
//
// Important: The timing calculation assumes that there are no waitstates
// for code memory access. If there are waitstates you may have to reduce
// the value below until you get acceptable timing. It is highly recommended
// to use this library only on devices without flash waitstates and
// predictable code execution timing.
//
///////////////////////////////////////////////////////////////////////

#define ws2812_cpuclk 168000000

///////////////////////////////////////////////////////////////////////
// End user defined area
///////////////////////////////////////////////////////////////////////

//#if (ws2812_cpuclk<8000000)
//	#error "Minimum clockspeed for ARM ws2812 library is 8 Mhz!"
//#endif

//#if (ws2812_cpuclk>60000000)
//	#error "Maximum clockspeed for ARM ws2812 library is 60 Mhz!"
//#endif

#define port_set_b    0x40020418        
#define port_clr_b    0x4002041A  

#define port_set_ch_1      port_set_b
#define port_clr_ch_1      port_clr_b
#define port_set_ch_2      port_set_b
#define port_clr_ch_2      port_clr_b
#define port_set_ch_3      port_set_b
#define port_clr_ch_3      port_clr_b
#define port_set_ch_4      port_set_b
#define port_clr_ch_4      port_clr_b
#define port_set_ch_5      port_set_b
#define port_clr_ch_5      port_clr_b
#define port_set_ch_6      port_set_b
#define port_clr_ch_6      port_clr_b

#define pin_mask_ch_1      0x01
#define pin_mask_ch_2      0x02
#define pin_mask_ch_3      0x04
#define pin_mask_ch_4      0x08
#define pin_mask_ch_5      0x10
#define pin_mask_ch_6      0x20
///////////////////////////////////////////////////////////////////////
// Main function call
//
// Call with address to led color array (order is Green-Red-Blue)
// Numer of bytes to be transmitted is leds*3
///////////////////////////////////////////////////////////////////////


#endif /* LIGHT_WS2812_H_ */
