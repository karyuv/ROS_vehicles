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
      
      
      pub= n.advertise<turtlesim::Velocity>("/drocanny/vanishing_points", 500);//
      image_sub_ = it_.subscribe("/ardrone/image_raw", 1, &ImageConverter::imageCb, this);
      image_pub_= it_.advertise("/arcv/Image",1);
      
      
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
     
        
        IplImage* out1 = cvCreateImage( cvGetSize(img), IPL_DEPTH_8U, 3 );   //make sure to feed the image(img) data to the parameters necessary for canny edge output 
        IplImage* gray_out = cvCreateImage( cvGetSize(img), IPL_DEPTH_8U, 1 ); 
        IplImage* canny_out = cvCreateImage( cvGetSize(img), IPL_DEPTH_8U, 1 );
        IplImage* gray_out1=cvCreateImage( cvGetSize(img), IPL_DEPTH_8U, 3 );
               
        cvSmooth( img, out1, CV_GAUSSIAN, 11, 11 );
         cvCvtColor(img , gray_out, CV_RGB2GRAY);
        cvCanny( gray_out, canny_out,50, 125, 3 );
      cvCvtColor(canny_out ,gray_out1, CV_GRAY2BGR);


     lines = cvHoughLines2( canny_out, storage, CV_HOUGH_STANDARD, 1, CV_PI/180, 80, 0, 0 );  
   
   for( i = 0; i < MIN(lines->total,100); i++ ) 
        {
             d=0;
             c=0;
            float* line = (float*)cvGetSeqElem(lines,i);
            float rho = line[0];
            float theta = line[1];
            CvPoint pt1, pt2;
            double a = cos(theta), b = sin(theta);
            double x0 = a*rho, y0 = b*rho;
            pt1.x = cvRound(x0 + 1000*(-b));
            pt1.y = cvRound(y0 + 1000*(a));
            pt2.x = cvRound(x0 - 1000*(-b));
            pt2.y = cvRound(y0 - 1000*(a));
cvLine(img, pt1, pt2, CV_RGB(255,0,0), 1, 8 );


}                  

















   
       
      cv::waitKey(2);
   cvShowImage( "Detected lines printed", canny_out);
      cvShowImage( "canny edge output", gray_out);
      cvShowImage( "img", img );

 
}

};

int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_converter");
  ImageConverter ic;
  ros::spin();
  return 0;
}
