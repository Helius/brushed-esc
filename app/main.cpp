#include <stdio.h>

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include <util/delay.h>

#include <stdlib.h>
#include <string.h>

#include <uart.h>
#include <misc.h>

OutPin led(&DDRB, &PORTB, 5);

class Motor {
	public:
		Motor(volatile uint8_t * value)
		: value(value)
		{}

		void setSpeed(uint8_t speed) 
		{
			if(speed == 0)
			{
				TCCR0A = 0;
			}
			else {
				TCCR0A = _BV(WGM01) | _BV(WGM00) | _BV(COM0A1) | _BV(COM0B1); // fast pwm, non-invert
			}
			*value = speed;
		}

	private:	
		volatile uint8_t * value;
};


class Motors {
	public:
		Motors()
			: rMotor(Motor(&OCR0A))
			, lMotor(Motor(&OCR0B))
	{
	}

	void setSpeed(uint8_t speed)
	{
		rMotor.setSpeed(speed);
		lMotor.setSpeed(speed);
	}

	private:
		Motor rMotor;
		Motor lMotor;
};

Motors motors;

// class reading PWM signal and return it as value
class PwmInput {

public:
	void levelChanged()
	{
		if(state == State::Stop) {
			state = State::Start;
			TCNT2 = 0;
		} else if(state == State::Start) {
			state = State::Stop;
			val = TCNT2;
		}
	}

	void timeout() 
	{
		state = State::Stop;
	}

	uint8_t value()
	{
		return val - 0x1f;
	}

private:
	enum class State {Stop, Start};
	uint8_t val = 0;
	State state = State::Stop;
};

PwmInput pwm;

ISR(TIMER2_OVF_vect)
{
	pwm.timeout();
}

ISR(INT0_vect)
{
	pwm.levelChanged();
}


int main(void)
{
	TCCR0A = _BV(WGM01) | _BV(WGM00) | _BV(COM0A1) | _BV(COM0B1); // fast pwm, non-invert
	TCCR0B = _BV(CS01) | _BV(CS00);  // prescaller is 32
	OCR0A  = 0;
	OCR0B  = 0;
	DDRD = _BV(PD5) | _BV(PD6);

	// timer2 just overflow at 256 prescaller
	TIMSK2 = _BV(TOIE2);
	TCCR2B = _BV(CS22) | _BV(CS21);

	uart_init(0);

	// setup external interrupt
	EICRA = (1 << ISC00); // any edge
	EIMSK = (1 << INT0);    // Enable INT0
	PORTD |= (1 << PIN2); // pullup


	sei();
	

	// waiting low level to protect fingers
	uint8_t armCnt = 0;
	while(armCnt < 6) 
	{
		_delay_ms(30);
		if(pwm.value() < 2) {
			armCnt++;
		} else {
			armCnt = 0;
		}
		printHex(pwm.value());
		uart_putchar('\n');
		uart_putchar('\r');
		led.toggle();
	}

	while(1)
	{
		_delay_ms(100);

		uint8_t speed = pwm.value(); // 0..32?
		printHex(speed);
		uart_putchar('\t');
		
		if(speed < 2) {
			speed = 0;
		} else if (speed < 31) {
			speed = speed*8;
		} else {
			speed = 0xff;
		}
		
		printHex(speed);
		uart_putchar('\n');
		uart_putchar('\r');

		motors.setSpeed(speed);
	
		led.toggle();
	}
}

