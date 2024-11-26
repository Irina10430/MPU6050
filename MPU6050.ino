#include<Wire.h>

#define temp 0x41
#define Gyro_X 0x43
#define Gyro_Y 0x45
#define Gyro_Z 0x47
#define Acc_X 0x3B
#define Acc_Y 0x3D
#define Acc_Z 0x3F

#define FS_SEL 125.0
#define CALIBRATE_NUM 500

//#define M_PI 3.14159265358

const int mpu_addr=0x68;  // I2C address of the MPU-6050
//Some of those boards have a pull-down resistor at AD0 (address = 0x68), others have a pull-up resistor (address = 0x69).
int16_t AcX,AcY,AcZ;

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

float w_x = 0;
float w_y = 0;
float w_z = 0;

float w_x0 = 0;
float w_y0 = 0;
float w_z0 = 0;

float angle_x = 0;
float angle_y = 0;
float angle_z = 0;

float angle_x0 = 0;
float angle_y0 = 0;
float angle_z0 = 0;
float b0 = 0.001;


unsigned long old_micros;

void setup()
{
  Wire.begin();
  Wire.setClock(400000UL); // Set I2C frequency to 400kHz
  Wire.beginTransmission(mpu_addr);
  Wire.write(0x6B);
  Wire.write(0); // wake up the mpu6050
  Wire.endTransmission(true);
  Serial.begin(115200);
  Serial.println("MPU-6050 test begin");

  

  Serial.println("Calibrate begin");
  w_x0 = 0;
  w_y0 = 0;
  w_z0 = 0;
    for(int i=0; i<CALIBRATE_NUM; i++)
    {
      w_x0 += Read_reg(Gyro_X)/FS_SEL;
      w_y0 += Read_reg(Gyro_Y)/FS_SEL;
      w_z0 += Read_reg(Gyro_Z)/FS_SEL;
      delay(10);
    }
  w_x0 /= CALIBRATE_NUM;
  w_y0 /= CALIBRATE_NUM;
  w_z0 /= CALIBRATE_NUM;
  Serial.println("Calibrate end");
  unsigned long old_micros = micros();
}

void loop() {
    w_x = Read_reg(Gyro_X)/FS_SEL - w_x0;
    w_y = Read_reg(Gyro_Y)/FS_SEL - w_y0;
    w_z = Read_reg(Gyro_Z)/FS_SEL - w_z0;

    unsigned long time_micros = micros();
    float dt = (time_micros - old_micros)/1000000.0;
    old_micros = time_micros;

    angle_x += w_x * dt;
    angle_y += w_y * dt;
    angle_z += w_z * dt;
    
    Serial.print(w_x);
    Serial.print(";\t");
    Serial.print(w_y);
    Serial.print(";\t");
    Serial.print(w_z);
    Serial.print(";\t");
    Serial.print(angle_x);
    Serial.print(";\t");
    Serial.print(angle_y);
    Serial.print(";\t");
    Serial.print(angle_z);
    Serial.println();
    delay(10);
  
}
