#include <esp_now.h>
#include <WiFi.h>

//address of other ESP
uint8_t controladdr[] = {0x24, 0x0A, 0xC4, 0x62, 0x34, 0x8C};

/*
 * dummy data structure that doesn't sent useful information
 * just contains one bit to be sent by esp-now
 * the only thing the timer needs to know is that something was sent
 */
typedef struct struct_message {
  bool dummy = false;
} struct_message;
struct_message myData;

/*
 * function tells the timer what to do when something is sent to its address
 * waits for 10 seconds and transfers data to control afterwards
 */
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  Serial.print("data recieved: control -> timer\nnow delaying...\n");
  delay(10000);
  esp_err_t result = esp_now_send(controladdr, (uint8_t *) &myData, sizeof(myData));
}

/*
 * function tells the contril what to do when it sends data
 * it just logs the success of the transfer
 */
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "data sent: timer -> control " : "Delivery Fail");
}
 
void setup() {
  // initializes the board as well as the protocol when data is transferred
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);
  esp_now_register_send_cb(OnDataSent);

  // adds control as peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, controladdr, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}
 
void loop() {}
