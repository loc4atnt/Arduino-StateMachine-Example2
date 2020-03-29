class LedTask {

  private:
    int blinkTime;
    unsigned long t0, t1;
    boolean ledState;
    int ledPin;
    boolean isTurnOn;// Biến xác định trạng thái tắt/bật của tiến trình
    void(LedTask::*runningFunc)();// Con trỏ trỏ tới hàm xử lý của một trạng thái

    void resetFunc() {
      t0 = millis();

      if (isTurnOn)
        runningFunc = &checkTimesFunc;
    }

    void checkTimesFunc() {
      t1 = millis();

      if ( (t1 - t0) > blinkTime )
        runningFunc = &checkLedStateFunc;
    }

    void checkLedStateFunc() {
      ledState = digitalRead(ledPin);

      if (ledState == HIGH)
        runningFunc = &turnOffLedFunc;
      else
        runningFunc = &turnOnLedFunc;
    }

    void turnOnLedFunc() {
      digitalWrite(ledPin, HIGH);

      if (true)
        runningFunc = &resetFunc;
    }

    void turnOffLedFunc() {
      digitalWrite(ledPin, LOW);

      if (true)
        runningFunc = &resetFunc;
    }

  public:
    void init(int ledPin, int blinkTime) {
      this->ledPin = ledPin;
      this->blinkTime = blinkTime;

      isTurnOn = false;

      pinMode(ledPin, OUTPUT);
      runningFunc = &resetFunc;// Trạng thái ban đầu là Reset
    }

    void run() {
      (this->*runningFunc)();// Chạy hàm được trỏ bởi con trỏ
    }

    // Đổi trạng thái tắt/bật tiến trình (Task)
    void toggle(boolean toggleState) {
      isTurnOn = toggleState;
      // Nếu tắt task thì tắt luôn cả led và chuyển về trạng thái reset
      if (toggleState == false) {
        runningFunc = &resetFunc;
        digitalWrite(ledPin, 0);
      }
    }

    boolean isTaskOn() {
      return isTurnOn;
    }
};

