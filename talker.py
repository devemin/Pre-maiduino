#!/usr/bin/env python

import rospy
from sensor_msgs.msg import JointState
from std_msgs.msg import Header

def talker():
    pub = rospy.Publisher('/joint_command', JointState, queue_size=10)
    rospy.init_node('joint_state_publisher')
    rate = rospy.Rate(10) # 10hz
    hello_str = JointState()
    hello_str.header = Header()
    hello_str.name = ['head_nx_joint', 'head_nx_joint', 'head_nx_joint', 'l_shy_joint']
    hello_str.position = [0.3, 0.5418, -1.7297, -0.3017]
    hello_str.velocity = []
    hello_str.effort = []
    i = -30

    while not rospy.is_shutdown():
      hello_str.header.stamp = rospy.Time.now()
      hello_str.position = [i*0.01, i*0.01, i*0.01, i*0.01 ]
      i += 5
      if i >= 30:
        i = -30
      pub.publish(hello_str)
      rate.sleep()

if __name__ == '__main__':
    try:
        talker()
    except rospy.ROSInterruptException:
        pass
