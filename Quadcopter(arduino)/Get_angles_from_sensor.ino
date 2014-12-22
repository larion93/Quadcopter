uint8_t devStatus;
Quaternion q; // [w, x, y, z] quaternion container
VectorFloat gravity;
volatile bool mpuInterrupt = false;
uint16_t fifoCount; // count of all bytes currently in FIFO
uint8_t fifoBuffer[64];
float ypr[3];
void dmpDataReady() 
{
    mpuInterrupt = true;
}
void Get_angles_from_sensor()
{
    // if programming failed, don't try to do anything
    if (!dmpReady) return;
    // wait for MPU interrupt or extra packet(s) available
    while (!mpuInterrupt && fifoCount < packetSize) 
    {
    }
    // reset interrupt flag and get INT_STATUS byte
    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();
    // get current FIFO count
    fifoCount = mpu.getFIFOCount();
    while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();
    // read a packet from FIFO
    mpu.getFIFOBytes(fifoBuffer, packetSize);
    // track FIFO count here in case there is > 1 packet available
    // (this lets us immediately read more without waiting for an interrupt)
    fifoCount -= packetSize;
    // display Euler angles in degrees
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
    RollAngle=ypr[1] * 180/M_PI;
    PitchAngle=ypr[2] * 180/M_PI;
}
