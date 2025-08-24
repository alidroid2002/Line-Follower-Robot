//motor
#define ena 3
#define in1 4
#define in2 5
#define in3 6
#define in4 7
#define enb 9
#define baseSpeed 180

//sensor
#define sensNum 5
int threshold = 0; // set after calibration
int sValues[sensNum], lastSensor;
int irPins[] = {A0, A1, A2, A3, A4}; 

//pid
float kp = 0.07;
float kd = 1.4;
float ki = 0.005;
int prevError;
float dt, lastTime;

void initialize()
{
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(ena, OUTPUT);
  pinMode(enb, OUTPUT);

  for (int i = 0; i < sensNum; i++)
  {
    pinMode(irPins[i], INPUT);
  }

  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);

  lastSensor = 0;
  prevError = 0;
  lastTime = 0;
}

void motor(int lSpeed, int rSpeed)
{
  if (lSpeed == 0)
  {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, HIGH);
  }
  else if (lSpeed > 0)
  {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  }
  else
  {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
  }

  if (rSpeed == 0)
  {
    digitalWrite(in3, HIGH);
    digitalWrite(in4, HIGH);
  }
  else if (rSpeed > 0)
  {
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
  }
  else
  {
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
  }

  analogWrite(ena, abs(lSpeed));
  analogWrite(enb, abs(rSpeed));
}

void calibrateTrack()
{
  int minVal = 1023;
  int maxVal = 0;
  unsigned long startTime = millis();

  while (millis() - startTime < 2500)
  {
    motor(-100,100); //spin

    for (int i = 0; i < sensNum; i++)
    {
      int val = analogRead(irPins[i]);
      if (val < minVal) minVal = val;
      if (val > maxVal) maxVal = val;
    }

    delay(5); //small delay 
  }

  motor(0,0); //stop

  threshold = (minVal + maxVal)/2;
}

int sensor()
{
  for (int i = 0; i < sensNum; i++)
  {
    if (analogRead(irPins[i]) > threshold)
      sValues[i] = 0;
    else
      sValues[i] = 1;
  }

  int error, state, weight, linePosition;
  state = sValues[0] + sValues[1] + sValues[2] + sValues[3] + sValues[4];

  if (state == 0) return 404;

  weight = 10 * sValues[0] + 20 * sValues[1] + 30 * sValues[2] + 40 * sValues[3] + 50 * sValues[4];
  linePosition = (weight / state);
  error = 30 - linePosition;

  if (sValues[0] == 1) lastSensor = 1;
  if (sValues[4] == 1) lastSensor = 2;

  return error;
}

void PIDlineflw()
{
  int error, strValue;
  float P, I, D;

  error = sensor();

  P = error;
  I += error * dt;
  D = (error - prevError) / dt;
  prevError = error;

  strValue = (kp * P) + (ki * I) + (kd * D);

  if (error == 404)
  {
    if (lastSensor == 1)
      motor(baseSpeed - strValue, -baseSpeed + strValue);
    else if (lastSensor == 2)
      motor(-baseSpeed + strValue, baseSpeed - strValue);
  }
  else
  {
    int left = constrain(baseSpeed + strValue, 0, 255);
    int right = constrain(baseSpeed - strValue, 0, 255);
    motor(left, right);
  }
}

void setup()
{
  initialize();
  calibrateTrack();
}

void loop()
{
  unsigned long now = millis();
  dt = (now - lastTime) / 1000.0;
  lastTime = now;
  PIDlineflw();
}
