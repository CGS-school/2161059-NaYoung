#include <Servo.h>

#define VRL 25
#define VYL 29
#define VGL 33
#define PRL 22
#define PGL 36
#define SW 53
#define TEST 49
#define SWITCH 8
#define SOUND 7
#define SERVO_PIN 4

Servo servo;

void setup()
{
  pinMode(VRL, OUTPUT);
  pinMode(VYL, OUTPUT);
  pinMode(VGL, OUTPUT);
  pinMode(PRL, OUTPUT);
  pinMode(PGL, OUTPUT);
  pinMode(TEST, OUTPUT);
  pinMode(SWITCH, INPUT);
  pinMode(SOUND, OUTPUT);
  pinMode(SERVO_PIN, INPUT);
  servo.attach(SERVO_PIN);

}

void keep_VGL_on();
void VTimeTaken(unsigned long VTT, int* PSW);
void OnOthers(unsigned long VTT[3]);
void turnSound(int);
void count_time(int);

void loop()
{
  // 0 : RED
  // 1 : GREEN
  // 2 : YELLOW
  unsigned long VTT[3] = {5000, 3000, 1000};
  static int VCount = 0;
  static int save_SW;
  int* PSW = &save_SW;
  keep_VGL_on();
  
  VTimeTaken(VTT[VCount], PSW);
  
  if (VCount == 0 && save_SW == 1)
      {
        save_SW = 0;
        digitalWrite(TEST, LOW);
        OnOthers(VTT);
      }
  if (VCount == 2)
    VCount = 0;
  else
    VCount++;


    //서보가 수평일때 == 0, 차 가능
  //서보가 수직일때 == 1, 차 불가능
  int is_servo_on = 0;
  
  int swValue;
  
  servo.write(90);
  is_servo_on = 1;
  count_time(is_servo_on);
  
  servo.write(0);
  is_servo_on = 0;
  count_time(is_servo_on);

}

void keep_VGL_on()
{
  digitalWrite(PRL, HIGH);
  digitalWrite(VGL, HIGH);
  digitalWrite(PGL, LOW);
  digitalWrite(VYL, LOW);
  digitalWrite(VRL, LOW);
}

void VTimeTaken(unsigned long VTT, int* PSW)
{
  unsigned long cur_time = 0, old_time = 0;
  unsigned long time_taken = 0;
  int swInput;
  
  while(time_taken < VTT)
  {
    old_time = millis();
    swInput = digitalRead(SW);
    if (swInput == LOW)
    {
      *PSW = 1;
      digitalWrite(TEST, HIGH);
    }
    cur_time = millis();
    time_taken += cur_time - old_time;
  }
}

void OnOthers(unsigned long VTT[3])
{
  int temp = 0;
  int *pTemp = &temp;
  digitalWrite(VYL, HIGH);
  digitalWrite(VGL, LOW);
  VTimeTaken(VTT[2], pTemp);
  digitalWrite(VYL, LOW);
  digitalWrite(VRL, HIGH);
  digitalWrite(PRL, LOW);
  digitalWrite(PGL, HIGH);
  VTimeTaken(VTT[1], pTemp);
}

void turnSound(int is_servo_on)
{
  
  int swValue = digitalRead(SWITCH);
  
  if(is_servo_on == 0 && swValue == LOW)
  {
    tone(SOUND, 523);
  }
  
  else
  {
    noTone(SOUND);
  }
  
}


void count_time(int is_servo_on)
{
  
  unsigned long cur_time = 0, old_time = 0;
  unsigned long time_taken = 0;
  
  if(is_servo_on == 1)
  {
    while(time_taken < 2000)
    {
      old_time = millis();
      turnSound(is_servo_on);
      cur_time = millis();
      time_taken += cur_time - old_time;
    }
  }
  
  else
  {
    while(time_taken < 5000)
    {
      old_time = millis();
      turnSound(is_servo_on);
      cur_time = millis();
      time_taken += cur_time - old_time;
    }
  }
  
}
