#!/usr/bin/env python
# need to add verification that des_ang and cur_ang arent null

import rospy
import math
import numpy as np
from std_msgs.msg import Int32MultiArray
from std_msgs.msg import Int16MultiArray

pub = rospy.Publisher('doc_ock/motor_pwm', Int16MultiArray, queue_size=10)
# this topic matches the length of the tension topic to the current_angles, for plotting
pub_tension = rospy.Publisher('doc_ock/string_tension_long', Int32MultiArray, queue_size=10)

flag = 0
tension = np.zeros(9)
# the minimum tension value for each joint
delta = [600, 400, 200]
# tension ref is for when there is a constant error in a specific tension reading, so you can publish to it
# and add a certain value
tension_ref = np.zeros(9)


def callback1(data):
    global flag
    global tension_ref
    global tension
    global delta
    pwm = np.zeros(9)
    # implementation of the tension term - if the tension is lower then delta, and the motor
    # is pushing, the value will remain zero
    for i in range(0, 9):
        if (delta[math.floor(i / 3)] < tension[i] + tension_ref[i]) or (data.data[i] < 0):
            pwm[i] = data.data[i]

    msg = Int16MultiArray()
    msg.data = pwm
    pub.publish(msg)

    msg2 = Int32MultiArray()
    msg2.data = tension
    pub_tension.publish(msg2)


def callback2(data):
    global tension
    tension = data.data


def callback3(data):
    global delta
    for i in range(0, 3):
        delta[i] = data.data[i]


def callback4(data):
    global tension_ref
    for i in range(0, 9):
        tension_ref[i] = data.data[i]


def String_Tension_Control():
    rospy.init_node('String_Tension_Control', anonymous=True)
    rospy.Subscriber('doc_ock/motor_pwm_raw', Int16MultiArray, callback1)
    rospy.Subscriber('doc_ock/string_tension', Int32MultiArray, callback2)
    rospy.Subscriber('doc_ock/delta', Int16MultiArray, callback3)
    rospy.Subscriber('doc_ock/tension_ref', Int32MultiArray, callback4)
    rospy.spin()


if __name__ == '__main__':
    try:
        String_Tension_Control()
    except rospy.ROSInterruptException:
        pass
