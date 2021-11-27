#include <WiFi.h>

const char* ssid = "Xperia XZ3_f7ca";
const char* password = "qwer1234";
const char* host = "maker.ifttt.com"; //IFTTT server網址
const char* event = "LINE01";  //IFTTT事件名稱
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

void loop() {
  int value1 = 25;
  int value2 = 50;

  ClientRequest(String(value1), String(value2));  //Client傳送資料
  
  delay(30000);
}  

void ClientRequest(String value1, String value2){
  
  WiFiClient client;  //建立Client物件
  const int httpPort = 80;  //預設通訊阜80
  String JsonString = "";  //此範例不會用到

  //Client連結Server
  if (client.connect(host, httpPort)) {
    
    //Webhook API
   String url += "/trigger/" + String(event) + "/with/key/" + String(apiKey);
    //Query String
    url += "?value1=" + value1 + "&value2=" + value2;

    //Client傳送
    client.println(String("POST ") + url + " HTTP/1.1");
    client.println(String("Host: ") + host); 
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(JsonString.length());
    client.println();
    client.println(JsonString);

    //等待5秒，每0.1秒偵測一次是否有接收到response資料  
    int timeout = 0;
    while(!client.available() && (timeout++ <= 50)){
      delay(100);
    }
  
    //如果無回應
    if(!client.available()) {
      Serial.println("No response...");
    }
    //用while迴圈一字一字讀取Response
    while(client.available()){
      Serial.write(client.read());
    }

    //停止Client
    client.stop(); 
  } 
}