#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl/point_types.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl_ros/point_cloud.h>
#include <boost/foreach.hpp>
#include <pcl/io/pcd_io.h>
#include <tf/transform_listener.h>
#include <laser_geometry/laser_geometry.h>
#include<sensor_msgs/LaserScan.h>
#include <turtlesim/Pose.h>
 int i=0;
 int count=0;
 int  ec;
 int  sc;
 int sl;
 int  el;
 int s=0;
 int t=0;
 int wc=0;
 int fspa=0;

 class My_Filter {
     public:
        My_Filter();
        
        void scanCallback(const sensor_msgs::LaserScan::ConstPtr& scan);
     private:
        ros::NodeHandle node_;
        laser_geometry::LaserProjection projector_;
        tf::TransformListener tfListener_;
         ros::Publisher pub;
        ros::Publisher point_cloud_publisher_;
        ros::Subscriber scan_sub_;
         };

My_Filter::My_Filter(){
        scan_sub_ = node_.subscribe<sensor_msgs::LaserScan> ("/scan", 100, &My_Filter::scanCallback, this);
       pub= node_.advertise<turtlesim::Pose>("/path", 500);
        point_cloud_publisher_ = node_.advertise<sensor_msgs::PointCloud2> ("/cloud", 100, false);
        tfListener_.setExtrapolationLimit(ros::Duration(0.1));
                     }

void My_Filter::scanCallback(const sensor_msgs::LaserScan::ConstPtr& scan)
{
turtlesim::Pose dist;

for(i=0;i<=180;i++) //begin scanning the frontier
{
if(i==20)
 fspa=0;
if(scan->ranges[i]<2) //if any obstacle below threshold value
{

if(s==1)
{
s=0;
//c=c+1;
ec=i;
if((ec-sc)>=count)
{
count=ec-sc;//calculate the difference of the gap between the obstacles  and store the location coordinate as well.
sl=sc;    
el=ec;
}
}
if(i>=20)
fspa=1;
if(i==20)  //if obstacle found at first scan itself get a token to check if its  a wall.
t=1;


}
else
{
t=0;
if(s==0)//if its a free gap make  a mark and wait untill a obstacle is found.
{
sc=i;
s=1;
}
}

if(i==160) //this gets invoked only when facing a wall
{

if(t==1)
{
t=0;
wc=wc+1;
printf("WALL!!::%d\n",wc);
dist.y=1;
//call for striding towards right or left(since its facing a wall).
//...
}
}


}

printf("\ngap between %d and %d and the width is %d",sl,el,count);
count=0;
s=0;//setting the flags back to 0
t=0;
if(fspa==0)//no ostacles at all
{
dist.x=0;
dist.y=0;
pub.publish(dist);
}
else
{
dist.x=90-((el+sl)/2);
pub.publish(dist);
dist.y=0;
fspa=0;
}

}



int main(int argc, char** argv)
{
    ros::init(argc, argv, "my_filter");
    
    My_Filter filter;

    ros::spin();

    return 0;
}
