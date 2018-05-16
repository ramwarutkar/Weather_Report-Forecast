#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

const char* ssid     = "htc connect";         // Hotspot Name
const char* password = "123***78";           //  Hotspot Password.... Guess .... !!!
String APIKEY = "6288ec82f3d667ef8e***b02c034f9bf";   // Generated API form Website
String CityID = "1259229"; // Location


WiFiClient client;
char servername[] = "api.openweathermap.org";
String result;

int  counter = 60;

String weatherDescription = "";
int weatherID;
String weatherLocation = "";
String Country;
float Temperature;
float Humidity;
float Pressure;

void setup() {
  Serial.begin(115200);
  Serial.println("Connecting");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected");
  delay(1000);
}

void loop() {
  if (counter == 60) //Get new data every 10 minutes
  {
    counter = 0;
    displayGettingData();
    delay(1000);
    getWeatherData();
  } else
  {
    counter++;
    displayWeather(weatherLocation, weatherDescription, weatherID);
    delay(5000);
    displayConditions(Temperature, Humidity, Pressure);
    delay(5000);
  }
}

void getWeatherData() //client function to send/receive GET request data.
{
  if (client.connect(servername, 80)) {  //starts client connection, checks for connection
    client.println("GET /data/2.5/weather?id=" + CityID + "&units=metric&APPID=" + APIKEY);
    client.println("Host: api.openweathermap.org");
    client.println("User-Agent: ArduinoWiFi/1.1");
    client.println("Connection: close");
    client.println();
  }
  else {
    Serial.println("connection failed"); //error message if no client connect
    Serial.println();
  }

  while (client.connected() && !client.available()) delay(1); //waits for data
  while (client.connected() || client.available()) { //connected or data available
    char c = client.read(); //gets byte from ethernet buffer
    result = result + c;
  }

  client.stop(); //stop client
  result.replace('[', ' ');
  result.replace(']', ' ');
  Serial.println(result);

  char jsonArray [result.length() + 1];
  result.toCharArray(jsonArray, sizeof(jsonArray));
  jsonArray[result.length() + 1] = '\0';

  StaticJsonBuffer<1024> json_buf;
  JsonObject &root = json_buf.parseObject(jsonArray);
  if (!root.success())
  {
    Serial.println("parseObject() failed");
  }

  String location = root["name"];
  String country = root["sys"]["country"];
  float temperature = root["main"]["temp"];
  float humidity = root["main"]["humidity"];
  String weather = root["weather"]["main"];
  String description = root["weather"]["main"];
  int ID = root["weather"]["id"];
  float pressure = root["main"]["pressure"];

  weatherDescription = description;
  weatherLocation = location;
  weatherID = ID;
  Country = country;
  Temperature = temperature;
  Humidity = humidity;
  Pressure = pressure;

}

void displayWeather(String location, String description, int ID)
{
  Serial.println("------------------");
  Serial.println(location);
  Serial.println(Country);
  Serial.println(description);
  Serial.println(ID);
  if (ID == 803)
  {
    Serial.println("Send Notification");
    Serial.println("");
  }
}

void displayConditions(float Temperature, float Humidity, float Pressure)
{
  Serial.print("T: ");
  Serial.print(Temperature);
  Serial.println("C");

  //Printing Humidity
  Serial.print("H: ");
  Serial.print(Humidity);
  Serial.println("%");

  //Printing Pressure
  Serial.print("P: ");
  Serial.print(Pressure);
  Serial.print("hPa");
  Serial.println("");

}

void displayGettingData()
{
  Serial.println("Getting Data");
}


