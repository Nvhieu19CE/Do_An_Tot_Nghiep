#include <WebServer.h>
#include <WiFi.h>
#include <esp32cam.h>


#define in1 12
#define in2 13
#define in3 15
#define in4 14
#define led 4
const char* WIFI_SSID = "Vu";
const char* WIFI_PASS = "920092Vu";
  
WebServer server(80);

static auto loRes = esp32cam::Resolution::find(320, 240);
static auto midRes = esp32cam::Resolution::find(350, 530);
static auto hiRes = esp32cam::Resolution::find(800, 600);
void serveJpg() {
  auto frame = esp32cam::capture();
  if (frame == nullptr) {
    Serial.println("CAPTURE FAIL");
    server.send(503, "", "");
    return;
  }
  //  Serial.printf("CAPTURE OK %dx%d %db\n", frame->getWidth(), frame->getHeight(), static_cast<int>(frame->size()));
  server.setContentLength(frame->size());
  server.send(200, "image/jpeg");
  WiFiClient client = server.client();
  frame->writeTo(client);
}

void handleJpgLo() {
  if (!esp32cam::Camera.changeResolution(loRes)) {
    Serial.println("SET-LO-RES FAIL");
  }
  serveJpg();
}

void handleJpgHi() {
  if (!esp32cam::Camera.changeResolution(hiRes)) {
    Serial.println("SET-HI-RES FAIL");
  }
  serveJpg();
}
void handleJpgMid() {
  if (!esp32cam::Camera.changeResolution(midRes)) {
    Serial.println("SET-MID-RES FAIL");
  }
  serveJpg();
}

void handlePostData() {
  String data;
  if (server.hasArg("payload")) {
    data = server.arg("payload");
  } else {
    data = "No data Rec";
  }
  Serial.println(data);
  if (data == "F")
    tien(60);
  else if (data == "R1")
    xoayPhai(60);
  else if (data == "R2")
    xoayPhai(60);
  else if (data == "R3")
    xoayPhai(60);
  else if (data == "L1")
    xoayTrai(60);
  else if (data == "L2")
    xoayTrai(60);
  else if (data == "L3")
    xoayTrai(60);
  else tien(0);
  delay(10);
  server.send(200, "text/plain", "");
}
void  setup() {
  pinMode(led, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  Serial.begin(115200);
  Serial.println();
  {
    using namespace esp32cam;
    Config cfg;
    cfg.setPins(pins::AiThinker);
    cfg.setResolution(hiRes);
    cfg.setBufferCount(2);
    cfg.setJpeg(80);
    bool ok = Camera.begin(cfg);
    Serial.println(ok ? "CAMERA OK" : "CAMERA FAIL");
  }

  connectWF();
  Serial.print("http://");
  Serial.println(WiFi.localIP());
  Serial.println("  /cam-lo.jpg");
  Serial.println("  /cam-hi.jpg");
  Serial.println("  /cam-mid.jpg");

  server.on("/cam-lo.jpg", handleJpgLo);
  server.on("/cam-hi.jpg", handleJpgHi);
  server.on("/cam-mid.jpg", handleJpgMid);
  server.on("/data", HTTP_POST, handlePostData);
  server.begin();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWF();
    analogWrite(led, 0);
  }
  else analogWrite(led, 180);
  server.handleClient();

}
void connectWF() {
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
}

void tien(int sp) {
  analogWrite(in1, 0);
  analogWrite(in2, sp);
  analogWrite(in3, 0);
  analogWrite(in4, sp);
}
void lui(int sp) {
  analogWrite(in2, 0);
  analogWrite(in1, sp);
  analogWrite(in4, 0);
  analogWrite(in3, sp);
}
void xoayTrai(int sp) {
  analogWrite(in2, 0);
  analogWrite(in1, sp);
  analogWrite(in3, 0);
  analogWrite(in4, sp);
}
void xoayPhai(int sp) {
  analogWrite(in1, 0);
  analogWrite(in2, sp);
  analogWrite(in4, 0);
  analogWrite(in3, sp);
}