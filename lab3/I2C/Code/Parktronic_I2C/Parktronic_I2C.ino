#define SOUND_PIN 4
#define ECHO_PIN 2
#define TRIG_PIN 3

// Подключаем библиотеку
#include <LiquidCrystal_I2C.h>

// Function declarations
void measureDistance(int *distance, int trigPin, int echoPin);
void displayDistance(LiquidCrystal_I2C *lcd, int distance);
void controlBuzzer(int distance, int buzzerPin);

// Создаем объект класса LiquidCrystal_I2C
LiquidCrystal_I2C lcd(0x27, 16, 2);



void setup() 
{  
  pinMode(SOUND_PIN, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  // Инициализация и начало работы с дисплеем
  lcd.init();

  // Включаем подсветку
  lcd.backlight();

  // Устанавливаем положение курсора, 0-й столбец, 0-я строка
  lcd.setCursor(0, 0);
  
  // Выводим текст
  lcd.print("Device Ready!");
}



void loop() 
{
	int distance = 0;
    measureDistance(&distance, TRIG_PIN, ECHO_PIN);
    displayDistance(&lcd, distance);
    controlBuzzer(distance, SOUND_PIN);
    delay(400); //для того чтобы не лагал tincercad
}

void measureDistance(int *distance, int trigPin, int echoPin) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);  // Минимальная задержка перед импульсом
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10); // Длина импульса 10 мкс
    digitalWrite(trigPin, LOW);
    
    unsigned long duration = pulseIn(echoPin, HIGH); // Засекаем время прихода эхо-импульса
    *distance = duration / 58; // Переводим в сантиметры
}

void displayDistance(LiquidCrystal_I2C *lcd, int distance) {
    static unsigned long t2 = 0;
    static bool flag = false; 
    static int modeFlag = 0;
  
      if ((distance >= 0)&&(distance < 336)){
        if((flag == false)&&(modeFlag == 0)){
          lcd->clear();
          lcd->print("Distance= ");
          lcd->print(distance);  // Display distance
          lcd->setCursor(14, 0);
          lcd->print("cm");     // Display unit
          flag = true;
        }else{
          lcd->setCursor(10, 0);
          lcd->print("    ");
          lcd->setCursor(10, 0);
          lcd->print(distance);
        }
        modeFlag = 0;
      }else{
        if(modeFlag == 0){
          lcd->clear();
          lcd->print("  Out of sight");
          flag = false;
          modeFlag = 1;
        }

    }
}

void controlBuzzer(int distance, int buzzerPin) {
    static unsigned long t3 = 0;
    
    if (distance > 200) {
        noTone(buzzerPin);
    } else if (distance > 100) {
         if (millis() - t3 >= 900) {
            t3 = millis();
            tone(buzzerPin, 400);
        }
        if (millis() -t3 >= 100) {
            noTone(buzzerPin);
        }
    } else if (distance > 40) {
       if (millis() - t3 >= 600) {
            t3 = millis();
            tone(buzzerPin, 700);
        }
        if (millis() >= t3 + 100) {
            noTone(buzzerPin);
        }
    } else {
        tone(buzzerPin, 900); // Непрерывный сигнал при опасном расстоянии
    }
}