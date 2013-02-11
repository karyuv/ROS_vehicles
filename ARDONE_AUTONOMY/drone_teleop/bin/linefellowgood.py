#!/usr/bin/env python
import roslib; roslib.load_manifest('drone_teleop')
import rospy
import rosbag
from geometry_msgs.msg import Twist
from std_msgs.msg import Empty
from std_msgs.msg import Float64
from turtlesim.msg import Velocity
from turtlesim.msg import Pose

import sys, select, termios, tty
import time

xpos=0
ypos=0
xdis=0
move_bindings = {
		68:('linear', 'y', 0.1), #left
		67:('linear', 'y', -0.1), #right
		65:('linear', 'x', 0.1), #forward
		66:('linear', 'x', -0.1), #back
		'w':('linear', 'z', 0.1),
		's':('linear', 'z', -0.1),
		'a':('angular', 'z', 1),
		'd':('angular', 'z', -1),
	       }

def callback(RecMsg):
    
    global xpos #displacement from the bottom line to be followed
    global ypos		
    global xdis	
    xpos = RecMsg.linear
    ypos = RecMsg.angular
    
    	
def callback1(laserp):
   
    global xdis	
    xdis=  laserp.x	
    
def turnleft():
    twist.angular.z=-1
    pub.publish(twist)	
    time.sleep(1)	
    twist.angular.z=0
    pub.publish(twist)		

def turnright():
    twist.angular.z=1
    pub.publish(twist)	
    time.sleep(1)	
    twist.angular.z=0
    pub.publish(twist)		
				
def getKey():
	tty.setraw(sys.stdin.fileno())
	select.select([sys.stdin], [], [], 0)
	key = sys.stdin.read(1)
	termios.tcsetattr(sys.stdin, termios.TCSADRAIN, settings)
	return key

if __name__=="__main__":
    	settings = termios.tcgetattr(sys.stdin)
	count=1
	vxxy=1
	Err=0.
	k=0.
	count2=0
	count1=0
	count3=0
	count4=0
	count5=0
	e=1
	rospy.init_node('drone_teleop')
	pub = rospy.Publisher('cmd_vel', Twist)
	land_pub = rospy.Publisher('/ardrone/land', Empty)
	reset_pub = rospy.Publisher('/ardrone/reset', Empty)
	takeoff_pub = rospy.Publisher('/ardrone/takeoff', Empty)
      	rospy.Subscriber('/drocanny/vanishing_points',Velocity,callback)
	rospy.Subscriber('/drone/walldis',Pose,callback1)
        toggle_pub=rospy.Publisher('/ardrone/togglecam', Empty)
	time.sleep(5)	
	toggle_pub.publish(Empty())
	time.sleep(5)
	takeoff_pub.publish(Empty())
	time.sleep(5)
	twist=Twist()
	twist.linear.x = 0; twist.linear.y = 0; twist.linear.z = 0
	twist.angular.x = 0; twist.angular.y = 0; twist.angular.z = 0
	pub.publish(twist)
	
	
	while(true):
			
			

			time.sleep(0.015)
			twist.linear.y=0.0011*(ypos)#gain used 0.0009,0.0011
			twist.linear.x=0.1
			print twist.linear.y		
			pub.publish(twist)
			time.sleep(0.015)
			twist.linear.x=0.0
			pub.publish(twist)	
		
			
						
			
			
			
			
			
			
	
	termios.tcsetattr(sys.stdin, termios.TCSADRAIN, settings)





