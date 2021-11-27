#include <ArduinoJson.h>
#include <WiFi.h>

const char* ssid = "Xperia XZ3_f7ca";
const char* password = "qwer1234";
const char* host1 = "api.openweathermap.org";
const char* resource = "/data/2.5/weather?id=1668341&appid=8956b98c700c7714cadc75da8b594967"; 
char jsonRead[600]; //Ū��response���x�sJSON��ƪ��ܼơA�����O�����ܼ�

const char* host = "maker.ifttt.com"; //IFTTT server���}
const char* event = "LINE01";  //IFTTT�ƥ�W��
const char* apiKey = "ZqsSdsicipA92tECQcsyR";  //IFTTT Applet key

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
}

void loop(){

    //�Hchar*�榡�x�s�^�Ǫ�json
    char* json = ClientRequest();
    
    Serial.println(json);
    
    //�N�r���ഫ��JSON�A�x�s�bdoc��
    StaticJsonDocument<600> doc;
    deserializeJson(doc, json);
    
    const char* location = doc["name"]; 
    const char* weather = doc["weather"][0]["description"];
    double temp = doc["main"]["temp"];
    double humidity = doc["main"]["humidity"];
    String weather1 = "";
    weather1 += weather;
        
    Serial.print("*** ");
    Serial.print(location);
    Serial.println(" ***");
    Serial.print("Type: ");
    Serial.println(weather);
    Serial.print("Temp: ");
    Serial.print(temp - 273);
    Serial.println("C");
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println("%");
    Serial.println("--------------------");
    
    ClientRequest(String(temp-273), String(humidity), weather1);  //Client�ǰe���
 
    delay(3000); // the OWM free plan API does NOT allow more then 60 calls per minute
}
void ClientRequest(String value1, String value2 ,String value3){
  
  WiFiClient client;  //�إ�Client����
  const int httpPort = 80;  //�w�]�q�T��80
  String JsonString = "";  //���d�Ҥ��|�Ψ�
  String url = "";
  //Client�s��Server
  if (client.connect(host, httpPort)) {
    
    //Webhook API
   url += "/trigger/" + String(event) + "/with/key/" + String(apiKey);
    //Query String
    url += "?value1=" + value1 + "&value2=" + value2 + "&value3=" + value3;

    //Client�ǰe
    client.println(String("POST ") + url + " HTTP/1.1");
    client.println(String("Host: ") + host); 
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(JsonString.length());
    client.println();
    client.println(JsonString);

    //����5��A�C0.1�����@���O�_��������response���  
    int timeout = 0;
    while(!client.available() && (timeout++ <= 50)){
      delay(100);
    }
  
    //�p�G�L�^��
    if(!client.available()) {
      Serial.println("No response...");
    }
    //��while�j��@�r�@�rŪ��Response
    while(client.available()){
      Serial.write(client.read());
    }

    //����Client
    client.stop(); 
  } 
}
char* ClientRequest(){
  
  WiFiClient client;  //�إ�Client����
  const int httpPort = 80;  //�w�]�q�T��80
  String JsonString = "";  //���d�Ҥ��|�Ψ�

  //Client�s��Server
  if (client.connect(host1, httpPort)) {

    //Client�ǰe
    client.println(String("POST ") + resource + " HTTP/1.1");
    client.println(String("Host: ") + host1); 
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(JsonString.length());
    client.println();
    client.println(JsonString);

    //����5��A�C0.1�����@���O�_��������response���  
    int timeout = 0;
    while(!client.available() && (timeout++ <= 50)){
      delay(100);
    }
  
    //�p�G�L�^��
    if(!client.available()) {
      Serial.println("No response...");
    }

    //Checking for the end of HTTP stream
    while(!client.find("\r\n\r\n")){
      // wait for finishing header stream reading ...
    }

    //Ū����ƨ��x�s�bjsonRead��
    client.readBytes(jsonRead, 600);

    //����Client
    client.stop(); 

    //�^��
    return jsonRead;
  } 
}