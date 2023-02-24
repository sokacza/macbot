#include <Arduino.h>
#include <golink.h>
#include <ESP32Encoder.h>


#define SLEEP 16 // used to idle the motor and make sure the drive is working, no need to change
#define PMODE 27 // used to set the drive mode for the motor controller, leave as defined below
#define PH1 32   // PWM to set motor "torque/speed, etc..."
#define PH2 33   // Change the direction of motor travel
#define ENCA 26  // encoder Phase A Pin
#define ENCB 25  // encoder Phase B pin

#define POWER_CAP 100

//#define DIRECTION_FLIP

goLink myLink;
TaskHandle_t Task1;
TaskHandle_t Task2;
ESP32Encoder encoder;



/**
 * @brief the main task that checks if the golink needs to process any data
 * 
 * @param parameter 
 */
void Task1code(void *parameter)
{
  for (;;)
  {
    myLink.spin();
    vTaskDelay(5);
  }
}

/**
 * @brief Set the Pwm of the motor by using the dual phase approach
 * 
 * @param direction 
 * @param pwmVal 
 */
void setPwm(uint8_t direction, int16_t pwmVal)
{
  if (pwmVal > POWER_CAP) pwmVal = POWER_CAP;

  if (pwmVal < 0)
  {
    ledcWrite(0, 0);
    ledcWrite(0, 1);
    return;
  }

  if (direction == 1)
  {
#ifdef DIRECTION_FLIP
      ledcWrite(0, 0);
      ledcWrite(1, pwmVal);
#else
      ledcWrite(0, pwmVal);
      ledcWrite(1, 0);
#endif
  }
  else
  {
#ifdef DIRECTION_FLIP
      ledcWrite(0, pwmVal);
      ledcWrite(1, 0);
#else
      ledcWrite(0, 0);
      ledcWrite(1, pwmVal);
#endif
  }
}

/**
 * @brief the motor control task that will control motor speed using an IF control method (no PID yet)
 * 
 * @param parameter 
 */
void Task2code(void *parameter)
{
  int32_t direction = 0; 
  int32_t speed = 0;
  int32_t req_speed = 0;
  int32_t previous_pos = 0;
  int32_t current_pos = 0;
  int16_t request_pwm = 0;

  for (;;)
  {
    req_speed = myLink.getInt("spr");

    current_pos = encoder.getCount();
    myLink.setInt("enc", current_pos);

    speed = current_pos - previous_pos;
    previous_pos = current_pos;
    myLink.setInt("spa", speed);

    if (req_speed > 0)
    {
      direction = 1;
      if (speed >= 0)
      {
        if (speed < req_speed)
        {
          request_pwm += 2;
        }
        if (speed > req_speed)
        {
          request_pwm -= 2;
        }
      }
      else
      {
        request_pwm = 1;
      }
    }

    if (req_speed < 0)
    {
      direction = 0;
      if (req_speed < speed)
      {
        request_pwm += 2;
      }
      if (req_speed > speed)
      {
        request_pwm -= 2;
      }
    }

    if (req_speed == 0)
    {
      request_pwm = 1;
    }
    setPwm(direction, request_pwm);
    // Serial.print(" Current Speed: ");
    // Serial.print(speed, DEC);
    // Serial.print(" Request Speed: ");
    // Serial.print(req_speed, DEC);
    // Serial.print(" PWM: ");
    // Serial.println(request_pwm, DEC);
    vTaskDelay(50);
  }
}

/**
 * @brief Arduino setup routine
 * 
 */
void setup()
{
  myLink.init(250E3, 4); // <<<<<<<<========   CHANGE THIS NUMBER FOR DIFFERENT NODES
  myLink.setBroadcastRate(100);
  Serial.begin(115200);

  /*Create a half resolution quadrature encoder using the internal counter*/
  encoder.attachHalfQuad(ENCA, ENCB);
  encoder.clearCount();

  /* setup the pins for the motor control */
  pinMode(PMODE, OUTPUT);

  ledcSetup(0, 10000, 8);
  ledcSetup(1, 10000, 8);
  ledcAttachPin(PH1, 0);
  ledcAttachPin(PH2, 1);

  xTaskCreatePinnedToCore(
      Task1code, /* Function to implement the task */
      "Task1",   /* Name of the task */
      10000,     /* Stack size in words */
      NULL,      /* Task input parameter */
      0,         /* Priority of the task */
      &Task1,    /* Task handle. */
      0);        /* Core where the task should run */

  xTaskCreatePinnedToCore(
      Task2code, /* Function to implement the task */
      "Task2",   /* Name of the task */
      10000,     /* Stack size in words */
      NULL,      /* Task input parameter */
      2,         /* Priority of the task */
      &Task2,    /* Task handle. */
      1);        /* Core where the task should run */
}

/**
 * @brief required by Arduino instead of main
 * 
 */
void loop()
{
  delay(1000);
}

/**
 * @brief callback for the golink error handler. You can uncomment the Serial.println if you dont need it. 
 * 
 * @param error 
 */
void goLink::myCallBack(std::string error)
{
  Serial.println(error.c_str());
}

