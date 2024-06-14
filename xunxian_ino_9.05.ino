#include <Servo.h>

const int steeringServoPin = 8;  // 舵机引脚
const int leftMotorPin1 = 6;     // 左电机引脚1
const int leftMotorPin2 = 7;     // 左电机引脚2
const int rightMotorPin1 = 5;    // 右电机引脚1
const int rightMotorPin2 = 4;    // 右电机引脚2
Servo steeringServo;
//将数值定义在前，有利于后续调整

const int threshold =400;  //灰度传感器门限
const double middle = 65.5;            //舵机中值
const double limit = 9;             //调节修正度数上限
const double speed1 = 180;      //左发动机动力
const double speed2 = 220;      //右发动机动力
double angle0 = 0;  //暂定转向角度
double angle = 0;  //转向角度
double label = 0;  //转动方向

void setup() {
  // 初始化舵机和电机引脚
  steeringServo.attach(steeringServoPin);
  pinMode(leftMotorPin1, OUTPUT);
  pinMode(leftMotorPin2, OUTPUT);
  pinMode(rightMotorPin1, OUTPUT);
  pinMode(rightMotorPin2, OUTPUT);
  // 设置舵机初始角度
  steeringServo.write(middle);
  Serial.println(1);
  digitalWrite(leftMotorPin2,LOW);
  digitalWrite(rightMotorPin2,HIGH);
  analogWrite(leftMotorPin1, speed1);//后轮差速
  analogWrite(rightMotorPin1, speed2);//防止打滑
}
void go() {
  while (true) {
      if (analogRead(A0) < threshold) {//右转二级
      angle = -4 * limit;//二级比一级优先执行
      label = -1;//变量label在超出检测范围后存储方向
    } else if (analogRead(A1) < threshold) {//右转一级
      angle = -2*limit;
      label=-1;
    } else if (analogRead(A4) < threshold) {//左转二级
      angle = 0.8 * limit;
      label = 1;
      //if(angle0 >= 35) angle=35;
    } else if (analogRead(A3) < threshold) {//左转一级
      angle = 0.5 *limit;
      label=1;
    } else if (analogRead(A2) < threshold) {//中间检测到，减小转向角
      label = 0;
      angle = angle*0.9;
    } else {
      if(label==1) angle =limit *label;
      else if(label ==-1)angle = 8* limit * label;//超过检测范围，转向三级
    }
    steeringServo.write(angle + middle);//转向角写入舵机
    analogWrite(leftMotorPin1, speed1-(angle*1));//后轮差速
    analogWrite(rightMotorPin1, speed2+(angle*1));//防止打滑
  }
}
void loop() {
  go();
}


