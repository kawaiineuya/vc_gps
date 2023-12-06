#define _LARGEFILE64_SOURCE

#include <ros/ros.h>
#include <sensor_msgs/NavSatFix.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string>
#include <iostream>
#include <cstring>
#include <sstream>
#include <typeinfo>
#include <math.h>

#include "oroca_ros_tutorials/msgTutorial.h" 



using namespace std;

void str_to_hex(const char* in, char* out, const char* offset)
{
  int in_idx = 0, out_idx = 0,cnt1 = 0;
  while (cnt1 != 8)
  {
    sprintf((out +out_idx), "%02x%s", in[in_idx], offset);
    in_idx += 1; out_idx += (strlen(offset)+1);
    cnt1++;
  }
}
void str_to_hex1(const char* in, char* out, const char* offset)
{
  int in_idx = 0, out_idx = 0,cnt1 = 0;
  while (cnt1 != 4)
  {
    sprintf((out +out_idx), "%02x%s", in[in_idx], offset);
    in_idx += 1; out_idx += (strlen(offset)+1);
    cnt1++;
  }
}
void str_to_hex2(const char* in, char* out, const char* offset)
{
  int in_idx = 0, out_idx = 0,cnt1 = 0;
  while (cnt1 != 3)
  {
    sprintf((out +out_idx), "%02x%s", in[in_idx], offset);
    in_idx += 1; out_idx += (strlen(offset)+1);
    cnt1++;
  }
}

int main(int argc, char **argv){
  ros::init(argc, argv, "vc_gps_pub");
  ros::NodeHandle nh1;
  ros::NodeHandle nh2;
  ros::Publisher pub1 = nh1.advertise<sensor_msgs::NavSatFix>("vc_gps_caddy", 100);
  ros::Publisher pub2 = nh2.advertise<sensor_msgs::NavSatFix>("vc_gps_hu", 100);
  sensor_msgs::NavSatFix msg1;
  sensor_msgs::NavSatFix msg2;
  ros::Rate loop_rate(4);
  
 printf("UART\n");
 int i = 1; 
 unsigned int j = 0;
 int hex = 0;
 //int count = 1;
 unsigned int r_time;


 char in1[100] = "";
 char out1[100] = {0x00, };
 char in2[100] = "";
 char out2[100] = {0x00, };
 char in3[100] = "";
 char out3[100] = {0x00, };
 char in4[100] = "";
 char out4[100] = {0x00, };
 char in5[100] = "";
 char out5[100] = {0x00, };
 char in6[100] = "";
 char out6[100] = {0x00, };
 char in7[2] = "";
 char out7[2]; 
 int rx_length;
 double Hu_latitude = 0.0;
 double Hu_Longitude = 0.0;
 float Hu_Altitude = 0.0;
 char Hu_Satelliotes[10] = "";
 char Hu_quality[10] = "";

 double caddy_latitude = 0.0;
 double caddy_Longitude = 0.0;
 float caddy_Altitude = 0.0;
 char caddy_quality[10] = "";
 char caddy_Satelliotes[10] = "";
 unsigned int caddy_real_time = 0;



 int uart_sys = -1;
 int flag = 0;
 uart_sys = open("/dev/ttyVC", O_RDWR|O_NOCTTY|O_LARGEFILE|O_TRUNC);
 if(uart_sys == -1)
  printf("ERR unable to open UART\n");

  struct termios options;
 tcgetattr(uart_sys, &options);
 options.c_cflag = B115200|CS8|CLOCAL|CREAD;
 options.c_iflag = IGNPAR;
 options.c_oflag = 0;
 options.c_lflag = 0;
 tcflush(uart_sys,TCIFLUSH);
 tcsetattr(uart_sys,TCSANOW, &options);
 unsigned char tx_buf[20];
 unsigned char *p_tx_buf;

 p_tx_buf = &tx_buf[0];

 *p_tx_buf++ = '0';
 *p_tx_buf++ = '1';
 
 //p_tx_buf[0] = '0';
 //p_tx_buf[1] = '1';

  int count;
  unsigned char rx_buf[255] = "/0";
 while(ros::ok()){
  printf("%d\n", uart_sys);
  if(uart_sys != -1){
   rx_length = read(uart_sys,(void*)rx_buf,32);
   //printf("%i byte : %s\n",rx_length,rx_buf);
  }
   if(rx_length == 32 || rx_buf[32] == 0x00)
   {
    // while(rx_buf[0] != 0x52 && rx_buf[0] != 0x48){
    //   rx_length = read(uart_sys,(void*)rx_buf,32);
    //   printf("%i byte : %s\n",rx_length,rx_buf);
    //   printf("%x\n", rx_buf[0]);
    //   for (int  i = 0; i < 32; i++)
    //       {
    //         in1[32-i] = rx_buf[i];
    //         printf("%d\n", in1[32-i]);
    //       }
    // }
    if(rx_length < 0){
      r_time = ros::Time::now().sec;
      //printf("%d\n", r_time);
      printf("error\n");

    }
    else if(rx_buf[0] != 0x52 && rx_buf[0] != 0x48){
    //ros::Rate loop_rate(2);
    printf("no data\n");
    for (int  i = 0; i < 32; i++)
      {
        in1[32-i] = rx_buf[i];
        printf("%x\n", in1[32-i]);
      }
    continue;
    }
    else{
      
      rx_buf[rx_length] = '\0';
      printf("%i byte : %s\n",rx_length,rx_buf);
      printf("%x\n", rx_buf[0]);
      r_time = ros::Time::now().sec;
      for (int  i = 2; i < 4; i++)
      {
        in1[4-i] = rx_buf[i];
        //printf("%d\n", in1[4-i]);
      }
      for (int  i = 6; i < 14; i++)
      {
        in2[13-i] = rx_buf[i];
        //printf("%d\n", in2[13-i]);
      }
      
      for (int  i = 14; i < 22; i++)
      {
        in3[21-i] = rx_buf[i];
        //printf("%d\n", in3[21-i]);
      }

      for (int  i = 22; i < 26; i++)
      {
        in4[25-i] = rx_buf[i];
        //printf("%d\n", in4[25-i]);
      }

    
      std::stringstream time_data;
      std::string str_time_data;
      unsigned int UTC_Time_data = 0;

      time_data << std::hex << r_time;
      str_time_data = time_data.str();
      stringstream convert(str_time_data);
      convert >> std::hex >> UTC_Time_data;
      UTC_Time_data = UTC_Time_data & 0xffff0000;
      
      //cout << UTC_Time_data << endl;

      in5[0] =rx_buf[26];
      //printf("%d\n",caddy_quality[0]);

    
      in6[0] =rx_buf[27];
      //printf("%d\n",caddy_Satelliotes[0]);
      
      if(in5[0] == 1 || in5[0] == 2){in5[1] = -1;}
      if(in5[0] == 3){in5[1] = 0;}
      if(in5[0] == 4){in5[1] = 1;}
      if(in5[0] == 5){in5[1] = 2;}

      const double varH = rx_buf[28] / 10.0;
      str_to_hex2(in1, out1, " ");
      str_to_hex(in2, out2, " ");
      str_to_hex(in3, out3, " ");
      str_to_hex1(in4, out4, " ");
      
      long unsigned int num1;    
      long unsigned int num2; 
      long unsigned int num3; 
      long unsigned int num4;      
        
      sscanf(out1, "%lx\n", &num1);     
      sscanf(out2, "%lx\n", &num2);
      sscanf(out3, "%lx\n", &num3);
      sscanf(out4, "%lx\n", &num4);
      UTC_Time_data = UTC_Time_data | num1;

      if(rx_buf[0] == 0x52)
      {

        caddy_latitude = *((double*)&num2);    
        caddy_Longitude = *((double*)&num3);
        caddy_Altitude = *((float*)&num4);
        printf("%08lx %.8f\n",num2,caddy_latitude);
        printf("%08lx %.8f\n",num3,caddy_Longitude);
        printf("%04lx %.2f\n",num4,caddy_Altitude);
        printf("%d\n",UTC_Time_data);

        msg1.latitude = caddy_latitude;
        msg1.longitude = caddy_Longitude;
        msg1.altitude = caddy_Altitude;
        msg1.status.status = in5[1]; 
        msg1.status.service = in6[0];
        msg1.header.stamp = ros::Time::now();
        //msg1.header.stamp.sec = UTC_Time_data;
        msg1.header.frame_id = "vc_gps_cart_link";
        msg1.position_covariance[0] = varH;
        msg1.position_covariance[4] = varH;
        msg1.position_covariance[8] = varH;
        msg1.position_covariance_type = 
          sensor_msgs::NavSatFix::COVARIANCE_TYPE_DIAGONAL_KNOWN;
        pub1.publish(msg1);

      }
      else if(rx_buf[0] == 0x48)
      {
        Hu_latitude = *((double*)&num2);    
        Hu_Longitude = *((double*)&num3);
        Hu_Altitude = *((float*)&num4);
        printf("%08lx %.8f\n",num2,Hu_latitude);
        printf("%08lx %.8f\n",num3,Hu_Longitude);
        printf("%04lx %.2f\n",num4,Hu_Altitude);
        printf("%d\n",UTC_Time_data);

        msg2.latitude = Hu_latitude;
        msg2.longitude = Hu_Longitude;
        msg2.altitude = Hu_Altitude;
        msg2.status.status = in5[1];
        msg2.status.service = in6[0];
        msg2.header.stamp = ros::Time::now();
        //msg2.header.stamp.sec = UTC_Time_data;
        msg2.position_covariance[0] = varH;
        msg2.position_covariance[4] = varH;
        msg2.position_covariance[8] = varH;
        msg2.position_covariance_type = 
          sensor_msgs::NavSatFix::COVARIANCE_TYPE_DIAGONAL_KNOWN;
        pub2.publish(msg2);

        }
      }

    }
    loop_rate.sleep();
  }
  return 0;
}