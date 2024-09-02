#include <stdio.h>
#include <rcl/error_handling.h>

#include "uros_master_node.hpp"
#include "sim_pub.hpp"
#include "sim_sub.hpp"
#include "pwmDriver.hpp"
#include "velDriver.hpp"

#define GPIO_PIN  GPIO_NUM_16
#define PULSE_WIDTH_MAX 19990    // max -> 19990
#define PULSE_WIDTH_MIN 0

#define DIR_PIN_COUNT  6
#define PWM_PIN_COUNT  3

uros_master_node* master_node = 0;
pwm_driver *pwmHandle = 0;

extern "C" void app_main(void)
{
    // front | left | right -> motor driver pins
    int dirPins[DIR_PIN_COUNT] = {23,22,25,26,32,33};  
    int pwmPins[PWM_PIN_COUNT] = {5,27,16};

    pwmHandle = new pwm_driver();
    pwmHandle->set_gpio_pin(dirPins, pwmPins, DIR_PIN_COUNT, PWM_PIN_COUNT);

    pwmHandle->setRange(PULSE_WIDTH_MAX, PULSE_WIDTH_MIN);
    pwmHandle->setupTimer();
    // pwmHandle->update(1);

    master_node = new uros_master_node("master_node");

    master_node->add_urosElement({
        // new simPub(),
        // new simSub(pwmHandle),
        new velDriver(pwmHandle),
    });
}