/* Balloon Shield Rev. 1.0
   Test and SD Code - Jesse Austin 11/20/14
   Updated 12/16/2014 - Chris Koehler for CCCE Workshop 2015
      -Modified from WRSG Arduino Workshop Code
   
   Sensor Code for:
   - Temperature x2
   - Humidity
   - Pressure
   - Accelerometer
   - 6 LED Visual Display */

//Include Files for SD card
    #include <SD.h>
    #include <SPI.h>
    
// Controls for the data logging system
    const int LOG_INTERVAL = 500;   // milli seconds between entries

// Time keeper 
    uint32_t timeStamp = 0;     // The time stamp used when recording data points

    const int chipSelect = 8;  // This is set to 8 for the SparkFun uSDCard shield

// Variable for file name
    char logFileName[16];

// Use this LED for your writing indicator
    int ledState = 0;

// Definitions
// Temperature Sensor #1    
    int temp1;
    float temp1Volt;
    float temp1C;
    float temp1F;
    char temp1FString[7];
    
// Temperature Sensor #2   
    int temp2;
    float temp2Volt;
    float temp2C;
    float temp2F;
    char temp2FString[7];
   
// Humidity Sensor    
    int humidity;
    float humidityVolt;
    float RH;
    char RHString[7];
    
// Presure Sensor         
    int pressure;
    float pressureVolt;
    float psi;
    char psiString[7];
    
// Accelerometer X  
    int accelX;
    float accelXVolt;
    float accelXG;
    char accelXGString[7];
    
// Accelerometer Z
    int accelZ;
    float accelZVolt;
    float accelZG;
    char accelZGString[7];
    
// This variable will hold the data from a read cycle
    String dataString = "";
    
//  Titles for column headings
    String sensorNames = "Time Stamp (ms), Temp1 (F), Temp2 (F), RH (%), Pres (psi), XG (g), ZG (g)";

//Geiger Counter Code
    #define LOG_PERIOD 5000                                //Logging period in milliseconds, recommended value 15000-60000.
    #define MAX_PERIOD 60000                               //Maximum logging period
    unsigned int multiplier;                               //variable for calculation CPM in this sketch
    unsigned long previousMillis;                          //variable for time measurement
    unsigned long currentMillis;
    
    // Geiger Counter A
        unsigned long Acounts = 0;                                  //variable for GM Tube events
        unsigned long Acpm = 0;                                     //variable for CPM
        char AcpmString[7];
        void Atube_impulse(){                                   //procedure for capturing events from Geiger Kit
          Acounts++;
        }

    // Geiger Counter B
        unsigned long Bcounts = 0;
        unsigned long Bcpm = 0;                                     //variable for CPM
        char BcpmString[7];
        void Btube_impulse(){                                   //procedure for capturing events from Geiger Kit
          Bcounts++;
        }
    
void setup() {
// put your setup code here, to run once:

//Geiger Counter Code

  multiplier = MAX_PERIOD / LOG_PERIOD;                //calculating multiplier, depend on your log period
  // Geiger Counter A
      //  Acounts = 0;
      //  cpm = 0;
      pinMode(2, INPUT);                                   // set pin INT0 input for capturing GM Tube events
      digitalWrite(2, HIGH);                               // turn on internal pullup resistors, solder C-INT on the PCB
      attachInterrupt(0, Atube_impulse, FALLING);           //define external interrupts
  // Geiger Counter B
      // Bcounts = 0;
      // Bcpm = 0;
      pinMode(1, INPUT);
      digitalWrite(1, HIGH);
      attachInterrupt(0,Btube_impulse,FALLING);

      Serial.begin(9600);

// setup the LED Visual Display
      pinMode(3, OUTPUT);  //SD Card writing
      pinMode(4, OUTPUT);  //Arduino on
      pinMode(5, OUTPUT);  //Sensors/sketch running
      pinMode(6, OUTPUT);  //Sensors/sketch running
      pinMode(7, OUTPUT);  //Sensors/sketch running
      pinMode(9, OUTPUT);  //Sensors/sketch running
      
// turn on Arduino LED

      digitalWrite(4, HIGH);  // Leave on while power is on
    
// SD Card setup 
// make sure that the default chip select pin is set to output, even if you don't use it:
    pinMode(10, OUTPUT);    // To make the SPI (microSD card) interface work Set pin 10 to an Output.  
    pinMode(chipSelect, OUTPUT);     // Set the Chip Select pin for the SparkFun SD card to an Output.

// This function will set up the SD card so we can write to it.
// Also the header of the log file will be written to the file.
    SDCardInit();            
}

void loop() { 
//   put your main code here, to run repeatedly:

//   Turn script running leds OFF at begining of loop
     digitalWrite(5, LOW);
     digitalWrite(6, LOW);
     digitalWrite(7, LOW);
     digitalWrite(9, LOW);
     
//   Delay your time interval
     delay(LOG_INTERVAL);
     
//   Turn script running leds ON at begining of loop     
     digitalWrite(5, HIGH);
     digitalWrite(6, HIGH);
     digitalWrite(7, HIGH);
     digitalWrite(9, HIGH);
     
// Clear out dataString to start a new record.
     dataString = "";

// Log the time
     timeStamp = millis();
     dataString = String(timeStamp);
     Serial.print("Time ");
     Serial.print(timeStamp);

  // Internal Temperature
     temp1 = analogRead(A0);
     temp1Volt = temp1*(5.0/1023);
     temp1C = (temp1Volt - 0.5)/(0.01);
     temp1F = (temp1C*(9.0/5.0) + 32);
     Serial.print("\tTemp1 ");    
     Serial.print(temp1F, 2);
     dtostrf(temp1F, 6, 2, temp1FString);     // Convert sensorVoltage into a String
     dataString = dataString +"," + temp1FString;     // Add to the dataString

  // External Temperature
     temp2 = analogRead(A1);
     temp2Volt = temp2*(5.0/1023);
     temp2C = (temp2Volt - 0.5)/(0.01);
     temp2F = (temp2C*(9.0/5.0) + 32);
     Serial.print("\t Temp2 ");    
     Serial.print(temp2F, 2);
     dtostrf(temp2F, 6, 2, temp2FString);     // Convert sensorVoltage into a String
     dataString = dataString +"," + temp2FString;     // Add to the dataString

  // Humidity
     humidity = analogRead(A2);
     humidityVolt = humidity*(5.0/1023);
     RH = (((humidityVolt/5.0)-0.16)/0.0062);
     Serial.print("\t RH ");  
     Serial.print(RH, 2);
     dtostrf(RH, 6, 2, RHString);                // Convert RH into a String
     dataString = dataString +"," + RHString;    // Add to the dataString

  // Pressure
     pressure = analogRead(A3);
     pressureVolt = pressure*(5.0/1023);
     psi = (pressureVolt-0.5)*(15.0/4.0); 
     Serial.print("\t PSI ");    
     Serial.print(psi, 2);
     dtostrf(psi, 6, 2, psiString);              // Convert psi into a String
     dataString = dataString +"," + psiString;   // Add to the dataString

  // X Acceleration
     accelX = analogRead(A4);
     accelXVolt = accelX*(5.0/1023);
     accelXG = (accelXVolt - (3.3/2))/(0.330);
     Serial.print("\t XG ");    
     Serial.print(accelXG,3);
     dtostrf(accelXG, 6, 2, accelXGString);                // Convert AccelXG into a String
     dataString = dataString +"," + accelXGString;         // Add to the dataString

  // Z Acceleration
     accelZ = analogRead(A5);
     accelZVolt = accelZ*(5.0/1023);
     accelZG = (accelZVolt - (3.3/2))/(0.330);
     Serial.print("\t ZG ");    
     Serial.print(accelZG,3);
     dtostrf(accelZG, 6, 2, accelZGString);                // Convert AccelXG into a String
     dataString = dataString +"," + accelZGString;         // Add to the dataString

  // Geiger Counter Stuff
     currentMillis = millis();
     if(currentMillis - previousMillis > LOG_PERIOD) {  // Determines whether or not the LOG_PERIOD amount of time has passed
         previousMillis = currentMillis;                  //     so it knows whether or not to calculate & save 'cpm'
            
      // Geiger Counter A                              // Calculate Acpm, print it to serial monitor, & save it to SD card
         Acpm = Acounts * multiplier;
         Serial.print("\t CPM ");
         Serial.print(Acpm,1);
         dtostrf(Acpm, 6, 2, AcpmString);
         dataString = dataString +"," + AcpmString;
         Acounts = 0;                                     // Reset 'Acounts' to collect for the next time interval

      // Geiger Counter B
         Bcpm = Bcounts * multiplier;                     // Calculate Bcpm, print to serial monitor, & save to SD card
         Serial.print("\t CPM ");
         Serial.print(Bcpm,1);
         dtostrf(Bcpm, 6, 2, BcpmString);
         dataString = dataString + "," + BcpmString;
         Bcounts = 0;                                     // Reset 'Bcounts' to 0 for the next collection interval
     }   // end of if statement
      
     Serial.println();

  //  Write to the SD card and if a write to SD is successful then toggle the LED
  //  The function writeDataToSD returns TRUE if a write was successful and FALSE if it didn't write to the SD card.
  if(writeDataToSD())
  {
    //  Change the State of the LED from OFF->ON, or ON->OFF
    ++ledState;
    //  Handle the case where the LED state is now 2 and set it back to a valid LED binary value of 0
    if (ledState > 1) {
      ledState = 0;
    }
  }
  //  Update the activity LED to show a successful write cycle
  digitalWrite(3, ledState);   // Change the LED state

} // end of loop


