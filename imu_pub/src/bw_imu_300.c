#include <ros/ros.h>
#include <serial/serial.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
using namespace std;

struct ANG{
    string SXXXYY;//SX XX YY
    
}
struct ACC{
    string SXYYYY;//SX YY YY
}
struct ANG_VEL{
    string SXXXYY;//SX XX YY
}
struct QUAR{
    string SXYYYYYY;//SX YY YY YY
}

struct encode{
    string            _start;
    struct ANG      ang_data[3];
    struct ACC      acc_data[3];
    struct ANG_VEL  agv_data[3];
    struct QUAR     q;
    string            _end;
}

typedef unsigned char uint8;

const char hex_table[]={
    '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'
};
void to_hex_str(uint8 *s,int len,char* d){
    while(len--){
        *(d++)=hex_table[*s>>4];
        *(d++)=hex_table[*(s++)&0x0f];
    }

}


void function_my{
    while(1){
        serial ser;
        int len =64;
        ser.read(buf,len);
        if(buf[0]==0x77&&buf[2]==0xfd&&buf[3]==0x00)
        {
              memcpy();
              memset();
        }
        else{
              memset();
            }
        }
    }
    




int main()
{
uint8 buffer[48]={
0x77,0x2F,0x00,0x59,0x10,0x00,0x60,0x10,0x03,0x06,0x00,0x00,0x00,
                    0x10,0x01,0x07,0x10,0x05,0x43,0x01,0x01,0x54,
                    0x10,0x00,0x13,0x10,0x00,0x04,0x00,0x00,0x09, 0x10,0x87,0x06,0x35,0x00,0x01,0x76,0x91,0x00,0x02,0x06,0x94,0x00,0x49,0x11,0x75,0x5C
};
char a[97];
a[96]='\0';
to_hex_str(buffer,48,a);
//string s=a;
//cout<<a;

//define as a function: struct data_struct* Decode(char*a) 
{
//rpy
double yaw=(a[9]-'0')*100+(a[10]-'0')*10+(a[11]-'0')+(a[12]-'0')*0.1+(a[13]-'0')*0.01;
yaw=(a[8]=='1')?(-1*yaw):yaw;
cout<<yaw<<"Centigrade"<<endl;

double pitch=(a[15]-'0')*100+(a[16]-'0')*10+(a[17]-'0')+(a[18]-'0')*0.1+(a[19]-'0')*0.01;
pitch=(a[14]=='1')?(-1*pitch):pitch;
cout<<pitch<<"Centigrade"<<endl;

double roll=(a[21]-'0')*100+(a[22]-'0')*10+(a[23]-'0')+(a[24]-'0')*0.1+(a[25]-'0')*0.01;
roll=(a[20]=='1')?(-1*roll):roll;
cout<<roll<<"Centigrade"<<endl;

//x_y_z_acceleratio
double x_acc=(a[27]-'0')+(a[28]-'0')*0.1+(a[29]-'0')*0.01+(a[30]-'0')*0.001+(a[31]-'0')*0.0001;
x_acc=(a[26]=='1')?(-1*x_acc):x_acc;
cout<<x_acc<<"g"<<endl;

double y_acc=(a[33]-'0')+(a[34]-'0')*0.1+(a[35]-'0')*0.01+(a[36]-'0')*0.001+(a[37]-'0')*0.0001;
y_acc=(a[32]=='1')?(-1*y_acc):y_acc;
cout<<y_acc<<"g"<<endl;

double z_acc=(a[39]-'0')+(a[40]-'0')*0.1+(a[41]-'0')*0.01+(a[42]-'0')*0.001+(a[43]-'0')*0.0001;
z_acc=(a[38]=='1')?(-1*z_acc):z_acc;
cout<<z_acc<<"g"<<endl;

//x_y_z_angular_velocity
double x_av=(a[45]-'0')*100+(a[46]-'0')*10+(a[47]-'0')+(a[48]-'0')*0.1+(a[49]-'0')*0.01;
x_av=(a[44]=='1')?(-1*x_av):x_av;
cout<<x_av<<"du/s"<<endl;

double y_av=(a[51]-'0')*100+(a[52]-'0')*10+(a[53]-'0')+(a[54]-'0')*0.1+(a[55]-'0')*0.01;
y_av=(a[50]=='1')?(-1*y_av):y_av;
cout<<y_av<<"du/s"<<endl;

double z_av=(a[57]-'0')*100+(a[58]-'0')*10+(a[59]-'0')+(a[60]-'0')*0.1+(a[61]-'0')*0.01;
z_av=(a[56]=='1')?(-1*z_av):z_av;
cout<<z_av<<"du/s"<<endl;

//Quar
double q1=(a[63]-'0')+(a[64]-'0')*0.1+(a[65]-'0')*0.01+(a[66]-'0')*0.001+(a[67]-'0')*0.0001+(a[68]-'0')*0.00001+(a[69]-'0')*0.000001;
q1=(a[62]=='1')?(-1*q1):q1;
cout<<q1<<" "<<endl;

double q2=(a[71]-'0')+(a[72]-'0')*0.1+(a[73]-'0')*0.01+(a[74]-'0')*0.001+(a[75]-'0')*0.0001+(a[76]-'0')*0.00001+(a[77]-'0')*0.000001;
q2=(a[70]=='1')?(-1*q2):q2;
cout<<q2<<" "<<endl;

double q3=(a[79]-'0')+(a[80]-'0')*0.1+(a[81]-'0')*0.01+(a[82]-'0')*0.001+(a[83]-'0')*0.0001+(a[84]-'0')*0.00001+(a[85]-'0')*0.000001;
q3=(a[78]=='1')?(-1*q3):q3;
cout<<q3<<" "<<endl;

double q4=(a[87]-'0')+(a[88]-'0')*0.1+(a[89]-'0')*0.01+(a[90]-'0')*0.001+(a[91]-'0')*0.0001+(a[92]-'0')*0.00001+(a[93]-'0')*0.000001;
q4=(a[86]=='1')?(-1*q4):q4;
cout<<q4<<" "<<endl;
}


return 0;

}



