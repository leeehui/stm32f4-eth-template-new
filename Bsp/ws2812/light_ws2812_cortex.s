


;function declaration
;void ws2812_send(uint8_t *data, uint8_t *data1,uint8_t *data2,uint8_t *data3,uint8_t *data4,uint8_t *data5,int datlen);
;see 
; light_ws2812_conf.h for timing analysis

;usage of registers
; r0 *data0, first parameter passed by caller directly with r0
; r1 *data1, second parameter passed by caller directly with r1
; r2 *data2, third parameter passed by caller directly with r2
; r3 *data3, forth parameter passed by caller directly with r3
; r4 *data4, fifth parameter passed by caller through Mem[stack+0]
; r5 *data5, sixth parameter passed by caller through Mem[stack+4]
; r6 datalen, fifth parameter passed by caller through Mem[stack+8]
; r7 tempData for [*data]
; r8 counter inside one byte, inited as 0x80, and right shift every byte_loop
; r9 saved flag for deciding if corresponding pin should be cleared at T1
; r10 port address for set/clear
; r11 bit mask for specific pin <see reference manual of stm32f4>


#include "light_ws2812_conf.h"
	//RSEG    CODE:CODE(2)
        ;; Forward declaration of sections.
	PUBLIC ws2812_send


        SECTION .text:CODE:REORDER:NOROOT(2)
        thumb
ws2812_send:
        stmfd sp,{r4-r11}
        ldr r4, [sp]          ;*data4
        ldr r5, [sp, #4]      ;*data5  
        ldr r6, [sp, #8]     ;len
        
        mov r8, #0x80           ;counter i
byte_loop:
        
        mov r9, #0              ;flag
        //mov32 r11, #0x40200418  ;use portB for all channels, do not need to set inside the loop
       
        //calculate flag
        ldr r7, [r0]            ;data0 Note: cannot use post-indexing mode "ldr r7, [r0], #1"
        tst r7, r8              ;because r0 can only be added outside byte_loop. in other words, r0 must be kept unchanged inside byte_loop
        it eq
        orreq r9, r9, #0x01
        
        ldr r7, [r1]            ;data1
        tst r7, r8
        it eq
        orreq r9, r9, #0x02
        
        ldr r7, [r2]            ;data2
        tst r7, r8
        it eq
        orreq r9, r9, #0x04
        
        ldr r7, [r3]            ;data3
        tst r7, r8
        it eq
        orreq r9, r9, #0x08

        ldr r7, [r4]            ;data4
        tst r7, r8
        it eq
        orreq r9, r9, #0x10
        
        ldr r7, [r5]            ;data5
        tst r7, r8
        it eq
        orreq r9, r9, #0x20
        
        //T0(T3) : set output high
        ldr r11, =port_set_ch_1  ;all channels use same port
        
        ldr r10, =pin_mask_ch_1
        str r10, [r11]
        
        //ldr r11, =port_set_ch_2
        ldr r10, =pin_mask_ch_2
        str r10, [r11]
        
        //ldr r11, =port_set_ch_2
        ldr r10, =pin_mask_ch_2
        str r10, [r11]

        //ldr r11, =port_set_ch_3
        ldr r10, =pin_mask_ch_3
        str r10, [r11]
        
        //ldr r11, =port_set_ch_4
        ldr r10, =pin_mask_ch_4
        str r10, [r11]
        
        //ldr r11, =port_set_ch_5
        ldr r10, =pin_mask_ch_5
        str r10, [r11]

        //ldr r11, =port_set_ch_6
        ldr r10, =pin_mask_ch_6
        str r10, [r11]
        nop
        nop
        nop
        nop
        nop
        
        nop
        nop
        nop
        nop
        nop
        
        nop
        nop
        nop
        nop
        nop

        nop
        nop
        nop
        nop
        nop

        nop
        nop
        nop
        nop
        nop

        nop
        nop
        nop
        nop
        nop
        
        nop
        nop
        nop
        nop
        nop


/* 
        ldr r12, =w1_multi
delay_1:
        subs r12, r12, #1
        bne delay_1
*/
/*
#if (w1_multi&1)                     //nop delay
			ws2812_DEL1
#endif
#if (w1_multi&2)
			ws2812_DEL2
#endif
#if (w1_multi&4)
			ws2812_DEL4
#endif
#if (w1_multi&8)
			ws2812_DEL8
#endif
#if (w1_multi&16)
			ws2812_DEL16
#endif
#if (w1_multi&32)
			ws2812_DEL32
#endif
#if (w1_multi&64)
			ws2812_DEL64
#endif
#if (w1_multi&128)
			ws2812_DEL128
#endif
*/
        //T1: conditionally set 0
        ldr r11, =port_clr_ch_1     ;all channels use same port
        
        tst r9, #0x01
        itt ne 
        ldrne r10, =pin_mask_ch_1
        strne r10, [r11]
        
        tst r9, #0x02
        itt ne 
        ldrne r10, =pin_mask_ch_2
        strne r10, [r11]

        tst r9, #0x04
        itt ne 
        ldrne r10, =pin_mask_ch_3
        strne r10, [r11]

        tst r9, #0x08
        itt ne 
        ldrne r10, =pin_mask_ch_4
        strne r10, [r11]
        
        tst r9, #0x10
        itt ne 
        ldrne r10, =pin_mask_ch_5
        strne r10, [r11]

        tst r9, #0x20
        itt ne 
        ldrne r10, =pin_mask_ch_6
        strne r10, [r11]
        
        nop
        nop
        nop
        nop
        nop
        
        nop
        nop
        nop
        nop
        nop

        nop
        nop
        nop

 /*
        ldr r12, =w2_multi
delay_2:
        subs r12, r12, #1
        bne delay_2
*/
/*
#if (w2_multi&1)                            //nop delay
			ws2812_DEL1
#endif
#if (w2_multi&2)
			ws2812_DEL2
#endif
#if (w2_multi&4)
			ws2812_DEL4
#endif
#if (w2_multi&8)
			ws2812_DEL8
#endif
#if (w2_multi&16)
			ws2812_DEL16
#endif
#if (w2_multi&32)
			ws2812_DEL32
#endif
#if (w2_multi&64)
			ws2812_DEL64
#endif
#if (w2_multi&128)
			ws2812_DEL128
#endif
*/
      
        //T2 : unconditionally set output low
        ldr r11, =port_clr_ch_1  ;all channels use same port
        
        ldr r10, =pin_mask_ch_1
        str r10, [r11]
        
        //ldr r11, =port_set_ch_2
        ldr r10, =pin_mask_ch_2
        str r10, [r11]
        
        //ldr r11, =port_set_ch_2
        ldr r10, =pin_mask_ch_2
        str r10, [r11]

        //ldr r11, =port_set_ch_3
        ldr r10, =pin_mask_ch_3
        str r10, [r11]
        
        //ldr r11, =port_set_ch_4
        ldr r10, =pin_mask_ch_4
        str r10, [r11]
        
        //ldr r11, =port_set_ch_5
        ldr r10, =pin_mask_ch_5
        str r10, [r11]

        //ldr r11, =port_set_ch_6
        ldr r10, =pin_mask_ch_6
        str r10, [r11]

        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop


/*
        ldr r12, =w3_multi
delay_3:
        subs r12, r12, #1
        bne delay_3
*/
/*  
#if (w3_multi&1)
			ws2812_DEL1
#endif
#if (w3_multi&2)
			ws2812_DEL2
#endif
#if (w3_multi&4)
			ws2812_DEL4
#endif
#if (w3_multi&8)
			ws2812_DEL8
#endif
#if (w3_multi&16)
			ws2812_DEL16
#endif
#if (w3_multi&32)
			ws2812_DEL32
#endif
#if (w3_multi&64)
			ws2812_DEL64
#endif
#if (w3_multi&128)
			ws2812_DEL128
#endif
*/  
        lsrs r8, r8, #1
        beq byte_end
        b byte_loop
byte_end:
        subs r6, r6, #1
        beq all_end
        add r0, r0, #1          ;
        add r1, r1, #1
        add r2, r2, #1
        add r3, r3, #1
        add r4, r4, #1
        add r5, r5, #1
        mov r8, #0x80           ;counter i
        b byte_loop
all_end:    
        sub r0, sp, #32
        ldmfd r0,{r4-r11}
        mov pc, lr
        END