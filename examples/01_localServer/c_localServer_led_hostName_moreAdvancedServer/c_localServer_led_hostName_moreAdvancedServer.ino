/*
 *  LED - local Server - simple example
 *  ------------------------------------
 *
 * LED controlled by local Webpage (served by the ESPmodule it self)
 * create hostname using mDNS (Multicast DNS)
 * using a more advanced web server 
 *
 * for NodeMCU ESP8266 | autumn 2018 | tomek n. (https://cirg.io) for retune festival
 * based on the esp8266 libraray example
 *
 * (tested with Arduino IDE 1.6.5)
 *
 */

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>


int ledPin = LED_BUILTIN; // GPIO16 --> can also declared 16 or D0 

const char* ssid = "cirg";  // name of your network
const char* password = "workshop";  // pw for your network
const char* yourHostname = "esp_x";  // broadcast name, how to find you in the network

//your server
ESP8266WebServer server(80);  // this one is more advanced then the normal WiFiServer

// the two different webpages we are going to host

// note: you can use the processing (p5, processing.org) snippet to generate a string like the one below from a html file
// check out the processing examples in the »x_p5_htmlParsing« folder

const char* serverOnIndex =  " <html style='background-color: #fff'><font color=black>Led is ON! </font><br /> <br /><a href=\'/off\'><button>Turn Off </button></a><br /></html> ";
const char* serverOffIndex =  " <html style='background-color: #000'><font color=white>Led is OFF! </font><br /> <br /><a href=\'/on\'><button>Turn On </button></a><br /></html> ";

void setup() {

  // start serial connection // for debuging 
  Serial.begin(115200);
  delay(10);

  // setup the LEDs
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH); // wiring for the onboard LED at the NodeMCU is inverted

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
    Serial.print(".");  // just print some thing to the serial console to see you are still alive
  }

  // Set up mDNS responder and start server
  if (WiFi.waitForConnectResult() == WL_CONNECTED) {   // just double checking
    MDNS.begin(yourHostname);
    startServer();
    MDNS.addService("http", "tcp", 80);
    printInfos();
  } else {
    Serial.println("WiFi Failed");
  }

}

void loop() {

  // if some one likes an update
  server.handleClient();
  delay(1);
}


void startServer() {

  // a webpage for the LED --> On
  server.on("/on", []() {
    Serial.println("led on request received");
    server.sendHeader("Connection", "close");
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "text/html", serverOnIndex);// the 200 is the html Status Code for »ok« 
    // check out https://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html for more infos in the hmtl Status Code Definitions
   
    // trun of the led
    digitalWrite(ledPin, LOW); // my board has an inverted LED pin : /
  });

  // a webpage for the LED --> Off
  server.on("/off", HTTP_GET, []() {
    Serial.println("led off request received");
    server.sendHeader("Connection", "close");
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "text/html", serverOffIndex); // the 200 is the html Status Code for »ok« 
    // check out https://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html for more infos in the hmtl Status Code Definitions
   
   // trun off the led
    digitalWrite(ledPin, HIGH); // my board has an inverted LED pin : /
  });

  // a webpage to show the IP of the ESP8288 - module
  server.on("/ip", []() {
    Serial.println("led on request received");
    server.sendHeader("Connection", "close");
    server.sendHeader("Access-Control-Allow-Origin", "*");
     IPAddress ipno = WiFi.localIP();
     char ipno2[26] ; 
     sprintf(ipno2, "%d.%d.%d.%d", ipno[0], ipno[1], ipno[2], ipno[3]);
    server.send(200, "text/plain", ipno2);
  });

  // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"
  server.onNotFound(handleNotFound);        

  // start the server 
  server.begin();
}



void printInfos() { // just printing some infos to the serial terminal

  Serial.println("");
  Serial.println("WiFi connected");

  Serial.println("Server started");

  // Print the IP address, so we know where to connect to
  Serial.print("my IP adress is: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.print("To turn the LED on connect to: http://");
  Serial.print(yourHostname);
  Serial.println(".local/on");

  Serial.print("To turn the LED off connect to: http://");
  Serial.print(yourHostname);
  Serial.println(".local/off");
  
  Serial.println();

}


void handleNotFound(){
  server.send(404, "text/plain", "404: ahhhh, some thing went wrong, but your are close, i guess, at least it's the right device, your options are /on /off /toggle /ip"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}



