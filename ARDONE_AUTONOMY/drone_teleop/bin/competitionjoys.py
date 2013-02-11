#!/usr/bin/env python
import roslib; roslib.load_manifest('drone_teleop')
import rospy
import rosbag
from geometry_msgs.msg import Twist
from std_msgs.msg import Empty
from std_msgs.msg import Float64
from turtlesim.msg import Velocity
from turtlesim.msg import Pose
from sensor_msgs.msg import Joy
#from joy.msg import Joy

import sys, select, termios, tty
import time
msg = """
Reading from the keyboard  and Publishing to Twist!
  ---------------------------
up/down:       move forward/backward
left/right:    move left/right
w/s:           increase/decrease altitude
a/d:           turn left/right
1 / 2:           takeoff / land
4:             reset (toggle emergency state)
3:              toggle_cam_view(newly added)
6:		HOVER (right side top trigger)
q:              QUIT Auto


anything else: stop

please don't have caps lock on.                      
CTRL+c to quit                           
"""     #control portal for the user                            
xpos=0
ypos=0
xdis=0
k=0
k1=0;
joy_axes=[0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
joy_buttons= [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
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
motiontrigger =0
def callback(RecMsg):
    
    global xpos #displacement from the bottom line to be followed
    global ypos		
    global xdis	
    global k	
    xpos = RecMsg.linear
    ypos = RecMsg.angular
    global k1
    global i
def callback1(laserp):
   
    global xdis	
    xdis=  laserp.x	
    
def callback2(joystick_cmd):
	global joy_axes
	global joy_buttons
	joy_axes = joystick_cmd.axes
	joy_buttons=joystick_cmd.buttons

def turnleft():
    twist.angular.z=1
    pub.publish(twist)	
    time.sleep(1)	
    twist.angular.z=0
    pub.publish(twist)		

def turnright():
    twist.angular.z=-1
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
def hover():
	twist.linear.x = 0; twist.linear.y = 0; twist.linear.z = 0
	twist.angular.x = 0; twist.angular.y = 0; twist.angular.z = 0
	pub.publish(twist)
def motion():
	twist.linear.x = 0.1*joy_axes[5] #forward(+) and backward(-)(pitch) 
	twist.linear.y = 0.1*joy_axes[2] #lateral movement::LEFT(1):RIGHT(-1)(ROLL)
	twist.linear.z = 0.1*joy_axes[3] #altitude movement:UP(+):DOWN(-)
	twist.angular.x = 0 
	twist.angular.y = 0 
	twist.angular.z = 0.1*joy_axes[4]#angular::left(+)::right(-)	
	pub.publish(twist)
def circledetect():
	hover()
	toggle_pub.publish(Empty())
	time.sleep(0.25)
	count=0
	circlefound=0
	while(count<40):
		if xdis==1:
			circlefound=circlefound+1
			hover()
			
		if circlefound>10:
			turnright()
			toggle_pub.publish(Empty())	
			time.sleep(0.25)
			hover()
			count=40
		if count == 39:
			k=0
			toggle_pub.publish(Empty())	
			time.sleep(0.25)
			hover()
		count=count+1
		
	
if __name__=="__main__":
    	settings = termios.tcgetattr(sys.stdin)
	print msg
	
	auto = True
	pub = rospy.Publisher('cmd_vel', Twist)
	land_pub = rospy.Publisher('/ardrone/land', Empty)
	reset_pub = rospy.Publisher('/ardrone/reset', Empty)
        toggle_pub=rospy.Publisher('/ardrone/togglecam', Empty)
	takeoff_pub =rospy.Publisher('/ardrone/takeoff', Empty)
	rospy.Subscriber('/drocanny/vanishing_points',Velocity,callback)
	rospy.Subscriber('/drone/walldis',Pose,callback1)
	rospy.Subscriber('/joy',Joy,callback2)
        twist = Twist()
	rospy.init_node('drone_teleop')
	
	
	try:
		while(True):
                        i=0
			#print 'manual mode::'
			#key = 	getKey()
                        buttonvalue=  joy_buttons
			if buttonvalue[1] == 1:
				land_pub.publish(Empty())
			if buttonvalue[3] == 1:
				print k
				reset_pub.publish(Empty())
				time.sleep(1.5)
				land_pub.publish(Empty())
				
			if buttonvalue[0] == 1:				
				takeoff_pub.publish(Empty())
			if buttonvalue[2] == 1:
		     		toggle_pub.publish(Empty())
				time.sleep(0.25)
			if buttonvalue[7]==1:
				time.sleep(0.25)
				print 'autonomous node'			
				while(True):
					k=k+1
					if(k==98):
						turnright()
					if(k==133):
						turnright()
					if(k==153):
						turnleft()
					if(k==173):
						turnright()	
					buttonvalue= joy_buttons					
														
					i=i+1
					time.sleep(0.10)
					
					if(i%2 < 1):		
                                       		twist.linear.y=0.0011*(ypos)
					else:
						twist.linear.y=0.0

					
                                        if(i%2 > 0):# 20
						twist.linear.x=0.1	
					else:
						twist.linear.x=0.0
						twist.linear.y=0.0
					pub.publish(twist)
					#if(xdis==1 and i>=7):
					#	print 'hover******************************************QUIT AUTO******* MANUAL MODE!'
					#	twist.linear.x=-0.1
					#	pub.publish(twist)
					#	time.sleep(0.5)
					#	hover()	
					#	break	
					if(i>=10):
						i=0
					if(buttonvalue[7]==1):
						print 'quit AUTO \n Manual MODE'
						break
					if buttonvalue[2] == 1:
		     				toggle_pub.publish(Empty())
						time.sleep(0.25)
				time.sleep(0.25)	
			if buttonvalue[5] == 1:
				hover()
			
			# takeoff and landing

			motion()
			#pub.publish(twist)

	except Exception as e:
		print e
		print repr(e)

	finally:
		twist = Twist()
		twist.linear.x = 0; twist.linear.y = 0; twist.linear.z = 0
		twist.angular.x = 0; twist.angular.y = 0; twist.angular.z = 0
		pub.publish(twist)

    		termios.tcsetattr(sys.stdin, termios.TCSADRAIN, settings)


