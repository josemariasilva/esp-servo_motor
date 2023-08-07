#include "servo.h"

mcpwm_timer_handle_t timer = NULL;
mcpwm_oper_handle_t oper = NULL;
mcpwm_cmpr_handle_t comparator = NULL;
mcpwm_gen_handle_t generator = NULL;

uint32_t example_angle_to_compare(int angle){
    return (angle - SERVO_MIN_DEGREE) * (SERVO_MAX_PULSEWIDTH_US - SERVO_MIN_PULSEWIDTH_US) / (SERVO_MAX_DEGREE - SERVO_MIN_DEGREE) + SERVO_MIN_PULSEWIDTH_US;
}

Servo::Servo(const char *TAG, int gpio): TAG(TAG), gpio_num(gpio){
}

Servo::~Servo(){}   

void Servo::write(int a){
    angle = a;
    if((angle <= 90 && (angle >= -90))){
        ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(comparator, example_angle_to_compare(angle)));
    }
    else{
        ESP_LOGE(TAG, "Exceed angle servo motor");
    }
    

    
}

void Servo::setTimerServo() const{
    
    mcpwm_timer_config_t timer_config;
    
        timer_config.group_id = 0;
        timer_config.clk_src = MCPWM_TIMER_CLK_SRC_DEFAULT;
        timer_config.resolution_hz = SERVO_TIMEBASE_RESOLUTION_HZ;
        timer_config.period_ticks = SERVO_TIMEBASE_PERIOD;
        timer_config.count_mode = MCPWM_TIMER_COUNT_MODE_UP;
    

    ESP_ERROR_CHECK(mcpwm_new_timer(&timer_config, &timer));
}

void Servo::setOperatorServo() const{
    
    mcpwm_operator_config_t operator_config = {
        .group_id = 0, // operator must be in the same group to the timer
    };
    ESP_ERROR_CHECK(mcpwm_new_operator(&operator_config, &oper));

    //ESP_LOGI(TAG, "Connect timer and operator");
    ESP_ERROR_CHECK(mcpwm_operator_connect_timer(oper, timer));
}

void Servo::setCmpServo() const{
    
    mcpwm_comparator_config_t comparator_config;
        comparator_config.flags.update_cmp_on_tez = true;

    ESP_ERROR_CHECK(mcpwm_new_comparator(oper, &comparator_config, &comparator));
}

void Servo::setGeneratorServo() const{
    
    mcpwm_generator_config_t generator_config;
        generator_config.gen_gpio_num = gpio_num;
    ESP_ERROR_CHECK(mcpwm_new_generator(oper, &generator_config, &generator));
    
}

void Servo::initialization() const{

    Servo::setTimerServo();
    Servo::setOperatorServo();
    Servo::setCmpServo();
    Servo::setGeneratorServo();

    ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(comparator, example_angle_to_compare(0)));
    //ESP_LOGI(TAG, "Set generator action on timer and compare event");
    // go high on counter empty
    ESP_ERROR_CHECK(mcpwm_generator_set_action_on_timer_event(generator,
                    MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH)));
    // go low on compare threshold
    ESP_ERROR_CHECK(mcpwm_generator_set_action_on_compare_event(generator,
                    MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, comparator, MCPWM_GEN_ACTION_LOW)));

    //ESP_LOGI(TAG, "Enable and start timer");
    ESP_ERROR_CHECK(mcpwm_timer_enable(timer));
    ESP_ERROR_CHECK(mcpwm_timer_start_stop(timer, MCPWM_TIMER_START_NO_STOP));
}

int Servo::read() const{
    return angle;
}