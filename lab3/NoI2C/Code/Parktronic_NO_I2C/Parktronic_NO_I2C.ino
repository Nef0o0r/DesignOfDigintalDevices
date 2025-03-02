// Подключаем библиотеку
#include <LiquidCrystal.h>
#define SOUND_PIN 3
#define LCD_DB4_PIN 4
#define LCD_DB5_PIN 5
#define LCD_DB6_PIN 6
#define LCD_DB7_PIN 7
#define LCD_RS_PIN 8
#define LCD_EN_PIN 9
#define ECHO_PIN 11
#define TRIG_PIN 12

// Function declarations
void measureDistance(int *distance, int trigPin, int echoPin);
void displayDistance(LiquidCrystal *lcd, int distance);
void controlBuzzer(int distance, int buzzerPin);

// Создаем объект класса LiquidCrystal
LiquidCrystal lcd(LCD_RS_PIN, LCD_EN_PIN, LCD_DB4_PIN, LCD_DB5_PIN, LCD_DB6_PIN, LCD_DB7_PIN);



void setup() 
{  
  pinMode(SOUND_PIN, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  // Начинаем работу с дисплеем
  lcd.begin(16, 2);
  
  //
  lcd.clear();
  
  //Скрываем курсор
  lcd.noCursor();
  
  
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
    static unsigned long t1 = 0;
    if (micros() - t1 >= 10) {
        t1 = micros();
        digitalWrite(trigPin, HIGH);
    }
    if (micros() >= t1 + 10) {
        t1 = micros();
        digitalWrite(trigPin, LOW);
    }
    *distance = pulseIn(echoPin, HIGH) / 58;
}

void displayDistance(LiquidCrystal *lcd, int distance) {
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
        if (millis() >= t3 + 100) {
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

