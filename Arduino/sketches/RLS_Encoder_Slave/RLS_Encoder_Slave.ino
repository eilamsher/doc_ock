#include <i2c_t3.h>
#define PPR17 131072.0

// union for sending a float array over I2C
union u_tag {
  byte b[8];
  float fval[2];
} u;

//------------Functions---------------
void requestEvent(void);

//------------Variables---------------
byte b[6];
long pos;
float temp1, temp2;

//------------Setup---------------
void setup()
{
  pinMode(LED_BUILTIN, OUTPUT); // LED
  // Setup for Slave mode, address 0x66, pins 18/19, external pullups, 400kHz

  //Wire.begin(I2C_SLAVE, 0x66, I2C_PINS_18_19, I2C_PULLUP_EXT, 400000);
  Wire.begin(I2C_SLAVE, 0x69, I2C_PINS_16_17, I2C_PULLUP_EXT, 400000);
  // register events
  Wire.onRequest(requestEvent);

  Serial1.begin(115200);
  Serial2.begin(115200);
}

//------------Loop---------------
void loop()
{
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
  temp1 = 246.0 - 175 + pos * 360.0 / PPR17;
  if (temp1 > 180)
    temp1 -= 360;



  pos = b[3];
  pos = (pos << 8);
  pos = pos | b[4];
  pos = (pos << 8);
  pos = pos | b[5];
  pos = (pos >> 7);
  temp2 = 1.1 + 6 - 130 + pos * 360.0 / PPR17;
  if (temp2 > 180)
    temp2 -= 360;

  u.fval[0] = temp1;
  u.fval[1] = -temp2;
  delay(1);
}


//
// handle Tx Event (outgoing I2C data)
//
void requestEvent(void)
{
  Wire.write(u.b, 8);
}
