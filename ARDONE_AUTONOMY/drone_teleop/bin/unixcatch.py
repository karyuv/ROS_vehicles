#!/usr/bin/env python

import os, sys
import tty
from select import select

import roslib; roslib.load_manifest('drone_teleop')
import rospy
from std_msgs.msg import String

class NotTTYException(Exception): pass

class TerminalFile:
    def __init__(self,infile):
        if not infile.isatty():
            raise NotTTYException()
        self.file=infile

        #prepare for getch
        self.save_attr=tty.tcgetattr(self.file)
        newattr=self.save_attr[:]
        newattr[3] &= ~tty.ECHO & ~tty.ICANON
        tty.tcsetattr(self.file, tty.TCSANOW, newattr)

    def __del__(self):
        #restoring stdin
        import tty  #required this import here
        tty.tcsetattr(self.file, tty.TCSADRAIN, self.save_attr)

    def getch(self):
        if select([self.file],[],[],0)[0]:
            c=self.file.read(1)
        else:
            c=''
        return c




def talker():
    pub = rospy.Publisher('keystrokesCatcher', String)
    rospy.init_node('talker')
    s=TerminalFile(sys.stdin)    
    
    while not rospy.is_shutdown():
	
        str = s.getch() #"%s"%s.getch()
        rospy.loginfo(str)
        pub.publish(String(str))
        rospy.sleep(0.01)
	
if __name__ == '__main__':
    try:
        talker()
    except rospy.ROSInterruptException: pass
