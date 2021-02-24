/*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$$-------Project:       PS2 KEYBOARD HOST----------------------------$$
$$-------Description:   Bidirectional communication with ps2 keyboard---------'$$
$$-------Protocol:      Software-based---------------------------------$$
$$-------Platform:     ATMEL AVR MCU --------------------------------$$
$$-------Hardware:    ATMEGA328P-----------------------------------$$
$$-------Software:     C Language-------------------------------------$$
$$-------Compiler:      Imagecraft C Compiler for AVR (ICCAVR) version 8-----'$$
$$----------------------------------------------------------------$$
$$----------------------------------------------------------------$$
$$-------Author:       Engr Sunday Ezeilo (OND, B.Eng)--------------------'$$
$$-------Company:     zeilotech integrated systems------------------------$$
$$-------Email:         chezsunnez@gmail.com-----------------------------$$
$$-------Phone:        2348064717555, 2348088974499-------------------$$
$$-------Release:      December, 2016---------------------------------$$
$$-------License:      Free-------------------------------------------$$
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/

#include<iom328pv.h>
#include<stdio.h>
#include<stdlib.h>
#include<macros.h>
#include<string.h>
#include<ctype.h>
#include<stdint.h>
//**********************************************************************************************
//**********************************************************************************************
#define setbit(x, y)    (x|=1<<y)
#define clearbit(x, y)  (x&=~(1<<y))
#define checkbit(x, y)  (x&(1<<y))
#define toggleBit(x,y)    (x^=(1<<y))
//**********************************************************************************************
//**********************************************************************************************
#define F_CPU (uint32_t)20000000UL
//**********************************************************************************************
//**********************************************************************************************
#define T0ClkSel (uint8_t)4UL		   //prescaling of 256 at 20 MHz used!!!
#define T0Prescaler (uint16_t)256UL
#define T0CompVal (uint8_t)((F_CPU/T0Prescaler)*0.001)	 //1 msec timebase here

#define T1Clock (uint8_t)1UL		   //prescaling of 8 used!!!
#define T1Prescaler (uint8_t)1UL
#define T1CompVal (uint16_t)((F_CPU/T1Prescaler)*0.000001)	 //1 u sec timebase here
//**********************************************************************************************
//**********************************************************************************************
#pragma interrupt_handler INT0_isr: iv_INT0
#pragma interrupt_handler INT1_isr: iv_INT1
#pragma interrupt_handler PCINT0_isr: iv_PCINT0
#pragma interrupt_handler PCINT1_isr: iv_PCINT1
#pragma interrupt_handler PCINT2_isr: iv_PCINT2
#pragma interrupt_handler wdt_isr: iv_WDT
#pragma interrupt_handler timer2_compa_isr: iv_TIMER2_COMPA
#pragma interrupt_handler timer2_compb_isr: iv_TIMER2_COMPB
#pragma interrupt_handler timer2_ovf_isr: iv_TIMER2_OVF
#pragma interrupt_handler timer1_capt_isr: iv_TIMER1_CAPT
#pragma interrupt_handler timer1_compa_isr: iv_TIMER1_COMPA
#pragma interrupt_handler timer1_compb_isr: iv_TIMER1_COMPB
#pragma interrupt_handler timer1_ovf_isr: iv_TIMER1_OVF
#pragma interrupt_handler timer0_compa_isr: iv_TIMER0_COMPA
#pragma interrupt_handler timer0_compb_isr: iv_TIMER0_COMPB
#pragma interrupt_handler timer0_ovf_isr: iv_TIMER0_OVF
#pragma interrupt_handler spi_stc_isr: iv_SPI_STC
#pragma interrupt_handler usart_rxc_isr: iv_USART0_RX
#pragma interrupt_handler usart_udre_isr: iv_USART0_UDRE
#pragma interrupt_handler usart_txc_isr: iv_USART0_TX
#pragma interrupt_handler adc_isr: iv_ADC
#pragma interrupt_handler ee_ready_isr: iv_EE_READY
#pragma interrupt_handler analog_comp_isr: iv_ANALOG_COMP
#pragma interrupt_handler twi_isr: iv_TWI
#pragma interrupt_handler spm_rdy_isr: iv_SPM_READY
//**********************************************************************************************
//**********************************************************************************************
//void INT0_isr(void){}
//void INT1_isr(void){}
void PCINT0_isr(void){}
void PCINT1_isr(void){}
void PCINT2_isr(void){}
void wdt_isr(void){}
void timer2_compa_isr(void){}
void timer2_compb_isr(void){}
void timer2_ovf_isr(void){}
void timer1_capt_isr(void){}
void timer1_compa_isr(void){}
void timer1_compb_isr(void){}
void timer1_ovf_isr(void){}
//void timer0_compa_isr(void){}
void timer0_compb_isr(void){}
void timer0_ovf_isr(void){}
void spi_stc_isr(void){}
//void usart_rxc_isr(void){}
void usart_udre_isr(void){}
void usart_txc_isr(void){}
void adc_isr(void){}
void ee_ready_isr(void){}
void analog_comp_isr(void){}
void twi_isr(void){}
void spm_rdy_isr(void){}
//**********************************************************************************************
//**********************************************************************************************
void writeUart(volatile uint8_t c)	   //insert delay here
{
	while(!(UCSR0A&(1<<UDRE0)));
	UDR0=c;
	while(!(UCSR0A&(1<<TXC0)));	  //wait until last bit sent out here for mux switchover
	UCSR0A|=1<<TXC0;
}
//************************
//**************************
/*void writeTextUart(uint8_t *s)
{
 	while(*s)writeUart(*s++);
}*/
//**************************
//**************************
/*void debug(const char *s)
{
 	while(*s)writeUart(*s++);
}*/
//**************************
//**************************
#define rxdPort PORTD
#define rxdPin PIND
#define rxdDDR DDRD
#define rxd PD0
//**************************
//**************************
void uartInit(uint32_t baudRate)
{
 	UCSR0B &=~(1<<RXCIE0);
	rxdPort|=1<<rxd;	  	 		  					    //pull this pin up here for load
	rxdDDR&=~(1<<rxd);
	baudRate=((F_CPU/baudRate)/8)-1;  		  		//replace ((F_CPU/baudRate)/16)-1; for non-double speed mode
	UBRR0H=baudRate>>8;
 	UBRR0L=baudRate;
 	UCSR0A|=(1<<U2X0);  // set double speed mode. delete this line if double speed mode is not desired
	UCSR0B=(1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
	UCSR0C=(3<<UCSZ00);
}
//**********************************************************************************************
//**********************************************************************************************
void initTimer0(void)
{
 	 TIFR0|=1<<OCF0A;
	 TCCR0B&=~0x07;
	 TCCR0A=0x00;
	 TCCR0A|=1<<WGM01;
	 TCNT0=0x00;
	 OCR0A=T0CompVal;
	 TIMSK0|=(1<<OCIE0A);
	 TCCR0B|=T0ClkSel;
}

void initTimer1(void)
{
 	 TIFR1|=(1<<TOV1)|(1<<OCF1A);
	 TCCR1B&=~0x07;
	 TCCR1A=0x00;
	 TCNT1=0x0000;
	 //TIMSK1|=(1<<TOIE1);
	 TCCR1B|=T1Clock;
}
/*void watchDogDisable(void)
{
	   WDTCSR|=(1<<WDCE)|(1<<WDE);
       WDTCSR=0x00;
}

void watchDogEnable(void)
{
	   MCUSR&=~(1<<WDRF);
  	   WDTCSR=0x00;
  	   WDTCSR=(1<<WDE)|(1<<WDP2)|(1<<WDP1);		 	   		 	   //timeout of 1 sec chosen
}*/
//**********************************************************************************************
//**********************************************************************************************
uint8_t resetKb(void);
uint8_t detectKb(void);
void delay_us(uint16_t);
void delay_ms(uint16_t);
uint8_t scanKey(uint8_t);
uint8_t checkFrame(void);
uint8_t writeKeyboard(void);
uint8_t getOddParity(uint8_t);
uint8_t checkOddParity(uint16_t);
//**********************************************************************************************
//**********************************************************************************************
////////////////////////////////////////////
         //   control keys scancodes   //
////////////////////////////////////////////
#define CAPS 0x58
#define R_SHIFT 0x59
#define L_SHIFT 0x12
#define NUM_LOCK 0x77
#define KEY_UP 0xF0
#define EXTD_KEY 0xE0
////////////////////////////////////////////
    //   Function Keys return values   //
////////////////////////////////////////////
#define HOME (uint8_t)11UL
#define ENTER '\n' //(uint8_t)10UL //LF = 0x0A
#define BACKSPACE '\b'//(uint8_t)8UL
#define TAB '\t'    //(uint8_t)9UL //horizontal tab = 0x09
#define L_CTRL (uint8_t)18UL
#define R_CTRL (uint8_t)19UL
#define L_ALT (uint8_t)20UL
#define R_ALT (uint8_t)21UL
#define SCROLL_LOCK (uint8_t)22UL
#define INS (uint8_t)23UL
#define UP_ARROW (uint8_t)24UL
#define DOWN_ARROW (uint8_t)25UL
#define R_ARROW (uint8_t)26UL
#define ESC (uint8_t)27UL
#define L_ARROW (uint8_t)28UL
#define DEL (uint8_t)127UL

#define F1 (uint8_t)128UL
#define F2 (uint8_t)129UL
#define F3 (uint8_t)130UL
#define F4 (uint8_t)131UL
#define F5 (uint8_t)132UL
#define F6 (uint8_t)133UL
#define F7 (uint8_t)134UL
#define F8 (uint8_t)135UL
#define F9 (uint8_t)136UL
#define F10 (uint8_t)137UL
#define F11 (uint8_t)138UL
#define F12 (uint8_t)139UL
#define GUI (uint8_t)140UL      //Windows Logo key
#define PG_UP (uint8_t)141UL
#define PG_DN (uint8_t)142UL
#define END (uint8_t)143UL
#define PAUSE_BREAK (uint8_t)144UL
#define PRINT_SCREEN (uint8_t)145UL
#define APPS (uint8_t)146UL
////////////////////////////////////////////
    //   End of Function Keys return values   //
////////////////////////////////////////////
////////////////////////////////////////////
       //   Keyboard-To-Host commands   //
////////////////////////////////////////////
#define KB_ACK 0xFA
#define ERROR_1 0x00
#define ERROR_2 0xFF
#define ECHO_RESPONSE 0xEE
#define LED_STATUS_CMD 0xED
#define SELF_TEST_PASSED 0xAA
#define RESEND_REQUEST 0xFE
////////////////////////////////////////////
       //    Host-To-Keyboard commands   //
////////////////////////////////////////////
#define SCROLL_LOCK_LED_CTRL_BIT 0x00
#define NUM_LOCK_LED_CTRL_BIT 0x01
#define CAPS_LOCK_LED_CTRL_BIT 0x02
////////////////////////////////////////////
       //    End of Keyboard commands   //
////////////////////////////////////////////
//**********************************************************************************************
//**********************************************************************************************
const uint8_t shiftedNumbers[]={')','!','@','#','$','%','^','&','*','('};
const uint8_t shiftedSymbols[]={'<','>','?',':','"','{','}','_','+','|','~',' '}; //replace '?' with '\?' if escape sequence is required forsome compilers
const uint8_t asciiSymbols[]={',','.','/',';','\'','[',']','-','=','\\','`',' '};
const uint8_t symbolsScancodes[]={0x41,0x49,0x4A,0x4C,0x52,0x54,0x5B,0x4E,0x55,0x5D,0x0E,0x29};
const uint8_t alphaScancodes[]={
    0x1C, 0x32, 0x21, 0x23, 0x24, 0x2B, 0x34, 0x33, 0x43,
    0x3B, 0x42, 0x4B, 0x3A, 0x31, 0x44, 0x4D, 0x15, 0x2D,
    0x1B, 0x2C, 0x3C, 0x2A, 0x1D, 0x22, 0x35, 0x1A
};
const uint8_t numbersScancodes[]={0x45,0x16, 0x1E, 0x26, 0x25, 0x2E, 0x36, 0x3D, 0x3E,0x46};
const uint8_t numKeysScancodes[]={0x70, 0x69, 0x72, 0x7A, 0x6B, 0x73, 0x74, 0x6C, 0x75, 0x7D};
//**********************************************************************************************
//**********************************************************************************************
struct{
    uint8_t rightShift,leftShift,scancode,writeStatus,keyBreak,reset,busy,
	extdKey,ledCmd,numLock,capsLock,scrollLock,noRepeat,extKeyUp,detect;
	volatile uint8_t startBit,frameComplete,comMode;
	volatile uint16_t dataFrame,tempCmd,cmdWord;
}kb;
//**********************************************************************************************
//**********************************************************************************************
#define KB_TO_HOST (uint8_t)2UL
#define HOST_TO_KB (uint8_t)3UL

#define writeSuccess (uint8_t)0UL
#define writeFail (uint8_t)1UL
//**********************************************************************************************
//**********************************************************************************************
#define KB_PORT PORTD
#define KB_DDR DDRD
#define KB_PIN PIND
#define KB_CLK PD2
#define KB_DT PD3
//**********************************************************************************************
//**********************************************************************************************
 void resetKBVars(void)
{
    kb.busy=0;kb.detect=0; kb.ledCmd=0; kb.startBit=0;
	kb.extdKey=0; kb.numLock=0;	kb.cmdWord=0; kb.capsLock=0;
	kb.leftShift=0; kb.noRepeat=0; kb.keyBreak=0; kb.scancode=0;
    kb.scrollLock=0; kb.rightShift=0; kb.dataFrame=0; kb.frameComplete=0;
    kb.writeStatus=writeFail; kb.comMode=KB_TO_HOST;
}
//**********************************************************************************************
//**********************************************************************************************
void initExtInt(void)
{
	//KB_PORT|=(1<<KB_CLK)|(1<<KB_DT);    //keep in idle state      //pull-up enabled on clock I/O, though keyboards are provided with internal pull-up
	//KB_DDR&=~((1<<KB_CLK)|(1<<KB_DT));  //DDRD, PD2, PD3      //clock I/O is always input, since the keyboard generates clock in all data transaction
    EIFR|=(1<<INTF0)|(1<<INTF1);
    EICRA=0x00;
    EICRA|=(1<<ISC01)|(1<<ISC11);				 		  //enable detect zero cross at falling edge
    EIMSK=0x00;
    EIMSK|=(1<<INT0)|(1<<INT1);
}
//**********************************************************************************************
//**********************************************************************************************
uint8_t scanKey(uint8_t i)
{
    uint8_t ctr=0;
	if(kb.extdKey)
	{
	    switch(i)
		{
			case 0x6B: ctr=L_ARROW;break;
			case 0x74: ctr=R_ARROW;break;
			case 0x75: ctr=UP_ARROW;break;
			case 0x72: ctr=DOWN_ARROW;break;
			case 0x70: ctr=INS;break;
			case 0x6C: ctr=HOME;break;
			case 0x7D: ctr=PG_UP;break;
			case 0x7A: ctr=PG_DN;break;
			case 0x71: ctr=DEL;break;
			case 0x69: ctr=END;break;
			case 0x5A: ctr=ENTER;break;
			case 0x4A: ctr='/';break;
			case 0x2F:  if(!kb.noRepeat){kb.noRepeat=1;ctr=APPS;}//avoid APPS key repeating when held down, so LED doesn't flicker
                break;
            case 0x11:  if(!kb.noRepeat){kb.noRepeat=1;ctr=R_ALT;}//avoid ALT key repeating when held down, so LED doesn't flicker
                break;
            case 0x14:  if(!kb.noRepeat){kb.noRepeat=1;ctr=R_CTRL;}//avoid Ctrl key repeating when held down, so LED doesn't flicker
                break;
            default:	  break;
        }
        return ctr;
    }
    if(kb.numLock)  //note that this loop must come before the following 'switch' block, to ensure that
    {                  //Num Lock takes priority over the 2nd function of number keys (ie. when the Num Lock is off)
        if(i==0x71)return '.';
        for(ctr=0;ctr<10;ctr++)
        {
            if(i==numKeysScancodes[ctr])return(ctr+'0');
        }
    }
    switch(i)
    {//scan for 2nd function of Num keys, not including "Enter' and '/' keys which have extended scan codes.
        case 0x7C: return '*';      //all keys with extended scan codes have been taken care of in the 'switch' block above
		case 0x7B: return '-';
		case 0x79: return '+';
	    case 0x71: return DEL;
	    case 0x70: return INS;
	    case 0x69: return END;
	    case 0x72: return DOWN_ARROW;
	    case 0x7A: return PG_DN;
	    case 0x6B: return L_ARROW;
	    case 0x74: return R_ARROW;
	    case 0x6C: return HOME;
	    case 0x75: return UP_ARROW;
	    case 0x7D: return PG_UP;
		default: break;
    }
    switch (i)
	{
	    case 0x0D: return TAB;
	    case 0x76: return ESC;
	    case 0x05: return F1;
	    case 0x06: return F2;
	    case 0x04: return F3;
	    case 0x0C: return F4;
	    case 0x03: return F5;
	    case 0x0B: return F6;
	    case 0x83: return F7;
	    case 0x0A: return F8;
	    case 0x01: return F9;
	    case 0x09: return F10;
	    case 0x78: return F11;
	    case 0x07: return F12;
	    case 0x5A: return ENTER;
		case 0x66: return BACKSPACE;
		case 0x11:  if(!kb.noRepeat){kb.noRepeat=1;return L_ALT;}  //avoid ALT key repeating when held down, so LED doesn't flicker
            break;
	    case 0x14:  if(!kb.noRepeat){kb.noRepeat=1;return L_CTRL;} //avoid Ctrl key repeating when held down, so LED doesn't flicker
            break;
	    default:
	        for(ctr=0;ctr<26;ctr++)
            {
                if(i==(alphaScancodes[ctr]))
                {
                    if(kb.capsLock)
                  	{
                  	    if(kb.rightShift||kb.leftShift)return(ctr+'a');
                        return (ctr+'A');
                  	}
                    else if(kb.rightShift||kb.leftShift)return(ctr+'A');
                    return (ctr+'a');
                }
            }
            for(ctr=0;ctr<10;ctr++)
            {
                if(i==numbersScancodes[ctr])
		    	{
		    	    if(kb.rightShift||kb.leftShift)return(shiftedNumbers[ctr]);
		         	return (ctr+'0');
		    	}
            }
            for(ctr=0;ctr<12;ctr++)
            {
                if(i==symbolsScancodes[ctr])
		    	{
		    	    if(kb.rightShift||kb.leftShift)return(shiftedSymbols[ctr]);
			     	return (asciiSymbols[ctr]);
		    	}
            }
            break;
    }
 	return 0;
}
//**********************************************************************************************
//**********************************************************************************************
uint8_t readKey(uint8_t scancode)
{
    static uint8_t pauseBreakCnt=0, printScreenCnt=0,printScreenUp=0;
	uint8_t c=0;
    if(scancode==0xE1){pauseBreakCnt++; return 0;}
    if(pauseBreakCnt)
    {
        switch(scancode)
        {
            case 0x14: pauseBreakCnt++; break;
            case 0x77: pauseBreakCnt++;
                if(pauseBreakCnt>=8)
                {
                    pauseBreakCnt=0;
                    return PAUSE_BREAK;
                }
                break;
			case 0xF0:  pauseBreakCnt++; break;
			default: break;
        }
        return 0;
    }
    if((!printScreenUp)&&(printScreenCnt))
    {
        switch(scancode)
        {
            case 0x12: printScreenCnt++; return 0;
			case 0x7C: printScreenCnt++;
                if(printScreenCnt>=4)
                {
                    printScreenCnt=0;
                    kb.extdKey=0;
                    return PRINT_SCREEN;
                }
                return 0;
			default: break;
        }
    }
    if(printScreenCnt)
	{
	    switch(scancode)
	  	{
	  	    case 0xF0:  printScreenCnt++;
                           printScreenUp=1;
                           kb.keyBreak=1;
                           kb.noRepeat=0;return 0;		//still acknowledge general key-up
			case 0x7C:  printScreenCnt++; return 0;
            case 0x12:  printScreenCnt++;
                if(printScreenCnt>=6)
                {
                    kb.extdKey=0;
                    kb.keyBreak=0;
                    printScreenUp=0;
                    printScreenCnt=0;
                    printScreenUp=1;
                }
                return 0;
			default:    break;
        }
    }
    switch(scancode)
	{
	    case EXTD_KEY: printScreenCnt++;kb.extdKey=1;printScreenUp=0;
            if(kb.keyBreak){kb.keyBreak=0;}
            break;
        case KEY_UP: kb.keyBreak=1; kb.noRepeat=0;
            printScreenUp=1; break;
        case CAPS: if(kb.keyBreak){kb.keyBreak=0;break;}
            if(!kb.noRepeat)        //avoid CAPS_LOCK repeating when held down, so LED doesn't flicker
            {
                kb.capsLock^=0x01;
                kb.noRepeat=1;
                kb.ledCmd^=(1<<CAPS_LOCK_LED_CTRL_BIT);    //toggle bit 2 (CAPS LOCK) of status LED
                return HOST_TO_KB;
            }
            break;
        case R_SHIFT: if(kb.keyBreak){kb.keyBreak=0; kb.rightShift=0; break;}
            kb.rightShift=1; break;
        case L_SHIFT: if(kb.keyBreak){kb.keyBreak=0; kb.leftShift=0; break;}
            kb.leftShift=1; break;
		case NUM_LOCK: if(kb.keyBreak){kb.keyBreak=0;break;}
            if(!kb.noRepeat)//avoid NUM_LOCK repeating when held down, so LED doesn't flicker
            {
                kb.numLock^=0x01;
                kb.noRepeat=1;
                kb.ledCmd^=(1<<NUM_LOCK_LED_CTRL_BIT);    //toggle bit 1 (NUM LOCK) of status LED
                return HOST_TO_KB;
            }
            break;
		case 0x7E: if(kb.keyBreak){kb.keyBreak=0;break;}
            if(!kb.noRepeat)//avoid SCROLL_LOCK repeating when held down, so LED doesn't flicker
            {
                kb.scrollLock^=0x01;
                kb.noRepeat=1;
                kb.ledCmd^=(1<<SCROLL_LOCK_LED_CTRL_BIT);    //toggle bit 0 (SCROLL LOCK) of status LED
                return SCROLL_LOCK;
            }
            break;
		default:
		    if(kb.keyBreak&&kb.extdKey)//key up is set again in the 'if statement' for printScreenCnt above, even if it is cleared on key up
            {
                c=0;printScreenCnt=0;printScreenUp=0;kb.extdKey=0;kb.keyBreak=0;
                switch(scancode)
                {//return GUI (Windows Logo) key on key release
                    case 0x1F: c=GUI;break; //1F=left GUI
                    case 0x27: c=GUI;break; //27=right GUI
                    default:    break;
                }
                return c;
            }
            if(kb.keyBreak){kb.keyBreak=0;printScreenCnt=0;printScreenUp=0;kb.extdKey=0;break;}//discard any other key returned on key up
            if((c=scanKey(scancode))){kb.extdKey=0;printScreenCnt=0;printScreenUp=0;break;}
            break;
    }
	return c;
}
//**********************************************************************************************
//**********************************************************************************************
#define KB_MAX_BIT_TIME (uint16_t)(65*T1CompVal) //65us
#define HOST2KB_START_COND (uint16_t)60UL //65 us
uint8_t writeKeyboard(void)
{
    kb.writeStatus=writeFail;
    kb.tempCmd=kb.cmdWord;
    if(!(getOddParity(kb.tempCmd)))kb.tempCmd|=0x100;    //append parity bit
    kb.tempCmd|=0x200;    //append stop bit (bit 9)
    kb.busy=1;
    kb.comMode=HOST_TO_KB;
    EIMSK&=~((1<<INT0)|(1<<INT1));  //disable EXT INT
    KB_PORT|=(1<<KB_DT);
    KB_PORT&=~(1<<KB_CLK);  //pull clock low for at least 65 us
    KB_DDR|=(1<<KB_CLK)|(1<<KB_DT);
    delay_us(HOST2KB_START_COND);
    KB_PORT&=~(1<<KB_DT);   //pull data low
    delay_us(5);
    KB_PORT|=(1<<KB_CLK); //release the clock line
    KB_DDR&=~(1<<KB_CLK);
    delay_us(5);
    EIFR|=(1<<INTF0);EICRA|=(1<<ISC01);EIMSK|=(1<<INT0);
    kb.startBit=0;
    TCNT1=0;
    while((!kb.startBit)&&((TCNT1<(1000*T1CompVal))));  //1 ms timeout
    if(!kb.startBit)goto exit;
    EIFR|=(1<<INTF0);
    EICRA|=(1<<ISC01)|(1<<ISC00);   //change INT0 to rising edge
    TCNT1=0;
    kb.frameComplete=0;
    while((!kb.frameComplete)&&(TCNT1<(1500*T1CompVal)));    //1.5 ms timeout
    if(!kb.frameComplete){goto exit;}
    TCNT1=0;
    KB_PORT|=(1<<KB_CLK);
    KB_DDR&=~(1<<KB_DT);    //change DT to input read the ack bit from the keyboard
    delay_us(5);
    while((KB_PIN&(1<<KB_DT))&&(TCNT1<KB_MAX_BIT_TIME));
    if(TCNT1<KB_MAX_BIT_TIME)kb.writeStatus=writeSuccess;
    exit:
        KB_PORT|=(1<<KB_DT);    //release data line upon receiving ack
        KB_DDR|=(1<<KB_DT);
        kb.comMode=KB_TO_HOST;
        initExtInt();   //reset INT
        kb.startBit=0;
        kb.busy=0;
        return(kb.writeStatus);
}
//**********************************************************************************************
//**********************************************************************************************
uint8_t resetKb(void)
{
    uint8_t c,tryWrite=0;
    kb.cmdWord=0xFF;   //host-to-keyboard reset command
    while(tryWrite<3)
    {
        if(!(c=writeKeyboard()))break;
        tryWrite++;
    }
    return c;
}
//**********************************************************************************************
//**********************************************************************************************
uint8_t toggleLed(void)
{
    uint8_t c,tryWrite=0;
    kb.cmdWord=LED_STATUS_CMD;
    while(tryWrite<3)
    {
        if(!(c=writeKeyboard()))break;
        tryWrite++;
    }
    if(!c)
    {
        TCNT1=0;
        kb.frameComplete=0;
        kb.comMode=KB_TO_HOST;
        while((!kb.frameComplete)&&(TCNT1<(2000*T1CompVal))); //wait response from KB for 2 ms max
        if((kb.frameComplete)&&((kb.dataFrame&0xff)==KB_ACK))
        {
            kb.comMode=HOST_TO_KB;
            kb.cmdWord=kb.ledCmd;
            return(kb.writeStatus=writeKeyboard());
        }
    }
    return c;
}
//**********************************************************************************************
//**********************************************************************************************
uint8_t detectKb(void)
{
    uint8_t c,tryWrite=0;
    kb.cmdWord=ECHO_RESPONSE;   //command byte to get echo from the keyboard
    while(tryWrite<3)
    {
        if(!(c=writeKeyboard()))break;
        tryWrite++;
    }
    if(!c)
    {
        TCNT1=0;
        kb.frameComplete=0;
        kb.comMode=KB_TO_HOST;
        while((!kb.frameComplete)&&(TCNT1<(2000*T1CompVal))); //wait response from KB for 2 ms max
        if((kb.frameComplete)&&((kb.dataFrame&0xff)==ECHO_RESPONSE))return 0;
    }
    return 1;
}
//**********************************************************************************************
//**********************************************************************************************
void getKb(void)
{
    static uint8_t c;
    while(1)
    {
        if(!kb.reset){resetKb();kb.reset=1;}
        if(kb.frameComplete)
        {
            kb.frameComplete=0;
            if((!(kb.dataFrame&0x200))||(checkOddParity(kb.dataFrame&0x1ff)))
            {
                kb.dataFrame=0;
            }else{
                kb.scancode=kb.dataFrame;
                kb.dataFrame=0;
                if((kb.scancode==ERROR_1)||(kb.scancode==ERROR_2)||(kb.scancode==SELF_TEST_PASSED))resetKBVars();
                //else if(kb.scancode==SELF_TEST_PASSED)debug("keyboard detected\r");
                else{
                    c=readKey(kb.scancode);
                    if((c==HOST_TO_KB)||(c==SCROLL_LOCK))
                    {
                        kb.comMode=HOST_TO_KB;
                        if(c==SCROLL_LOCK)writeUart(c);
                        toggleLed();
                    }else{if(c)writeUart(c);}
                }
            }
        }
        if((!kb.busy)&&(kb.detect))
        {
            kb.detect=0;
            if(!detectKb())writeUart(ECHO_RESPONSE);//send 'keyboard presence' to the user
            else{writeUart(ERROR_2);}   //send 'keyboard' absence to the user
        }
        //user code can be placed here, if the same chip is to be used for other applications.
        //However, it should be noted that INT0, INT1 and TIMER 1 are dedicated to keyboard routines
    }
}
//**********************************************************************************************
//**********************************************************************************************
volatile uint8_t msTick=0,sysReady=0;
int main(void)
{
	resetKBVars();
    kb.reset=0;
	uartInit(57600);    //57.6 kb/s
	sysReady=1;
	initTimer1();
	//initTimer0();
	initExtInt();
	asm("sei");
	//watchDogEnable();
	while(1){getKb();}
	return 0;
}
//**********************************************************************************************
//**********************************************************************************************
void delay_us(uint16_t t)
{
    while(t)
	{
	    TCNT1=0x00;
		while(TCNT1<T1CompVal);
		t--;
	}
}
//**********************************************************************************************
//**********************************************************************************************
void delay_ms(uint16_t dly)
{
 	while(dly)
	{
	    msTick=0;
		while(!msTick);
		dly--;
	}
}
//**********************************************************************************************
//**********************************************************************************************
void timer0_compa_isr(void)
{
    //volatile static uint16_t cnt=0;
    asm("wdr");
    msTick=1;
    //if(++cnt>=5000){cnt=0;kb.detect=1;}
}
//**********************************************************************************************
//**********************************************************************************************
uint8_t checkOddParity(uint16_t i)
{
    uint8_t ones=0,bitCnt;
    for(bitCnt=0;bitCnt<9;bitCnt++)
    {
        if(i&0x01)ones++;
        i>>=1;
    }
    if(ones%2)return 0; //return 0 if there is odd number of ones (odd parity) in the word
    return 1;
}
//**********************************************************************************************
//**********************************************************************************************
uint8_t getOddParity(uint8_t i)
{
    uint8_t ones=0,bitCnt;
    for(bitCnt=0;bitCnt<8;bitCnt++)
	{
	    if(i&0x01)ones++;
	    i>>=1;
	}
	if(!(ones%2))return 0;
	return 1;
}
//**********************************************************************************************
//**********************************************************************************************
void INT0_isr(void)
{
    static volatile uint16_t dataFrame=0;
    static volatile uint8_t bitCnt=0;
    volatile uint8_t sreg;
    //watchDogDisable();
    sreg=SREG;
    asm("cli");
    if(kb.comMode==KB_TO_HOST)
    {
        if((!sysReady)||(!kb.startBit))goto exitIsr;
        dataFrame>>=1;
        if(KB_PIN&(1<<KB_DT))dataFrame|=0x200;
        if(++bitCnt>9)
        {
            bitCnt=0; kb.startBit=0;
            kb.dataFrame=dataFrame;
            dataFrame=0;
            kb.frameComplete=1;
            EIFR|=(1<<INTF1);
            EIMSK|=(1<<INT1);   //re-enable INT1
            kb.busy=0;
        }
    }else if(kb.comMode==HOST_TO_KB){
        if(!kb.startBit){bitCnt=0;kb.startBit=1;}
        else
        {
            if(++bitCnt<11)
            {
                if(kb.tempCmd&0x01)KB_PORT|=(1<<KB_DT);
                else{KB_PORT&=~(1<<KB_DT);}
                kb.tempCmd>>=1;
            }else{
                bitCnt=0;
                kb.frameComplete=1;         //use frameComplete flag to detect ack bit from the keyboard
                EICRA&=~(1<<ISC00);goto exitIsr;  //set flag and change INT0 to falling edge to detect KB ack bit
            }
        }
    }
    exitIsr:
        EIFR|=(1<<INTF0);
        //asm("sei");
        SREG=sreg;
        //watchDogEnable();
}
//**********************************************************************************************
//**********************************************************************************************
void INT1_isr(void)
{
    volatile uint8_t sreg;
    //watchDogDisable();
    sreg=SREG;
    asm("cli");
    if(!sysReady)goto exitIsr;
    if(kb.comMode==KB_TO_HOST)
    {
        kb.busy=1;
        EIMSK&=~(1<<INT0);
        KB_DDR&=~(1<<KB_CLK);   //turn clk_pin to input
        TCNT1=0;
        while((KB_PIN&(1<<KB_CLK))&&(TCNT1<KB_MAX_BIT_TIME));
        if(TCNT1<KB_MAX_BIT_TIME)
        {
            EIMSK&=~(1<<INT1);
            EIFR|=(1<<INTF0)|(1<<INTF1);
            EIMSK|=(1<<INT0);
            KB_DDR&=~((1<<KB_DT)|(1<<KB_CLK));  //reconfigure data pin as input I/O
            kb.startBit=1;
        }
    }
    exitIsr:
        //asm("sei");
        SREG=sreg;
        //watchDogEnable();
}
//**********************************************************************************************
//**********************************************************************************************
void usart_rxc_isr(void)
{
    volatile uint8_t temp;
    if((temp=UDR0)==ECHO_RESPONSE)kb.detect=1;
}
//**********************************************************************************************
//**********************************************************************************************
