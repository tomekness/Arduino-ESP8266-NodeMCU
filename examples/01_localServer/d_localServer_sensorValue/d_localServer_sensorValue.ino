/*
 *  Sensor Input (LDR) - local Server - simple example
 *  ---------------------------------------------------
 *
 * A simple web server printing the value of the analogInput pin (ADC / A0)
 *
 * for NodeMCU ESP8266 | autumn 2018 | tomek n. (https://cirg.io) for retune festival
 * based on the example by https://www.instructables.com/id/Quick-Start-to-Nodemcu-ESP8266-on-Arduino-IDE/
 *
 * (tested with Arduino IDE 1.6.5)
 *
 */

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

int ledPin = LED_BUILTIN; // GPIO16 --> can also declared 16 or D0 
int ldrPin = A0;  // light sensor Pin (ADC / A0)

const char* ssid = "cirg";  // name of your network
const char* password = "workshop";  // pw for your network

const char* yourHostname = "esp_x";  // broadcast name, how to find you in the network

//your server
WiFiServer server(80);


void setup() {

  // start serial connection // for debuging 
  Serial.begin(115200);
  delay(10);

  // setup the LED
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);  // wiring for the onboard LED at the NodeMCU is inverted

  // setup ldr Sensor pin
  pinMode(ldrPin, INPUT);

  // start printing some infos to the serial console // for debuging 
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  // connect to the local wifi
  WiFi.mode(WIFI_STA);  // set wifi mode to Station
  WiFi.begin(ssid, password);

  // while there is no wifi connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Set up mDNS responder:
  while (!MDNS.begin(yourHostname)) {
    Serial.println("Error setting up MDNS responder!");
    delay(1000);
  }
  Serial.println("mDNS responder started");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Add service to MDNS-SD
  MDNS.addService("http", "tcp", 80);

  // Print the IP address, so we know where to connect to
  Serial.println();
  Serial.println();
  Serial.print("Use this URL to connect:  ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
  Serial.print("or use this nice looking adresse to connect:  ");
  Serial.print("http://");
  Serial.print(yourHostname);
  Serial.println(".local/");

}



void loop() {

  // Check if a client has connected
  WiFiClient client = server.available();

  if (client) { // if a browser connect
    digitalWrite(ledPin, LOW); // turn on the led // my board has an inverted LED pin : /
    Serial.println("new client");
    
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;

    while (client.connected()) {

      // Read the first line of the request
      String request = client.readStringUntil('\r');
      Serial.println(request);
      client.flush();

      // read in the sensor value
      int sensorValue = analogRead(ldrPin); // read in the sensor

      // send a standard http response header
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println("Connection: close");  // the connection will be closed after completion of the response

      // this next line makes your browser refresh the page every sec !!!
      client.println("Refresh: 1");  // refresh the page automatically every 1 sec

      client.println();
      client.println("<!DOCTYPE HTML>");

      // we want to change the background color (black to white) of the webpage
      // acordingly to the LDR sensor value 
      client.print("<html style=' background-color: rgb( ");

      // map sensor value (range: 0-1023) to the RGB value range (0–255);
      int rbgByte = map(sensorValue, 0, 1023, 0, 255);
      client.print(rbgByte);    // R
      client.print(",");
      client.print(rbgByte);    // G
      client.print(",");
      client.print(rbgByte);    // B
      client.print(");'>");

      // and as well show the actualy LDR value in the browser
      client.println(" <font color='red'>");
      client.print("analog input is: ");
      client.print(sensorValue);
      client.println("<br />");
      client.println("</font>");

      client.println("</html>");
      break;

    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
    Serial.println();
    Serial.println();
  } else {
    digitalWrite(ledPin, HIGH); // my board has an inverted LED pin : /
  }
}

