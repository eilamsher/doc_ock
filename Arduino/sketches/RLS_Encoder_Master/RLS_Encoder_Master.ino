//=====[ INCULDE ]==============================================================

#include <ros.h>
#include <std_msgs/Float32MultiArray.h>
#include <i2c_t3.h>

//=====[ Constants ]========================================
#define PPR17 131072.0

IntervalTimer myTimer;

// union for sending a float array over I2C
union u_tag {
  byte b[8];
  float fval[2];
} u;

ros::NodeHandle nh;
std_msgs::Float32MultiArray ang;

ros::Publisher pub("doc_ock/current_angles", &ang);

//=====[ VARIABLES ]============================================================
byte b[6];
float enc_val[6];
long pos;
uint8_t slave_add[2] = {0x66, 0x69}; // target Slave addresses

void setup() {
  Wire.begin(I2C_MASTER, 0x00, I2C_PINS_18_19, I2C_PULLUP_EXT, 400000);
  Wire.setDefaultTimeout(200000); // 200ms

  nh.initNode();
  nh.advertise(pub);
  ang.data_length = 6;
  Serial2.begin(115200);
  Serial1.begin(115200);
  // set rate to 200 Hz
  myTimer.begin(Publish_Timer, 5000); 

}

void Publish_Timer() {
  Serial1.flush();
  while (Serial1.available()) {
    b[0] = Serial1.read();
    b[1] = Serial1.read();
    b[2] = Serial1.read();
    break;
  }
  Serial2.flush();
  while (Serial2.available()) {
    b[3] = Serial2.read();
    b[4] = Serial2.read();
    b[5] = Serial2.read();
    break;
  }

  pos = b[0];
  pos = (pos << 8);
  pos = pos | b[1];
  pos = (pos << 8);
  pos = pos | b[2];
  pos = (pos >> 7);
  enc_val[0] = 217.15 + pos * 360.0 / PPR17;

  if (enc_val[0] > 180)
    enc_val[0] -= 360;

  pos = b[3];
  pos = (pos << 8);
  pos = pos | b[4];
  pos = (pos << 8);
  pos = pos | b[5];
  pos = (pos >> 7);
  enc_val[1] = 37.6 + pos * 360.0 / PPR17;

  if (enc_val[1] > 180)
    enc_val[1] -= 360;

  enc_val[1] = -enc_val[1];

  for (int i = 0; i < 2; i++) {
    // Read from Slave
    Wire.requestFrom(slave_add[i], 8); // Read from Slave (string len unknown, request full buffer)

    if (!Wire.getError()) {
      Wire.read(u.b, Wire.available());
      enc_val[2 * i + 2] = u.fval[0];
      enc_val[2 * i + 3] = u.fval[1];
    }
  }
  ang.data = enc_val;
  pub.publish(&ang);
}

void loop() {
  nh.spinOnce();
  delay(1);
}
