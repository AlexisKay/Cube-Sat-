
//This function is called when the system starts or after a power reset to enable recording dat to the SD card.

boolean SDCardInit()
{
  Serial.println("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect))
  {
    Serial.println("Card failed, or not present");
    delay(100);
    // Can't do anything more.  Try another time....
    return 0;
  }

  // The Card is present so find an unused file name

  Serial.println("searching for an unused file name... ");
  // Start with LOG0.CSV and count up until an unused file name is found.

  for (long i=0; i < 1000; i++) 
  {
    sprintf(logFileName, "LOG%d.CSV", i);  
    if (!SD.exists(logFileName))
    {
      break;
    }    
  }

  Serial.print("The Log filename is:  ");
  Serial.println(logFileName);

  File dataFile = SD.open(logFileName, FILE_WRITE);

  // Write the header including sensor names to the newly opened file
  dataString = sensorNames;

  if (dataFile) 
  {
    dataFile.println(dataString);
    dataFile.close();
    Serial.println("SD Card initialized and data written.");
    return(1);                           // Able to write to SD card
  }
  else
  {
    Serial.println("SD card present but unable to write to file");
    sprintf(logFileName, "LOG0.CSV");           // Clear out the file name to force the program to find a unused file name
    return(0);                            // Unable to write to SD card
  }  
}

