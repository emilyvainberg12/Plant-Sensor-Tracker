// #ifndef __CC3200R1M1RGC__
// #include <SPI.h>
// #endif
#include <WiFi.h>
//#include <queue>

#define LIGHT_SENSOR 24  // light sensor pin
#define WATER_SENSOR 25
#define SAMPLES 5
#define NUMSTORED 5
#define TIMEPERIOD 10

int waterVals[NUMSTORED] = {0};
int lightVals[NUMSTORED] = {0};

// your network name also called SSID
char ssid[] = "UCLA_WEB";

// your network key Index number (needed only for WEP)
int keyIndex = 0;

WiFiServer server(80);

void setup() {
  Serial.begin(9600);      // initialize serial communication

  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to Network named: ");
  // print the network name (SSID);
  Serial.println(ssid);
  // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
  WiFi.begin(ssid);
  while ( WiFi.status() != WL_CONNECTED) {
    // print dots while we wait to connect
    Serial.print(".");
    delay(300);
  }
  
  Serial.println("\nYou're connected to the network");
  Serial.println("Waiting for an ip address");
  
  while (WiFi.localIP() == INADDR_NONE) {
    // print dots while we wait for an ip addresss
    Serial.print(".");
    delay(300);
  }

  Serial.println("\nIP Address obtained");
  
  // you're connected now, so print out the status  
  printWifiStatus();

  Serial.println("Starting webserver on port 80");
  server.begin();                           // start the web server on port 80
  Serial.println("Webserver started!");
}

void loop() {
   int time = millis() / 1000;
   if(time % 10 == 0){
       for(int i = NUMSTORED - 2; i >= 0; i--){
         waterVals[i+1] = waterVals[i];
         lightVals[i+1] = lightVals[i];
       }
       waterVals[0] = analogRead(WATER_SENSOR);
       lightVals[0] = analogRead(LIGHT_SENSOR);
   }
  
  int i = 0;
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    char buffer[150] = {0};                 // make a buffer to hold incoming data
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (strlen(buffer) == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println();
            // break out of the while loop:
            break;
          }
          else {      // if you got a newline, then clear the buffer:
            memset(buffer, 0, 150);
            i = 0;
          }
        }
        else if (c != '\r') {    // if you got anything else but a carriage return character,
          buffer[i++] = c;      // add it to the end of the currentLine
        }

        if (endsWith(buffer, "GET /ellie")) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:application/json");
          client.println();
          
          client.print("{\"moisture\": [");
          for(int j = 0; j < SAMPLES; j++)
          {
            client.print(waterVals[j]);
            if(j < SAMPLES - 1)
              client.print(", ");
          }
          client.println("],");
          
          client.print("\"light\": [");
          for(int j = 0; j < SAMPLES; j++)
          {
            client.print(lightVals[j]);
            if(j < SAMPLES - 1)
              client.print(", ");
          }
          client.println("]}");
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
}

//
//a way to check if one array ends with another array
//
boolean endsWith(char* inString, char* compString) {
  int compLength = strlen(compString);
  int strLength = strlen(inString);
  
  //compare the last "compLength" values of the inString
  int i;
  for (i = 0; i < compLength; i++) {
    char a = inString[(strLength - 1) - i];
    char b = compString[(compLength - 1) - i];
    if (a != b) {
      return false;
    }
  }
  return true;
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}
