#include <ros/ros.h>
#include <serial/serial.h>
#include <std_msgs/String.h>
#include <std_msgs/Empty.h>
#include <sensor_msgs/NavSatFix.h>

serial::Serial ser;
using namespace std;

double hexstr2double(const std::string& hexstr)
{
    double d = 0.0;
    *reinterpret_cast<unsigned long long*>(&d) = std::stoull(hexstr, nullptr, 16);
    return d;
}

int main (int argc, char** argv){
    ros::init(argc, argv, "serial_example_node");
    ros::NodeHandle nh;
    ros::Publisher pubCaddy = nh.advertise<sensor_msgs::NavSatFix>("vc_gps_caddy", 100);
    ros::Publisher pubHu = nh.advertise<sensor_msgs::NavSatFix>("vc_gps_hu", 100);
    sensor_msgs::NavSatFix msg;
    try
    {
        // ser.setPort("/dev/ttyVC");
        ser.setPort("/dev/ttyIMU");
        ser.setBaudrate(115200);
        serial::Timeout to = serial::Timeout::simpleTimeout(1000);
        ser.setTimeout(to);
        ser.open();
    }
    catch (serial::IOException& e)
    {
        ROS_ERROR_STREAM("Unable to open port ");
        return -1;
    }

    if(ser.isOpen()){
        ROS_INFO_STREAM("Serial Port initialized");
    }else{
        return -1;
    }

    ros::Rate loop_rate(5);
    while(ros::ok()){

        ros::spinOnce();

        if(ser.available()){
            ROS_INFO_STREAM("Reading from serial port");
            std_msgs::String result;
            result.data = ser.read(32);
            ROS_INFO_STREAM("Read: " << result.data);
            
                        
        }
        loop_rate.sleep();

    }
}
