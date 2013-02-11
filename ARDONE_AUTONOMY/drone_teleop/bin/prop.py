#!/usr/bin/env python
import roslib; roslib.load_manifest('drone_teleop')
import rospy

from geometry_msgs.msg import Twist
from std_msgs.msg import Empty
from std_msgs.msg import Float64
from turtlesim.msg import Velocity
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
	count=1
	vxxy=1
	Err=0.
	k=0.
	count2=0
	count1=0
	count3=0
	e=0
	rospy.init_node('drone_teleop')
	pub = rospy.Publisher('cmd_vel', Twist)
	land_pub = rospy.Publisher('/ardrone/land', Empty)
	reset_pub = rospy.Publisher('/ardrone/reset', Empty)
	takeoff_pub = rospy.Publisher('/ardrone/takeoff', Empty)
      	rospy.Subscriber('/drocanny/vanishing_points',Velocity,callback)
        toggle_pub=rospy.Publisher('/ardrone/togglecam', Empty)
	print msg
	
		
	
	while(True):
			
			
               		takeoff_pub.publish(Empty())	  
			count2=count2+1 
			twist=Twist()
			twist.linear.x=0.2		
			if(count2>98000):
				
				
								 
				Err=xpos-160.
				
			        k=1./320.
			        #print k
				while(True):
					count3=count3+1
					#twist.linear.x=0.0
					print'gate1'
					twist.linear.x=0.3
                	        	twist.angular.z=Err*k # PROPORTIONAL CONTROLLER DESGINED FOR FOLLOWING
					
			        	print twist.angular.z 
					if(count3>98000):
					  	count3=0;
					  	break
				count=count+1			          
				while(True):   
			        	count1=count1+1 
					print'gate2'  
					twist.angular.z=0
					twist.linear.x=0.1
	 			       	if(count1>10000):
					  	count1=0
					  	break
                  	#if(count>130000):
				#print 'hello'
				#land_pub.publish(Empty())
				
			#twist.angular.z =-0.25
                   	#     print count
			#if(count>801234):
			#	land_pub.publish(Empty())
                   	
               		 #else:                        
			#	twist.angular.z =0.25
			
		
			
		#print
			pub.publish(twist)
    			termios.tcsetattr(sys.stdin, termios.TCSADRAIN, settings)


