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
#include <highgui.h>

/*here is a simple program which demonstrates the use of ros and opencv to do image manipulations on video streams given out as image topics from the monocular vision
of robots,here the device used is a ardrone(quad-rotor).*/
using namespace std;
using namespace cv;
namespace enc = sensor_msgs::image_encodings;
//void toggleCamCallback(const std_msgs::Empty &msg);
static double r=0,g=0,b=0;
static const char WINDOW[] = "Image window";
void my_mouse_callback(int event, int x, int y, int flags, void* param);
class ImageRecorder
{
  ros::NodeHandle nh_;
  ros::NodeHandle n;
  ros::Publisher pub ;
  ros::Publisher tog;
 // turtlesim::Publisher pub
  image_transport::ImageTransport it_;    
  image_transport::Subscriber image_sub_; //image subscriber 
  image_transport::Publisher image_pub_; //image publisher(we subscribe to ardrone image_raw)
  std_msgs::String msg;
public:
  ImageRecorder()
    : it_(nh_)
  {
      
     // tog= nh_.advertise("/ardrone/togglecam", 1);
      //pub= n.advertise<turtlesim::Velocity>("/drocanny/vanishing_points", 500);//
      image_sub_ = it_.subscribe("/ardrone/image_raw", 1, &ImageRecorder::imageCb, this);
     image_pub_= it_.advertise("/arcv/Image",1);
      
      cvNamedWindow("Houg1",CV_WINDOW_NORMAL);
  }

  ~ImageRecorder()
  {
    cv::destroyWindow(WINDOW);
  }

  void imageCb(const sensor_msgs::ImageConstPtr& msg)
  {
    
     sensor_msgs::CvBridge bridge;//we need this object bridge for facilitating conversion from ros-img to opencv
   IplImage* img = bridge.imgMsgToCv(msg,"rgb8");  //image being converted from ros to opencv using cvbridge
  

        CvSeq* lines = 0;
       int i,c,d;
       float c1[50]; 
       float m;
       float dis;
       int k=0,k1=0; 
      int count=0;  
     float m1[50];
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
      IplImage* out1 = cvCreateImage( cvGetSize(img), IPL_DEPTH_8U, 3 );   //make sure to feed the image(img) data to the parameters necessary for canny edge output 
      IplImage* gray_out = cvCreateImage( cvGetSize(img), IPL_DEPTH_8U, 1 ); 
      IplImage* canny_out = cvCreateImage( cvGetSize(img), IPL_DEPTH_8U, 1 );
      IplImage* gray_out1=cvCreateImage( cvGetSize(img), IPL_DEPTH_8U, 3 );
      IplImage* img1 = cvCreateImage( cvGetSize(img), IPL_DEPTH_8U, 3 ); 
	IplImage* imgHSV = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 3);
 IplImage* imgThreshed = cvCreateImage(cvGetSize(img), 8, 1);
	int nFrames = 50;
	//cvCvtColor(img, imgHSV, CV_BGR2HSV);
	//cvInRangeS(img, cvScalar(105,35,0), cvScalar(219,165,68), imgThreshed); 
	
	
		//IplImage* imgThreshed = cvCreateImage(cvGetSize(param), IPL_DEPTH_8U, 1);
	
cvCvtColor(img , gray_out, CV_RGB2GRAY);
cvShowImage( "Houg12gg12",gray_out);
cvShowImage( "Houg1",img);
cvSetMouseCallback("Houg12gg12",&my_mouse_callback,(void*) gray_out);
//printf("!!!!lolololololol\n");

if(b>0)
{
printf("R=%f:::::G=%f::::B=%f\n",r,g,b);
cvInRangeS(gray_out, cvScalar(200,0,0), cvScalar(250,0,0), imgThreshed); 
//cvInRangeS(img, cvScalar(0,0,0), cvScalar(10,10,10), imgThreshed);
printf("success");
cvShowImage( "Houg1212",imgThreshed); 
}
      cv::waitKey(3);
   sensor_msgs::ImagePtr out = sensor_msgs::CvBridge::cvToImgMsg(img, "rgb8");//image converted from opencv to ros for publishing
   image_pub_.publish(out);

}

};


void my_mouse_callback(
 int event, int x, int y, int flags, void* param )
{




switch( event ) {

  case CV_EVENT_MOUSEMOVE: {
		


  		}
  break;

  case CV_EVENT_LBUTTONDOWN: {

CvScalar para=cvGet2D(param, x, y);
	r = para.val[2];
	g = para.val[1];
	 b = para.val[0];
	printf("X,Y::%d::%d::R=%f:::::G=%f::::B=%f\n",x,y,r,g,b);
	//cvInRangeS(param, cvScalar(b-10,g-10,r-10), cvScalar(b+10,g+10,r+10), imgThreshed); 


 
  }
  break; 

  case CV_EVENT_LBUTTONUP: {
    
  }
  break; 
}


}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_converter");
  ImageRecorder ic;
  ros::spin();
 	
  return 0;
}
