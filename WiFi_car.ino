#include <ESP8266WiFi.h>
#define LED_BUILTIN 2
#define BUILTIN_LED 0
const char* ssid = "IITH-Guest-PWD-IITH@2019";
const char* password = "IITH@2019";

int MOTOR1 = LED_BUILTIN; // led connected to D0
int MOTOR2 = BUILTIN_LED; // led connected to D4
int MOTOR3 = 1; // led connected to D0
int MOTOR4 = 3; // led connected to D0

WiFiServer server(80);

void setup()
{
  Serial.begin(115200);
  pinMode(MOTOR1, OUTPUT);
  digitalWrite(MOTOR1, LOW);

  Serial.print("Connecting to the Newtork");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");  
  server.begin();  // Starts the Server
  Serial.println("Server started");

  Serial.print("IP Address of network: "); // Prints IP address on Serial Monitor
  Serial.println(WiFi.localIP());
  Serial.print("Copy and paste the following URL: https://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}

void loop()
{
  WiFiClient client = server.available();
  if (!client)
  {
    return;
  }
  Serial.println("Waiting for new client");
  while(!client.available())
  {
    delay(1);
  }

  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  int value = 0;
  if(request.indexOf("/CAR=STOP") != -1)
  {
    digitalWrite(MOTOR1, HIGH); // Turn ON LED
    digitalWrite(MOTOR2, HIGH);
    value = 1;
  }
  if(request.indexOf("/CAR=STRAIGHT") != -1)
  {
    digitalWrite(MOTOR1, LOW); // Turn ON LED
    digitalWrite(MOTOR2, LOW);
    value = 0;
  }
  if(request.indexOf("/CAR=BACK") != -1)
  {
    digitalWrite(MOTOR3, LOW); // Turn ON LED
    digitalWrite(MOTOR4, LOW);
    digitalWrite(MOTOR1, HIGH); // Turn ON LED
    digitalWrite(MOTOR2, HIGH);
    value = 4;
  }
  if(request.indexOf("/CAR=RIGHT") != -1)
  {
    digitalWrite(MOTOR1, LOW); // Turn OFF LED
    digitalWrite(MOTOR2, HIGH);
    value = 2;
  }
  if(request.indexOf("/CAR=LEFT") != -1)
  {
    digitalWrite(MOTOR1, HIGH); // Turn OFF LED
    digitalWrite(MOTOR2, LOW);
    value = 3;
  }

/*------------------HTML Page Creation---------------------*/

  client.println("HTTP/1.1 200 OK"); // standalone web server with an ESP8266
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");

  client.print("THE LOOSER: ");
 
  if(value == 0)
  {
    client.print("STRAIGHT");
  }
  else if (value == 1) 
  {
    client.print("STOP");
  }
  else if (value == 4) 
  {
    client.print("BACK");
  }
  else if (value == 2) 
  {
    client.print("RIGHT");
  }
  else if (value == 3) 
  {
    client.print("LEFT");
  }
  
  client.println("<br><br>");
  client.println("<a href=\"/CAR=STRAIGHT\"\"><button>STARIGHT</button></a>");
  client.println("<a href=\"/CAR=BACK\"\"><button>BACK</button></a>");
  client.println("<a href=\"/CAR=STOP\"\"><button>STOP</button></a>");
  client.println("<a href=\"/CAR=RIGHT\"\"><button>RIGHT</button></a>");
  client.println("<a href=\"/CAR=LEFT\"\"><button>LEFT</button></a><br />");
  client.println("</html>");

  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
}
