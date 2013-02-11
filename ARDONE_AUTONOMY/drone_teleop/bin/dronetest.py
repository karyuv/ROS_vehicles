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
anything else: stop

please don't have caps lock on.
CTRL+c to quit
"""

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
	takeoff_pub = rospy.Publisher('/ardrone/takeoff', Empty)
        toggle_pub=rospy.Publisher('/ardrone/togglecam', Empty)
        count=1
	rospy.init_node('drone_teleop')

	
	while(True):
                takeoff_pub.publish(Empty())	       
		toggle_pub.publish(Empty())
                # takeoff and landing
		
                count=count+1         
		twist = Twist()
                if(count>401234):
			twist.angular.z =-0.25
                        print count
			if(count>801234):
				land_pub.publish(Empty())
                   	
                else:                        
			twist.angular.z =0.25
			
		
			
		#print
		pub.publish(twist)
    		termios.tcsetattr(sys.stdin, termios.TCSADRAIN, settings)


