#ifndef PWM_DRIVER
#define PWM_DRIVER

#include <driver/mcpwm_prelude.h>
#include "driver/gpio.h"
#include <string.h>

#define TIMER_RESOLUTION_HZ     1 * 1000 * 1000   // 1 MHz (tick resolution)
#define TIME_PERIOD             20000             // in clock ticks
#define MAX_PULSE_WIDTH         2500              // 2500 us
#define MIN_PULSE_WIDTH         500               // 500 us

#define REQ_OPR_H  2
#define REQ_CMP_H  3
#define REQ_GEN_H  3

class pwm_driver {

public:
    pwm_driver();
    void set_gpio_pin(int dirPins[], int pwmPins[], int dirCount, int pwmCount);
    void setupTimer();
    void setRange(float max_pwm, float min_pwm);
    void update(float uptime_front, float uptime_left, float uptime_right);

private:
    gpio_config_t opcfg;
    gpio_num_t pin_number;

    int dirPins[6] = {0};
    int pwmPins[3] = {0};
    float speeds[3] = {0.0f};

    struct unitHandler
    {
        mcpwm_timer_handle_t timerHandle;
        mcpwm_oper_handle_t operHandle[REQ_OPR_H] = {nullptr};
        mcpwm_cmpr_handle_t cmpHandle[REQ_CMP_H] = {nullptr};
        mcpwm_gen_handle_t genHandle[REQ_GEN_H] = {nullptr};
    } unit0;

    float MAX_PWM = MAX_PULSE_WIDTH;
    float MIN_PWM = MIN_PULSE_WIDTH;

    int DIR_PIN_CNT;
    int PWM_PIN_CNT;

    static int map(float val);
    
};

#endif