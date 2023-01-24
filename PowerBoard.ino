
#include <LiquidCrystal.h>

// Initialize the LCD library with the pins for the display
const uint8_t LCD_EN = 1;
const uint8_t LCD_RS = 0;
const uint8_t LCD_D4 = 8;
const uint8_t LCD_D5 = 9;
const uint8_t LCD_D6 = 10;
const uint8_t LCD_D7 = 11;

const uint8_t PIN_FREQUENCY_OUT = 3;
const uint8_t SWITCH_FREQUENCY = 50;

static float FREQUENCY = 1;
static float DELAYTIME = 500.0 / FREQUENCY;

static bool PIN_MODE = false;
float POTENTIOMETER_VALUE = 0;

LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
float PREVIOUS_POTENTIOMETER_VALUE = 0;

static uint32_t START_TIME_MILLISECOND_PRECISION = millis();
static uint32_t START_TIME_MICROSECOND_PRECISION = micros();

void setup()
{
  // Set the LCD display size
  lcd.begin(20, 2);

  // Print a message to the LCD
  lcd.print("Potentiometer Value:");
}

void TRIG(uint32_t &StarTimilis, uint32_t &START_TIME_MICROSECOND_PRECISION, float DELAYTIME, bool &PIN_MODE)
{

  // CPU TIME CALCULATION
  uint32_t EndTimemil = millis();
  uint32_t EndTimemic = micros();
  uint32_t milElapsed = EndTimemil - StarTimilis;
  int32_t micElapsed = EndTimemic - START_TIME_MICROSECOND_PRECISION - (milElapsed * 1000);

  if (micElapsed < 0)
  {
    micElapsed = 0;
  }

  float totalElap = milElapsed + micElapsed * pow(10, -3); // Precision is 10^-5 Seconds, totalELap is in ms

  if (totalElap >= DELAYTIME)
  {
    if (PIN_MODE)
    {
      digitalWrite(PIN_FREQUENCY_OUT, LOW);
      PIN_MODE = false;
    }
    else
    {
      digitalWrite(PIN_FREQUENCY_OUT, HIGH);
      PIN_MODE = true;
    }
    StarTimilis = millis();
    START_TIME_MICROSECOND_PRECISION = micros();
  }
}

// Function used to Map 1-1023 to 0-10,000
float map_to_float(float x, float a, float b, float c, float d)
{
  float f = x / (b - a) * (d - c) + c;
  return f;
}

void loop()
{

  // CHECK Timing for TRIG
  if (FREQUENCY <= SWITCH_FREQUENCY)
  {
    TRIG(START_TIME_MILLISECOND_PRECISION, START_TIME_MICROSECOND_PRECISION, DELAYTIME, PIN_MODE);
  }

  // Collects 200 samples of Potentiometer value and takes the average
  float X = 0;
  for (int i = 0; i < 200; i++)
  {
    X = X + analogRead(A0);
    // CHECK Timing for TRIG
    if (FREQUENCY <= SWITCH_FREQUENCY)
    {
      TRIG(START_TIME_MILLISECOND_PRECISION, START_TIME_MICROSECOND_PRECISION, DELAYTIME, PIN_MODE);
    }
  }
  POTENTIOMETER_VALUE = X / 200;

  // CHECK Timing for TRIG
  if (FREQUENCY <= SWITCH_FREQUENCY)
  {
    TRIG(START_TIME_MILLISECOND_PRECISION, START_TIME_MICROSECOND_PRECISION, DELAYTIME, PIN_MODE);
  }

  // If potentiometer value changes: update LCD, Frequency, delaytime
  if (abs(POTENTIOMETER_VALUE - PREVIOUS_POTENTIOMETER_VALUE) >= 1)
  {
    // If the FREQUENCY drops below SWITCH_FREQUENCY: disable the tone
    if (FREQUENCY = SWITCH_FREQUENCY && (POTENTIOMETER_VALUE - PREVIOUS_POTENTIOMETER_VALUE) < 1)
    {
      noTone(PIN_FREQUENCY_OUT);
    }

    FREQUENCY = pow(10.0, map_to_float((float)POTENTIOMETER_VALUE, 0.0, 1023.0, 0.0, 4.0));
    FREQUENCY = (uint16_t)(FREQUENCY);
    DELAYTIME = 500 / FREQUENCY;

    lcd.clear();
    lcd.print("Potentiometer Value:");

    // Move the cursor to the second row and the first column
    lcd.setCursor(0, 1);
    // Print the converted potentiometer value to the LCD
    lcd.print(POTENTIOMETER_VALUE);
    lcd.print("      ");
    lcd.print(uint16_t(FREQUENCY));
    lcd.print("Hz");
  }
  PREVIOUS_POTENTIOMETER_VALUE = POTENTIOMETER_VALUE;

  // CHECK Timing for TRIG
  if (FREQUENCY <= SWITCH_FREQUENCY)
  {
    TRIG(START_TIME_MILLISECOND_PRECISION, START_TIME_MICROSECOND_PRECISION, DELAYTIME, PIN_MODE);
  }
  else
  {
    // Use tone() for higher frequencies
    noTone(PIN_FREQUENCY_OUT);
    tone(PIN_FREQUENCY_OUT, FREQUENCY);
    // Wait a bit before reading the potentiometer again
    delay(100);
  }
}
