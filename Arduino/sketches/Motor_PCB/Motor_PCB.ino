/*
  A 10101010
  B 01010101
  Motor num - PWM,IN1,IN2
  Motor 1 - 3, QA6, QA7
  Motor 2 - 4, QA4, QA5
  Motor 3 - 5, QA2, QA3
  Motor 4 - 6, 7, 8
  Motor 5 - 9, QA1, QA0
  Motor 6 - 23, QB0, QB1
  Motor 7 - 22, QB3, QB2
  Motor 8 - 21, QB5, QB4
  Motor 9 - 20, QB7, QB6
  Motors 0  1  2  /3  4  5  /6 7  8
  Angle p 17 13 5  /25 33 21 /1 9  29
  order   9  21 33 /5  17 29 /1 13 25
*/

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif
#include <ros.h>
#include <std_msgs/Int16MultiArray.h>

ros::NodeHandle nh;

const int latchPin = 1; // (74HC595 pin 12)
const int clockPin = 2; // (74HC595 pin 11)
const int dataPin = 0; //  (74HC595 pin 14)

int data2[9];
byte mot_select = 0, temp_read, pwm_pins[] = {3, 4, 5, 6, 9, 23, 22, 21, 20}, mot_order[9] = {4, 7, 3, 8, 2, 1, 6, 0, 5};
bool dir[9], mot_dir[8], mot_spec_dir[9] = {0, 1, 0, 0, 1, 1, 1, 1, 1};
bool flag = 0;

void callback( const std_msgs::Int16MultiArray& cmd_msg) {
  for (int i = 0; i < 9; i++) {
    data2[i] = cmd_msg.data[mot_order[i]];
  }
  if (!digitalRead(10) || !digitalRead(11))
    manual_control();
  motor_dir_shiftout(data2);

}
ros::Subscriber<std_msgs::Int16MultiArray> sub("doc_ock/motor_pwm", callback);

void setup() {
  for (int i = 0; i < 9; i++) {
    pinMode(pwm_pins[i], OUTPUT);

  }
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  pinMode(17, INPUT_PULLUP);
  pinMode(19, INPUT_PULLUP);
  pinMode(18, OUTPUT);
  digitalWrite(18, LOW);
  nh.initNode();
  nh.subscribe(sub);

}

void loop() {
  if (!digitalRead(10) || !digitalRead(11)) {
    manual_control();
    motor_dir_shiftout(data2);
    flag = 1;
  }
  else if (flag) {
    for (int i = 0; i < 9; i++)
      data2[i] = 0;
    motor_dir_shiftout(data2);
    flag = 0;
  }
  nh.spinOnce();
  delay(1);
}

void Motor_order_organize(int dat[]) {
  for (int i = 0; i < 9; i++) {
    data2[i] = dat[mot_order[i]];
  }
}

void motor_dir_shiftout(int shift_data[]) {
  int i;

  //get directions
  for (i = 0; i < 9; i++)
    dir[i] = shift_data[i] >= 0;


  //xor
  for (i = 0; i < 9; i++)
    dir[i] = !dir[i] != !mot_spec_dir[i];


  //organize by Motor order
  mot_dir[0] = dir[4];
  for (i = 1; i < 4; i++)
    mot_dir[i] = dir[3 - i];

  for (i = 4; i < 8; i++)
    mot_dir[i] = dir[i + 1];



  digitalWrite(latchPin, 0);
  //clear everything out just in case to
  //prepare shift register for bit shifting
  digitalWrite(dataPin, 0);
  digitalWrite(clockPin, 0);
  for (i = 7; i >= 0; i--)  {
    send_bit(!mot_dir[i]);
    send_bit(mot_dir[i]);
  }
  //stop shifting

  digitalWrite(clockPin, 0);
  digitalWrite(8, !dir[3]);
  digitalWrite(7, dir[3]);
  digitalWrite(latchPin, 1);

  for (int i = 0; i < 9; i++)
    analogWrite(pwm_pins[i], abs(shift_data[i]));
}


void send_bit(bool val) {
  digitalWrite(clockPin, 0);
  digitalWrite(dataPin, val);
  //register shifts bits on upstroke of clock pin
  digitalWrite(clockPin, 1);
  //zero the data pin after shift to prevent bleed through
  digitalWrite(dataPin, 0);
}


void manual_control() {
  int data[9];

  byte a = !digitalRead(17) + digitalRead(19);
  for (int i = 0; i < 9; i++)
    data[i] = 0;
  if (!digitalRead(10))
    for (int i = a * 3; i < a * 3 + 3; i++)
      data[i] = -50;
  else
    for (int i = a * 3; i < a * 3 + 3; i++)
      data[i] = 50;

  for (int i = 0; i < 9; i++)
    data2[i] = data[mot_order[i]];
}
