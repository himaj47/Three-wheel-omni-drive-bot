#include "sim_sub.hpp"

const char * sub_topic_name = "pwm_uptime";

pwm_driver * simSub::pwmHandle = 0;

const rosidl_message_type_support_t * sub_type_support = ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32);
std_msgs__msg__Int32 message;

void simSub::init() {
    rclc_subscription_init_best_effort(&subscriber, node, sub_type_support, sub_topic_name);

    rclc_executor_add_subscription(exec, &subscriber, &message, subscription_callback, ON_NEW_DATA);
}

void simSub::subscription_callback(const void * msgin)
{
  // Cast received message to used type
  const std_msgs__msg__Int32 * msg = (const std_msgs__msg__Int32 *)msgin;
  // float pwmVal = msg->data;
  // pwmHandle->update(pwmVal);  // 0-255

}
