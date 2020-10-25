#!/usr/bin/env python

import rospy
import numpy as np
from std_msgs.msg import Float32MultiArray
from std_msgs.msg import Float32

mat = np.array(((0.0, 0.0, 0.0, 0.0, 0.0, 0.0), (0.0, 0.0, 0.0, 0.0, 0.0, 30.0), (0.0, 0.0, 0.0, 0.0, 0.0, -30.0), (0.0, 0.0, 0.0, 0.0, 0.0, 0.0),
                (0.0, 0.0, 0.0, 0.0, 30.0, 0.0), (0.0, 0.0, 0.0, 0.0, -30.0, 0.0), (0.0, 0.0, 0.0, 0.0, 0.0, 0.0),))

mat2 = np.array(((0.0, 0.0, 0.0, 0.0, 0.0, 0.0), (0.0, 0.0, 0.0, 0.0, 30.0, 30.0), (0.0, 0.0, 0.0, 0.0, 30.0, -30.0), (0.0, 0.0, 0.0, 0.0, -30.0, -30.0),
                (0.0, 0.0, 0.0, 0.0, -30.0, 30.0), (0.0, 0.0, 0.0, 0.0, 0.0, 0.0)))

pub = rospy.Publisher('doc_ock/desired_ramp_angles', Float32MultiArray, queue_size=10)
index = 0
des_ang = mat[0]
first_flag = 0
msg = Float32MultiArray()
epsilon = 1


def callback1(data):
    global mat
    global index
    global des_ang
    global first_flag
    global msg
    global epsilon

    flag = 1

    if first_flag and index < 7:
        for i in range(4, 6):
            if abs(data.data[i] - des_ang[i]) > epsilon:
                flag = 0
        if flag:
            des_ang = mat2[index]
            index += 1
            msg.data = des_ang
            pub.publish(msg)


def callback2(data):
    global epsilon
    global first_flag
    epsilon = data.data
    first_flag = 1


def One_Joint_Pattern():
    rospy.init_node('One_Joint_Pattern', anonymous=True)
    rospy.Subscriber('doc_ock/current_angles', Float32MultiArray, callback1)
    rospy.Subscriber('doc_ock/epsilon', Float32, callback2)
    rospy.spin()


if __name__ == '__main__':
    try:
        One_Joint_Pattern()
    except rospy.ROSInterruptException:
        pass
