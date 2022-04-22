#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

//#define DEBUG
#define tuje_open D1
#define taras_open D2
#define tyl_open D3

const char *ssid = "UPC5671009";
const char *pass = "r5wxdspekybX";
String host = "http://watergarden.ga";

struct variable {
  String Name;
  // 0 - bool, 1 - int, 2 - float, 3 - string
  int type;
  bool b_val;
  int i_val;
  float f_val;
  String s_val;
};

variable vars[20];
int v_num;

void parse(String list) {
  int it = 0, v_it = 0;
  while(list[it] != ']')
  {
    while(list[it] != '(')
    {
      if(list[it] == ']')
        return;
      it++;
    }
    it++;
    int x = 0;
    while(list[it] >= '0' && list[it] <= '9')
    {
      x = x * 10 + list[it] - '0';
      it++;
    }
    v_it = x;
    
    while(list[it] != '\'') it++;
    it++;
    vars[v_it].Name = "";
    while(list[it] != '\'')
    {
      vars[v_it].Name += list[it];
      it++;
    }
    it++;

    
    String type = "";
    while(list[it] != '\'') it++;
    it++;
    while(list[it] != '\'')
    {
      type += list[it];
      it++;
    }
    it++;

    if(type == "bool") vars[v_it].type = 0;
    if(type == "int") vars[v_it].type = 1;
    if(type == "float") vars[v_it].type = 2;
    if(type == "string") vars[v_it].type = 3;

    String value = "";
    while(list[it] != '\'') it++;
    it++;
    while(list[it] != '\'')
    {
      value += list[it];
      it++;
    }
    it++;

    if(type == "bool")
    {
      if(value == "true")
        vars[v_it].b_val = true;
      else
        vars[v_it].b_val = false;
    }
    if(type == "int")
    {
      vars[v_it].i_val = 0;
      bool neg = false;
      for(int i=0; i<value.length(); ++i)
      {
        if(value[i] == '-')
        {
          neg = true;
          continue;
        }
        vars[v_it].i_val = vars[v_it].i_val * 10 + value[i] - '0';
      }
      vars[v_it].i_val *= -1;
    }
    if(type == "float")
    {
      vars[v_it].f_val = 0;
      bool neg = false;
      bool dot = false;
      float div = 1.0;
      for(int i=0; i<value.length(); ++i)
      {
        if(value[i] == '-')
        {
          neg = true;
          continue;
        }
        if(value[i] == '.')
        {
          dot = true;
          continue;
        }
        if(dot) div *= 10;
        vars[v_it].f_val = vars[v_it].f_val * 10 + value[i] - '0';
      }
      vars[v_it].f_val *= -1;
      vars[v_it].f_val /= div;
    }
    if(type == "string") vars[v_it].s_val = value;

    while(list[it] != ')') it++;
    it++;
  }
  v_num = v_it;
}

void setup() {
  #ifdef DEBUG
    Serial.begin(115200);
    Serial.setTimeout(2000);

    while(!Serial) { }
  #endif

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  //Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    //Serial.print(".");
  }
  //Serial.println();

  //Serial.print("Connected, IP address: ");
  //Serial.println(WiFi.localIP());

  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  digitalWrite(D1, HIGH);
  digitalWrite(D2, HIGH);
  digitalWrite(D3, HIGH);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED)
  {
    WiFi.reconnect();
    //Serial.print("Reconnecting!");
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      //Serial.print(".");
    }
  }

  WiFiClient client;

  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
 
    HTTPClient http;  //Declare an object of class HTTPClient

    String path = "/variables";
 
    http.begin(client, host+path);  //Specify request destination
    int httpCode = http.GET();                                  //Send the request
 
    if (httpCode > 0) { //Check the returning code
 
      String payload = http.getString();   //Get the request response payload
      // Serial.println(payload);
      parse(payload);
 
    }
 
    http.end();   //Close connection
 
  }

  // Serial.print(v_num);
  for(int i=1; i<=v_num; ++i)
  {
    //Serial.print(vars[i].Name);
    //Serial.print(vars[i].b_val);
    if(vars[i].Name == "tuje_open")
    {
      if(vars[i].b_val)
        digitalWrite(tuje_open, LOW);
      else
        digitalWrite(tuje_open, HIGH);
    }
    if(vars[i].Name == "taras_open")
    {
      if(vars[i].b_val)
        digitalWrite(taras_open, LOW);
      else
        digitalWrite(taras_open, HIGH);
    }
    if(vars[i].Name == "tyl_open")
    {
      if(vars[i].b_val)
        digitalWrite(tyl_open, LOW);
      else
        digitalWrite(tyl_open, HIGH);
    }
  }

  //Serial.println("yo");
 
  delay(30000);    //Send a request every 30 seconds
}
