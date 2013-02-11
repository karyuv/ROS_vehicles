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
#include <turtlesim/Pose.h>
#include <highgui.h>

/*here is a simple program which demonstrates the use of ros and opencv to do image manipulations on video streams given out as image topics from the monocular vision
of robots,here the device used is a ardrone(quad-rotor).*/
using namespace std;
using namespace cv;
namespace enc = sensor_msgs::image_encodings;
//void toggleCamCallback(const std_msgs::Empty &msg);
static double r=0,g=0,b=0;
static const char WINDOW[] = "Image window";

class ImageConverter
{
  ros::NodeHandle nh_;
  ros::NodeHandle n;
 ros::Publisher pub ;

  image_transport::ImageTransport it_;    
  image_transport::Subscriber image_sub_; //image subscriber 
  image_transport::Publisher image_pub_; //image publisher(we subscribe to ardrone image_raw)
  std_msgs::String msg;
public:
 ImageConverter()
    : it_(nh_)
  {
        //turtlesim::Publisher pub1;
     // tog= nh_.advertise("/ardrone/togglecam", 1);
      pub= n.advertise<turtlesim::Pose>("/drone/walldis", 500);
      image_sub_ = it_.subscribe("/ardrone/image_raw", 1, &ImageConverter::imageCb, this);
     image_pub_= it_.advertise("/arcv/Image",1);
      
      //cvNamedWindow("Houg1",CV_WINDOW_NORMAL);
  }

  ~ImageConverter()
  {
    cv::destroyWindow(WINDOW);
  }

  void imageCb(const sensor_msgs::ImageConstPtr& msg)
  {
    
     sensor_msgs::CvBridge bridge;//we need this object bridge for facilitating conversion from ros-img to opencv
   
	//IplImage* img = cvCreateImage( cvGetSize(img), IPL_DEPTH_8U, 3 );
	IplImage* img = bridge.imgMsgToCv(msg,"bgr8");  //image being converted from ros to opencv using cvbridge
  


	//cvShowImage( "IMG!!!!!!!",img);


turtlesim::Pose dist;

      
IplImage* out1 = cvCreateImage( cvGetSize(img), IPL_DEPTH_8U, 3 );    
IplImage* gray_out = cvCreateImage( cvGetSize(img), IPL_DEPTH_8U, 1 ); 
IplImage* canny_out = cvCreateImage( cvGetSize(img), IPL_DEPTH_8U, 1 );
IplImage* gray_out1=cvCreateImage( cvGetSize(img), IPL_DEPTH_8U, 3 );
IplImage* img1 = cvCreateImage( cvGetSize(img), IPL_DEPTH_8U, 3 ); 
IplImage* imgThreshedred = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
IplImage* imgThreshedgreen = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
IplImage* imgThreshedblue = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
IplImage* imgThreshedminus = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
IplImage* imgThreshedINrange = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);		
int nFrames = 50;	
cvSplit(img, imgThreshedblue, imgThreshedgreen, imgThreshedred, NULL);
cvSub(imgThreshedblue,imgThreshedgreen,imgThreshedminus,0);
cvSub(imgThreshedminus,imgThreshedred,imgThreshedminus,0);


cvInRangeS(imgThreshedred, cvScalar(254), cvScalar(255), imgThreshedINrange); 
cvShowImage( "imgTHRESHOLDred",imgThreshedred);
cvShowImage( "imgTHRESHOLDgreen",imgThreshedgreen);
cvShowImage( "imgTHRESHOLDblue",imgThreshedblue);
cvShowImage( "imgTHRESHOLDminus",imgThreshedminus);
cvShowImage( "imgTHRESHOLDINrange",imgThreshedINrange);
cvShowImage( "IMG",img);





  cvWaitKey(2);   
  
}
};




int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_converter");
  ImageConverter ic;
  ros::spin();
 	
  return 0;
}
