#define LIGHT_SENSOR 24  // light sensor pin
#define WATER_SENSOR 25 // water sensor pin
#define NUMSTORED 200 // number of samples stored
#define SAMPLES 12 // number of samples displayed
#define NUMDAYS 7 // how many days to store averages of
#define SAMPLESPERDAY 12 // how many samples mark the end of a "day"

//#include <SPI.h>

int waterVals[NUMSTORED] = {0};
int waterAverages[NUMDAYS] = {0};
int lightVals[NUMSTORED] = {0};
int lightAverages[NUMDAYS] = {0};
int samplecount = 0;
int lightintensitycount = 0;
bool tooIntense = false;

void setup() {
  Serial.begin(9600);      // initialize serial communication
}

void loop() {
  // data analysis and updating
  int time = millis() / 100;
  if(time % 100 == 0){
      // update data
      for(int i = NUMSTORED - 2; i >= 0; i--){
        waterVals[i+1] = waterVals[i];
        lightVals[i+1] = lightVals[i];
      }
      waterVals[0] = analogRead(WATER_SENSOR);
      lightVals[0] = analogRead(LIGHT_SENSOR);
      
      samplecount++;
      // update averages
      if(samplecount == SAMPLESPERDAY){
        for(int i = NUMDAYS - 2; i >= 0; i--){
          waterAverages[i+1] = waterAverages[i];
          lightAverages[i+1] = lightAverages[i];
        }
        int sumWater = 0;
        int sumLight = 0;
        for(int i = 0; i < SAMPLESPERDAY; i++){
          sumWater += waterVals[i];
          sumLight += lightVals[i];
        }
        waterAverages[0] = sumWater / SAMPLESPERDAY;
        lightAverages[0] = sumLight / SAMPLESPERDAY;
        samplecount = 0;
        if(lightVals[0] > 600){
          lightintensitycount++;
        }
        else{
          lightintensitycount = 0;
          tooIntense = false;
        }
        if(lightintensitycount > SAMPLESPERDAY / 2){
          tooIntense = true;
        }
      }
    // print data
    Serial.print("{\"moisture\": [");
    for(int j = 0; j < SAMPLES; j++)
    {
      Serial.print(waterVals[j]);
      if(j < SAMPLES - 1)
        Serial.print(", ");
    }
    Serial.println("],");
    
    Serial.print("\"light\": [");
    for(int j = 0; j < SAMPLES; j++)
    {
      Serial.print(lightVals[j]);
      if(j < SAMPLES - 1)
        Serial.print(", ");
    }
    Serial.println("], ");
    
    Serial.print("\"light average\": [");
    for(int j = 0; j < NUMDAYS; j++)
    {
      Serial.print(lightAverages[j]);
      if(j < NUMDAYS - 1)
        Serial.print(", ");
    }
    Serial.println("], ");

    Serial.print("\"water average\": [");
    for(int j = 0; j < NUMDAYS; j++)
    {
      Serial.print(waterAverages[j]);
      if(j < NUMDAYS - 1)
        Serial.print(", ");
    }
    Serial.println("], ");
    Serial.print("\"intense light\": ");
    Serial.println(tooIntense);    
    Serial.println("}");
    Serial.println();
  }
  delay(100);
}
