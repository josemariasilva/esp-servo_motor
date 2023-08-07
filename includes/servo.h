#ifndef SERVO_H
#define SERVO_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/mcpwm_prelude.h"

#define SERVO_MIN_PULSEWIDTH_US 500  // Minimum pulse width in microsecond
#define SERVO_MAX_PULSEWIDTH_US 2500 // Maximum pulse width in microsecond
#define SERVO_MIN_DEGREE -135        // Minimum angle
#define SERVO_MAX_DEGREE 135         // Maximum angle

#define SERVO_PULSE_GPIO 0                   // GPIO connects to the PWM signal line
#define SERVO_TIMEBASE_RESOLUTION_HZ 1000000 // 1MHz, 1us per tick
#define SERVO_TIMEBASE_PERIOD 20000          // 20000 ticks, 20ms

uint32_t example_angle_to_compare(int angle);

class Servo
{
private:
    const char *TAG;
    int angle = 0;
    int setp = 1;
    int gpio_num;
    void setTimerServo() const;
    void setOperatorServo() const;
    void setCmpServo() const;
    void setGeneratorServo() const;

public:
    void write(int);
    int read() const;
    Servo(const char *, int);
    ~Servo();
    void initialization() const;
};

#endif