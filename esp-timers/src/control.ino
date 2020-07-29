#include <esp_now.h>
#include <WiFi.h>

//address of other ESP
uint8_t timeraddr[] = {0xFC, 0xF5, 0xC4, 0x2D, 0xD2, 0xA8};
unsigned long time_sent;
unsigned long time_recieved;
int i = 0;
int TEST_REPEAT = 10;

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
 * function tells the control what to do when something is sent to its address
 * to easily replicate the experiment, it will send an additional 10 messages to the timer
 */
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
    time_recieved = millis();
    Serial.print("data recieved: timer -> control\ntotal time(ms): ");
    Serial.print(time_recieved-time_sent);
    Serial.print("\n");
    Serial.print("time was off by(ms): ");
    Serial.print(abs(  (time_recieved-time_sent) - 10000));
    Serial.print("\n");
    if (i < TEST_REPEAT) {
     time_sent = millis();
     esp_err_t result = esp_now_send(timeraddr, (uint8_t *) &myData, sizeof(myData)); 
     i++;
    }
}

/*
 * function tells the contril what to do when it sends data
 * it just logs the success of the transfer
 */
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "data sent: control -> timer" : "Delivery Fail");
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

  // adds timer as peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, timeraddr, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    return;
  }

  //the first data transfer to the timer that will set off other transfers
  time_sent = millis();
  esp_err_t result = esp_now_send(timeraddr, (uint8_t *) &myData, sizeof(myData));
  

}
 
void loop() {}
