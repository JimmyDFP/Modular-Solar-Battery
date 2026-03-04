/**
 * @file BMS_Controller_Code.ino
 * @author TBD
 * @brief Manages the battery of our portable charger, disconnecting 
 *        the battery when necessary using a mosfet.
 * @version 0.1
 * @date 2026-03-04
 */

#include <Wire.h>
#include <Adafruit_INA219.h>

Adafruit_INA219 ina219; /// Uses the INA219 library to create an INA219 object to manage the current sensor

/** @brief The pin connected to the gate pin of the mosfet. Currently set to D8 */
const byte gatePin1 = 8;      
/** @brief The pin connected to the TMP36 temperature sensor's VOUT pin. Currently set to A0. Analog Pin required. */
const byte tempSensPin = A0;  
/** @brief The pin connected to a high voltage to compare to Arduino ground. Currently set to A1. Analog Pin required.
    @warning Voltages must be lower than 5 Volts for Arduino. This may change depending on microcontroller */
const byte readVoltPin = A1;  

const byte TEMPERATURE_CUTOFF = 25; /// Threshold for temperature in C to cut off battery

/**
 * @brief Standard Arduino setup.
 * Initializes Serial communication, I2C communication, pin modes, calls 
 * the current sensor setup helper method and sets the mosfets to off, not
 * allowing current through the mosfet.
 */
void setup() {
  pinMode(gatePin1, OUTPUT);
  digitalWrite(gatePin1, LOW);
  //Establishes BAUD rate so we can put text into the serial monitor
  //To get the serial monitor on Arduino IDE open Tools > Serial Monitor 
  Serial.begin(9600);
  Wire.begin();
  currentSensSetup();
  
}

/**
 * @brief Manages the current, temperature, and voltage to shut off access from the battery when needed.
 * TBD...
 */
void loop() {
  //currentSenseDebug()
  //readVoltDebug()
  //tempSenseDebug()
  float currentTemp =readC(tempSensPin);
  if (currentTemp > TEMPERATURE_CUTOFF) {
    setMosfet(0, gatePin1); //Shuts off when temperature above 25 C
  } else if (currentTemp > TEMPERATURE_CUTOFF -2){ 
    setMosfet(1, gatePin1); //Turns back on when temperature cools down to 23 C
  }
  delay(1000);
}

/**
 * @brief Setup helper method for the INA219 Current Sensor.
 * Sets up the INA219 object, if it fails will loop indefinitly. Once the
 * current sensor is set up, will print to the serial monitor.
 */
void currentSensSetup(){

  if (!ina219.begin()) {
    Serial.println("INA219 was not found");
    while (1); //Loops indefinitely if the current sensor fails to setup
  }
  ina219.setCalibration_32V_2A();
  Serial.println("INA219 ready.");

}

/**
 * @brief Manages the state of the Mosfet through the gate pin.
 * When a false statement is given, will set the gate pin to low, thus shutting off the mosfet, 
 * otherwise will set the gate pin high and allowing current through the mosfet.
 * @param theMosState represents the desired state of the mosfet as a boolean.
 * @param thePin represents the pin connected to the mosfet.
 */
void setMosfet(bool theMosState, byte thePin) {
  if (theMosState) {
    digitalWrite(thePin, HIGH);
  } else {
    digitalWrite(thePin, LOW);
  }
}

/**
 * @brief Reads the volt difference between the microcontroller's ground and the given pin.
 * Uses one of the microcontroller analog capable pins to read a voltage difference. Uses the 
 * microcontroller ground as a reference.
 * @warning Voltage difference must be between 0-5 volts for arduino. May change depending on
 *          microcontroller.
 * @param thePin represents the pin being used.
 * @return A float value representing the voltage difference between microcontroller ground
 *         and the parameter pin.
 */
float readVolt(int thePin){
  int reading = analogRead(thePin);
  float Voltage = reading * (5.0/1024.0);
  return Voltage;
}

/**
 * @brief Gets a temperature value in celsius from the TMP36 temperature sensor.
 * Gets the celsius of the TMP36 sensor using the readVolt method. 
 * @param thePin represents the pin being used.
 * @return A float value representing the temperature in celsius of the TMP36.
 */
float readC(int thePin){
  float voltage = readVolt(thePin);
  float tempC = (voltage - 0.5) * 100.0;
  return tempC;
}

/**
 * @brief Gets a temperature value in fahrenheit from the TMP36 temperature sensor.
 * Gets the fahrenheit of the TMP36 sensor using the readC method and converts it to 
 * fahrenheit. 
 * @param thePin represents the pin being used.
 * @return A float value representing the temperature in fahrenheit of the TMP36.
 */
float readF(int thePin){
  float tempC = readC(thePin);
  float tempF = tempC * 9.0 / 5.0 +32.0;
  return tempF;
}

/**
 * @brief Fetches all power data from the INA219 sensor to print.
 * Prints out power data from the INA219 to the serial monitor for debugging purposes.
 * @note Uses the I2C bus (Pins A4/SDA, A5/SCL on Arduino Nano).
 */
void currentSenseDebug() {
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
} 

/**
 * @brief Gets the temperature values from the TMP36 to print.
 * Calls the readC and readF methods and prints them out in order for debugging the
 * TMP36 temperature sensor.
 */
void tempSenseDebug(){
  Serial.print("Temperature in C:");
  Serial.print(readC(tempSensPin));
  Serial.println(" C");

  Serial.print("Temperature in F:");
  Serial.print(readF(tempSensPin));
  Serial.println(" F");
}

/**
 * @brief Gets the voltage values to print.
 * Calls the readVolt method and prints them out in order for debugging the
 * on microcontroller voltage reading.
 */
void readVoltDebug(){
  Serial.print("Voltage: ");
  Serial.print(readVolt(readVoltPin));
  Serial.println(" V");
}

/*




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


*/