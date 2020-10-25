#!/usr/bin/env python

import rospy
from std_msgs.msg import Int16MultiArray

pub = rospy.Publisher('doc_ock/motor_pwm', Int16MultiArray, queue_size=10)


def callback(data):
    pub.publish(data)


def Mimic():
    rospy.init_node('Mimic', anonymous=True)
    rospy.Subscriber('doc_ock/motor_pwm_raw', Int16MultiArray, callback)
    rospy.spin()


if __name__ == '__main__':
    try:
        Mimic()
    except rospy.ROSInterruptException:
        pass
