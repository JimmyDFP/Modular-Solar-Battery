// use i^2c communicatio and adafruits driver (each should be provided by an arduino apparently to make prototyping easy)
#include <Wire.h>
#include <Adafruit_INA219.h>
//note for adafruit you do need to install from sketch, include libaries, manage libraries, and adafruit INA219

Adafruit_INA210 ina219; //creates the INA210 object

void setup() {
  pinMode(gatePin, OUTPUT);
  digitalWrite(gatePin, LOW); // we wanna start initally with the mosfet off
  //Establishes BAUD rate so we can put text into the serial monitor
  //To get the serial monitor on Arduino IDE open Tools > Serial Monitor 
  Serial.begin(9600);
  //start I2C
  Wire.begin();

  //initialized INA219
  if (!ina219.begin()) {
    Serial.println("INA219 was not found")
    while (1); //stop if sensor is not detected.
  }
  //sets calibration, mostly that the Vbus can max hold 32V and current sensor can hold max 2A current
  ina219.setCalibration_32V_2A();

//if everything goes well and its connected, its ready how is
  Serial.println("INA219 ready.");
}


//this is the main code now... how to basically run both the mostfet etc.
void loop() {
currentSense();
delay(1000)
}


//for this function its a little weird, for some reason we dont need pins in INA219, as it connects via, 5V > VCC, GND > GND, A4 pin > SDA, A5 pin > SCL, and just reads outright without stating pins
//the logic told to me is that a4-a5 no v oltage is read on those pins, no signal goes into analog read(), and no pin is manually toggled, instead arduino uses its own hardware, I^2c, to which pins are allowed other functions
// THIS MEANS WE NEEEEEEEEEEED TO USE A4 and A5wait so  in specific for the code, these CANNOT be changed
void currentSense() {
  float busVoltage   = ina219.getBusVoltage_V();     // Volts
  float shuntVoltage = ina219.getShuntVoltage_mV();  // mV
  float current_mA   = ina219.getCurrent_mA();       // mA
  float power_mW     = ina219.getPower_mW();         // mW

  Serial.print("Bus Voltage: ");
  Serial.print(busVoltage);
  Serial.println(" V");

  Serial.print("Shunt Voltage: ");
  Serial.print(shuntVoltage);
  Serial.println(" mV");

  Serial.print("Current: ");
  Serial.print(current_mA);
  Serial.println(" mA");

  Serial.print("Power: ");
  Serial.print(power_mW);
  Serial.println(" mW");
  //all this below is just telling the things of a current sensor, the ina219 coding is from apparently MADE for arduino from ADAfruit, coding to make it easy to implement etc.
} 

void setMosfet(bool turnOff) {
  if (turnOff) {
    digitalWrite(gatePin, LOW);
  } else {
    digitalWrite(gatePin, HIGH);
  }
} 


float readVolt(int pin){
  int voltReading = analogRead(pin);
  float Voltage = reading * (5.0/1024.0);
  return voltage;
}
float readC(int pin){
  float voltage = readVolt(pin);
  float tempC = (voltage - 0.5) * 100.0;
  return tempC;
}
float readF(int pin){
  float tempC = readC(pin);
  float tempF = tempC * 9.0 / 5.0 +32.0
  return tempF
}