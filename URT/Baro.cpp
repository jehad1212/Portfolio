#include "MS5607_02BA03.h"

// Starts up the device by resetting it, filling in calibration values, and validating the checksum.
// Assumes that SPI.begin(); has already been called.
MS5607_02BA03::MS5607_02BA03(int CS) {
    CS_ = CS;
    pinMode(CS_, OUTPUT);

    settings_ = SPISettings(MS5607_02BA03_CLK_SPEED, MSBFIRST, SPI_MODE0);

    reset();
    calibrate();
    
    uint8_t checksum = readPROM(PROM_READ_7) & 0x00F;
    if (checksum != calculateChecksum()) {
        Serial.println("MS5607_02BA03 checksum is incorrect.");
    } else {
        Serial.println("MS5607_02BA03 checksum is correct.");
    }
}

// Destructor currently not used
MS5607_02BA03::~MS5607_02BA03() {}

// Resets the device memory
void MS5607_02BA03::reset() {
    SPI.beginTransaction(settings_);
    digitalWrite(CS_, LOW);

    SPI.transfer(RESET);
    delayMicroseconds(RESET_DELAY_US); // Can put this externally if needed

    digitalWrite(CS_, HIGH);
    SPI.endTransaction();
}

// Fills in the internal calibration values to convert data into temperature and pressure
void MS5607_02BA03::calibrate() {
    C1_ = readPROM(PROM_READ_1);
    C2_ = readPROM(PROM_READ_2);
    C3_ = readPROM(PROM_READ_3);
    C4_ = readPROM(PROM_READ_4);
    C5_ = readPROM(PROM_READ_5);
    C6_ = readPROM(PROM_READ_6);
}

// Validates the calibration data with the checksum. See AN520 for further information.
uint8_t MS5607_02BA03::calculateChecksum() {
    uint16_t C7 = readPROM(PROM_READ_7);

    uint16_t n_prom[8];
    n_prom[0] = readPROM(PROM_READ_0);
    n_prom[1] = C1_;
    n_prom[2] = C2_;
    n_prom[3] = C3_;
    n_prom[4] = C4_;
    n_prom[5] = C5_;
    n_prom[6] = C6_;
    n_prom[7] = C7;

    uint32_t n_rem = 0x00;
    n_prom[7] = (0xFF00 & (n_prom[7])); 

    for (uint8_t cnt = 0; cnt < 16; cnt++) { 
        if (cnt % 2 == 1)  {
            n_rem ^= (uint16_t) ((n_prom[cnt >> 1]) & 0x00FF);
        } else  {
            n_rem ^= (uint16_t) (n_prom[cnt >> 1] >> 8);
        }

        for (uint8_t n_bit = 8; n_bit > 0; n_bit--) {
            if (n_rem & (0x8000)) {
                n_rem = (n_rem << 1) ^ 0x3000;
            } else {
                n_rem = (n_rem << 1);
            }
        }
    }

    n_rem = (0x000F & (n_rem >> 12)); 
    return (n_rem ^ 0x00); 
}

// Tells the ADC to convert either temperature or pressure. Wait ADC_DELAY_US until calling readADC
void MS5607_02BA03::sendCommand(uint8_t command) {
    SPI.beginTransaction(settings_);
    digitalWrite(CS_, LOW);

    SPI.transfer(command);
    
    digitalWrite(CS_, HIGH);
    SPI.endTransaction();
}

// Reads and returns data stored in addr in PROM
uint16_t MS5607_02BA03::readPROM(uint8_t addr) {
    SPI.beginTransaction(settings_);
    digitalWrite(CS_, LOW);

    SPI.transfer(addr);
    uint16_t result = SPI.transfer(0x00) << 8 | SPI.transfer(0x00);
    
    digitalWrite(CS_, HIGH);
    SPI.endTransaction();

    return result;
} 

// Reads the result of the ADC after conversion
uint32_t MS5607_02BA03::readADC(){ 
    SPI.beginTransaction(settings_);
    digitalWrite(CS_, LOW);

    SPI.transfer(ADC_READ);
    uint32_t result = (SPI.transfer(0x00) << 16 | SPI.transfer(0x00) << 8) | SPI.transfer(0x00);
    
    digitalWrite(CS_, HIGH);
    SPI.endTransaction();

    return result;
} 

// Calculates temperature in units of 0.01 degrees C
int32_t MS5607_02BA03::calculateTemperature(uint32_t D2) {
    int32_t dT = D2 - (C5_*pow2(8));
    int32_t temp = 2000 + ((dT*C6_)/pow2(23));

    if (temp < 2000) {
        int64_t t2 = ((dT * dT)/pow2(31));
        temp = temp - t2;
    }

    return temp;
}

// Calculates pressure in units of 0.01 mbar
int32_t MS5607_02BA03::calculatePressure(uint32_t D1, uint32_t D2) {
    int32_t dT = D2 - (C5_*pow2(8));
    int32_t temp = 2000 + ((dT*C6_)/pow2(23));

    int64_t off = C2_*pow2(17) + (C4_*(dT/pow2(6)));
    int64_t sens = C1_*pow2(16) + (C3_*(dT/pow2(7))); 
    
    int64_t off2 = 0;
    int64_t sens2 = 0;

    if (temp < 2000) {
        int32_t t = temp - 2000;
        off2 = 61*((t*t)/pow2(4));
        sens2 = 2*t*t;
    }

    if (temp < -1500) {
        int32_t t = temp + 1500;
        off2 = off2 + 15*t*t;
        sens2  = sens2 + 8*t*t;
    }

    off = off - off2;
    sens = sens - sens2;
    
    int32_t pressure = (((D1*sens)/pow2(21)) - off) / pow2(15);
    return pressure;
}

// Calculates powers of 2 using bit shifting
uint64_t MS5607_02BA03::pow2(uint8_t exponent) {
    uint64_t answer = 1;
    return (answer << exponent);
}
