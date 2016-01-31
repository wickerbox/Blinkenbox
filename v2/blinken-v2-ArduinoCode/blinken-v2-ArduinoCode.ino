#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#define ledPin 0
#define redPin 13
#define greenPin 14
#define bluePin 12
#define MAXADC 1023
#define neoPin 20
#define numPix 4

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(numPix, neoPin, NEO_GRB + NEO_KHZ800);
int delayval = 500;

int value = 0;
int i;
int red = 0;
int green = 0; 
int blue = 0;

const char* html = "<html>\
  <head>\
    <meta http-equiv='refresh' content='5'/>\
    <meta name='viewport' content='width=device-width, initial-scale=1'>\
    <title>OSH Park Blinkenbox</title>\
    <link rel=\"stylesheet\" type=\"text/css\" href=\"http://192.155.84.92/blinken.css\">\
  </head>\
<body>\
<div class='content'>BlinkenBox Control System Deluxe 3000X<br /><br />\
<form action='/redup'><button type='submit' align='center' value='/redup' class='triangle-up-red wide'></button></form>\
<form action='/greenup'><button type='submit' align='center' class='triangle-up-green wide'></button></form>\
<form action='/blueup'><button type='submit' align='center' class='triangle-up-blue wide'></button></form><br><br>\
<form action='/reddown'><button type='submit' align='center' class='triangle-down-red wide'></button></form>\
<form action='/greendown'><button type='submit' align='center' class='triangle-down-green wide'></button></form>\
<form action='/bluedown'><button type='submit' align='center' class='triangle-down-blue wide'></button></form><br><br><br><br>\
<div class='button-wrapper'>\
<form action='/presetblue'><button type='submit' align='center' class='round-button-blue wide'></button></form>\
<form action='/presetred'><button type='submit' align='center' class='round-button-red wide'></button></form>\
<form action='/presetgreen'><button type='submit' align='center' class='round-button-green wide'></button></form></div>\
<div class='button-wrapper'>\
<form action='/presetaqua'><button type='submit' align='center' class='round-button-aqua wide'></button></form>\
<form action='/presetpurple'><button type='submit' align='center' class='round-button-purple wide'></button></form>\
<form action='/presetyellow'><button type='submit' align='center' class='round-button-yellow wide'></button></form>\
</div>\
<div style='clear:both;'></div>\
</div>\
</body>\
</html>";

//const char* ssid     = "HOME-3842";
//const char* password = "B18DF2225F96F92C";
const char* ssid     = "Birdhouse";
const char* password = "Blondy123";
ESP8266WebServer server(80);

const char* host = "www.adafruit.com";
   
void handle_root() {
  server.send(200, "text/html", html);
  delay(100);
}

void setup() {
  
  Serial.begin(115200);
  delay(10);
  
  connect_to_wifi();

  server.on("/", handle_root);

  server.on("/redup", upRed);
  server.on("/blueup", upBlue);
  server.on("/greenup", upGreen);
  server.on("/reddown", dnRed);
  server.on("/bluedown", dnBlue);
  server.on("/greendown", dnGreen);

  server.on("/presetred", [](){ setColor(MAXADC, 0, 0);server.send(200, "text/html", html);delay(100);});
  server.on("/presetblue", [](){ setColor(0, 0, MAXADC);server.send(200, "text/html", html);delay(100);});
  server.on("/presetgreen", [](){ setColor(0, MAXADC, 0);server.send(200, "text/html", html);delay(100);});
  server.on("/presetaqua", [](){ setColor(0, MAXADC, MAXADC);server.send(200, "text/html", html);delay(100);});
  server.on("/presetyellow", [](){ setColor(700, 300, 0);server.send(200, "text/html", html);delay(100);});
  server.on("/presetpurple", [](){ setColor(400, 0, 500);server.send(200, "text/html", html);delay(100);});
  
  server.begin();
  Serial.println("HTTP server started");
  
  pixels.begin();
}   
 
void loop() {
  server.handleClient();

  for(int i=0;i<numPix;i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(0,150,0)); // Moderately bright green color.

    pixels.show(); // This sends the updated pixel color to the hardware.

    delay(delayval); // Delay for a period of time (in milliseconds).
  }
}

void upRed(void) {
  if (red <= 920) red += 100;
  setColor(red, green, blue);
  server.send(200, "text/html", html);
  delay(100);
}

void upGreen(void) {
  if (green <= 920) green += 100;
  setColor(red, green, blue);
  server.send(200, "text/html", html);
  delay(100);
}

void upBlue(void) {
  if (blue <= 920) blue += 100;
  setColor(red, green, blue);
  server.send(200, "text/html", html);
  delay(100);
}

void dnRed(void) {  
  if (red >=100 ) red -= 100;
  setColor(red, green, blue);
  server.send(200, "text/html", html);
  delay(100);
}

void dnBlue(void) {
  if (blue >=100 ) blue -= 100;
  setColor(red, green, blue);
  server.send(200, "text/html", html);
  delay(100);
}

void dnGreen(void) {
  if (green >=100 ) green -= 100;
  setColor(red, green, blue);
  server.send(200, "text/html", html);
  delay(100);
}

void setColor(int setred, int setgreen, int setblue)
{
  red = setred;
  green = setgreen;
  blue = setblue;
  
  Serial.println("setColor.");
  #ifdef COMMON_ANODE
    red = MAXADC - red;
    green = MAXADC - green;
    blue = MAXADC - blue;
  #endif
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);  
}

// Connect to the Wifi
void connect_to_wifi() {
    // We start by connecting to a WiFi network
 
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid,password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

}

//  Simple HTTP get webclient test
void adafruit_wifi_test() {

  delay(5000);
  ++value;
 
  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // We now create a URI for the request
  String url = "/testwifi/index.html";
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(10);
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  //Serial.println("closing connection");
  
  }
