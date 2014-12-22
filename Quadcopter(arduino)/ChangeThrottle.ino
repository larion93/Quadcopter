void GetThrottleAndDesiredAngles()
{  
    if (Serial.available() > 0) 
    {
      incomingByte = Serial.read();
      switch (incomingByte)
      {
        case 81:
          throttle_checking(-1);
        break;
        case 69:
          throttle_checking(1);
        break;
        case 87: //numpad8
          Desired_Pitch_Angle=angle_checking(Desired_Pitch_Angle,1,Max_Pitch_Angle,Min_Pitch_Angle);
        break;
        case 83: //numpad2
          Desired_Pitch_Angle=angle_checking(Desired_Pitch_Angle,-1,Max_Pitch_Angle,Min_Pitch_Angle);
        break;
        case 65: //numpad4
          Desired_Roll_Angle=angle_checking(Desired_Roll_Angle,-1,Max_Roll_Angle,Min_Roll_Angle);
        break;
        case 68: //numpad6
          Desired_Roll_Angle=angle_checking(Desired_Roll_Angle,1,Max_Roll_Angle,Min_Roll_Angle);
        break;
        case 90: //numpad6
          IsArm=false;
          motor1.write(0);
          motor2.write(0);
          motor3.write(0);
          motor4.write(0);
        break;
        case 85: 
          Roll_P_coef=angle_checking(Roll_P_coef,0.001,Max_Roll_Angle,Min_Roll_Angle);
        break;
        case 74: 
          Roll_P_coef=angle_checking(Roll_P_coef,-0.001,Max_Roll_Angle,Min_Roll_Angle);
        break;
        case 73: 
          Roll_I_coef=angle_checking(Roll_I_coef,0.0001,Max_Roll_Angle,Min_Roll_Angle);
        break;
        case 75: 
          Roll_I_coef=angle_checking(Roll_I_coef,-0.0001,Max_Roll_Angle,Min_Roll_Angle);
        break;
        case 79: 
          Roll_D_coef=angle_checking(Roll_D_coef,0.001,Max_Roll_Angle,Min_Roll_Angle);
        break;
        case 76: 
          Roll_D_coef=angle_checking(Roll_D_coef,-0.001,Max_Roll_Angle,Min_Roll_Angle);
        break;
      }
    }
}
void throttle_checking(short number)
{
  if (Desired_Throttle+number<=100 && Desired_Throttle+number>=0)
  {
    Desired_Throttle+=number;
  }
}

double angle_checking(double angle, double number, double maxval, double minval)
{
  if (angle+number<=maxval && angle+number>=minval)
  {
    angle+=number;
  }
  return angle;
}
