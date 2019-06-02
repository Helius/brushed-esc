char htoa(uint8_t a);
void printHex(uint8_t value);
void printNumb(int16_t numb);

// OutPin led(&DDRB, &PORTB, 5);
class OutPin {
public:
	OutPin() = delete;
	OutPin(volatile uint8_t * ddr, volatile uint8_t * port, int pin)
		: port(port)
	{
		pinM = 1 << pin;
		*ddr |= pinM;
	}
	void set()
	{
		*port |= pinM;
	}
	void clear()
	{
		*port &= ~pinM; 
	}
	void toggle()
	{
		*port ^= pinM;
	}
private:
	volatile uint8_t * port;
	int pinM;
};
