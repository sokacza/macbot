#include <Arduino.h>
#include <string.h>
#include <golink.h>
#include <map>
#include <string>

#define POWER_RELAY   32
#define FIVE_SENSE    33
#define CURRENT_SENSE 34
#define VBUS_SENSE    35

goLink myLink;
TaskHandle_t Task1;
TaskHandle_t Task2;

/**
 * Do not remove or add anything to this task
 * **/
void Task1code( void * parameter) {
  for(;;) {
    myLink.spin();
    vTaskDelay(5);
  }
}

/**
 * Add code for program function here, prefereable create external functions and call those
 * **/
void Task2code( void * parameter) {
  for(;;) {
    //if (myLink.getInt("pwr") ==1) digitalWrite(POWER_RELAY, HIGH);
    //else digitalWrite(POWER_RELAY, LOW);
    myLink.setInt("v5", analogRead(FIVE_SENSE));
    myLink.setInt("vb", analogRead(VBUS_SENSE));
    myLink.setInt("ib", analogRead(VBUS_SENSE));
    vTaskDelay(500);
  }
}

void setup() {
  myLink.init(250E3, 2);
  myLink.setBroadcastRate(1000);
  Serial.begin(115200);
  pinMode(POWER_RELAY, OUTPUT);
  digitalWrite(POWER_RELAY, HIGH);

  xTaskCreatePinnedToCore(
      Task1code, /* Function to implement the task */
      "Task1", /* Name of the task */
      10000,  /* Stack size in words */
      NULL,  /* Task input parameter */
      0,  /* Priority of the task */
      &Task1,  /* Task handle. */
      0); /* Core where the task should run */

      xTaskCreatePinnedToCore(
      Task2code, /* Function to implement the task */
      "Task2", /* Name of the task */
      10000,  /* Stack size in words */
      NULL,  /* Task input parameter */
      2,  /* Priority of the task */
      &Task2,  /* Task handle. */
      1); /* Core where the task should run */
}


void loop() {
  delay(1000);
}

void goLink::myCallBack(std::string error){
  Serial.println(error.c_str());
}
