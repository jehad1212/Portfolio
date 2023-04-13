#include <SD.h>

#include "MS5607_02BA03.h"

// #define DEBUG
#define BARO_CS 28

MS5607_02BA03 *barometer;
int dataCount = 0;
int dataPoints = 2*60*10;

const char* filename = "barometer-data5.csv";

void setup(){
    Serial.begin(9600);    
    SPI.begin();

    // Wait for Serial to start up for printing
    delay(2500);
    
    barometer = new MS5607_02BA03(BARO_CS);

    if (!SD.begin(BUILTIN_SDCARD)) {
        Serial.println("ERROR: SD Card failed, or not present.");
        while (1);
    }

    File dataFile = SD.open(filename, FILE_WRITE);
    if (dataFile) {
        dataFile.println("Time (s), Temperature (0.01 deg C), Pressure (0.01 mbar)");
    } else {
        Serial.println("ERROR: Could not open the logging file.");
    }
}

void loop(){
    
    barometer->sendCommand(D1_OSR_4096);
    delayMicroseconds(ADC_DELAY_US);
    uint32_t D1 = barometer->readADC();

    barometer->sendCommand(D2_OSR_4096);
    delayMicroseconds(ADC_DELAY_US);
    uint32_t D2 = barometer->readADC();

    int32_t temp = barometer->calculateTemperature(D2);
    int32_t pressure = barometer->calculatePressure(D1, D2);

    Serial.println("-----");
    Serial.println("The temperature (in 0.01 deg C) is: ");
    Serial.println(temp);
    Serial.println("The pressure (in 0.01 mbar) is: ");
    Serial.println(pressure);

    if (dataCount < dataPoints) {
        File dataFile = SD.open(filename, FILE_WRITE);
        if (dataFile) {
            dataFile.printf("%f, %d, %d\n",0.5*dataCount, temp, pressure);
            dataFile.close();
            Serial.println(dataCount);
        } else {
            Serial.println("ERROR: Could not open the logging file.");
        }
    }

    if (dataCount >= dataPoints) {
        Serial.println("Logging finished.");
    }

    dataCount = dataCount + 1;

    #ifdef DEBUG
        Serial.println("-----");
        Serial.println(barometer->readPROM(PROM_READ_0));
        Serial.println(barometer->readPROM(PROM_READ_1));
        Serial.println(barometer->readPROM(PROM_READ_2));
        Serial.println(barometer->readPROM(PROM_READ_3));
        Serial.println(barometer->readPROM(PROM_READ_4));
        Serial.println(barometer->readPROM(PROM_READ_5));
        Serial.println(barometer->readPROM(PROM_READ_6));
        Serial.println(barometer->readPROM(PROM_READ_7));
        Serial.println(barometer->readPROM(PROM_READ_7));
        Serial.println(barometer->calculateChecksum());
        Serial.println(barometer->readPROM(PROM_READ_7) & 0x00F);
    #endif

    delay(500);

}

