add_definitions("-DUART_BAUDRATE=115200")
add_definitions("-DUART_DOUBLE_SPEED")

add_avr_library(
   hallib
   uart.cpp
   uart.h
	 I2C.cpp
	 I2C.h
)
