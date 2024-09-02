#ifndef PIN_MAP_HPP
#define PIN_MAP_HPP

#include <driver/gpio.h>

// gpio number for pwm
#define GPIO_PIN  GPIO_NUM_16

// custom transport -> serial
#define UART_TXD 1
#define UART_RXD 3

// direction pins (l298N)
#define MD1_IN2 11
#define MD1_IN1 10
#define MD1_IN3 12
#define MD1_IN4 13

#define MD2_IN1 14
#define MD2_IN2 15

// enable/pwm pins (l298N)
#define MD1_EA 16
#define MD1_EB 17
#define MD2_EA 18

#endif //PIN_MAP_HPP