#include<Wire.h>

#define temp 0x41
#define Gyro_X 0x43
#define Gyro_Y 0x45
#define Gyro_Z 0x47
#define Acc_X 0x3B
#define Acc_Y 0x3D
#define Acc_Z 0x3F

#define M_PI 3.14159265358

const int mpu_addr=0x68;  // I2C address of the MPU-6050
//Some of those boards have a pull-down resistor at AD0 (address = 0x68), others have a pull-up resistor (address = 0x69).
int16_t AcX,AcY,AcZ;

void setup()
{
  Wire.begin();
  Wire.setClock(400000UL); // Set I2C frequency to 400kHz
  Wire.beginTransmission(mpu_addr);
  Wire.write(0x6B);
  Wire.write(0); // wake up the mpu6050
  Wire.endTransmission(true);
  Serial.begin(9600);
  Serial.println("MPU-6050 test begin");
}

double Read_reg(uint16_t addr)
{
  Wire.beginTransmission(mpu_addr);
  Wire.write(addr);
  Wire.endTransmission(false);
  Wire.requestFrom(mpu_addr,2);
  double X=Wire.read()<<8|Wire.read();
  Wire.endTransmission(true);
  return X;
}

void Write_reg(uint16_t addr, uint16_t data)
{
  Wire.beginTransmission(mpu_addr);
  Wire.write(addr);
  Wire.write(data>>8);
  Wire.write(data&0xFF);
  Wire.endTransmission(true);
}

volatile float ax;
volatile float ay;
volatile float az;

float b = 0.1;

void loop() {

if(1){    //////////// Accelerometer test
  ax = ax*(1-b) + b*Read_reg(Acc_X)/16384.0;
  ay = ay*(1-b) + b*Read_reg(Acc_Y)/16384.0;
  az = az*(1-b) + b*Read_reg(Acc_Z)/16384.0;

  float phi_x = atan2(ax,az)*180/M_PI;
  float phi_y = atan2(ay,az)*180/M_PI;
  float phi_z = atan2(ax,ay)*180/M_PI;  
  
  Serial.print(Read_reg(temp)/340.0+36.53);
  Serial.print("; phi_x = ");
  Serial.print(phi_x);
  Serial.print("; phi_y = ");
  Serial.print(phi_y);
  Serial.print("; phi_z = ");
  Serial.print(phi_z);
  Serial.println("; IRA is very pretty !!!");
}
else
{  
//  Serial.print(Read_reg(Gyro_X)/250.0);
//  Serial.print(" ");
//  Serial.print(Read_reg(Gyro_Y)/250.0);
//  Serial.print(" ");
//  Serial.print(Read_reg(Gyro_Z)/250.0);
//  Serial.print(" ");

  Serial.print(Read_reg(Acc_X)/16384.0);
  Serial.print(" ");
  Serial.print(Read_reg(Acc_Y)/16384.0);
  Serial.print(" ");
  Serial.println(Read_reg(Acc_Z)/16384.0);
}
  delay(100);
  
  if(Read_reg(Acc_Z)==0) // reset MPU when hang
  {
    Wire.beginTransmission(mpu_addr);
    Wire.write(0x6B);
    Wire.write(0); // wake up the mpu6050
    Wire.endTransmission(true);
  }  

//  Write_reg(0x27, 23);
//  Serial.println(Read_reg(0x27));
  
}
