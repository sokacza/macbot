#include <Arduino.h>
#include <string.h>
#include <golink.h>
#include <map>
#include <string>

goLink myLink;
TaskHandle_t Task1;
TaskHandle_t Task2;
int counter;

void Task1code( void * parameter) {
  for(;;) {
    myLink.spin();
    //vTaskDelay(1);
  }
}

void Task2code( void * parameter) {
  for(;;) {
    Serial.println(myLink.getInt("meow2"));
    myLink.setInt("meow1", counter--);
    delay(350);
  }
}

void setup() {
  myLink.init(500E3, 4);
  myLink.setBroadcastRate(500);
  Serial.begin(115200);
  myLink.setInt("meow1", -13);
  myLink.setFloat("float1", -3.14);
  myLink.setString("new1", "hello world \r\nrrrr");

  xTaskCreatePinnedToCore(
      Task1code, /* Function to implement the task */
      "Task1", /* Name of the task */
      5000,  /* Stack size in words */
      NULL,  /* Task input parameter */
      0,  /* Priority of the task */
      &Task1,  /* Task handle. */
      0); /* Core where the task should run */

      xTaskCreatePinnedToCore(
      Task2code, /* Function to implement the task */
      "Task2", /* Name of the task */
      10000,  /* Stack size in words */
      NULL,  /* Task input parameter */
      1,  /* Priority of the task */
      &Task2,  /* Task handle. */
      1); /* Core where the task should run */
}


void loop() {
  delay(1000);
}

void goLink::myCallBack(std::string error){
  Serial.println(error.c_str());
}
