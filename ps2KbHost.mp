Project:      .\ps2KbHost
Build Number: 996
Date:         Thu Dec 15 18:41:52 2016
Compiler:     ICCAVR
Version:      8.24.01
Device:       ATMega328P

NOTE: AVR Code addresses are word addresses
All other addresses (including ones in FLASH) are byte addresses
  ROM 14% full. 4314 out of 32768 bytes used (does not include absolute areas).
  RAM 33 bytes used (does not include stack usage).

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                        func_lit   00B0     52 =     82. bytes (rel,con,rom,proclab)

       Addr  Global Symbol
      -----  --------------------------------
       00B0  __func_lit_start
       0102  __func_lit_end

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                             lit   0102     5C =     92. bytes (rel,con,rom,lit)

       Addr  Global Symbol
      -----  --------------------------------
       0102  __lit_start
       0102  _shiftedNumbers
       010C  _shiftedSymbols
       0118  _asciiSymbols
       0124  _symbolsScancodes
       0130  _alphaScancodes
       014A  _numbersScancodes
       0154  _numKeysScancodes
       015E  __lit_end

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                           idata   015E      8 =      8. bytes (rel,con,rom,lit)

       Addr  Global Symbol
      -----  --------------------------------
       015E  __idata_start
       0166  __idata_end

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                            text   0166   1024 =   4132. bytes (rel,con,rom,code)

       Addr  Global Symbol
      -----  --------------------------------
       00B3  __text_start
       00B3  __start
       00D5  _exit
       00D6  _PCINT0_isr
       00D7  _PCINT1_isr
       00D8  _PCINT2_isr
       00D9  _wdt_isr
       00DA  _timer2_compa_isr
       00DB  _timer2_compb_isr
       00DC  _timer2_ovf_isr
       00DD  _timer1_capt_isr
       00DE  _timer1_compa_isr
       00DF  _timer1_compb_isr
       00E0  _timer1_ovf_isr
       00E1  _timer0_compb_isr
       00E2  _timer0_ovf_isr
       00E3  _spi_stc_isr
       00E4  _usart_udre_isr
       00E5  _usart_txc_isr
       00E6  _adc_isr
       00E7  _ee_ready_isr
       00E8  _analog_comp_isr
       00E9  _twi_isr
       00EA  _spm_rdy_isr
       00EB  _writeUart
       00FF  _uartInit
       014A  _initTimer0
       015F  _initTimer1
       0175  _resetKBVars
       01A2  _initExtInt
       01B2  _scanKey
       0338  _readKey
       04C9  _writeKeyboard
       0575  _resetKb
       058B  _toggleLed
       05D2  _detectKb
       060D  _getKb
       067D  _main
       0690  _delay_us
       06A6  _delay_ms
       06B4  _timer0_compa_isr
       06BF  _checkOddParity
       06DB  _getOddParity
       06F6  _INT0_isr
       0786  _INT1_isr
       07CD  _usart_rxc_isr
       07E1  <LIBRARY>
       07E1  push_arg4
       07E3  push_arg2
       07E6  mod16s
       07EA  div16s
       0804  mod16u
       0806  div16u
       0807  xdiv16u
       0820  div32u
       0822  mod32u
       0826  div32s
       0828  mod32s
       0854  long_div_prolog
       0867  long_div_epilog
       0877  tst_R16_R19
       087C  tst_R24_R27
       0881  neg_R24_R27
       088A  push_xgsetF000
       088F  pop_xgsetF000
       0894  push_xgsetF00C
       089B  pop_xgsetF00C
       08A2  push_xgset303C
       08A9  pop_xgset303C
       08B0  neg32
       08B9  lsr32
       08C5  __text_end

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                            data   0100      8 =      8. bytes (rel,con,ram)

       Addr  Global Symbol
      -----  --------------------------------
       0100  __data_start
       0100  main.c:pauseBreakCnt
       0101  main.c:printScreenCnt
       0102  main.c:printScreenUp
       0103  _msTick
       0104  _sysReady
       0105  main.c:dataFrame
       0107  main.c:bitCnt
       0108  __data_end

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                             bss   0108     19 =     25. bytes (rel,con,ram)

       Addr  Global Symbol
      -----  --------------------------------
       0108  main.c:c
       0108  __bss_start
       0109  _kb
       0121  __bss_end

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                          vector   0004     64 =    100. bytes (abs,ovr,rom,code)

Files Linked      [ module(s) ]

C:\iccv8avr\lib\crtatmega.o	[ crtatmega.s ]
.objs\main.o	[ main.c ]
<library>	[ asave.s, div16s.s, div16u.s, div32.s, gpushy.s, gpushz.s, gpusha.s, lneg.s, lsr32.s ]

User Global Definitions

ram_end = 0x8ff
hwstk_size = 0x14

Area Definitions

-bfunc_lit:0xb0.0x8000
-bdata:0x100.0x8ff

Additional Flags
  -e:0x8000
