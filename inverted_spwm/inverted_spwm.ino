#define PWM1_HIGH digitalWrite(9, HIGH) 

#define PWM1_LOW  digitalWrite(9, LOW)

#define PWM2_HIGH digitalWrite(8, HIGH) 

#define PWM2_LOW  digitalWrite(8, LOW)

unsigned char i = 0;
float pulses = 10.6;
float pulse;
int SPWM[50];
float stepAngle;
float sum;
float dutyCycle;
int pulseIndex;
int pulsesIndex;
int dutyCycleIndex;

class Blinker
{
  int ledPin;
  long onTime;
  long offTime;

  int ledState;
  unsigned long previousMillis;
  unsigned long currentMillis;
  public:
  Blinker(int pin, long on, long off)
  {
    ledPin = pin;
    pinMode(ledPin, OUTPUT);

    onTime = on;
    offTime = off;
    ledState = LOW;
    previousMillis = 0;
  }
  void Update()
  {
    currentMillis = millis();
    if((ledState == HIGH) && (currentMillis - previousMillis >= onTime))
    {
      ledState = LOW;
      previousMillis = currentMillis;
      digitalWrite(ledPin, ledState);
    }
    else if ((ledState == LOW) && (currentMillis - previousMillis >= offTime))
    {
      ledState = HIGH;
      previousMillis = currentMillis;
      digitalWrite(ledPin, ledState);
    }
  }
};

Blinker Blink(13, 200, 800);
void setup() {
  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);
  PWM1_LOW;
  PWM2_LOW;  
  dutyCycle = 10000.0 / pulses; 
  pulse = pulses / 2.0;
  stepAngle = 180.0 / pulses;
  pulseIndex = int(pulse);
  pulsesIndex = int(pulses);
  dutyCycleIndex = int(dutyCycle);
  for (i = 0; i < pulseIndex; i++)
  {
    SPWM[i] = int(sin(radians(stepAngle * i)) * dutyCycle);
  }

  for (i = pulseIndex; i >= 1; i--)
  {
    SPWM[pulseIndex + (pulseIndex - i)] = int(sin(radians(stepAngle * i)) * dutyCycle);
  }
}


void loop() {
  for(i = 0; i < pulsesIndex; i++)
  {  
    PWM2_LOW;
    PWM1_HIGH;
    delayMicroseconds(SPWM[i]);
    PWM1_LOW;  
    delayMicroseconds(dutyCycleIndex - SPWM[i]);  
  }
  Blink.Update();
  for(i = 0; i < pulsesIndex; i++)
  {
    PWM1_LOW;   
    PWM2_HIGH; 
    delayMicroseconds(SPWM[i]);
    PWM2_LOW;
    delayMicroseconds(dutyCycleIndex - SPWM[i]);
  }
}
