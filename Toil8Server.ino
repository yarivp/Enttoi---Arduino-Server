#include <RH_ASK.h>
#include <SPI.h> // Not actualy used but needed to compile
#include <EtherCard.h>

// OAUTH key from http://arduino-tweet.appspot.com/
#define TOKEN   "Insert-your-token-here"

// ethernet interface mac address, must be unique on the LAN
byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };

const char website[] PROGMEM = "enttoi-gw.azurewebsites.net";

static byte session;

byte Ethernet::buffer[700];
Stash stash;

RH_ASK driver;
bool Door1State = false;
bool Door2State = false;

static void sendToToil8 (int cabin, int state, String data) {

  byte sd = stash.create();
  Serial.println("Update");
  //const char tweet[] = "";
  stash.print(data);
  stash.save();
  int stash_size = stash.size();

  // Compose the http POST request, taking the headers below and appending
  // previously created stash in the sd holder.
  /*Stash::prepare(PSTR("POST http://$F/update HTTP/1.0" "\r\n"
    "Host: $F" "\r\n"
    "Content-Length: $D" "\r\n"
    "\r\n"
    "$H"),
  website, website, stash_size, sd);*/
  if(cabin == 0)
  {
    if(state == 0)
    {
      Stash::prepare(PSTR("POST /sensor HTTP/1.1\r\nAuthorization: xxx\r\nHost: enttoi-gw.azurewebsites.net:80\r\nAccept: */*\r\nContent-Length: 76\r\nContent-Type: application/json\r\n\r\n{\r\n\"sensorType\": \"cabin_door\",\r\n\"sensorId\": 0,\r\n\"state\":"),
      website, website, stash_size, sd);
    }
    else
    {
      Stash::prepare(PSTR("POST /sensor HTTP/1.1\r\nAuthorization: xxx\r\nHost: enttoi-gw.azurewebsites.net:80\r\nAccept: */*\r\nContent-Length: 76\r\nContent-Type: application/json\r\n\r\n{\r\n\"sensorType\": \"cabin_door\",\r\n\"sensorId\": 0,\r\n\"state\":"),
      website, website, stash_size, sd);
    }
  }
  if(cabin == 1)
  {
    if(state == 0)
    {
      Stash::prepare(PSTR("POST /sensor HTTP/1.1\r\nAuthorization: xxx\r\nHost: enttoi-gw.azurewebsites.net:80\r\nAccept: */*\r\nContent-Length: 76\r\nContent-Type: application/json\r\n\r\n{\r\n\"sensorType\": \"cabin_door\",\r\n\"sensorId\": 0,\r\n\"state\":"),
      website, website, stash_size, sd);
    }
    else
    {
      Stash::prepare(PSTR("POST /sensor HTTP/1.1\r\nAuthorization: xxx\r\nHost: enttoi-gw.azurewebsites.net:80\r\nAccept: */*\r\nContent-Length: 76\r\nContent-Type: application/json\r\n\r\n{\r\n\"sensorType\": \"cabin_door\",\r\n\"sensorId\": 0,\r\n\"state\":"),
      website, website, stash_size, sd);
    }
  }

  // send the packet - this also releases all stash buffers once done
  // Save the session ID so we can watch for it in the main loop.
  session = ether.tcpSend();
}

void setup()
{
    Serial.begin(9600);  // Debugging only
    if (!driver.init())
         
    //Serial.begin(57600);
  
    if (ether.begin(sizeof Ethernet::buffer, mymac) == 0) 
      
    if (!ether.dhcpSetup())
   
  
    ether.printIp("IP:  ", ether.myip);
    ether.printIp("GW:  ", ether.gwip);  
    ether.printIp("DNS: ", ether.dnsip);  
  
    if (!ether.dnsLookup(website))
      
  
    ether.printIp("SRV: ", ether.hisip);
}

void UpdateServer(int DoorID, bool DoorState)
{
    String command = "{\r\n\"sensorType\": \"cabin_door\",\r\n\"sensorId\": ";
    command += DoorID;
    command += ",\r\n\"state\":";
    command += DoorState;
    command += "\r\n}";
    sendToToil8(DoorID, DoorState, command);
}

void loop()
{
  Serial.print("-1");
    uint8_t buf[7];
    uint8_t buflen = sizeof(buf);
    if (driver.recv(buf, &buflen)) // Non-blocking
    {
      Serial.print("0");
      if(buf[4]-48 == 0)
      {
        Serial.print("1");
        if(!Door1State ==  buf[6]-48)
        {
          Door1State = buf[6]-48;
          UpdateServer(0, Door1State);
        }
      }
      if(buf[4]-48 == 1)
      {
        Serial.print("2");
        if(!Door2State ==  buf[6]-48)
        {
          Door2State = buf[6]-48;
          UpdateServer(1, Door2State);
        }
      }
    }
}

