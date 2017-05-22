# STC15 demo code

The code in this directory belongs to STC.

I didn't modify the source, only converted it from GB2312 to UTF-8 with UNIX line terminators.

| Filename | Description |
---
| 01-demo.c | ADC (interrupt) |
| 02-demo.c | ADC (query) |
| 03-demo.c | Use 9th ADC inside BandGap voltage to calculate the operating voltage Vcc |
| 04-demo.c | EEPROM (single-byte IO operation display) |
| 05-demo.c | EEPROM (single byte operation and use the serial port sends data) |
| 06-demo.c | EEPROM (continuous operation and use multi-byte serial send data) |
| 07-demo.c | IO port test |
| 08-demo.c | PCA to achieve 16bit timer |
| 09-demo.c | PCA expand the external interrupt |
| 10-demo.c | The PCA 16 capture pulse width measurement |
| 11-demo.c | PCA output PWM (6bit 7bits 8bits) |
| 12-demo.c | PCA simulation to achieve many of the PWM output (8 or more) |
| 13-demo.c | PCA High-speed pulse output |
| 14-demo.c | PCA to achieve 16bit timer 16bit PWM capture 6-7-8 |
| 15-demo.c | SPI master-slave system interrupt each function test |
| 16-demo.c | SPI master-slave system query each function test |
| 17-demo.c | SPI single master unit from the system interrupt function test |
| 18-demo.c | SPI single master single query from the system function test |
| 19-demo.c | SPI master mode to read and write external serial Flash example (query) |
| 20-demo.c | SPI master mode to read and write external serial Flash example (interrupt) |
| 21-demo.c | Serial an automatic address recognition |
| 22-demo.c | Serial Port 1 (using Timer 1 Mode 0 (16bit auto-reload) for baud rate generator) |
| 23-demo.c | Serial Port 1 (use Timer 1 mode 2 (8 auto-reload) for baud rate generator) |
| 24-demo.c | Serial Port 1 (use Timer 2 as a baud rate generator) |
| 25-demo.c | Serial 2 (use Timer 2 as a baud rate generator) |
| 26-demo.c | Serial 3 (use Timer 2 as a baud rate generator) |
| 27-demo.c | Serial 3 (Using Timer 3 for baud rate generator) |
| 28-demo.c | Serial 4 (use Timer 2 as a baud rate generator) |
| 29-demo.c | Serial 4 (using a timer 4 for baud rate generator) |
| 30-demo.c | Area and the program from the RAM area to obtain a global unique ID number |
| 31-demo.c | Master clock (internal IRC or external clock) output |
| 32-demo.c | Peripherals (CCP) to switch between multiple ports |
| 33-demo.c | Peripherals (SPI) to switch between multiple ports |
| 34-demo.c | Peripherals (serial port 1) to switch between multiple ports |
| 35-demo.c | Peripherals (serial port 2) to switch between multiple ports |
| 36-demo.c | Peripheral devices (serial 3) to switch between the plurality of ports |
| 37-demo.c | Peripherals (serial 4) to switch between multiple ports |
| 38-demo.c | External Interrupt (PCA function expansion external interrupt) |
| 39-demo.c | External Interrupt (T0 extended to falling interrupt) |
| 40-demo.c | External Interrupt (T1 extended to falling interrupt) |
| 41-demo.c | External Interrupt (T2 extended to falling interrupt) |
| 42-demo.c | External interrupt INT0 (rising edge) |
| 43-demo.c | External interrupt INT0 (falling) |
| 44-demo.c | External interrupt INT1 (rising edge) |
| 45-demo.c | External interrupt INT1 (falling) |
| 46-demo.c | External interrupt INT2 (falling) |
| 47-demo.c | External interrupt INT3 (falling) |
| 48-demo.c | External interrupt INT4 (falling) |
| 49-demo.c | LVD Low Voltage Detect Interrupt External Interrupt |
| 50-demo.c | Timer 0 T0 pin of the internal clock or an external clock outputs programmable clock divider |
| 51-demo.c | Timer 0 Mode 0 (16bit auto-reload) |
| 52-demo.c | Timer 0 16 analog PWM auto-reload mode software |
| 53-demo.c | Timer 1 on the internal clock or an external clock T1 pin programmable clock divider output |
| 54-demo.c | Timer 1 Mode 0 (16bit auto-reload) |
| 55-demo.c | Timer 1 Mode 0 (16bit auto-reload) for serial port 1 baud rate generator |
| 56-demo.c | Timer 1 Mode 2 (8bit auto reload) for serial port 1 baud rate generator |
| 57-demo.c | Timer 2 as a baud rate generator serial |
| 58-demo.c | Timer 2 as a baud rate generator serial port 2 |
| 59-demo.c | Timer 2 as a baud rate generator serial 3 |
| 60-demo.c | Timer 2 as a baud rate generator serial 4 |
| 61-demo.c | Timer 2 T2 pin of the internal clock or an external clock outputs programmable clock divider |
| 62-demo.c | Timer 2 is extended to external falling edge interrupt |
| 63-demo.c | Timer 2 16 auto-reload mode |
| 64-demo.c | Timer 3 for the serial port baud rate generator 3 |
| 65-demo.c | Timer 3 of the internal clock or an external clock pin T3 programmable clock divider output |
| 66-demo.c | Timer 3 of 16 auto-reload mode |
| 67-demo.c | 4 Timer 4 for serial baud rate generator |
| 68-demo.c | Timer 4 of the internal clock or an external clock T4 pin programmable clock divider output |
| 69-demo.c | Timer 4 of 16 auto-reload mode |
| 70-demo.c | Watchdog timer overflow reset test |
| 71-demo.c | Operating mode (slow mode) |
| 72-demo.c | Operating mode (power-down mode) |
| 73-demo.c | Operating mode (idle mode) |
| 74-demo.c | Operating mode (software reset) |
| 75-demo.c | Power-down mode (INT0 falling edge wake-up) |
| 76-demo.c | Power-down mode (INT1 falling edge wake-up) |
| 77-demo.c | Power-down mode (INT2 edge wake) |
| 78-demo.c | Power-down mode (INT3 edge wake) |
| 79-demo.c | Power-down mode (INT4 edge wake) |
| 80-demo.c | Power-down mode (PCA extended to external falling edge interrupt wake-up) |
| 81-demo.c | Power-down mode (power-down wake-up timer wakeup) |
| 82-demo.c | Power-down mode (wake up with RxD2 serial interrupt 2) |
| 83-demo.c | Power-down mode (with RxD serial interrupt a wake-up) |
| 84-demo.c | Power-down mode (LVD Low voltage detection interrupt wake-up) |
| 85-demo.c | Power-down mode instruction execution process description |

