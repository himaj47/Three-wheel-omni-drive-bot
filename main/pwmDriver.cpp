#include "pwmDriver.hpp"

#define GET_BIT(PIN)  (1ULL<<PIN)

pwm_driver* pwm_drv = 0;

mcpwm_timer_config_t timer_cfg = {
    .group_id = 0,
    .clk_src = MCPWM_TIMER_CLK_SRC_DEFAULT,
    .resolution_hz = TIMER_RESOLUTION_HZ,
    .count_mode = MCPWM_TIMER_COUNT_MODE_UP,
    .period_ticks = TIME_PERIOD,
};

mcpwm_operator_config_t operator_config = {
    .group_id = 0,
    .flags = {}
};

mcpwm_comparator_config_t comparator_config = {
    .flags = {1,0,0}
};

pwm_driver::pwm_driver() {
    pwm_drv = this;

    // gpio configurations
    opcfg.pin_bit_mask = (uint64_t)(0);
    opcfg.pull_down_en = GPIO_PULLDOWN_DISABLE;
    opcfg.pull_up_en = GPIO_PULLUP_DISABLE;
    opcfg.mode = GPIO_MODE_OUTPUT;
    opcfg.intr_type = GPIO_INTR_DISABLE;
}

void pwm_driver::set_gpio_pin(int dirPins[], int pwmPins[], int dirPinCount, int pwmPinCount) {

    DIR_PIN_CNT = dirPinCount;
    PWM_PIN_CNT = pwmPinCount;

    memcpy(this->dirPins, dirPins, sizeof(int) * dirPinCount);
    memcpy(this->pwmPins, pwmPins, sizeof(int) * pwmPinCount);

    for (int i = 0; i < dirPinCount; i++) {
        opcfg.pin_bit_mask |= GET_BIT(dirPins[i]);
    }
    
    gpio_config(&opcfg);
}

void pwm_driver::setupTimer() {
    mcpwm_new_timer(&timer_cfg, &unit0.timerHandle);

    mcpwm_new_operator(&operator_config, &unit0.operHandle[0]);
    mcpwm_new_operator(&operator_config, &unit0.operHandle[1]);

    mcpwm_operator_connect_timer(unit0.operHandle[0], unit0.timerHandle);
    mcpwm_operator_connect_timer(unit0.operHandle[1], unit0.timerHandle);

    mcpwm_new_comparator(unit0.operHandle[0], &comparator_config, &unit0.cmpHandle[0]);
    mcpwm_new_comparator(unit0.operHandle[0], &comparator_config, &unit0.cmpHandle[1]);
    mcpwm_new_comparator(unit0.operHandle[1], &comparator_config, &unit0.cmpHandle[2]);

    mcpwm_generator_config_t generator_config = {
        .gen_gpio_num = pwmPins[0],
        .flags = {}
    };

    generator_config.gen_gpio_num = pwmPins[0];
    mcpwm_new_generator(unit0.operHandle[0], &generator_config, &unit0.genHandle[0]);

    generator_config.gen_gpio_num = pwmPins[1];
    mcpwm_new_generator(unit0.operHandle[0], &generator_config, &unit0.genHandle[1]);

    generator_config.gen_gpio_num = pwmPins[2];
    mcpwm_new_generator(unit0.operHandle[1], &generator_config, &unit0.genHandle[2]);


    for (int i = 0; i < REQ_GEN_H; i++) {
        mcpwm_generator_set_action_on_timer_event(unit0.genHandle[i],
        MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH));

        mcpwm_generator_set_action_on_compare_event(unit0.genHandle[i],
                MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, unit0.cmpHandle[i], MCPWM_GEN_ACTION_LOW));
    }

    mcpwm_timer_enable(unit0.timerHandle);
    mcpwm_timer_start_stop(unit0.timerHandle, MCPWM_TIMER_START_NO_STOP);
}

// set uptime
void pwm_driver::setRange(float max_pwm, float min_pwm) {
    MAX_PWM = max_pwm;
    MIN_PWM = min_pwm;
}

// analogWrite (0-255)
void pwm_driver::update(float uptime_front, float uptime_left, float uptime_right) {

    speeds[0] = uptime_front;
    speeds[1] = uptime_left;
    speeds[2] = uptime_right;

    gpio_set_level((gpio_num_t) dirPins[0], 1);  // high
    gpio_set_level((gpio_num_t) dirPins[1], 0);  // low
    gpio_set_level((gpio_num_t) dirPins[2], 1);  // high
    gpio_set_level((gpio_num_t) dirPins[3], 0);  // low
    gpio_set_level((gpio_num_t) dirPins[4], 1);  // high
    gpio_set_level((gpio_num_t) dirPins[5], 0);  // low

    if (speeds[0] < 0.0f) {
        gpio_set_level((gpio_num_t) dirPins[0], 0);  // low
        gpio_set_level((gpio_num_t) dirPins[1], 1);  // high
    }

    if (speeds[1] < 0.0f) {
        gpio_set_level((gpio_num_t) dirPins[2], 0);  // low
        gpio_set_level((gpio_num_t) dirPins[3], 1);  // high
    }

    if (speeds[2] < 0.0f) {
        gpio_set_level((gpio_num_t) dirPins[4], 0);  // low
        gpio_set_level((gpio_num_t) dirPins[5], 1);  // high
    }
    
    int up_fr = abs(uptime_front);
    int up_lf = abs(uptime_left);
    int up_rt = abs(uptime_right);

    // mcpwm_comparator_set_compare_value(unit0.cmpHandle[0], abs(int(uptime_front)));
    // mcpwm_comparator_set_compare_value(unit0.cmpHandle[1], abs(int(uptime_left)));
    // mcpwm_comparator_set_compare_value(unit0.cmpHandle[2], abs(int(uptime_right)));
    mcpwm_comparator_set_compare_value(unit0.cmpHandle[0], up_fr);
    mcpwm_comparator_set_compare_value(unit0.cmpHandle[1], up_lf);
    mcpwm_comparator_set_compare_value(unit0.cmpHandle[2], up_rt);
}

// map(0, 255, max_pulse_width, min_pulse_width) : arduino map function
int pwm_driver::map(float val) {
    return int(((val*(pwm_drv->MAX_PWM - pwm_drv->MIN_PWM))/255.0f) + pwm_drv->MIN_PWM);
}