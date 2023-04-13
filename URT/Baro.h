#ifndef MS5607_02BA03_H
#define MS5607_02BA03_H

#include <SPI.h>
#include <Arduino.h>

#define MS5607_02BA03_CLK_SPEED 1000000

#define RESET 0x1E

#define D1_OSR_256 0x40
#define D1_OSR_512 0x42
#define D1_OSR_1024 0x44
#define D1_OSR_2048 0x46
#define D1_OSR_4096 0x48

#define D2_OSR_256 0x50
#define D2_OSR_512 0x52
#define D2_OSR_1024 0x54
#define D2_OSR_2048 0x56
#define D2_OSR_4096 0x58

#define ADC_READ 0x00

#define PROM_READ_0 0xA0
#define PROM_READ_1 0xA2
#define PROM_READ_2 0xA4
#define PROM_READ_3 0xA6
#define PROM_READ_4 0xA8
#define PROM_READ_5 0xAA
#define PROM_READ_6 0xAC
#define PROM_READ_7 0xAE

#define RESET_DELAY_US 2800 // Microseconds
#define ADC_DELAY_US 8220 // Microseconds

// Datasheet
// https://www.te.com/commerce/DocumentDelivery/DDEController?Action=srchrtrv&DocNm=MS5607-02BA03&DocType=Data+Sheet&DocLang=English
// C Code Exmaple from Manufacturer
// https://www.te.com/commerce/DocumentDelivery/DDEController?Action=showdoc&DocId=Specification+Or+Standard%7FAN520_C-code_example_for_MS56xx%7FA%7Fpdf%7FEnglish%7FENG_SS_AN520_C-code_example_for_MS56xx_A.pdf%7FCAT-BLPS0003
class MS5607_02BA03 {
    public:
        MS5607_02BA03(int CS); 
        ~MS5607_02BA03();

        void reset(); 
        void calibrate();
        uint8_t calculateChecksum();

        void sendCommand(uint8_t command);
        uint16_t readPROM(uint8_t addr); 
        uint32_t readADC(); 

        int32_t calculateTemperature(uint32_t D2);
        int32_t calculatePressure(uint32_t D1, uint32_t D2);
        uint64_t pow2(uint8_t exponent);

    private:
        uint8_t CS_;

        uint16_t C1_;
        uint16_t C2_;
        uint16_t C3_;
        uint16_t C4_;
        uint16_t C5_;
        uint16_t C6_;

        SPISettings settings_;
};    

#endif
