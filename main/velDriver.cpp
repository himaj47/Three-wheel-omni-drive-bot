#include "velDriver.hpp"

const char * cmd_vel_topic_name = "cmd_vel";
pwm_driver * velDriver::pwmHandle = 0;

const rosidl_message_type_support_t * cmd_vel_type_support = ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Twist);
geometry_msgs__msg__Twist twist;

void velDriver::init() {
    rclc_subscription_init_best_effort(&sub_cmd_vel, node, cmd_vel_type_support, cmd_vel_topic_name);

    rclc_executor_add_subscription(exec, &sub_cmd_vel, &twist, subscription_callback, ALWAYS);
}

void velDriver::subscription_callback(const void * msgin) {
    const geometry_msgs__msg__Twist * vel = (const geometry_msgs__msg__Twist *)msgin;

    float front_vel = vel->linear.x;   // front wheel velocity
    float left_vel = vel->linear.y;    // left  wheel velocity
    float right_vel = vel->linear.z;   // right wheel velocity

    pwmHandle->update(front_vel, left_vel, right_vel);

}