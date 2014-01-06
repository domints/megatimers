/*
 *  Interrupt and PWM utilities for 16 bit Timers on ATmega2580
 *  Oct 2013 by Dominik Szymański to support Timer 1, 3, 4 & 5 of Arduino Mega
 *
 *  This is free software. You can redistribute it and/or modify it under
 *  the terms of Creative Commons Attribution 3.0 United States License. 
 *  To view a copy of this license, visit http://creativecommons.org/licenses/by/3.0/us/ 
 *  or send a letter to Creative Commons, 171 Second Street, Suite 300, San Francisco, California, 94105, USA.
 *
 */

#include "MegaTimers.h"

TimerOne Timer1;              // preinstatiate
TimerThree Timer3;              // preinstatiate
TimerFour Timer4;              // preinstatiate
TimerFive Timer5;              // preinstatiate

//ISR's
ISR(TIMER1_OVF_vect)          // interrupt service routine that wraps a user defined function supplied by attachInterrupt
{
  Timer1.isrCallback();
}
ISR(TIMER3_OVF_vect)          // interrupt service routine that wraps a user defined function supplied by attachInterrupt
{
  Timer3.isrCallback();
}
ISR(TIMER4_OVF_vect)          // interrupt service routine that wraps a user defined function supplied by attachInterrupt
{
  Timer4.isrCallback();
}
ISR(TIMER5_OVF_vect)          // interrupt service routine that wraps a user defined function supplied by attachInterrupt
{
  Timer5.isrCallback();
}
//END of ISR

//::initialize
void TimerOne::initialize(long microseconds)
{
  TCCR1A = 0;                 // clear control register A 
  TCCR1B = _BV(WGM13);        // set mode as phase and frequency correct pwm, stop the timer
  setPeriod(microseconds);
}
void TimerThree::initialize(long microseconds)
{
  TCCR3A = 0;                 // clear control register A 
  TCCR3B = _BV(WGM33);        // set mode as phase and frequency correct pwm, stop the timer
  setPeriod(microseconds);
}
void TimerFour::initialize(long microseconds)
{
  TCCR4A = 0;                 // clear control register A 
  TCCR4B = _BV(WGM43);        // set mode as phase and frequency correct pwm, stop the timer
  setPeriod(microseconds);
}
void TimerFive::initialize(long microseconds)
{
  TCCR5A = 0;                 // clear control register A 
  TCCR5B = _BV(WGM53);        // set mode as phase and frequency correct pwm, stop the timer
  setPeriod(microseconds);
}
//END of ::initialize

//::setPeriod
void TimerOne::setPeriod(long microseconds)
{
  long cycles = (F_CPU * microseconds) / 2000000;                                // the counter runs backwards after TOP, interrupt is at BOTTOM so divide microseconds by 2
  if(cycles < RESOLUTION)              clockSelectBits = _BV(CS10);              // no prescale, full xtal
  else if((cycles >>= 3) < RESOLUTION) clockSelectBits = _BV(CS11);              // prescale by /8
  else if((cycles >>= 3) < RESOLUTION) clockSelectBits = _BV(CS11) | _BV(CS10);  // prescale by /64
  else if((cycles >>= 2) < RESOLUTION) clockSelectBits = _BV(CS12);              // prescale by /256
  else if((cycles >>= 2) < RESOLUTION) clockSelectBits = _BV(CS12) | _BV(CS10);  // prescale by /1024
  else        cycles = RESOLUTION - 1, clockSelectBits = _BV(CS12) | _BV(CS10);  // request was out of bounds, set as maximum
  ICR1 = pwmPeriod = cycles;                                                     // ICR1 is TOP in p & f correct pwm mode
  TCCR1B &= ~(_BV(CS10) | _BV(CS11) | _BV(CS12));
  TCCR1B |= clockSelectBits;                                                     // reset clock select register
}
void TimerThree::setPeriod(long microseconds)
{
  long cycles = (F_CPU * microseconds) / 2000000;                                // the counter runs backwards after TOP, interrupt is at BOTTOM so divide microseconds by 2
  if(cycles < RESOLUTION)              clockSelectBits = _BV(CS30);              // no prescale, full xtal
  else if((cycles >>= 3) < RESOLUTION) clockSelectBits = _BV(CS31);              // prescale by /8
  else if((cycles >>= 3) < RESOLUTION) clockSelectBits = _BV(CS31) | _BV(CS30);  // prescale by /64
  else if((cycles >>= 2) < RESOLUTION) clockSelectBits = _BV(CS32);              // prescale by /256
  else if((cycles >>= 2) < RESOLUTION) clockSelectBits = _BV(CS32) | _BV(CS30);  // prescale by /1024
  else        cycles = RESOLUTION - 1, clockSelectBits = _BV(CS32) | _BV(CS30);  // request was out of bounds, set as maximum
  ICR3 = pwmPeriod = cycles;                                                     // ICR1 is TOP in p & f correct pwm mode
  TCCR3B &= ~(_BV(CS30) | _BV(CS31) | _BV(CS32));
  TCCR3B |= clockSelectBits;                                                     // reset clock select register
}
void TimerFour::setPeriod(long microseconds)
{
  long cycles = (F_CPU * microseconds) / 2000000;                                // the counter runs backwards after TOP, interrupt is at BOTTOM so divide microseconds by 2
  if(cycles < RESOLUTION)              clockSelectBits = _BV(CS40);              // no prescale, full xtal
  else if((cycles >>= 3) < RESOLUTION) clockSelectBits = _BV(CS41);              // prescale by /8
  else if((cycles >>= 3) < RESOLUTION) clockSelectBits = _BV(CS41) | _BV(CS40);  // prescale by /64
  else if((cycles >>= 2) < RESOLUTION) clockSelectBits = _BV(CS42);              // prescale by /256
  else if((cycles >>= 2) < RESOLUTION) clockSelectBits = _BV(CS42) | _BV(CS40);  // prescale by /1024
  else        cycles = RESOLUTION - 1, clockSelectBits = _BV(CS42) | _BV(CS40);  // request was out of bounds, set as maximum
  ICR4 = pwmPeriod = cycles;                                                     // ICR1 is TOP in p & f correct pwm mode
  TCCR4B &= ~(_BV(CS40) | _BV(CS41) | _BV(CS42));
  TCCR4B |= clockSelectBits;                                                     // reset clock select register
}
void TimerFive::setPeriod(long microseconds)
{
  long cycles = (F_CPU * microseconds) / 2000000;                                // the counter runs backwards after TOP, interrupt is at BOTTOM so divide microseconds by 2
  if(cycles < RESOLUTION)              clockSelectBits = _BV(CS50);              // no prescale, full xtal
  else if((cycles >>= 3) < RESOLUTION) clockSelectBits = _BV(CS51);              // prescale by /8
  else if((cycles >>= 3) < RESOLUTION) clockSelectBits = _BV(CS51) | _BV(CS50);  // prescale by /64
  else if((cycles >>= 2) < RESOLUTION) clockSelectBits = _BV(CS52);              // prescale by /256
  else if((cycles >>= 2) < RESOLUTION) clockSelectBits = _BV(CS52) | _BV(CS50);  // prescale by /1024
  else        cycles = RESOLUTION - 1, clockSelectBits = _BV(CS52) | _BV(CS50);  // request was out of bounds, set as maximum
  ICR5 = pwmPeriod = cycles;                                                     // ICR1 is TOP in p & f correct pwm mode
  TCCR5B &= ~(_BV(CS50) | _BV(CS51) | _BV(CS52));
  TCCR5B |= clockSelectBits;                                                     // reset clock select register
}
//END of ::setPeriod

//::setPwmDuty
void TimerOne::setPwmDuty(char pin, int duty)
{
  unsigned long dutyCycle = pwmPeriod;
  dutyCycle *= duty;
  dutyCycle >>= 10;
  if(pin == 11) OCR1A = dutyCycle;
  if(pin == 12) OCR1B = dutyCycle;
  if(pin == 13) OCR1C = dutyCycle;
}
void TimerThree::setPwmDuty(char pin, int duty)
{
  unsigned long dutyCycle = pwmPeriod;
  dutyCycle *= duty;
  dutyCycle >>= 10;
  if(pin == 5) OCR3A = dutyCycle;
  if(pin == 2) OCR3B = dutyCycle;
  if(pin == 3) OCR3C = dutyCycle;
}
void TimerFour::setPwmDuty(char pin, int duty)
{
  unsigned long dutyCycle = pwmPeriod;
  dutyCycle *= duty;
  dutyCycle >>= 10;
  if(pin == 6) OCR4A = dutyCycle;
  if(pin == 7) OCR4B = dutyCycle;
  if(pin == 8) OCR4C = dutyCycle;
}
void TimerFive::setPwmDuty(char pin, int duty)
{
  unsigned long dutyCycle = pwmPeriod;
  dutyCycle *= duty;
  dutyCycle >>= 10;
  if(pin == 40) OCR5C = dutyCycle;
  if(pin == 39) OCR5B = dutyCycle;
  if(pin == 38) OCR5A = dutyCycle;
}
//END of ::setPwmDuty

//::pwm
void TimerOne::pwm(char pin, int duty, long microseconds)  // expects duty cycle to be 10 bit (1024)
{
  if(microseconds > 0) setPeriod(microseconds);
  // sets data direction register for pwm output pin
	// activates the output pin
  if(pin == 11) { DDRB |= _BV(PORTB5); TCCR1A |= _BV(COM1A1); }
  if(pin == 12) { DDRB |= _BV(PORTB6); TCCR1A |= _BV(COM1B1); }
  if(pin == 12) { DDRB |= _BV(PORTB7); TCCR1A |= _BV(COM1C1); }
  setPwmDuty(pin, duty);
  start();
}
void TimerThree::pwm(char pin, int duty, long microseconds)  // expects duty cycle to be 10 bit (1024)
{
  if(microseconds > 0) setPeriod(microseconds);
  // sets data direction register for pwm output pin
	// activates the output pin
  if(pin == 5) { DDRE |= _BV(PORTE3); TCCR3A |= _BV(COM3A1); }
  if(pin == 2) { DDRE |= _BV(PORTE4); TCCR3A |= _BV(COM3B1); }
  if(pin == 3) { DDRE |= _BV(PORTE5); TCCR3A |= _BV(COM3C1); }
  setPwmDuty(pin, duty);
  start();
}
void TimerFour::pwm(char pin, int duty, long microseconds)  // expects duty cycle to be 10 bit (1024)
{
  if(microseconds > 0) setPeriod(microseconds);
  // sets data direction register for pwm output pin
	// activates the output pin
  if(pin == 6) { DDRH |= _BV(PORTH3); TCCR4A |= _BV(COM4A1); }
  if(pin == 7) { DDRH |= _BV(PORTH4); TCCR4A |= _BV(COM4B1); }
  if(pin == 8) { DDRH |= _BV(PORTH5); TCCR4A |= _BV(COM4C1); }
  setPwmDuty(pin, duty);
  start();
}
void TimerFive::pwm(char pin, int duty, long microseconds)  // expects duty cycle to be 10 bit (1024)
{
  if(microseconds > 0) setPeriod(microseconds);
  // sets data direction register for pwm output pin
	// activates the output pin
  if(pin == 38) { DDRL |= _BV(PORTL3); TCCR4A |= _BV(COM5A1); }
  if(pin == 39) { DDRL |= _BV(PORTL4); TCCR4A |= _BV(COM5B1); }
  if(pin == 40) { DDRL |= _BV(PORTL5); TCCR4A |= _BV(COM5C1); }
  setPwmDuty(pin, duty);
  start();
}
//END of ::pwm

//::disablePWM
void TimerOne::disablePwm(char pin)
{
  if(pin == 11) TCCR1A &= ~_BV(COM1A1);   // clear the bit that enables pwm on PB5
  if(pin == 12) TCCR1A &= ~_BV(COM1B1);   // clear the bit that enables pwm on PB6
  if(pin == 13) TCCR1A &= ~_BV(COM1C1);   // clear the bit that enables pwm on PB7
}
void TimerThree::disablePwm(char pin)
{
  if(pin == 5) TCCR3A &= ~_BV(COM3A1);   // clear the bit that enables pwm on PE3
  if(pin == 2) TCCR3A &= ~_BV(COM3B1);   // clear the bit that enables pwm on PE4
  if(pin == 3) TCCR3A &= ~_BV(COM3C1);   // clear the bit that enables pwm on PE5
}
void TimerFour::disablePwm(char pin)
{
  if(pin == 6) TCCR4A &= ~_BV(COM4A1);   // clear the bit that enables pwm on PH3
  if(pin == 7) TCCR4A &= ~_BV(COM4B1);   // clear the bit that enables pwm on PH4
  if(pin == 8) TCCR4A &= ~_BV(COM4C1);   // clear the bit that enables pwm on PH5
}
void TimerFive::disablePwm(char pin)
{
  if(pin == 38) TCCR5A &= ~_BV(COM5A1);   // clear the bit that enables pwm on PL3
  if(pin == 39) TCCR5A &= ~_BV(COM5B1);   // clear the bit that enables pwm on PL4
  if(pin == 40) TCCR5A &= ~_BV(COM5C1);   // clear the bit that enables pwm on PL5
}
//END of ::disablePwm

//::attachInterrupt
void TimerOne::attachInterrupt(void (*isr)(), long microseconds)
{
  if(microseconds > 0) setPeriod(microseconds);
  isrCallback = isr;                                       // register the user's callback with the real ISR
  TIMSK1 = _BV(TOIE1);                                     // sets the timer overflow interrupt enable bit
  sei();                                                   // ensures that interrupts are globally enabled
  start();
}
void TimerThree::attachInterrupt(void (*isr)(), long microseconds)
{
  if(microseconds > 0) setPeriod(microseconds);
  isrCallback = isr;                                       // register the user's callback with the real ISR
  TIMSK3 = _BV(TOIE3);                                     // sets the timer overflow interrupt enable bit
  sei();                                                   // ensures that interrupts are globally enabled
  start();
}
void TimerFour::attachInterrupt(void (*isr)(), long microseconds)
{
  if(microseconds > 0) setPeriod(microseconds);
  isrCallback = isr;                                       // register the user's callback with the real ISR
  TIMSK4 = _BV(TOIE4);                                     // sets the timer overflow interrupt enable bit
  sei();                                                   // ensures that interrupts are globally enabled
  start();
}
void TimerFive::attachInterrupt(void (*isr)(), long microseconds)
{
  if(microseconds > 0) setPeriod(microseconds);
  isrCallback = isr;                                       // register the user's callback with the real ISR
  TIMSK5 = _BV(TOIE5);                                     // sets the timer overflow interrupt enable bit
  sei();                                                   // ensures that interrupts are globally enabled
  start();
}
//END of ::attachInterrupt

//::detachInterrupt
void TimerOne::detachInterrupt()
{
  TIMSK1 &= ~_BV(TOIE1);                                   // clears the timer overflow interrupt enable bit 
}
void TimerThree::detachInterrupt()
{
  TIMSK3 &= ~_BV(TOIE3);                                   // clears the timer overflow interrupt enable bit 
}
void TimerFour::detachInterrupt()
{
  TIMSK4 &= ~_BV(TOIE4);                                   // clears the timer overflow interrupt enable bit 
}
void TimerFive::detachInterrupt()
{
  TIMSK5 &= ~_BV(TOIE5);                                   // clears the timer overflow interrupt enable bit 
}
//END of ::detachInterrupt

//::start
void TimerOne::start()
{
  TCCR1B |= clockSelectBits;
}
void TimerThree::start()
{
  TCCR3B |= clockSelectBits;
}
void TimerFour::start()
{
  TCCR4B |= clockSelectBits;
}
void TimerFive::start()
{
  TCCR5B |= clockSelectBits;
}
//END of ::start

//::stop
void TimerOne::stop()
{
  TCCR1B &= ~(_BV(CS10) | _BV(CS11) | _BV(CS12));          // clears all clock selects bits
}
void TimerThree::stop()
{
  TCCR3B &= ~(_BV(CS30) | _BV(CS31) | _BV(CS32));          // clears all clock selects bits
}
void TimerFour::stop()
{
  TCCR4B &= ~(_BV(CS40) | _BV(CS41) | _BV(CS42));          // clears all clock selects bits
}
void TimerFive::stop()
{
  TCCR5B &= ~(_BV(CS50) | _BV(CS51) | _BV(CS52));          // clears all clock selects bits
}
//END of ::stop

//::restart
void TimerOne::restart()
{
  TCNT1 = 0;
}
void TimerThree::restart()
{
  TCNT3 = 0;
}
void TimerFour::restart()
{
  TCNT4 = 0;
}
void TimerFive::restart()
{
  TCNT5 = 0;
}
//END of ::restart