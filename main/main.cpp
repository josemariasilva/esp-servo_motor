#include "servo.h"

extern "C" void app_main(void)
{
    Servo s1("SERVO", 0);
    s1.initialization();

    while (1)
    {
        for (int i = 0; i < 90; i++)
        {
            ESP_LOGI("Servo", "Angle of rotation: %d", s1.read());
            s1.write(i);
            //vTaskDelay(pdMS_TO_TICKS(100));
        }
        for (int i = 90; i >= 0; i--)
        {
            ESP_LOGI("Servo", "Angle of rotation: %d", s1.read());
            s1.write(i);
            //vTaskDelay(pdMS_TO_TICKS(100));
        }
    }
}