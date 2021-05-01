#include <iostream>
#include <unistd.h>
#include <wiringPi.h>
// pin assign
#define left_motor1_pin 14
#define left_motor2_pin 15
#define left_pwm_pin 18

#define right_motor1_pin 23
#define right_motor2_pin 24
#define right_pwm_pin 12

int main()
{
    // GPIOの初期化
    if (wiringPiSetupGpio() == -1)
    {
        return -1;
    }

    // GPIO出力モードをOUTPUTに設定する
    wiringPiSetupSys();
    pinMode(left_motor1_pin, OUTPUT);
    pinMode(left_motor2_pin, OUTPUT);
    pinMode(right_motor1_pin, OUTPUT);
    pinMode(right_motor2_pin, OUTPUT);

    system("gpio -g mode 18 pwm");
    system("gpio pwm-ms");
    system("gpio pwmc 100");
    system("gpio pwmr 1024");

    system("gpio -g pwm 18 0");
    system("gpio -g pwm 13 0");

    digitalWrite(left_motor1_pin, LOW);
    digitalWrite(left_motor2_pin, HIGH);
    digitalWrite(right_motor1_pin, LOW);
    digitalWrite(right_motor2_pin, HIGH);

    std::string s;
    char left_command[20];
    char right_command[20];
    while (true)
    {
        std::cout << "input key" << std::endl;
        std::cin >> s;
        if (std::equal(s.begin(), s.end(), "q") == true)
        {
            break;
        }
        else
        {
            int value = stoi(s);
            std::cout << value << std::endl;
            sprintf(left_command, "gpio -g pwm 18 %d" , value);
            sprintf(right_command, "gpio -g pwm 13 %d", 0);
            system(left_command);
            system(right_command);
        }
    }
    system("gpio -g pwm 18 0");
    system("gpio -g pwm 13 0");
    return 0;
}