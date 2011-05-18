/////////////////////////////////////////////////////////////////////////
////                                                                 ////
//// RR2_USB_Cdc_Monitor.c                                           ////
////                                                                 ////
/////////////////////////////////////////////////////////////////////////
#include <18F4550.h>
#device ADC=8
#fuses HSPLL,NOWDT,NOPROTECT,NOLVP,NODEBUG,USBDIV,PLL5,CPUDIV1,VREGEN
#use delay(clock=48000000)

#include ".\include\usb_cdc.h"

#use fast_io(b)
#use fast_io(d)
#use fast_io(e)

//variables
unsigned int8  an1=0;
unsigned int8  an2=0;
unsigned int8  an3=0;
unsigned int8  an4=0;

unsigned int8  i=0;
unsigned int8  j=0;
unsigned int16 k=0;
unsigned int8  counter=255;

void main()
{ 

   set_tris_d(0x00); 
   set_tris_e(0x00); //salidas mux
   set_tris_b(0x00); //salidas demux
   
   output_D(0x00);

   setup_adc (adc_clock_div_32); 
   setup_adc_ports (AN0_TO_AN3);
   
   usb_init_cs();
   do 
   { 
      //output_toggle(PIN_D0);
      usb_task();
      if (usb_enumerated()) 
      { 
         output_toggle(PIN_D2);
      
         if (usb_cdc_kbhit()) 
         { 
            i = toupper(usb_cdc_getc()); 
            if(i=='S')
            {
               usb_cdc_putc(255);
//!               counter=0;
//!               for(k=0;k<416;k++)
//!               {
//!                  if(counter>240)counter=240;
//!                  usb_cdc_putc(counter);
//!                  counter++;
//!               }
               
               for(i=0;i<13;i++)
               {
                  for(j=0;j<8;j++)
                  {
                     output_d(i);
                     output_b(i);   //direccion demux
                     output_e(j);   //direccion mux
                     
                     set_adc_channel (3);
                     delay_us(20);
                     an4=read_adc();
                     usb_cdc_putc(an4);
                     
                     set_adc_channel (2);
                     delay_us(20);
                     an3=read_adc();
                     usb_cdc_putc(an3);
                     
                     set_adc_channel (1);
                     delay_us(20);
                     an2=read_adc();
                     usb_cdc_putc(an2);
                                   
                     set_adc_channel (0);
                     delay_us(20);
                     an1=read_adc();
                     usb_cdc_putc(an1);
                     
                  }//for(j)
               }//for(i)
               usb_cdc_putc(254);
            }//if(i==S)            
         }//if (usb_cdc_kbhit()) 
      }//if (usb_enumerated()) 
   } while (TRUE); 
}
