#include <Adafruit_HMC5883_U.h>
#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_Sensor.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Wire.h>

ESP8266WebServer server(80); // web server variable
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

#define SERVOMIN 100
#define SERVOMAX 500
#define delay_button_forward 100
#define delay_up_down 100
#define TOLERANCE 5
#define BLYNK_PRINT Serial

// change this config
const char* ssid = "Agung 6";
const char* password = "himalaya321";

char forward[] = "forward";
char backward[] = "backward";
char u_turn[] = "u-turn";
char turn[] = "turn";

float servo;
int on_off, button_forward, button_backward, button_u_turn, turn_left, turn_right;
int count_step = 0;
float heading, upper_limit, lower_limit, position, setpoint;

void read_compass()
{
  sensors_event_t event;
  mag.getEvent(&event);
  heading = atan2(event.magnetic.y, event.magnetic.x) * 180 / PI + 180;
}

void evenLegs(int angle, int servo_num)
{
  if (servo_num == 1)
    angle -= 15;
  else if (servo_num == 2)
    angle -= 15;
  else if (servo_num == 4)
    angle -= 5;
  else if (servo_num == 5)
    angle -= 10;
  servo = map(angle, 0, 180, SERVOMIN, SERVOMAX);
  pwm.setPWM(2 * servo_num, 0, servo);
}

void oddLegs(int angle, int servo_num)
{
  if (servo_num == 0)
    angle -= 13;
  else if (servo_num == 1)
    angle -= 12;
  else if (servo_num == 2)
    angle += 10;
  else if (servo_num == 3)
    angle += 10;
  else if (servo_num == 4)
    angle -= 12;
  servo = map(angle, 0, 180, SERVOMIN, SERVOMAX);
  pwm.setPWM(2 * servo_num + 1, 0, servo);
}

void stop()
{
  evenLegs(90, 0);
  evenLegs(90, 1);
  evenLegs(90, 2);
  evenLegs(90, 3);
  evenLegs(90, 4);
  evenLegs(90, 5);
  oddLegs(90, 0);
  oddLegs(90, 1);
  oddLegs(90, 2);
  oddLegs(90, 3);
  oddLegs(90, 4);
  oddLegs(90, 5);
}

void controlServo(int value_servo1[6], int value_servo2[6])
{
  evenLegs(70, 0);
  evenLegs(70, 2);
  evenLegs(70, 4);
  delay(delay_up_down);
  oddLegs(value_servo1[0], 0);
  oddLegs(value_servo1[2], 2);
  oddLegs(value_servo1[4], 4);
  oddLegs(value_servo1[1], 1);
  oddLegs(value_servo1[3], 3);
  oddLegs(value_servo1[5], 5);
  delay(delay_up_down);
  evenLegs(110, 0);
  evenLegs(110, 2);
  evenLegs(110, 4);
  delay(delay_button_forward);

  evenLegs(70, 1);
  evenLegs(70, 3);
  evenLegs(70, 5);
  delay(delay_up_down);
  oddLegs(value_servo2[0], 0);
  oddLegs(value_servo2[2], 2);
  oddLegs(value_servo2[4], 4);
  oddLegs(value_servo2[1], 1);
  oddLegs(value_servo2[3], 3);
  oddLegs(value_servo2[5], 5);
  delay(delay_up_down);
  evenLegs(110, 1);
  evenLegs(110, 3);
  evenLegs(110, 5);
  delay(delay_button_forward);
}

void movement(char move[], int turn_val = 0)
{
  if (move == forward)
  {
    int servo1[6] = {90, 60, 120, 120, 90, 90};
    int servo2[6] = {60, 90, 90, 90, 120, 60};
    controlServo(servo1, servo2);
  }
  else if (move == backward)
  {
    int servo1[6] = {60, 90, 90, 90, 120, 60};
    int servo2[6] = {90, 60, 120, 120, 90, 90};
    controlServo(servo1, servo2);
  }
  else if (move == turn)
  {
    int servo1[6] = {
        90 + turn_val,
        90 - turn_val,
        90 + turn_val,
        90 - turn_val,
        90 + turn_val,
        90 - turn_val};
    int servo2[6] = {
        90 - turn_val,
        90 + turn_val,
        90 - turn_val,
        90 + turn_val,
        90 - turn_val,
        90 + turn_val};
    controlServo(servo1, servo2);
  }
  else if (move == u_turn)
  {
    int servo1[6] = {68, 113, 68, 113, 68, 113};
    int servo2[6] = {113, 68, 113, 68, 113, 68};
    controlServo(servo1, servo2);
  }
}

void setupEndpoint() {
  server.on("/on_off", HTTP_POST, stop);
  server.on("/forward", HTTP_POST, stop);
  server.on("/backward", HTTP_POST, stop);
  server.on("/turn_left", HTTP_POST, stop);
  server.on("/turn_right", HTTP_POST, stop);
  server.on("/u_turn", HTTP_POST, stop);
  server.begin();
  Serial.println("Server berjalan...");
}

void setup()
{
  Serial.begin(9600);

  // connect to wifi
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi terhubung ");
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  pwm.begin();
  pwm.setPWMFreq(50);
  Wire.begin();
  mag.begin();
  read_compass();
  setpoint = heading;
  delay(2000);
}

void loop()
{
  read_compass();
  upper_limit = setpoint + TOLERANCE;
  if (upper_limit > 360)
    upper_limit -= 360;
  lower_limit = setpoint - TOLERANCE;
  if (lower_limit < 0)
    lower_limit += 360;
  position = heading;
  if (position > upper_limit)
    movement(turn, 5);
  if (position < lower_limit)
    movement(turn, -5);

  // on/off robot
  if (on_off == 1)
  {
    if (button_forward != button_backward)
    {
      // move forward
      if (button_forward == 1)
      {
        movement(forward);
        count_step += 2;
      } // move backward
      else if (button_backward == 1)
      {
        movement(backward);
        count_step += 2;
      }
    }

    if (turn_left != turn_right)
    {
      // turn right or left
      if (turn_left == 1)
      {
        movement(turn, 10);
        setpoint = heading;
        count_step += 2;
      }
      else if (turn_right == 1)
      {
        movement(turn, -10);
        setpoint = heading;
        count_step += 2;
      }
    }

    if (button_u_turn == 1)
    {
      for (int i = 0; i < 4; i++)
      {
        movement(u_turn);
      }
      setpoint += 180;
      if (setpoint > 360)
        setpoint -= 360;
    }
  }
  else
  { // stop the robot
    stop();
    count_step = 0;
  }
  Serial.println(count_step);
}
