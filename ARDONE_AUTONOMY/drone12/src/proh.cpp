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
      
     
      image_sub_ = it_.subscribe("/ardrone/image_raw", 1, &ImageConverter::imageCb, this);
      image_pub_= it_.advertise("/arcv/Image",1);
      
     
  }

  ~ImageConverter()
  {
    cv::destroyWindow(WINDOW);
  }

  void imageCb(const sensor_msgs::ImageConstPtr& msg)
  {
     int i;
     sensor_msgs::CvBridge bridge;//we need this object bridge for facilitating conversion from ros-img to opencv
   IplImage* img = bridge.imgMsgToCv(msg,"rgb8");  //image being converted from ros to opencv using cvbridge
  

      

       CvMemStorage* storage = cvCreateMemStorage(0);
        
        CvSeq* lines = 0;
       
         
       
        IplImage* out2 = cvCreateImage( cvGetSize(img), IPL_DEPTH_8U, 3);  
        IplImage* gray_out = cvCreateImage( cvGetSize(img), IPL_DEPTH_8U, 1 ); 
        IplImage* canny_out = cvCreateImage( cvGetSize(img), IPL_DEPTH_8U, 1 );
        IplImage* gray_out1=cvCreateImage( cvGetSize(img), IPL_DEPTH_8U, 3 );
	IplImage* canny_out1 = cvCreateImage( cvGetSize(img), IPL_DEPTH_8U, 1 );
	IplImage* canny_out2 = cvCreateImage( cvGetSize(img), IPL_DEPTH_8U, 1 );
               
        
        
      cvCvtColor(img , gray_out, CV_RGB2GRAY);
        cvCanny( gray_out, canny_out, 50, 125, 3 );
      cvCvtColor(canny_out ,gray_out1, CV_GRAY2BGR);
       lines = cvHoughLines2( canny_out, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI/180, 80,50, 10 );
        for( i = 0; i < lines->total; i++ )
        {    
	  
             CvPoint* line = (CvPoint*)cvGetSeqElem(lines,i);
             cvLine( img, line[0], line[1], CV_RGB(0,255,0), 1, 8 );
            
       } 
 
      cvShowImage( "Detected lines printed", canny_out);//lines projected onto the real picture
      cvShowImage( "canny edge output", gray_out);
      cvShowImage( "GRAY_OUT1", img );
       
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
