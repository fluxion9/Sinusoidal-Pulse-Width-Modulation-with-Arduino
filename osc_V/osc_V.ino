#include <Wire.h>

#define PWM1_HIGH digitalWrite(9, HIGH) 

#define PWM1_LOW  digitalWrite(9, LOW)

#define PWM2_HIGH digitalWrite(10, HIGH) 

#define PWM2_LOW  digitalWrite(10, LOW)
int enabled = 0;
String wireString;
unsigned char i = 0;
int pulseIndex, pulsesIndex, dutyCycleIndex;
float pulse, pulses, stepAngle, dutyCycle, sum, period;
float p = 10.6;
float frequency = 50.0;
int SPWM[100];

void setup() {
  Wire.begin(101);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  GENERATE_SPWM(p);
}

void loop() {
    if(enabled)
  {
    while(enabled) 
    {
      for(i = 0; i < pulsesIndex; i++)
      {  
        PWM2_LOW;
        PWM1_HIGH;
        delayMicroseconds(SPWM[i]);
        PWM1_LOW;  
        delayMicroseconds(dutyCycleIndex - SPWM[i]);  
      } 
      for(i = 0; i < pulsesIndex; i++)
      {
           PWM1_LOW;   
           PWM2_HIGH; 
           delayMicroseconds(SPWM[i]);
           PWM2_LOW;
           delayMicroseconds(dutyCycleIndex - SPWM[i]);
      }
    }
  }
  else 
  {
    PWM1_LOW;
    PWM2_LOW;  
  }
}

void GENERATE_SPWM(float _pulses)
{
  pulses = _pulses;
  period = 1000000.0 / frequency;
  dutyCycle = period / (2.0 * pulses); 
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

void requestEvent(void) {
  //
}

void receiveEvent(int howMany) {
while(Wire.available())
  {
    char data = Wire.read();
    wireString += data;
  }
  if(wireString.length() > 3)
  {
    if(wireString == "start")
    {
      enabled = 1;
    }
    else if(wireString == "stop")
    {
      enabled = 0;
    }
    else if(wireString.startsWith("p"))
    {
      String varp = wireString;
      int pIndex = varp.indexOf('p');
      int index = varp.indexOf(',');
      varp = varp.substring(pIndex + 1, index);
      p = varp.toFloat();
      GENERATE_SPWM(p);
    }
    else if(wireString.startsWith("f"))
    {
      String varf = wireString;
      int fIndex = varf.indexOf('f');
      int index = varf.indexOf(',');
      varf = varf.substring(fIndex + 1, index);
      frequency = varf.toFloat();
      GENERATE_SPWM(p);
    }
  }
  wireString = "";
}
