/*
  AnalogReadSerial
  Reads an analog input on pin 0, prints the result to the serial monitor.
  Graphical representation is available using serial plotter (Tools > Serial Plotter menu)
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.
*/

// the setup routine runs once when you press reset:
void setup()
{
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

static int  MAX_SAMPLES         = 250;
int         SAMPLES_COUNT       = 0;
int         SAMPLES_AVERAGE     = 0;
int         SAMPLES_MAX_VALUE   = 0;
int         SAMPLES_START_VALUE = -1;

// the loop routine runs over and over again forever:
void loop()
{
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  SAMPLES_AVERAGE += sensorValue;
  if (sensorValue > SAMPLES_MAX_VALUE)
  {
    SAMPLES_MAX_VALUE = sensorValue;
  }
  if (SAMPLES_COUNT > MAX_SAMPLES)
  {
    SAMPLES_COUNT = -1;
    SAMPLES_AVERAGE = (SAMPLES_AVERAGE / MAX_SAMPLES);
    Serial.println(SAMPLES_AVERAGE);
    SAMPLES_MAX_VALUE = 0;
    SAMPLES_AVERAGE = 0;
  }
  SAMPLES_COUNT++;
  // print out the value you read:
  delay(1);
}
