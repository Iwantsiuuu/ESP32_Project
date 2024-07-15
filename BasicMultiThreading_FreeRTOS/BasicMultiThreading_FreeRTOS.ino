#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#define TAKS_STACK_SIZE 2048
#define TASK_PRIORITY 7

#ifndef LED_BUILTIN
#define LED_BUILTIN 13  // Specify the on which is your LED
#endif

// Prototype function tasks for Blink & AnalogRead.
void Task_Blink(void *pvParameters);
void Task_Print(void *pvParameters);

TaskHandle_t analogTaskHandle;
TaskHandle_t blinkTaskHandle;

const long onDuration = 1000;   // OFF time for LED
const long offDuration = 500;  // ON time for LED
int LEDState = HIGH;           // initial state of LED

long rememberTime = 0;  // this is used by the code

// The setup function runs once when you press reset or power on the board.
void setup() {
  // Initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
  Serial.printf("Basic Multi Threading Arduino Example\n");

  // Set up two tasks to run independently.
  xTaskCreate(Task_Blink, "Task Blink", TAKS_STACK_SIZE, NULL, TASK_PRIORITY, &blinkTaskHandle);
  xTaskCreate(Task_Print, "Analog Read", TAKS_STACK_SIZE, NULL, (TASK_PRIORITY - 1), &analogTaskHandle);

  // Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.
}

void loop() {}

void Task_Blink(void *pvParameters) {

  pinMode(LED_BUILTIN, OUTPUT);         // define LED_BUILTIN as output
  digitalWrite(LED_BUILTIN, LEDState);  // set initial state  uint32_t prevTime;

  for (;;) {
    if (LEDState == HIGH) {
      if ((xTaskGetTickCount() - rememberTime) >= onDuration) {
        LEDState = LOW;                      // change the state of LED
        rememberTime = xTaskGetTickCount();  // remember Current millis() time
      }
    } else {
      if ((xTaskGetTickCount() - rememberTime) >= offDuration) {
        LEDState = HIGH;                     // change the state of LED
        rememberTime = xTaskGetTickCount();  // remember Current millis() time
      }
    }

    digitalWrite(LED_BUILTIN, LEDState);  // turn the LED ON or OFF
    vTaskDelay(1 / portTICK_PERIOD_MS);
  }
}

void Task_Print(void *pvParameters) {
  for (;;) {
    // print out the value you read:
    Serial.println("task2");
    vTaskDelay(1000 / portTICK_PERIOD_MS);  // 100ms delay
  }
}
