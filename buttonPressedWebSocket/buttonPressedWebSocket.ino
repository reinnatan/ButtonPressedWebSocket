#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>

//declare config ssid
//setup ssid and password
const char* ssid = "";
const char* password = "";

//declare buttonPin
bool state = false;
int button_state = 0;
int prev_button_state = LOW; 
const int buttonPin = D1;

// WebSocket server details (can be local or cloud)
// setup ip websocket server & websocket server
const char *ws_host = "";
const uint16_t ws_port = 8081;
const char* ws_path = "/";
WebSocketsClient webSocket;

void webSocketEvent(WStype_t type, uint8_t* payload, size_t length) {
  if (type==WStype_DISCONNECTED) {
      Serial.println("[WebSocket] Disconnected");
      
  }else if(type==WStype_CONNECTED){
      uint32_t chipId = ESP.getChipId();
      String chipIdStr = "chipId-"+String(chipId, HEX);
      webSocket.sendTXT(chipIdStr);
      Serial.println("[WebSocket] Connected to server");
      Serial.println(chipIdStr);
  }else if(type==WStype_TEXT){
      Serial.printf("[WebSocket] Server says: %s\n", payload);
  }
}

void setup() {
  // setup pin out 
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);

  // setup wifi connection
  WiFi.begin(ssid, password);
  Serial.println("WiFi connected");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  //setup websocket connection
  webSocket.begin(ws_host, ws_port, ws_path);
  webSocket.onEvent(webSocketEvent);
  webSocket.setReconnectInterval(5000);
}
void loop() {
  // put your main code here, to run repeatedly:
  webSocket.loop();

  button_state = digitalRead(buttonPin);
  if(prev_button_state == HIGH && button_state == LOW){
    uint32_t chipId = ESP.getChipId();
    String chipIdStr = "updateRoom-"+String(chipId, HEX);
    webSocket.sendTXT(chipIdStr);
  }else if(prev_button_state == LOW && button_state == HIGH){
    Serial.println("The button is released");
  }
  prev_button_state = button_state;

}
