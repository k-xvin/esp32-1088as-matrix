#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <WiFi.h>
#include <HTTPClient.h>

// Define the number of devices we have in the chain and the hardware interface
// NOTE: These pin numbers will probably not work with your hardware and may
// need to be adapted
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4

// https://github.com/MajicDesigns/MD_MAX72XX/issues/34
#define CLK_PIN   13
#define DATA_PIN  11
#define CS_PIN    15

// Hardware SPI connection
MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

//https://techtutorialsx.com/2017/05/19/esp32-http-get-requests/

const char* ssid = "network";
const char* password = "password";
void init_wifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  Serial.print("RRSI: ");
  Serial.println(WiFi.RSSI());
}

String get_request(String url){
  if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status
    HTTPClient http;
  
    http.begin(url); //Specify the URL
    int httpCode = http.GET();                                        //Make the request
  
    if (httpCode > 0) { //Check for the returning code
  
        String payload = http.getString();
        return payload;
      }
  
    else {
      Serial.println("Error on HTTP request");
    }
  
    http.end(); //Free the resources
  }
  else {
    Serial.println("not connected");
  }
}

void setup(void){
  Serial.begin(57600);
  P.begin();
  init_wifi();
}

bool requested = false;
String response = "placeholder";
void loop(void){
  if(P.displayAnimate()){
    // Serial.println("getting msg");
    // https://pastebin.com/pkTeQ2Vj
    response = get_request("https://pastebin.com/raw/pkTeQ2Vj");
    requested = true;
    P.displayScroll(response.c_str(), PA_RIGHT, PA_SCROLL_LEFT, 55);

    // wait 5s before going into the next one
    delay(5000);
  }
  else {
    P.displayAnimate();
    // Serial.println("displaying msg");
  }

}