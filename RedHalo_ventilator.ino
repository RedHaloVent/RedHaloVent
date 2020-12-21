#include "ArduinoMotorShieldR3.h"
ArduinoMotorShieldR3 md;


//Filtering// 
//this is needed to eliminate the errors in measurement

#define LPF_MACRO(output, input, accumulator, filterk)  { accumulator += input; output = (int32_t)(accumulator/filterk); accumulator -=output; }    // Macro for analogue measurement averaging

//static long accumulator = (analogRead(A0)*2.96)*256;     // delay needed to smooth initial motor current spike
static long accumulator = md.getM1CurrentMilliamps()*256;     // delay needed to smooth initial motor current spike


//reads current
int getCurrentM1()
{
#define FILTERK 256
  long filtered = 0;
  int input = 0;

  // 5 V / 1024 ADC counts = 4.88 mV per count
  // 3.3 V = 2.0 A; 3.3 V / 4.88 mv per count = 676 counts
  // 2.0 A / 676 counts = 2.96 mA per count
  //input = analogRead(A0)*2.96;  
  input = md.getM1CurrentMilliamps();

  LPF_MACRO(filtered, input, accumulator, FILTERK);

  return (filtered);
}

// motor pins
int DIR_A = 12;
int PWM_A = 3;
int current = getCurrentM1();     // current reading

unsigned long pauseTimer = millis();      // stop watch for pauses
unsigned long accelTime = millis(); 
int mSpeed = 0;

int revTimeSetting = 10000;       // time it takes to reverse in ms (this will be experimentally determined)
int revSpeed = 400;             // max speed of reverse stroke
int maxFwdSpeed = -400;         // max speed of motor
int IEval = 2; // 1:(IEval)     //default IE 
int currentLimit = 300;        // current limit(this will probably change)
int postInhaleDwell = 0;
unsigned long postExhaleDwell = 0; 
unsigned long lastBreathTime = 0;
unsigned long breathPeriod = 0;
int default_RR = 20;
int default_TCT = (60 / default_RR) * 1000; //RR to TCT 3 sec as default



void setup()
{
 
  Serial.begin(9600);
   md.init();

  // set up motor pins
  pinMode(DIR_A, OUTPUT);
  pinMode(PWM_A, OUTPUT);
  setMotor1Speed(0);
}
void loop()
{
  int default_inhaleTime = default_TCT / (IEval + 1);  // set inhale time
  
  lastBreathTime = millis();
  inhale();
  exhale();
  postExhalePause();

}



// *********** Inhale ***********
void inhale ()
{
  int current = getCurrentM1();
  accelTime = millis();               // log time in ms
    mSpeed = 0;                     // clear motor speed variable
      setMotor1Speed(mSpeed);         // drive motor
   while (current < currentLimit)          // drive until current setting
  {
    if (mSpeed > maxFwdSpeed)            // accelaration
    {
      if (millis() - accelTime > 3)     // accelarate the motor by 1 every 3ms
      {
        mSpeed--;                 // - is forward direction
        accelTime = millis();
      }
    }
    setMotor1Speed(mSpeed);
    Serial.println(current);          // displays current
    current = getCurrentM1();
  }
}

// *********** Post Inhale ***********
void postInhalePause ()
{
  pauseTimer = millis();
  while (millis() - pauseTimer < postInhaleDwell)                     // dwell
  {
    setMotor1Speed(0);
    Serial.print("paused ");
    Serial.println(current);
  }
}


// *********** Exhale ***********
void exhale()
{
  unsigned long revTimer = millis();   // log time in ms

  accelTime = millis();               // log time in ms for acceloration
  int revAccelSpeed = 0;                     // clear motor speed variable
  while (millis() - revTimer < revTimeSetting)           // reverse back for a set time
  {
    if (revAccelSpeed < revSpeed)            // acceloration
    {
      if (millis() - accelTime > 1)     // accelorate the motor by 2 every 3ms
      {
        revAccelSpeed++;                 // - is forward direction
        accelTime = millis();
      }
    }
    setMotor1Speed(revAccelSpeed);
    Serial.print("reverse , ");
    Serial.println(current);
    current = getCurrentM1();
  }
}


// *********** Post Exhale ***********
void postExhalePause()
{
  while (millis() - lastBreathTime < breathPeriod)                     // dwell
  {
    setMotor1Speed(0);
    Serial.print("paused ");
    Serial.println(millis() - lastBreathTime);
  }
}



// ****** Motor Driver ******
void setMotor1Speed(int M1speed)
{
  if (M1speed < 0) {
    M1speed = -M1speed;  // Make speed a positive quantity
    digitalWrite(DIR_A, LOW);
  }
  else {
    digitalWrite(DIR_A, HIGH);
  }
  if (M1speed > 400) M1speed = 400;  // Max PWM dutycycle
  analogWrite(PWM_A, M1speed * 51 / 80); // default to using analogWrite, mapping 400 to 255. Not sure what this is?
}
