#ifndef __FDILINK_DECODE_H
#define __FDILINK_DECODE_H
#include "stdint.h"

typedef uint64_t timestamp_t;

typedef struct
{
	float Gyroscope_X     ;
	float Gyroscope_Y     ;
	float Gyroscope_Z     ;
	float Accelerometer_X ;
	float Accelerometer_Y ;
	float Accelerometer_Z ;
	float Magnetometer_X  ;
	float Magnetometer_Y  ;
	float Magnetometer_Z  ;
	float IMU_Temperature ;
	float Pressure        ;
	float Pressure_Temperature;
	timestamp_t Timestamp;
}FDILINK_COMPONENT_40 __attribute__((paked));

typedef FDILINK_COMPONENT_40 FDILink_IMUData;

typedef struct
{
	float RollSpeed;
	float PitchSpeed;
	float HeadingSpeed;
	float Roll;
	float Pitch;
	float Heading;
	float Q1;
	float Q2;
	float Q3;
	float Q4;
	timestamp_t Timestamp;
}FDILINK_COMPONENT_41 __attribute__((paked));

typedef FDILINK_COMPONENT_41 FDILink_AHRSData;

typedef struct
{
	float BodyVelocity_X           ;
	float BodyVelocity_Y           ;
	float BodyVelocity_Z           ;
	float BodyAcceleration_X       ;
	float BodyAcceleration_Y       ;
	float BodyAcceleration_Z       ;
	double Location_North          ;
	double Location_East           ;
	double Location_Down           ;
	float Velocity_North           ;
	float Velocity_East            ;
	float Velocity_Down            ;
	float Acceleration_North       ;
	float Acceleration_East        ;
	float Acceleration_Down        ;
	float Pressure_Altitude        ;
	timestamp_t Timestamp          ;
}FDILINK_COMPONENT_42;

typedef FDILINK_COMPONENT_42 FDILink_INSGPSData;

typedef struct
{
	union{
		uint8_t GyroStatus;
		struct 
		{
			uint8_t GyroErrorUnchanged 	:1;
			uint8_t GyroErrorFrame 		:1;
			uint8_t GyroErrorOverrange 	:1;
			uint8_t GyroReserved 		:5;
		};
	};
	union{
		uint8_t AccStatus;
		struct 
		{
			uint8_t AccErrorUnchanged 	:1;
			uint8_t AccErrorFrame 		:1;
			uint8_t AccErrorOverrange 	:1;
			uint8_t AccReserved 		:5;
		};
	};
	union{
		uint8_t MagStatus;
		struct 
		{
			uint8_t MagErrorUnchanged 	:1;
			uint8_t MagErrorFrame 		:1;
			uint8_t MagErrorOverrange 	:1;
			uint8_t MagReserved 		:5;
		};
	};
	uint8_t Reserved;
}FDILINK_COMPONENT_50;

typedef FDILINK_COMPONENT_50 FDILink_ErrorData;



#endif //__FDILINK_DECODE_H
