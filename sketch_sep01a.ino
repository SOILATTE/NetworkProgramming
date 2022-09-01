#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid = "isec_303";
const char* password = "isec_303";
ESP8266WebServer server(80);

const int led = 14;
String s,s_head;

void handleRoot() {
  digitalWrite(led, HIGH);
  s=s_head+"<h1>켜짐</h1><br>"+"<h1>켜졌습니다.</h1><br>"
  +"<br><h2>🎈🎈✨축하합니다 연경이 사진 보고 가세요✨🎈🎈</h2><br>"
  +"<h3>연경언니에 대해 알아보기!</h3>"
   +"<a href='https://www.instagram.com/kimyk10/'><h3>연콩이 인스타</h3></a>"
  +"<a href='https://namu.wiki/w/%EA%B9%80%EC%97%B0%EA%B2%BD/%EC%84%A0%EC%88%98%20%EA%B2%BD%EB%A0%A5'><h3>연콩이 경력</h3></a>";
  server.send(200, "text/html", s); 
  //server.send(200, "text/plain", "hello from esp8266!");
}

void handleNotFound(){
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}
// WIFI_STA (Station mode, Stand-alone mode)
// 다른 공유기에 접속해서 IP를 할당받고, HTTP 통신을 사용하는 모드입니다
void setupWifi() {
  WiFi.mode(WIFI_STA); 
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup(void){
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  Serial.begin(115200);

  // 여기 프로그램 부분을 함수로처리
  setupWifi();

  // 스마트폰에 맟게 크기 조정, html에서 한글 출력하게 설정
  s_head="<meta name='viewport' content='width=device-width, initial-scale=1.0'/>";
  //s=s+"<meta http-equiv='refresh' content='5'/>";
  s_head=s_head+"<meta http-equiv='Content-Type' content='text/html;charset=utf-8' />";


  
  server.on("/", handleRoot);
  server.on("/inline", [](){
    //server.send(200, "text/plain", "this works as well");
    digitalWrite(led, LOW);
    s=s_head+"<h1>꺼져있습니다.</h1><br>"
    +"<h3>아쉽습니다 연경 사진은 없습니다. 빨리 켜서 연경언니 사진 보러가세요 엄청 귀엽다고요 안 보면 손해</h3>";
    server.send(200, "text/html", s); 
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
}
