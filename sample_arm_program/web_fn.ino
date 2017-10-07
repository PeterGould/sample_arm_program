#include <Arduino.h>

void main_page(){
  File f = SPIFFS.open("/chamber_main.html", "r");
  webServer.streamFile(f,"text/html");
  f.close();
}
