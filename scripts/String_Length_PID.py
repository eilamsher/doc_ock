#!/usr/bin/env python
# need to add verification that des_ang and cur_ang arent null

import rospy
import math
import numpy as np
from std_msgs.msg import Int16MultiArray
from std_msgs.msg import Float32MultiArray
num_of_joints = 3
num_of_wires = num_of_joints*3
des_x = np.zeros(num_of_joints)
des_y = np.zeros(num_of_joints)
cur_x = np.zeros(num_of_joints)
cur_y = np.zeros(num_of_joints)
error = np.zeros(num_of_wires)
last_error = np.zeros(num_of_wires)
cumm_error = np.zeros(num_of_wires)
current_length = np.zeros(num_of_wires)
last_length = np.zeros(num_of_wires)
# the order of the i wires. the first three are the first joint, and so on
wire_order = (90.0, 210.0, 330.0, 50.0, 170.0, 290.0, 10.0, 130.0, 250.0)
flag = 0
kp = 20
ki = 10
kd = 1
pid_debug = 0
max_integrator = 25
pub = rospy.Publisher('doc_ock/motor_pwm_raw', Int16MultiArray, queue_size=10)


def callback1(data):
    global cur_x
    global cur_y
    global flag
    max_pwm = 100
    # read current angles
    for i in range(0, num_of_joints):
        cur_x[i] = data.data[2 * i]
        cur_y[i] = data.data[2 * i + 1]
    # only start publishing after the first desired angles publish
    if flag:
        max_angle = 35
        # the main calculation
        w_len = PID()
        # if one of the angles exceeds maximum angle, zero all
        for i in range(0, num_of_joints):
            if (abs(cur_x[i]) > max_angle) or (abs(cur_y[i]) > max_angle):
                w_len = np.zeros(num_of_joints * 3)
        # if one of the pwm values exceeds a max val, reduce it to max val and all other values in ratio
        for i in range(0, 9):
            pwm_val = abs(w_len[i])
            if pwm_val > max_pwm:
                for j in range(0, 9):
                    w_len[j] = w_len[j] * max_pwm / pwm_val

        w_len = np.around(w_len)

        msg = Int16MultiArray()
        msg.data = w_len
        pub.publish(msg)

def callback2(data):
    global des_x
    global des_y
    global flag
    flag = 1

    for i in range(0, num_of_joints):
        des_x[i] = data.data[2 * i]
        des_y[i] = data.data[2 * i + 1]


def callback3(data):
    global kp
    global ki
    global kd
    global max_integrator
    global cumm_error
    kp = data.data[0]
    ki = data.data[1]
    kd = data.data[2]
    max_integrator = data.data[3]


def length_calc(wires, ax, ay):
    """ receives a single joint x y angles, and wires containing active wires in joint
        and returns the length of active wires
        """
    length = np.zeros(num_of_wires)
    z = 19
    r = 29.5
    factor = math.pi / 180.0
    ax = factor * ax
    ay = factor * ay

    sx = math.sin(ax)
    sy = math.sin(ay)
    cx = math.cos(ax)
    cy = math.cos(ay)
    for i in range(0, len(wires)):
        theta = wires[i] * factor
        x = r * math.cos(theta)
        y = r * math.sin(theta)
        v = np.array(
            [cy * x - x + sy * z, sx * sy * x + cx * y - y - sx * cy * z, -cx * sy * x + sx * y + cx * cy * z + z])
        length[i + (num_of_wires - len(wires))] = np.linalg.norm(v)
    return length


def length_diff():
    """ returns the length of all wires
        """
    global cur_x
    global cur_y
    global des_x
    global des_y
    global wire_order
    des_length = np.zeros(num_of_wires)
    cur_length = np.zeros(num_of_wires)
    for i in range(0, num_of_joints):
        cur_length += length_calc(wire_order[i * 3:num_of_wires], cur_x[i], cur_y[i])
        des_length += length_calc(wire_order[i * 3:num_of_wires], des_x[i], des_y[i])
    return [des_length - cur_length, cur_length]


def PID():
    """ receives a all joints x y angles, and return pwm values for all motors
        """
    global kp
    global ki
    global kd
    global error
    global last_error
    global cumm_error
    global current_length
    global last_length
    global max_integrator
    global pid_debug
    dt = 0.005

    last_error = error
    last_length = current_length
    [error, current_length] = length_diff()

    # anti windup for the integrator
    for i in range(0, num_of_wires):
        # reset integrator when passing desired angle
        if last_error[i] * error[i] <= 0:
            cumm_error[i] = 0
        # limiting integrator to a maximum value
        if abs(ki * cumm_error[i] * dt) < max_integrator:
            cumm_error[i] += error[i]

    up = kp * error
    ui = ki * cumm_error * dt
    ud = kd * (current_length - last_length) / dt
    u = up + ui + ud
    return u


def String_Length_PID():
    rospy.init_node('String_Length_PID', anonymous=True)
    rospy.Subscriber('doc_ock/current_angles', Float32MultiArray, callback1)
    rospy.Subscriber('doc_ock/desired_angles', Float32MultiArray, callback2)
    rospy.Subscriber('doc_ock/PID', Float32MultiArray, callback3)
    rospy.spin()


if __name__ == '__main__':
    try:
        String_Length_PID()
    except rospy.ROSInterruptException:
        pass
