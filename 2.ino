const byte LED_PIN = 3;
const byte BUTTON_PIN = 2;

// 0：恆亮、1：慢閃、2：中速閃爍、3：快閃
byte mode = 0;

bool ledState = HIGH;

// 按鈕消抖
int lastReading = HIGH;
int stableButtonState = HIGH;

unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 30;

// LED 閃爍計時
unsigned long lastBlinkTime = 0;

// 每隔多少毫秒切換一次亮暗
const unsigned long blinkIntervals[] = {
  0,    // 恆亮
  800,  // 慢閃
  400,  // 中速閃爍
  150   // 快閃
};

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // 開機初始狀態：恆亮
  digitalWrite(LED_PIN, HIGH);
}

void loop() {
  unsigned long now = millis();
  int reading = digitalRead(BUTTON_PIN);

  // 偵測按鈕讀值變化，重新計算消抖時間
  if (reading != lastReading) {
    lastDebounceTime = now;
  }

  // 讀值穩定後，才確認按鈕狀態
  if (now - lastDebounceTime >= debounceDelay) {
    if (reading != stableButtonState) {
      stableButtonState = reading;

      // INPUT_PULLUP：按下時為 LOW
      // 只在剛按下的瞬間切換一次
      if (stableButtonState == LOW) {
        mode = (mode + 1) % 4;

        // 切換模式時先亮燈，並重新計時
        ledState = HIGH;
        digitalWrite(LED_PIN, ledState);
        lastBlinkTime = now;
      }
    }
  }

  lastReading = reading;

  // 依照目前模式控制 LED
  if (mode == 0) {
    ledState = HIGH;
    digitalWrite(LED_PIN, HIGH);
  } else {
    if (now - lastBlinkTime >= blinkIntervals[mode]) {
      lastBlinkTime = now;
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState);
    }
  }
}
