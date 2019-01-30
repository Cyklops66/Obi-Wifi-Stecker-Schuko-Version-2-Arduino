/*
 * Creation:    02.11.2016
 * Last Update: 20.12.2016
 *
 * Copyright (c) 2016 by Georg Kainzbauer <http://www.gtkdb.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * modified by Christian Zerrahn for Obi Wifi Schuko Version 2
 

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

// wireless network credentials:
const char* ssid = "YourSSID";
const char* password = "YOURWIFIPASSWD";
const char* host = "webcontrol";

bool relay_status = false;
bool inp_status = 1;

void setRelay(bool new_status){
  relay_status = new_status;
    
  if (relay_status){
    digitalWrite(4, HIGH);
    delay(50);
    digitalWrite(13, HIGH);
          Serial.println("Oni");     

  } else {
    digitalWrite(4, LOW);
    delay(50);
    digitalWrite(13, LOW);
      Serial.println("OFFi");     

  }

}

const int igitt=1;

// web server on port 80:
ESP8266WebServer server(80);

String getContent()
{
  // create content for the website:
  String content = "<html><head><Obi Wifi Schuko</title></head><body>";
  content += "OBI WIFI SCHUKO<br /><br />";
  content += "AN/AUS:  </a>&nbsp;<a href=\"AN\"><button>AN</button></a>&nbsp;<a href=\"AUS\"><button>AUS</button></a><br />";

  content += "</body></html>";

  return content;
}

void setup()
{
  pinMode(5, INPUT_PULLUP); // Push button
  pinMode(12, OUTPUT); // Green LED
  pinMode(13, OUTPUT); // PowerOn/Off LED
  pinMode(4, OUTPUT); // Relay off

  // initialize serial port for debugging purpose:
  Serial.begin(115200);

  // connect to the wireless network:
  WiFi.begin(ssid, password);

  // wait for wireless network connection and print connection settings:
  Serial.println("");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("Wireless network connection established.");
  Serial.print("SSID: ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  digitalWrite(12,HIGH); // Green Status LED



  // initialize mDNS:
  if (MDNS.begin(host))
  {
    Serial.println("mDNS responder started");
  }
  MDNS.addService("http", "tcp", 80);

  // start HTTP server:
  server.begin();
  Serial.println("HTTP server started");

  // print start page:
  server.on("/", [](){
    server.send(200, "text/html", getContent());
  });

  // control output1:
  server.on("/AN", [](){
    if (igitt)
    {
      digitalWrite(4, 1);
      digitalWrite(13,HIGH); //Power LED      
      Serial.println("On");     
    }
    server.send(200, "text/html", getContent());
    delay(1000);
  });


    // control output1:
  server.on("/AUS", [](){
    if (igitt)
    {
      digitalWrite(4, 0);
      digitalWrite(13,LOW); //Power LED
      delay(50);
      Serial.println("Off");
    }
    server.send(200, "text/html", getContent());
    delay(1000);
  });

  

}

void loop()
{
    bool inp = digitalRead(5);
    if (inp == 0 and inp != inp_status) {
    setRelay(!relay_status);
    delay(150);
    }
  server.handleClient();
}
