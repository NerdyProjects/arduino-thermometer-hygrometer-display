#include <BlueDot_BME280.h>

#include <LiquidCrystal.h>

LiquidCrystal lcd(2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12);
BlueDot_BME280 bme;                                     //Object for Sensor 1

void setup() {
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

  if (!bme.init()) {  
    lcd.print(F("No BME280 sensor"));
    while (1);
  }

  /* set normal mode standby time to 125ms -> ~2 sec IIR filter time */
  bme.writeByte(BME280_CONFIG, ((bme.parameter.IIRfilter << 2) & 0b00011100) | 0b01000000);
  delay(200);
}

void loop() {
  printValues();
  delay(2000);
}

void printValues() {
    float temp = bme.readTempC();
    int pre_comma = (int)temp;
    int post_comma = (temp - pre_comma) * 100;
    float humidity = bme.readHumidity();
    float abs_hum = (6.112f * exp((17.67f * temp)/(temp+243.5f)) * humidity * 2.1674f) / (273.15f+temp);
    lcd.clear();
    /*lcd.print(pre_comma);
    lcd.print('.');
    lcd.print(post_comma);
    lcd.print('C ');
*/
    lcd.print(temp);
    
    lcd.print("C   ");
    lcd.print((int)(bme.readPressure() + 0.5f));
    lcd.print("hPa");
    lcd.setCursor(0,1);

    lcd.print(humidity);
    lcd.print("%   ");
    lcd.print((int)(abs_hum + 0.5f));
    lcd.print("g/m3");
}
