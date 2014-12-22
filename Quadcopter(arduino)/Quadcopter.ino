#include <Wire.h>
#include <I2Cdev.h>
#include <helper_3dmath.h>
#include <MPU6050_6Axis_MotionApps20.h>
#include "Config.h"
#include <Servo.h>
Servo motor1;
Servo motor2;
Servo motor3;
Servo motor4;
MPU6050 mpu;
byte incomingByte = 0;
double PitchAngle=0, RollAngle=0;
double Desired_Pitch_Angle=0.000, Desired_Roll_Angle=0.000;
double Desired_Throttle=0.00;
double Roll_P_coef=0.000,Roll_I_coef=0.000,Roll_D_coef=0.000;
//double Pitch_P_coef=0.00,Pitch_I_coef=0.00,Pitch_D_coef=0.00;
bool dmpReady = false; 
double PID_Pitch_value=0.000, PID_Roll_value=0.000;
uint8_t mpuIntStatus;
uint16_t packetSize;
unsigned long  tpPitch = millis();
unsigned long  tpRoll = millis();
double pErrorPitch = 0.0, pErrorRoll = 0.0;
double IpPitch = 0.0, IpRoll = 0.0;
double motor_value1=0.00, motor_value2=0.00, motor_value3=0.00, motor_value4=0.00;
bool IsArm=true;
void setup()
{
  Serial.begin(115200);
  Wire.begin();
  TWBR = 24;
  mpu.initialize();
  mpu.dmpInitialize();
  mpu.setXAccelOffset(-1512);
  mpu.setYAccelOffset(2159);
  mpu.setZAccelOffset(1329);
  mpu.setXGyroOffset(27);
  mpu.setYGyroOffset(-22);
  mpu.setZGyroOffset(347);
  mpu.setDMPEnabled(true);
  attachInterrupt(0, dmpDataReady, RISING);
  mpuIntStatus = mpu.getIntStatus();
  dmpReady = true;
  packetSize = mpu.dmpGetFIFOPacketSize();
  motor1.attach(MOTOR_PIN1,MIN_SIGNAL,MAX_SIGNAL);
  motor2.attach(MOTOR_PIN2,MIN_SIGNAL,MAX_SIGNAL);
  motor3.attach(MOTOR_PIN3,MIN_SIGNAL,MAX_SIGNAL);
  motor4.attach(MOTOR_PIN4,MIN_SIGNAL,MAX_SIGNAL);
  motor1.write(0);
  motor2.write(0);
  motor3.write(0);
  motor4.write(0);
}
void loop()
{
  GetThrottleAndDesiredAngles();
  Get_angles_from_sensor();
  if (IsArm==true)
  {
    PID_Pitch_value = PIDCompute(Desired_Pitch_Angle - PitchAngle, PID_Pitch_Kp, PID_Pitch_Ki, PID_Pitch_Kd, PID_Pitch_MaxValue, PID_Pitch_MinValue, tpPitch, pErrorPitch, IpPitch);
    PID_Roll_value = PIDCompute(Desired_Roll_Angle - RollAngle, Roll_P_coef, Roll_I_coef, Roll_D_coef, PID_Roll_MaxValue, PID_Roll_MinValue, tpRoll, pErrorRoll, IpRoll);
    motor_value1=mapdouble(Desired_Throttle+PID_Pitch_value,0,100,MIN_SIGNAL,MAX_SIGNAL);
    motor_value4=mapdouble(Desired_Throttle-PID_Roll_value,0,100,MIN_SIGNAL,MAX_SIGNAL);
    motor_value3=mapdouble(Desired_Throttle-PID_Pitch_value,0,100,MIN_SIGNAL,MAX_SIGNAL);
    motor_value2=mapdouble(Desired_Throttle+PID_Roll_value,0,100,MIN_SIGNAL,MAX_SIGNAL);
    if (motor_value4<1000)
    {
      motor_value4=1000;
    }
    if (motor_value2<1000)
    {
      motor_value2=1000;
    }
    if (motor_value4>2000)
    {
      motor_value4=2000;
    }
    if (motor_value2>2000)
    {
      motor_value2=2000;
    }
    motor1.writeMicroseconds(motor_value1);
    motor2.writeMicroseconds(motor_value2);
    motor3.writeMicroseconds(motor_value3);
    motor4.writeMicroseconds(motor_value4);
  }
  Serial.print(Desired_Pitch_Angle);
  Serial.print(",");
  Serial.print(PitchAngle);
  Serial.print(",");
  Serial.print(PID_Pitch_value);
  Serial.print(",");
  Serial.print(Desired_Roll_Angle);
  Serial.print(",");
  Serial.print(RollAngle);
  Serial.print(",");
  Serial.print(PID_Roll_value);
  Serial.print(",");
  Serial.print(Desired_Throttle);
  Serial.print(",");
  Serial.print(motor_value2);
  Serial.print(",");
  Serial.print(motor_value4);
  Serial.print(",");
  Serial.print(Roll_P_coef,3);
  Serial.print(",");
  Serial.print(Roll_I_coef,4);
  Serial.print(",");
  Serial.println(Roll_D_coef,3);
}
double mapdouble(double x, double in_min, double in_max, double out_min, double out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
