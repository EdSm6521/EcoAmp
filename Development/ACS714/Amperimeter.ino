/*
  Analog Input
 Demonstrates analog input by reading an analog sensor on analog pin 0 and
 turning on and off a light emitting diode(LED)  connected to digital pin 13.
 The amount of time the LED will be on and off depends on
 the value obtained by analogRead().

 The circuit:
 * Potentiometer attached to analog input 0
 * center pin of the potentiometer to the analog pin
 * one side pin (either one) to ground
 * the other side pin to +5V
 * LED anode (long leg) attached to digital output 13
 * LED cathode (short leg) attached to ground

 * Note: because most Arduinos have a built-in LED attached
 to pin 13 on the board, the LED is optional.


 Created by David Cuartielles
 modified 30 Aug 2011
 */
 
int AnalogInputPin = 0; // Define analog input pin

// --------------------------------------------------------------------------------------------------------

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  double Current = currentSensor(analogRead(AnalogInputPin));  // Read  analog value
  //Serial.print(", VariationFromNull: "); 
  // printDouble(Current, 2);                                     // display Current
  //Serial.print(" A");
  //Serial.println("");
  Serial.println(fabs(Current+0.12f));
  delay(1);
}

// --------------------------------------------------------------------------------------------------------

// Print decimal numbers

void printDouble(double val, byte precision)
{

  Serial.print (int(val));                                     // Print int part
  if( precision > 0) {                                         // Print decimal part
    Serial.print(".");
    unsigned long frac, mult = 1;
    byte padding = precision -1;
    while(precision--) mult *=10;
    if(val >= 0) frac = (val - int(val)) * mult; else frac = (int(val) - val) * mult;
    unsigned long frac1 = frac;
    while(frac1 /= 10) padding--;
    while(padding--) Serial.print("0");
    Serial.print(frac,DEC) ;
  }
}

// Read 1.1V reference against AVcc

long readInternalVcc() {

  long result;
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2);                                                    // Wait for Vref to settle
  ADCSRA |= _BV(ADSC);                                         // Convert
  while (bit_is_set(ADCSRA,ADSC));
  result = ADCL;
  result |= ADCH<<8;
  result = 1126400L / result;                                  // Back-calculate AVcc in mV
  return result;
}

// Calculate current with Allegro ACS714

double currentSensor(int RawADC)
{

  int    Sensitivity    = 66; // mV/A
  long   InternalVcc    = readInternalVcc();
  double ZeroCurrentVcc = InternalVcc / 2;
  double SensedVoltage  = (RawADC * InternalVcc) / 1024;
  double Difference     = SensedVoltage - ZeroCurrentVcc;
  double SensedCurrent  = Difference / Sensitivity;
  //Serial.print("ADC: ");
  //Serial.print(RawADC);
  //Serial.print("/1024");
  //Serial.print(", Sensed Voltage: ");
  //printDouble(SensedVoltage, 1);
  //Serial.print("mV");
  //Serial.print(", 0A at: ");
  //printDouble(ZeroCurrentVcc, 1);
  //Serial.print("mV");
  return SensedCurrent;                                        // Return the Current
}

