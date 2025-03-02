#define LED_PIN 13 // вывод управления красным светодиодом
#define BUTTON_PIN 2 // вывод подключения кнопки

bool buttonPressed = false; // Флаг, указывающий, что кнопка нажата
bool ledLight = false; // Флаг, показывающий, горит ли лампа
unsigned long pressStartTime = 0; // Время начала нажатия кнопки
int tmp;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Используем встроенный подтягивающий резистор
}

void flashing() {
  digitalWrite(LED_PIN, LOW);
  delay(400);
  digitalWrite(LED_PIN, HIGH);
  delay(400);
}

void work(int arg) { // Убрали параметр LED_PIN
  switch (arg) {
    case 0:
      digitalWrite(LED_PIN, LOW);
      break;
    case 1:
      digitalWrite(LED_PIN, HIGH);
      break;
    case 2:
      flashing();
      break;
    default:
      break;
  }
}

void loop() {
  int buttonState = digitalRead(BUTTON_PIN); // Читаем состояние кнопки
  
  if (buttonState == LOW) {
    if (!buttonPressed) { // Если кнопка только что нажата
      buttonPressed = true; // Устанавливаем флаг
      pressStartTime = millis(); // Запоминаем время начала нажатия
    }
    // Если кнопка удерживается более 2 секунд
    if (millis() - pressStartTime >= 1000) {
      if (ledLight == false) {
        tmp = 1;
        work(tmp);
        delay(400);
        ledLight = true;
      } else {
        tmp = 0;
        ledLight = false;
      }
      pressStartTime = 0; // Сброс времени
      buttonPressed = false; // Сбрасываем флаг нажатия
    }
  } else {
    if (millis() - pressStartTime <= 1000){
      if (tmp == 2) tmp = 1;
      else if (tmp == 1) tmp = 2;
      pressStartTime = 0;
    }
    buttonPressed = false; // Сбрасываем флаг, если кнопка не нажата
  }
  work(tmp);
}
