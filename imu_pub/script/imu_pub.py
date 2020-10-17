#!/usr/bin/env python
 
 
import time
import serial       
import binascii
import rospy
from geometry_msgs.msg import Quaternion, Vector3
from sensor_msgs.msg import Imu
import codecs
 
s=serial.Serial("/dev/ttyUSB0",115200)
rospy.init_node("imu")
imuPub = rospy.Publisher("imu", Imu, queue_size=1)
rate=rospy.Rate(100)
 
Hex_str=codecs.decode('770500560560','hex')
s.write(Hex_str)
 
data= str(binascii.b2a_hex(s.read(6)))
print(data)
print('AUTO PUT DATA TYPE SETTING SUCCESS')
 
Hex_str=codecs.decode('7705000C0617','hex')
s.write(Hex_str)
data=str(binascii.b2a_hex(s.read(6)))
print(data)
print('SET 100Hz OUTPUT FREQUNSE')
 
while not rospy.is_shutdown():
    data= str(binascii.b2a_hex(s.read(48)))
    g_value=data[26:44]
    q_value=data[62:94]
    print(data)
    print(g_value)
    print(q_value)
    #x_acc   
    print(g_value[1:6])
    x_acc=int(g_value[1:6])/10000.0
    print(x_acc)
    if int(g_value[0]):
        x_acc=-1*x_acc
    #y_acc 
    y_acc=int(g_value[7:12])/10000.0
    if int(g_value[6]) :
        y_acc=-1*y_acc
    #z_acc  
    z_acc=int(g_value[13:])/10000.0
    if int(g_value[12]):
        z_acc=-1*z_acc
    print(z_acc)
 
    #q1  
    q1=int(q_value[1:9])/1000000.0
    if int(q_value[0]):
        q1=-1*q1
 
    #q2  
    q2=int(q_value[9:17])/1000000.0
    if int(q_value[8]):
        q2=-1*q2
 
 
    #q3
    q3=int(q_value[17:24])/1000000.0
    if int(q_value[16]):
        q3=-1*q3
 
    #q4   
    q4=int(q_value[25:])/1000000.0
    if int(q_value[24]):
        q4=-1*q4
    print(q4)
  
    imuMsg = Imu()
    stamp = rospy.get_rostime()
    imuMsg.header.stamp, imuMsg.header.frame_id = stamp, "imu_link"
    #imuMsg.orientation = eul_to_qua(ef_l)
    imuMsg.orientation.x=q1
    imuMsg.orientation.y=q2
    imuMsg.orientation.z=q3
    imuMsg.orientation.w=q4
 
    
    #imuMsg.orientation_covariance = cov_orientation
    #imuMsg.angular_velocity.x, imuMsg.angular_velocity.y, imuMsg.angular_velocity.z = x_ang_acc, y_ang_acc, z_ang_acc
    #imuMsg.angular_velocity_covariance = cov_angular_velocity
    imuMsg.linear_acceleration.x, imuMsg.linear_acceleration.y, imuMsg.linear_acceleration.z = x_acc,y_acc,z_acc
    #imuMsg.linear_acceleration_covariance = cov_linear_acceleration
    imuPub.publish(imuMsg)
    rate.sleep()
 
    