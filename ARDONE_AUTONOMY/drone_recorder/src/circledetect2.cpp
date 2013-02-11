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
 // turtlesim::Publisher pub1;
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
	int i1,j1,i2,j2;
        CvSeq* lines = 0;
       int i,c,d;
      int j;
       float c1[50]; 
       float m;
       float dis;
       int k=0,k1=0; 
      static int count=0;  
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
	//IplImage* imgHSV = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 3);
 	IplImage* imgThreshedred = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
	IplImage* imgThreshedgreen = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
	IplImage* imgThreshedblue = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
	IplImage* imgThreshedminus = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
	IplImage* imgThreshedINrange = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
	

 CvMemStorage* storage = cvCreateMemStorage(0);
   cvCvtColor(img, gray_out, CV_BGR2GRAY);
    cvSmooth(gray_out, gray_out, CV_GAUSSIAN, 9, 9); 

	//IplImage* imgThreshedCOI = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 3);

//imgThreshedCOI = img;

//cvSetImageCOI(imgThreshedCOI, 3);

	int nFrames = 50;
	//cvCvtColor(img, imgHSV, CV_BGR2HSV);
	//cvInRangeS(img, cvScalar(0,0,0), cvScalar(0,0,255), imgThreshed); 
	//cvSplit(img, imgThreshedblue, imgThreshedgreen, imgThreshedred, NULL);

	//imgThreshedred = imgThreshedred - imgThreshedblue;
	//imgThreshedred  = imgThreshedred - imgThreshedgreen;
	//cvSub(imgThreshedred,imgThreshedblue,imgThreshedminus,0);
	//cvSub(imgThreshedminus,imgThreshedgreen,imgThreshedminus,0);


//cvInRangeS(gray_out, cvScalar(230), cvScalar(255), imgThreshedINrange); 

CvSeq* circles = cvHoughCircles(gray_out, storage, 
        CV_HOUGH_GRADIENT, 2, 900,40,20 ,1,100);

for (i = 0; i < circles->total; i++) 
    {

         float* p = (float*)cvGetSeqElem( circles, i );
	printf("\n%d,%d",cvRound(p[0]),cvRound(p[1]));
	if(cvRound(p[0])>=160 && cvRound(p[0])<=164)
		{
		if(cvRound(p[1])>=60 && cvRound(p[1])<=85)
			{
			printf("\n@@@@@@@@@@@@@@@@@");
			dist.x =1;
	 		pub.publish(dist);
			}
		}	
         cvCircle( img, cvPoint(cvRound(p[0]),cvRound(p[1])), 
             3, CV_RGB(0,255,0), -1, 8, 0 );
        // cvCircle( img, cvPoint(cvRound(p[0]),cvRound(p[1])), 
          //   cvRound(p[2]), CV_RGB(255,0,0), 3, 8, 0 );
    }

//cvShowImage( "imgTHRESHOLDred",imgThreshedred);
//cvShowImage( "imgTHRESHOLDgreen",imgThreshedgreen);
//cvShowImage( "imgTHRESHOLDblue",imgThreshedblue);
//cvShowImage( "imgTHRESHOLDminus",imgThreshedminus);
//cvShowImage( "imgTHRESHOLDCOI",imgThreshedCOI);
//cvShowImage( "imgTHRESHOLDINrange",imgThreshedINrange);
cvShowImage( "lol",img);
	
		//IplImage* imgThreshed = cvCreateImage(cvGetSize(param), IPL_DEPTH_8U, 1);
//cvCvtColor(img , gray_out, CV_RGB2GRAY);

//count=0;
//for(i=0;i<=238;i++)
//{
//for(j=0;j<=240;j++)
//{
//cvWaitKey(1);//to prevent index count out exception..
 //double fw= cvGetReal2D(imgThreshedINrange,i,j);

//if(fw>=225)
//{
//count=count+1;
//printf(":::\n%f::::",count);
//printf("\n%f",c);
//if(j<119)
//{
//i1=i;
//j1=j;
//}

//else

//{
//IplImage* out1 = cvCreateImage( cvGetSize(img), IPL_DEPTH_8U, 3 );
//i2=i;
//j2=j;
//}
//dis=sqrt((i2-i1)^2+(j2-j1)^2);
//if(dis>=14.09 && dis<=320)
//{
//printf(":::\n%f::::",dis);
//dis=count;
//dist.x = dis;
//dist.y =0;

//pub.publish(dist);
//}
//}




//cvSetMouseCallback("Houg12gg12",&my_mouse_callback,(void*) gray_out);
//printf("!!!!lolololololol\n");
//cvInRangeS(gray_out, cvScalar(200,0,0),cvScalar(250,0,0),imgThreshed); 
//cvShowImage( "GRAYout",gray_out);








//cvInRangeS(gray_out, cvScalar(210,0,0),cvScalar(250,0,0),imgThreshed); 


  cvWaitKey(2);   
   //sensor_msgs::ImagePtr out = sensor_msgs::CvBridge::cvToImgMsg(img, "rgb8");//image converted from opencv to ros for publishing
   //image_pub_.publish(out);

}
};




int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_converter");
  ImageConverter ic;
  ros::spin();
 	
  return 0;
}
