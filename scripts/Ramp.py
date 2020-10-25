#!/usr/bin/env python
# need to add verification that des_ang and cur_ang arent null

import rospy
import numpy as np
from std_msgs.msg import Float32MultiArray
num_of_joints = 3
num_of_wires = num_of_joints*3
des = np.zeros(6)
cur = np.zeros(6)
des_ramp = np.zeros(6)
initial_angles = np.zeros(6)
flag = 0
pub_flag = 0
ang_flag = np.ones(6)
epsilon = 0
ramp_angle = 0.005
msg = Float32MultiArray()
pub = rospy.Publisher('doc_ock/desired_angles', Float32MultiArray, queue_size=10)


def callback1(data):
    global cur
    global des
    global des_ramp
    global flag
    global pub_flag
    global ang_flag
    global msg
    global initial_angles
    for i in range(0, 6):
        cur[i] = data.data[i]
    if flag:
        if pub_flag:
            for i in range(0, 6):
                if ang_flag[i]:
                    diff = des_ramp[i] - des[i]
                    if abs(diff) > epsilon:
                        direction = np.sign(diff)
                        des[i] = des[i] + direction * ramp_angle
                        # check if it the ramp has reached the desired angle
                        if (direction > 0 and des[i] > des_ramp[i]) or (direction < 0 and des[i] < des_ramp[i]):
                            des[i] = des_ramp[i]
                else:
                    des[i] = initial_angles[i]

            msg.data = des
            pub.publish(msg)
        else:
            for i in range(0, 6):
                if ang_flag[i]:
                    diff = des_ramp[i] - cur[i]
                    if abs(diff) > epsilon:
                        direction = np.sign(diff)
                        des[i] = cur[i] + direction * ramp_angle
                        print(cur[i])
                        print(des[i])
                else:
                    des[i] = initial_angles[i]
            pub_flag = 1
            msg.data = des
            pub.publish(msg)
    else:
        for i in range(0, 6):
            initial_angles[i] = data.data[i]
            flag = 1


def callback2(data):
    global des_ramp
    global flag
    global initial_angles
    global ang_flag

    for i in range(0, 6):
        des_ramp[i] = data.data[i]
        if (des_ramp[i] != 0) and not ang_flag[i]:
            ang_flag[i] = 1


def Ramp():
    rospy.init_node('Ramp', anonymous=True)
    rospy.Subscriber('doc_ock/current_angles', Float32MultiArray, callback1)
    rospy.Subscriber('doc_ock/desired_ramp_angles', Float32MultiArray, callback2)
    rospy.spin()


if __name__ == '__main__':
    try:
        Ramp()
    except rospy.ROSInterruptException:
        pass
