#ifndef SIM_SUB_HPP
#define SIM_SUB_HPP

#include "urosElement.hpp"
#include "pwmDriver.hpp"
#include <std_msgs/msg/int32.h>

class simSub : public urosElement{
public:
    simSub(pwm_driver *pwmHandler) {pwmHandle = pwmHandler;}

    // microros subscriber initialization
    void init();

    // subscription callback
    static void subscription_callback(const void * msgin);

private:
    rcl_subscription_t subscriber;
    static pwm_driver *pwmHandle;

};

#endif //SIM_SUB_HPP