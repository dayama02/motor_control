#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <iostream>
// #include "/home/ubuntu/package/wiringpi-2.50/wiringPi/wiringPi.h"
#include <wiringPi.h>

// pin assign
#define left_motor1_pin 23
#define left_motor2_pin 24
#define left_pwm_pin 18

#define right_motor1_pin 14
#define right_motor2_pin 15
#define right_pwm_pin 12

typedef struct WHEEL_VELOCITY{
  double v_l;
  double v_r;
} WheelVelocity;

WheelVelocity CalcWheelVelocity(double k, double x, double y){
  double theta = atan2(y, x);
  double v = k * hypot(x, y); // k * sqrt(x*x + y*y);

  WheelVelocity wv;
  wv.v_l = v * cos(theta - M_PI / 4.0);
  wv.v_r = v * sin(theta - M_PI / 4.0);

  return wv;
}
// コールバック関数。並進、回転速度の表示。
void callback(const geometry_msgs::Twist::ConstPtr &vel)
{
  ROS_INFO("callback");
  WheelVelocity wv = CalcWheelVelocity(320, vel->angular.z, vel->linear.x);
  
  int v_l = round(wv.v_l);
  int v_r = round(wv.v_r);
  char left_command[20];
  char right_command[20];
  
  // printf("x, %f, y, %f, L, %f, R, %f\r\n", 
  //        vel->angular.z, vel->linear.x, (wv.v_l), (wv.v_r));

  if(wv.v_l > 0.0){
    digitalWrite(left_motor1_pin , LOW);
    digitalWrite(left_motor2_pin , HIGH);
    sprintf(left_command , "gpio -g pwm 18 %d", v_l+704);   
     
  }
  else if(wv.v_l < 0.0){
    digitalWrite(left_motor1_pin , HIGH);
    digitalWrite(left_motor2_pin , LOW);
    sprintf(left_command, "gpio -g pwm 18 %d", -v_l+704);    
  }
  else{
    digitalWrite(left_motor1_pin, LOW);
    digitalWrite(left_motor2_pin, LOW);      
    sprintf(left_command, "gpio -g pwm 18 %d", 0);
  }

  if(wv.v_r > 0.0){
    digitalWrite(right_motor1_pin, LOW);
    digitalWrite(right_motor2_pin, HIGH);
    sprintf(right_command , "gpio -g pwm 13 %d", v_r+704);    
  }
  else if(wv.v_r < 0.0){
    digitalWrite(right_motor1_pin , HIGH);
    digitalWrite(right_motor2_pin , LOW);
    sprintf(right_command, "gpio -g pwm 13 %d", -v_r+704);    
  }
  else{
    digitalWrite(right_motor1_pin, LOW);
    digitalWrite(right_motor2_pin, LOW);    
    sprintf(right_command, "gpio -g pwm 13 %d", 0);
  }
  printf("%s\n", left_command);
  printf("%s\n", right_command);
  system(left_command);
  system(right_command);
}

int main(int argc, char **argv)
{
  std::cout << "motor_control_node start" << std::endl;
  ros::init(argc, argv, "motor_control_node");
  ros::NodeHandle nh;

  // GPIOの初期化
  if (wiringPiSetupGpio() == -1)
  {
    ROS_ERROR("GPIO setup failed");
    return -1;
  }

  // GPIO出力モードをOUTPUTに設定する
  wiringPiSetupSys();
  pinMode(left_motor1_pin, OUTPUT);
  pinMode(left_motor2_pin, OUTPUT);
  pinMode(right_motor1_pin, OUTPUT);
  pinMode(right_motor2_pin, OUTPUT);

  // GPIO出力モードをPWM_OUTPUTに設定する
  ROS_INFO("PWM Output");
  system("gpio -g mode 18 pwm"); // Pin assign
  system("gpio -g mode 13 pwm"); // Pin assign
  system("gpio pwm-ms");         // mark-space mode
  system("gpio pwmc 100");       // clock
  system("gpio pwmr 1024");      // range

  system("gpio -g pwm 18 0");
  system("gpio -g pwm 13 0");

  //subscriberの作成。トピック/cmd_velを購読する。バッファ数は10。
  ros::Subscriber sub = nh.subscribe("/cmd_vel", 10, callback);

  // コールバック関数を繰り返し呼び出す。whileループが必要な場合はspinOnce()を使う。
  ros::spin();

  return 0;
}
