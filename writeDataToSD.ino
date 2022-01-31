/*
 This function writes data to the SD card.  It returns a 1 (TRUE), if it 
 successfully wrote to the SD Card and if there was an error it returns a 0 (FALSE). 
 
 */

boolean writeDataToSD ()
{
  File dataFile = SD.open(logFileName, FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) 
  {
    dataFile.println(dataString);
    dataFile.close();
    return(1);                                              // Return a 1 since the write was successful LED Blink
  }  

  // if the file isn't open, notify that there was an error and re-initialize  the card
  else {
    Serial.println();
    Serial.print("error writing to file: ");
    Serial.println(logFileName);
    dataFile.close();
    delay(100);
    Serial.println("Re - Initializing SD card...");
    return(SDCardInit());                                    //Return a 0 if successful to disable LED Blink
  }

}

