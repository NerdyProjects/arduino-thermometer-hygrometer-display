#include <BlueDot_BME280.h>

#include <LiquidCrystal.h>

LiquidCrystal lcd(2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12);
BlueDot_BME280 bme;                                     //Object for Sensor 1

void setup() {
  byte degC[8] = {
    B01011,
    B10110,
    B01100,
    B00100,
    B00100,
    B00110,
    B00011,
  };
  // put your setup code here, to run once:
  bme.parameter.communication = 0;                    //I2C communication for Sensor 1 (bme1)
  bme.parameter.I2CAddress = 0x76;                    //I2C Address for Sensor 2 (bme2)
  bme.parameter.sensorMode = 0b11;                    //Setup Sensor mode for Sensor 2 
  bme.parameter.IIRfilter = 0b100;                   //IIR Filter for Sensor 2
  bme.parameter.humidOversampling = 0b101;            //Humidity Oversampling for Sensor 2
  bme.parameter.tempOversampling = 0b101;              //Temperature Oversampling for Sensor 2
  bme.parameter.pressOversampling = 0b101;             //Pressure Oversampling for Sensor 2 
  bme.parameter.pressureSeaLevel = 1013.25;            //default value of 1013.25 hPa (Sensor 2)
  bme.parameter.tempOutsideCelsius = 15;               //default value of 15°C
  lcd.begin(16,2);
  
  lcd.createChar(0, degC);

  if (!bme.init()) {  
    lcd.print(F("No BME280 sensor"));
    while (1);
  }
/*
 * This needs to happen in the library because the BME280 datasheet states: 
 * The “config” register sets the rate, filter and interface options of the device. Writes to the “config”
register in normal mode may be ignored. In sleep mode writes are not ignored.
 */
  /*bme.writeByte(BME280_CONFIG, ((bme.parameter.IIRfilter << 2) & 0b00011100) | 0b01000000);
  /* set normal mode standby time to 125ms -> ~2 sec IIR filter time */
  /*bme.writeByte(BME280_CONFIG, ((bme.parameter.IIRfilter << 2) & 0b00011100) | 0b01000000); */
  delay(150);
}

void loop() {
  printValues();
  delay(500);
}

void printValues() {
    float temp = bme.readTempC();
    int pre_comma = (int)temp;
    int post_comma = (temp - pre_comma) * 100;
    float humidity = bme.readHumidity();
    float abs_hum = (6.112f * exp((17.67f * temp)/(temp+243.5f)) * humidity * 2.1674f) / (273.15f+temp);
    float fH = (log10(humidity) - 2) / 0.4343 + (17.62 * temp) / (243.12 + temp);
    float fTPkt = 243.12 * fH / (17.62 - fH);
    lcd.setCursor(0,0);
    /*lcd.print(pre_comma);
    lcd.print('.');
    lcd.print(post_comma);
    lcd.print('C ');
*/
    lcd.print(temp);
    lcd.write(byte(0));
    lcd.print("  ");
    lcd.setCursor(7,0);
    lcd.print(bme.readPressure(),1);
    lcd.print("hPa");
    lcd.setCursor(0,1);

    lcd.print(humidity);
    lcd.print("%  ");
    lcd.setCursor(7,1);
    lcd.print("TP");
    lcd.print(fTPkt, 2);
    lcd.write(byte(0));
    lcd.print("   ");
}
