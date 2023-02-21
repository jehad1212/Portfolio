/**************************************************************************************************
 * @file:   PowerBoard.cpp
 * @brief:  Arduino code for power board testing
 * @author: Jehad Ibrahim
 * @date:   24/01/2023
 *************************************************************************************************/

#include <LiquidCrystal.h>

// Initialize the LCD library with the pins for the display
const uint8_t LCD_EN = 1;  // LCD Enable line pin number.
const uint8_t LCD_RS = 0;  // LCD Read Select pin number.
const uint8_t LCD_D4 = 8;  // LCD Data pin 4.
const uint8_t LCD_D5 = 9;  // LCD Data pin 5.
const uint8_t LCD_D6 = 10; // LCD Data pin 6.
const uint8_t LCD_D7 = 11; // LCD Data pin 7.

const uint8_t PIN_FREQUENCY_OUT = 3;     // Pulses on pin D3
const uint8_t SWITCH_FREQUENCY = 35;     // Switch between manual pulsing and tone() at 50 Hz
const uint8_t ITERATIONS = 200;          // Number of iterations to average the potentiometer
const uint16_t POTENTIOMETER_MAX = 1023; // Potentiometer max reading
const uint16_t EXPONENT = 4;             // map potentiometer reading from 0 to 10^4
const uint8_t POTENTIOMETER_CHANGE = 1;  // Threshold for potentiometer change

static float frequency = 1;                 // Initialize frequency
static float delaytime = 500.0 / frequency; // initialize delaytime

static bool pin_mode = false;  // Pulsating pin D3 position (high/low)
float potentiometer_value = 0; // initialize potentiometer value

LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
float previous_potentiometer_value = 0; // Previous Potentiometer Value

// CPU Start Time
static uint32_t start_time_microsecond_precision = micros();

void setup()
{
  // Set the LCD display size
  lcd.begin(20, 2);

  // Print a message to the LCD
  lcd.print("Potentiometer Value:");
}

/**************************************************************************************************
 * Pulsates pin D3
 *
 * @param   start_time_milisecond_precision     Start time in milliseconds
 * @param   start_time_microsecond_precision    Start time in microseconds
 * @param   delaytime                           time delay between pulse edges
 * @param   pin_mode                            Current state of pin D3
 *************************************************************************************************/
void TRIG(uint32_t &start_time_microsecond_precision, float delaytime, bool &pin_mode)
{

  // CPU TIME CALCULATION
  uint32_t EndTimemic = micros();
  int32_t micElapsed = EndTimemic - start_time_microsecond_precision;

  if (micElapsed < 0)
  {
    micElapsed = 0;
  }
  /// Precision is 10^-5 Seconds, totalELap is in ms
  float totalElap = micElapsed;

  if (totalElap >= delaytime)
  {
    if (pin_mode)
    {
      digitalWrite(PIN_FREQUENCY_OUT, LOW);
      pin_mode = false;
    }
    else
    {
      digitalWrite(PIN_FREQUENCY_OUT, HIGH);
      pin_mode = true;
    }
    start_time_microsecond_precision = micros();
  }
}

/**************************************************************************************************
 * Function used to Map 1-1023 to 0-10,000
 *
 * @param   x     Lowest value of potentiometer
 * @param   a     Highest value of potentiometer
 * @param   b     Lowest mapped value
 * @param   c     Highest mapped value
 *
 * @return        Converted value
 *************************************************************************************************/
float map_to_float(float x, float a, float b, float c, float d)
{
  float f = x / (b - a) * (d - c) + c;
  return f;
}

/**************************************************************************************************
 * Function used to snap frequency value
 *
 * @param   frequency   frequency value measure from potentiometer
 *
 * @return              snapped frequency value
 *************************************************************************************************/
uint16_t SNAP(uint16_t frequency)
{
  if (frequency > 10 && frequency < 100)
  {
    return frequency - frequency % 10;
  }
  else if (frequency >= 100 && frequency < 1000)
  {
    return frequency - frequency % 100;
  }
  else if (frequency >= 1000)
  {
    return frequency - frequency % 1000;
  }
  return frequency;
}
void loop()
{

  // CHECK Timing for TRIG
  if (frequency <= SWITCH_FREQUENCY)
  {
    TRIG(start_time_microsecond_precision, delaytime, pin_mode);
  }

  // Collects ITERATIONS samples of Potentiometer value and takes the average
  float X = 0;
  for (int i = 0; i < ITERATIONS; i++)
  {
    X = X + analogRead(A0);
    // CHECK Timing for TRIG
    if (frequency <= SWITCH_FREQUENCY)
    {
      TRIG(start_time_microsecond_precision, delaytime, pin_mode);
    }
  }
  potentiometer_value = X / ITERATIONS;

  // // CHECK Timing for TRIG
  if (frequency <= SWITCH_FREQUENCY)
  {
    TRIG(start_time_microsecond_precision, delaytime, pin_mode);
  }

  // If potentiometer value changes: update LCD, Frequency, delaytime
  if (abs(potentiometer_value - previous_potentiometer_value) >= POTENTIOMETER_CHANGE)
  {
    // If the frequency changes: disable the previous tone
    if (frequency >= SWITCH_FREQUENCY && abs(potentiometer_value - previous_potentiometer_value) > POTENTIOMETER_CHANGE)
    {
      noTone(PIN_FREQUENCY_OUT);
    }

    frequency = pow(10.0, map_to_float((float)potentiometer_value, 0.0, POTENTIOMETER_MAX, 0.0, EXPONENT));
    frequency = (uint16_t)(frequency);
    // Snap frequency value
    frequency = SNAP(frequency);
    delaytime = 1000 * (500 / frequency);

    lcd.clear();
    lcd.print("Frequency:");

    // Move the cursor to the second row and the first column
    lcd.setCursor(0, 1);
    // Print the converted potentiometer value to the LCD
    lcd.print(uint16_t(frequency));
    lcd.print("Hz");
  }
  previous_potentiometer_value = potentiometer_value;

  // CHECK Timing for TRIG
  if (frequency <= SWITCH_FREQUENCY)
  {
    TRIG(start_time_microsecond_precision, delaytime, pin_mode);
  }
  else
  {
    // Use tone() for higher frequencies
    tone(PIN_FREQUENCY_OUT, frequency);
    // Wait a bit before reading the potentiometer again
    delay(100);
  }
}
