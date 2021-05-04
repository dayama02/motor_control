#!/usr/bin/env python
# -*- coding: utf-8 -*-
import rospy
import rosparam
from std_msgs.msg import String
from geometry_msgs.msg import Twist

def callback(data):
    rospy.loginfo("recieved %1.1f %1.1f", data.linear.x, data.angular.z)
    now = rospy.Time.now()
    rospy.loginfo("now: %f", now.to_sec())

def listener():
    rospy.init_node('listener')
    rospy.Subscriber("/cmd_vel", Twist, callback)
    rospy.spin()

if __name__ == '__main__':
    listener()