#include <HX711.h>
#include <ros.h>
#include <std_msgs/Int32MultiArray.h>
#include <EEPROM.h>

// union int array, for saving as bytes on EEPROM
union u_tag {
  byte b[36];
  int ival[9];
} u;

ros::NodeHandle nh;
std_msgs::Int32MultiArray sca;

ros::Publisher pub("doc_ock/string_tension", &sca);

HX711 scale[9];
// assign motors to matching order in ROS
byte mot_order[9] = {7, 6, 0, 1, 2, 5, 8, 4, 3};
byte DOUT[9] = {22, 20, 18, 16, 15, 8, 6, 4, 0};
byte CLK[9] = {23, 21, 19, 17, 14, 9, 7, 5, 3};
float b[9] = {453.9559666, -114.2034549, 1970.588235, 1177.730193, 343.0962343, -2466.804979, 254.7770701, -2271.352313, -417.6563737};
float a[9] = {0.010053282, 0.009596929, 0.009803922, 0.010706638, 0.010460251, 0.010373444, 0.010615711, 0.008896797, 0.00989707};
float scale_val[9];
int scale_val_arranged[9];
int zero_scale[9];

long scale_read[9];

void setup() {
  nh.initNode();
  nh.advertise(pub);
  sca.data_length = 9;
  for (int i = 0; i < 36; i++)
    u.b[i] = EEPROM.read(i);

  for (int i = 0; i < 9; i++) {
    scale[i].begin(DOUT[i], CLK[i]);
    zero_scale[i] = u.ival[i];
  }

  pinMode(1, OUTPUT);
  digitalWrite(1, LOW);
  pinMode(2, INPUT_PULLUP);
}

void loop() {
  for (int i = 0; i < 9; i++) {
    scale_read[i] = scale[i].read();
    scale_val[i] = a[i] * scale_read[i] + b[i];
  }

  motor_order_organize();
  sca.data = scale_val_arranged;
  pub.publish(&sca);
  nh.spinOnce();
  delay(100);
}

void motor_order_organize() {
  bool butt = !digitalRead(2);

  for (int i = 0; i < 9; i++) {
    scale_val_arranged[i] = (int)scale_val[mot_order[i]];
    if (butt) {
      zero_scale[i] = scale_val_arranged[i];
      u.ival[i] = zero_scale[i];
    }
    scale_val_arranged[i] -= zero_scale[i];
  }

// if button is pressed, zero all readings
  if (butt)
    for (int i = 0; i < 36; i++)
      EEPROM.write(i, u.b[i]);
}
