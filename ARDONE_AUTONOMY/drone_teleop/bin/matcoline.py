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
msg = """
Reading from the keyboard  and Publishing to Twist!
  ---------------------------
up/down:       move forward/backward
left/right:    move left/right
w/s:           increase/decrease altitude
a/d:           turn left/right
t/l:           takeoff/land
r:             reset (toggle emergency state)
c:              toggle_cam_view(newly added)
q:              QUIT Auto
anything else: stop

please don't have caps lock on.                      
CTRL+c to quit                           
"""     #control portal for the user                            
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
   
    global i
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
def hover():
	twist.linear.x = 0; twist.linear.y = 0; twist.linear.z = 0
	twist.angular.x = 0; twist.angular.y = 0; twist.angular.z = 0
	pub.publish(twist)

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
        twist = Twist()
	rospy.init_node('drone_teleop')

	
	try:
		while(True):
                        i=0
			key = 	getKey()
                       
			# takeoff and landing
			if key == 'l':
				land_pub.publish(Empty())
			if key == 'r':
				reset_pub.publish(Empty())
				time.sleep(0.5)
				land_pub.publish(Empty())
			if key == 't':
				takeoff_pub.publish(Empty())
			if key == 'c':
		     		toggle_pub.publish(Empty())
			if key == 'h':
				hover()
                        if key == 'u':
		     		while(auto):
						
						
						i=i+1
						time.sleep(0.10)		
                                                twist.linear.y=0.0011*(ypos)
                                                if(i<8):
							twist.linear.x=0.1	
						else:
							twist.linear.x=0.0
						pub.publish(twist)
						if(i>=60):
							i=0
						if key == 'q':
							hover()
							auto = False			
#						turnleft()
#						time.sleep(3)
#						turnright()
#						time.sleep(3)
#						land_pub.publish(Empty())	
						#Err=0-xpos #getting the error	
						#twist.linear.y=0.0011*(ypos)#gain used 0.0009,0.0011
					
						#twist.angular.z=Err*0.01
						#f.data=twist.linear.y
						#bag.write('linear',f)	
						#print twist.linear.y	
		
					
			
			if ord(key) in move_bindings.keys():
                                key = ord(key)

			if key in move_bindings.keys():
				(lin_ang, xyz, speed) = move_bindings[key]
				setattr(getattr(twist, lin_ang), xyz, speed)
			else:
				if (key == '\x03'):
					break
			
			pub.publish(twist)

	except Exception as e:
		print e
		print repr(e)

	finally:
		twist = Twist()
		twist.linear.x = 0; twist.linear.y = 0; twist.linear.z = 0
		twist.angular.x = 0; twist.angular.y = 0; twist.angular.z = 0
		pub.publish(twist)

    		termios.tcsetattr(sys.stdin, termios.TCSADRAIN, settings)


