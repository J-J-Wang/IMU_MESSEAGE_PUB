
#include <ros/ros.h>
#include <serial/serial.h>
#include <sensor_msgs/Imu.h>
#include <sensor_msgs/MagneticField.h>

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <tf/tf.h>
using namespace std;

#include "FDILink.h"
#include "fdilink_decode.h"
#include "CRC_Table.c"
#include "FDILink.c"

//static FDILink_t FDILink_Handle;
static FDILink_Status_t FDILink_Handle;
int main(int argc, char *argv[])
{
    FDILink_Init(&FDILink_Handle);
    ros::init(argc, argv, "imu_pub_node");
    ros::NodeHandle nh;
    ros::Publisher imu_pub=nh.advertise<sensor_msgs::Imu>("imu",20);
    serial::Serial ser;
    ser.setPort("/dev/ttyUSB0");
    ser.setBaudrate(921600);
    serial::Timeout to=serial::Timeout::simpleTimeout(1000);
    ser.setTimeout(to);
    try{   
        ser.open();
    }catch(serial::IOException& e)
    {
        ROS_ERROR_STREAM("Unable to the port(Authority)");
        return -1;
    }

    if(ser.isOpen())
    {
        ROS_INFO_STREAM("Serial Port initialized");
    }else
    {
        ROS_ERROR_STREAM("Failure to open port(Authority)");
        return -1;
    }
  
    ros::Rate loop_rate(100);
    uint8_t buffer[128];
   // float roll=0,pitch=0,yaw=0;
   // int count = 0;
    while (ros::ok())
    {            
       
        size_t len=ser.available();
        len=len>128?128:len;
        sensor_msgs::Imu msg_imu;
        if(len!=0)
        {             
             len=ser.read(buffer,len);
             for(int i=0;i<len;i++)
             {
                if(FDILink_Receive(&FDILink_Handle, buffer[i]))
                {
                    //count++;
                    switch(FDILink_Handle.RxType)
                    {
                        case 0x40:
                        {
                            FDILink_IMUData* imu_data = (FDILink_IMUData*)FDILink_Handle.Buffer;
                            msg_imu.angular_velocity.x=imu_data->Gyroscope_Y;
                            msg_imu.angular_velocity.y=imu_data->Gyroscope_X;
                            msg_imu.angular_velocity.z=-imu_data->Gyroscope_Z;
                            msg_imu.linear_acceleration.x=imu_data->Accelerometer_Y;
                            msg_imu.linear_acceleration.y=imu_data->Accelerometer_X;
                            msg_imu.linear_acceleration.z=-imu_data->Accelerometer_Z;
                            // cout<<"x="<<imu_data->Gyroscope_X
                            // <<" y="<<imu_data->Gyroscope_Y
                            // <<" z="<<imu_data->Gyroscope_Z<<endl
                            // <<" xa="<<imu_data->Accelerometer_X
                            // <<" ya="<<imu_data->Accelerometer_Y
                            // <<" za="<<imu_data->Accelerometer_Z
                            // <<endl;
                            break;
                        }
                        case 0x41:
                        {
                            FDILink_AHRSData* ahrs_data = (FDILink_AHRSData*)FDILink_Handle.Buffer;
                             msg_imu.orientation.x=ahrs_data->Q2;
                             msg_imu.orientation.y=ahrs_data->Q3;
                             msg_imu.orientation.z=ahrs_data->Q4;
                             msg_imu.orientation.w=ahrs_data->Q1;
                             
                            //  yaw=ahrs_data->Heading;                            
                            //  pitch=ahrs_data->Pitch;
                            //  roll-=ahrs_data->Roll;
                            //cout<<"roll"<<roll<<"pitch"<<pitch<<"yaw"<<yaw<<endl;
                           //  msg_imu.orientation=tf::createQuaternionMsgFromRollPitchYaw(roll,pitch,yaw);
                            // cout<<"q=("<<ahrs_data->Q1
                            // <<","<<ahrs_data->Q2
                            // <<","<<ahrs_data->Q3
                            // <<","<<ahrs_data->Q4
                            // <<")"<<endl;
                           
                            break;
                        }
                    }            
                             
                }       
            }  
        }   
        static int lastCount = 0;
        // if(count != lastCount)// && count % 10 == 0)
        // {
            //char buffer[100];
            printf("angular_velocity\r\n x=%8.3f, y=%8.3f, z=%8.3f\r\n", msg_imu.angular_velocity.x,msg_imu.angular_velocity.y,msg_imu.angular_velocity.z);
            printf("linear_acceleration\r\n xa=%8.3f, ya=%8.3f, za=%8.3f\r\n", msg_imu.linear_acceleration.x,msg_imu.linear_acceleration.y,msg_imu.linear_acceleration.z);
            printf("q= %8.3f, %8.3f, %8.3f, %8.3f\r\n", msg_imu.orientation.x, msg_imu.orientation.y, msg_imu.orientation.z, msg_imu.orientation.w);
            
            // lastCount = count;
            // cout<<"angular_velocity"<<endl
            // <<" x="<<msg_imu.angular_velocity.x
            // <<" y="<<msg_imu.angular_velocity.y
            // <<"z="<<msg_imu.angular_velocity.z<<endl
            // <<"linear_acceleration"<<endl
            // <<" xa="<<msg_imu.linear_acceleration.x
            // <<" ya="<<msg_imu.linear_acceleration.y
            // <<" za="<<msg_imu.linear_acceleration.z
            // <<endl;
            // cout<<"q= "
            // <<msg_imu.orientation.x<<" "
            // <<msg_imu.orientation.y<<" "
            // <<msg_imu.orientation.z<<" "
            // <<msg_imu.orientation.w<<" "<<endl; 
           
       // }   
        msg_imu.header.stamp=ros::Time::now();
        msg_imu.header.frame_id="imu";
        imu_pub.publish(msg_imu);                
        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
}