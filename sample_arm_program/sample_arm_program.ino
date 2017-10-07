extern void main_page();
extern void web_parser();
extern void arm_update();
extern void settings_report();

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include "FS.h"
#include <sample_arm.h>
#include <TimedAction.h>

//web setup
const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;
ESP8266WebServer webServer(80);
int web_buffer[10];
String web_string_buffer[10];
sample_arm arm;
TimedAction update_arm(50,arm_update);
TimedAction report_settings(1000,settings_report);

//global variables
void setup() {
  Serial.begin(9600);
  arm.begin();
  SPIFFS.begin();
  //wifi setup//////////////////////////////////////////////////
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  String ap_name = arm.get_name();
  WiFi.softAP(ap_name.c_str());
  // modify TTL associated  with the domain name (in seconds)
  // default is 60 seconds
  dnsServer.setTTL(300);
  // set which return code will be used for all other domains (e.g. sending
  // ServerFailure instead of NonExistentDomain will reduce number of queries
  // sent by clients)
  // default is DNSReplyCode::NonExistentDomain
  dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);

  // start DNS server for a specific domain name
  dnsServer.start(DNS_PORT, "www.ocsampler.com", apIP);

  //direct web pages and functions
  webServer.on("/",main_page);
  webServer.on("/web_parser",web_parser);
  webServer.begin();

}

void loop() {
  dnsServer.processNextRequest();
  webServer.handleClient();
  update_arm.check();
  report_settings.check();
}
