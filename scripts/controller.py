#!/usr/bin/env python
# -*- coding: utf-8 -*-

import wiringpi as pi
import readchar
import time
import sys

#GPIO define
left_motor1_pin = 14
left_motor2_pin = 15
left_pwm_pin    = 18

right_motor1_pin = 23
right_motor2_pin = 24
right_pwm_pin    = 12

# 引数
# param = sys.argv

# 第1引数
# go : 回転
# back : 逆回転
# break : ブレーキ
# order = param[1]

# 第2引数 秒数
# second = int(param[2])

# GPIO出力モードをOUTPUTに設定する
pi.wiringPiSetupGpio()
pi.pinMode( left_motor1_pin , pi.OUTPUT )
pi.pinMode( left_motor2_pin , pi.OUTPUT )
pi.pinMode( right_motor1_pin, pi.OUTPUT )
pi.pinMode( right_motor2_pin, pi.OUTPUT )

# GPIO出力モードをPWM_OUTPUTに設定する
pi.pinMode( left_pwm_pin ,    pi.PWM_OUTPUT )
pi.pinMode( right_pwm_pin,    pi.PWM_OUTPUT )
pi.pwmSetMode(0)
pi.pwmSetRange(1024)
pi.pwmSetClock(375)
pi.pwmWrite(left_pwm_pin , 1024)
#pi.pwmWrite(right_pwm_pin, 1024)

while(True):
    print("input command: wasd")
    order  = readchar.readchar()
    sys.stdout.write(order)
    second = 0.5
    if order == "w":
        if second == 0:
            print("回転 止めるときはbreak 0コマンド！")
        else:
            print(str(second)+"秒回転")
        pi.digitalWrite( left_motor1_pin , pi.LOW )
        pi.digitalWrite( left_motor2_pin , pi.HIGH )
        pi.digitalWrite( right_motor1_pin, pi.LOW )
        pi.digitalWrite( right_motor2_pin, pi.HIGH )
        time.sleep(second)
    elif order == "d":
        if second == 0:
            print("右旋回 止めるときはbreak 0 コマンド！")
        else:
            print(str(second)+"右旋回")
        pi.digitalWrite( left_motor1_pin, pi.LOW)
        pi.digitalWrite( left_motor2_pin, pi.HIGH)
        pi.digitalWrite( right_motor1_pin, pi.HIGH)
        pi.digitalWrite( right_motor2_pin, pi.LOW)
        time.sleep(second)    
    elif order == "a":
        if second == 0:
            print("左旋回 止めるときはbreak 0 コマンド！")
        else:
            print(str(second)+"左旋回")
        pi.digitalWrite( left_motor1_pin, pi.HIGH)
        pi.digitalWrite( left_motor2_pin, pi.LOW)
        pi.digitalWrite( right_motor1_pin, pi.LOW)
        pi.digitalWrite( right_motor2_pin, pi.HIGH)
        time.sleep(second)    
    elif order == "s":
        if second == 0:
            print("逆回転 止めるときはbreak 0コマンド！")
        else:
            print(str(second)+"秒逆回転")    
        pi.digitalWrite( left_motor1_pin , pi.HIGH )
        pi.digitalWrite( left_motor2_pin , pi.LOW )
        pi.digitalWrite( right_motor1_pin, pi.HIGH )
        pi.digitalWrite( right_motor2_pin, pi.LOW )
        time.sleep(second)
    elif order == "q":
        break
    else:
        pass

    # 第2引数が0の場合は、ブレーキをしない
    # 第1引数がbreakの場合は、ブレーキ
    if order == "break" or second != 0:
        print("ブレーキ！")
        pi.digitalWrite( left_motor1_pin , pi.HIGH )
        pi.digitalWrite( left_motor2_pin , pi.HIGH )
        pi.digitalWrite( right_motor1_pin, pi.HIGH )
        pi.digitalWrite( right_motor2_pin, pi.HIGH )

