#include "LedTask.h"
#define btnPin 2// Chân nút nhấn
#define led1Pin 8
#define led2Pin 9

LedTask ledTask1, ledTask2;// Tạo 2 đối tượng tiến trình blink led

void (*runningFunc)();// Con trỏ trỏ tới hàm xử lý của một trạng thái (Tiến trình 3 - nút nhấn)
unsigned long t0, t1;

void setup() {
  pinMode(btnPin, INPUT_PULLUP);

  ledTask1.init(led1Pin, 1000);// Cài đặt tiến trình 1: Blink led ở chân số 8, chu kỳ 1000ms
  ledTask2.init(led2Pin, 30);// Cài đặt tiến trình 2: Blink led ở chân số 9, chu kỳ 30ms

  runningFunc = &reset;// Trạng thái ban đầu của tiến trình 3: Reset
}

void loop() {
  runningFunc();// Chạy hàm được trỏ bởi con trỏ của tiến trình 3
  ledTask1.run();// Chạy tiến trình 1
  ledTask2.run();// Chạy tiến trình 2
}

void reset() {// Trạng thái RESET
  t0 = millis();
  t1 = t0;

  if (true)
    runningFunc = &checkButton;
}

void checkButton() {// Trạng thái CHECK_BUTTON
  boolean btnState = digitalRead(btnPin);

  if (btnState == LOW) // Nút được nhấn
    runningFunc = &updateHoldTime;
  else
    runningFunc = &checkHoldBtnTime;
}

void updateHoldTime() {// Trạng thái UPDATE_HOLD_TIME
  t1 = millis();
  unsigned long deltaTime = t1 - t0;

  if (deltaTime > 1000) {
    runningFunc = &turnOnLed2BlinkTask;
  } else {
    runningFunc = &checkButton;
  }
}

void turnOnLed2BlinkTask() {// Trạng thái TURN_ON_BLINK_LED2
  ledTask2.toggle(true);

  if (true)
    runningFunc = &checkButton;
}

void checkHoldBtnTime() {// Trạng thái CHECK_HOLD_TIME
  if (ledTask2.isTaskOn()){
    ledTask2.toggle(false);
  }
  unsigned long deltaTime = t1 - t0;

  if (deltaTime > 1000)
    runningFunc = &toggleLed1BlinkTask;
  else if (deltaTime > 10)// debounce button (Giữ nút trên 10ms mới tính là 1 lần nhấn)
    runningFunc = &toggleBoth2Led;
  else
    runningFunc = &reset;
}

void toggleLed1BlinkTask() {// Trạng thái TOGGLE_BLINK_LED1_TASK
  boolean isLed1BlinkTaskOn = ledTask1.isTaskOn();
  ledTask1.toggle(!isLed1BlinkTaskOn);

  if (true)
    runningFunc = &reset;
}

void toggleBoth2Led() {// Trạng thái TOGGLE_2_LED
  if (ledTask1.isTaskOn()){
    ledTask1.toggle(false);
  }
  toggleLed(led1Pin);
  toggleLed(led2Pin);

  if (true)
    runningFunc = &reset;
}

void toggleLed(int pin) {
  boolean lastState = digitalRead(pin);
  digitalWrite(pin, !lastState);
}

