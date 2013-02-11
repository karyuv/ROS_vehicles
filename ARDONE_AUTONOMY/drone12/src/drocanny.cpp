#include <ros/ros.h>
#include <stdio.h>
#include <iostream>
#include "std_msgs/String.h"
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>     //make sure to include the relevant headerfiles
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <cv_bridge/CvBridge.h>
#include <cvaux.h>
#include<math.h>
#include <cxcore.h>
#include "turtlesim/Velocity.h"

/*here is a simple program which demonstrates the use of ros and opencv to do image manipulations on video streams given out as image topics from the monocular vision
of robots,here the device used is a ardrone(quad-rotor).*/
using namespace std;
using namespace cv;
namespace enc = sensor_msgs::image_encodings;

static const char WINDOW[] = "Image window";

float prevVelocity_angular ,prevVelocity_linear ,newVelocity_angular ,newVelocity_linear ;
float derive_angular, derive_linear, dt = 0.5;
float horizontalcount;

class ImageConverter
{
  ros::NodeHandle nh_;
  ros::NodeHandle n;
 ros::Publisher pub ;
  ros::Publisher tog;
  image_transport::ImageTransport it_;    
  image_transport::Subscriber image_sub_; //image subscriber 
  image_transport::Publisher image_pub_; //image publisher(we subscribe to ardrone image_raw)
  std_msgs::String msg;
public:
  ImageConverter()
    : it_(nh_)
  {
      
      //tog= nh_.advertise("/ardrone/togglecam", 1);
      pub= n.advertise<turtlesim::Velocity>("/drocanny/vanishing_points", 500);//
      image_sub_ = it_.subscribe("/ardrone/image_raw", 1, &ImageConverter::imageCb, this);
      image_pub_= it_.advertise("/arcv/Image",1);
      
     // cvNamedWindow("Hough",CV_WINDOW_NORMAL);
  }

  ~ImageConverter()
  {
    cv::destroyWindow(WINDOW);
  }

  void imageCb(const sensor_msgs::ImageConstPtr& msg)
  {

     sensor_msgs::CvBridge bridge;//we need this object bridge for facilitating conversion from ros-img to opencv
   IplImage* img = bridge.imgMsgToCv(msg,"rgb8");  //image being converted from ros to opencv using cvbridge
  // cvCircle( img,cvPoint(50, 50), 10, CV_RGB(255,200,0)); //drawing circle on the video
   turtlesim::Velocity velMsg;

      

       CvMemStorage* storage = cvCreateMemStorage(0);
        
        CvSeq* lines = 0;
       int i,c,d;
       float c1[50]; 
       float m,angle;
	float buf;
	float m1;
       float dis;
       int k=0,k1=0; 
      int count=0;  
   //  float m1[50];
      float xv;
      float yv;
      int vc=0;
     float xvan=0,yvan=0;
      static float xvan1=0,yvan1=0;
    float num=0;
   static float count1=0;
  float dv;
float vxx,vyy;
        //printf("WQRWERTEWRWGTWTGFWETWETEWTWETWETWETWE");
         cvSetImageROI(img, cvRect(0, 0,170, 140));
        IplImage* out1 = cvCreateImage( cvGetSize(img), IPL_DEPTH_8U, 3 );   //make sure to feed the image(img) data to the parameters necessary for canny edge output 
        IplImage* gray_out = cvCreateImage( cvGetSize(img), IPL_DEPTH_8U, 1 ); 
        IplImage* canny_out = cvCreateImage( cvGetSize(img), IPL_DEPTH_8U, 1 );
        IplImage* gray_out1=cvCreateImage( cvGetSize(img), IPL_DEPTH_8U, 3 );
	IplImage* canny_out1 = cvCreateImage( cvGetSize(img), IPL_DEPTH_8U, 1 );
	IplImage* canny_out2 = cvCreateImage( cvGetSize(img), IPL_DEPTH_8U, 1 );
               
        cvSmooth( img, out1, CV_GAUSSIAN, 11, 11 );
        
      cvCvtColor(out1 , gray_out, CV_RGB2GRAY);
        cvCanny( gray_out, canny_out, 50, 125, 3 );
      cvCvtColor(canny_out ,gray_out1, CV_GRAY2BGR);


//cvCircle(out1,cvPoint(165,250), 5, cvScalar(0,255,0),1);
       lines = cvHoughLines2( canny_out, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI/180, 80,50, 10 );
        for( i = 0; i < lines->total; i++ )
        {    
	    // cvLine( out1, line[0], line[1], CV_RGB(0,255,0), 2, 8 );
             CvPoint* line = (CvPoint*)cvGetSeqElem(lines,i);
		/*if(line[0].x > 150 && line[1].x>150)
			{
			continue;
			}
		if(line[0].x < 40 && line[0].x<40 )
			{
			continue;
			}
               if(abs(line[0].y-line[1].y)<=80)
                    {
			horizontalcount = 1;
                   continue;
                    }	
		     cvLine( out1, line[0], line[1], CV_RGB(0,255,0), 2, 8 );*/
			
  /* if(line[0].x-line[1].x == 0)
{
  printf("\n line goes from::(%d,%d)to(%d,%d)\n and the slope is inf",line[0].x,line[0].y,line[1].x,line[1].y,m);
cvLine( out1, line[0], line[1], CV_RGB(0,255,0), 2, 8 );
cvLine( gray_out1, line[0], line[1], CV_RGB(0,255,0), 2, 8 );
xv=line[0].x-line[1].x;
yv=line[0].y-line[1].y;
velMsg.linear = atan2(xv,-yv)*180 /3.14159265;
velMsg.angular =0;
pub.publish(velMsg);
pub.publish(velMsg);
}*/
//else
{           
//m=((float)(line[0].y-line[1].y)/(line[0].x-line[1].x));
//m1=abs(m);
//if(m1 > 5)
{
cvLine( out1, line[0], line[1], CV_RGB(0,255,0), 1, 8 );
cvLine( gray_out1, line[0], line[1], CV_RGB(0,255,0), 2, 8 );
//printf("\n ***GOOD**line goes from::(%d,%d)to(%d,%d)\n and the slope is %f",line[0].x,line[0].y,line[1].x,line[1].y,m);
xv=line[0].x-line[1].x;
yv=line[0].y-line[1].y;
velMsg.linear = atan2(xv,yv)*180 /3.14159265;
angle=velMsg.linear;
if(velMsg.linear<-90)
{
  velMsg.linear=velMsg.linear+180;
}
buf=(line[0].x+line[1].x)/2;

if(abs(85-buf)<=15)
{
velMsg.angular =0;
}
else
{
velMsg.angular =(85-(line[0].x+line[1].x)/2);
}

//////////////////////////////////////////////////////////////////////////////////////  Derivative control
//newVelocity_angular = velMsg.angular;
//newVelocity_linear = velMsg.linear;

//derive_angular = (prevVelocity_angular - newVelocity_angular )/dt;
//derive_angular = (prevVelocity_linear  - newVelocity_linear  )/dt;
/////////////////////////////////////////////////////////////////////////////


///   |    velMsg.ang:: 0 ::velMsg.lin::180::

////   _   velMsg.ang::90 ::velMsg.lin :: 90


if(abs(velMsg.angular)>50)
{
velMsg.angular =0;
}





printf("\nX::Y::X1::Y1::%d:%d:%d:%d",line[0].x,line[0].y,line[1].x,line[1].y);

pub.publish(velMsg);

//prevVelocity_angular = newVelocity_angular;
//prevVelocity_linear = newVelocity_linear;
} 

/*if(m1 < 3)
{
cvLine( out1, line[0], line[1], CV_RGB(255,0,0), 1, 8 );
cvLine( gray_out1, line[0], line[1], CV_RGB(255,0,0), 2, 8 );
printf("\n ***BAD**line goes from::(%d,%d)to(%d,%d)\n and the slope is %f",line[0].x,line[0].y,line[1].x,line[1].y,m);

} */

  
}

cvSeqRemove(lines,i);
 
}
          





//printf("\n%f********%f\n",xvan/num,yvan/num);
 
      cvShowImage( "OUT1", out1 );//lines projected onto the real picture
       cvShowImage( "GRAY_OUT1", gray_out1 );
	//cvShowImage( "canny_OUT1", canny_out1 );
	//cvShowImage( "canny_OUT2", canny_out2 );
       
      cv::waitKey(3);
   sensor_msgs::ImagePtr out = sensor_msgs::CvBridge::cvToImgMsg(img, "rgb8");//image converted from opencv to ros for publishing
   image_pub_.publish(out);
 cvClearMemStorage(storage);
cvReleaseMemStorage(&storage);
}

};

int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_converter");
  ImageConverter ic;
  ros::spin();
  return 0;
}
