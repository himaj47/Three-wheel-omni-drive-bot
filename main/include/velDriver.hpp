#ifndef VEL_DRIVER_HPP
#define VEL_DRIVER_HPP

#include "urosElement.hpp"
#include "pwmDriver.hpp"

#include <geometry_msgs/msg/twist.h>

class velDriver : public urosElement {
public: 
    velDriver(pwm_driver *pwmHandler){pwmHandle = pwmHandler;};

    // micro ros initialization
    void init();

    static void subscription_callback(const void * msgin);

private:
    rcl_subscription_t sub_cmd_vel;
    static pwm_driver *pwmHandle;
};

#endif //VEL_DRIVER_HPP