#!/usr/bin/env python
import roslib; roslib.load_manifest('drone_teleop')
import rospy

from geometry_msgs.msg import Twist
from std_msgs.msg import Empty

import sys, select, termios, tty

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
anything else: stop

please don't have caps lock on.                      
CTRL+c to quit                           
"""     #control portal for the user                            
xpos=0
ypos=0
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
    
    global xpos #X coordinate of the vanishing point
    global ypos	#y coordinate of the vanishing point given out by dronecanny.cppp	
    xpos = RecMsg.linear
    ypos = RecMsg.angular

def getKey():
	tty.setraw(sys.stdin.fileno())
	select.select([sys.stdin], [], [], 0)
	key = sys.stdin.read(1)
	termios.tcsetattr(sys.stdin, termios.TCSADRAIN, settings)
	return key

if __name__=="__main__":
    	settings = termios.tcgetattr(sys.stdin)
	print msg
	
	pub = rospy.Publisher('cmd_vel', Twist)
	land_pub = rospy.Publisher('/ardrone/land', Empty)
	reset_pub = rospy.Publisher('/ardrone/reset', Empty)
        toggle_pub=rospy.Publisher('/ardrone/togglecam', Empty)
	takeoff_pub =rospy.Publisher('/ardrone/takeoff', Empty)
        count3=0
	rospy.init_node('drone_teleop')
	twist=Twist()	
	(lin_ang, xyz, speed) = ('linear', 'x', 0.0)
	setattr(getattr(twist, lin_ang), xyz, speed)
	pub.publish(twist)
	try:
		while(True):
                       	
			while(count3<=50):		
				toggle_pub.publish(Empty())
				count3=count3+1
				print count3			
						
			Err=xpos-160.	
			k=1./320.
			#(lin_ang, xyz, speed) = ('linear', 'x', 0.0)
			#setattr(getattr(twist, lin_ang), xyz, speed)
			#pub.publish(twist)
			#(lin_ang, xyz, speed) = ('angular', 'z', 0.0)
			#setattr(getattr(twist, lin_ang), xyz, speed)
			#pub.publish(twist)
			count1=0
			(lin_ang, xyz, speed) = ('angular', 'x',0.1)
			setattr(getattr(twist, lin_ang), xyz, speed)
			pub.publish(twist)
			(lin_ang, xyz, speed) = ('angular', 'z', 0.0)
			setattr(getattr(twist, lin_ang), xyz, speed)
			pub.publish(twist)
			(lin_ang, xyz, speed) = ('linear', 'x', 0.0)
			setattr(getattr(twist, lin_ang), xyz, speed)
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


