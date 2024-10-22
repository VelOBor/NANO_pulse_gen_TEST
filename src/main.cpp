#include <Arduino.h>

int butpin = 5;   // Кнопанька имитации остановки энкодера
int potpin = A3;   //потенциометр регулировки скорости вращения  
int hall_1_pin = 2;  //выход датчик холла 1
int hall_2_pin = 3;  //выход датчик холла 2

int min_freq = 1; // установка минимальной частоты

bool hall_sensor0, hall_sensor1; //переменная датчиков холла
int but_state = 0;

int freq; //частота
int period; //длительность
int pot_val; //данные с потенциометра

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(hall_1_pin, OUTPUT); //выход сигнала  имитация датчик холла 1
  pinMode(hall_2_pin, OUTPUT); //выход сигнала  имитация датчик холла 2
  pinMode(butpin, INPUT_PULLUP); //вход кнопки имитации остановки энкодеар
  pinMode(potpin, INPUT); //подключение потенциометра
  digitalWrite(hall_1_pin, LOW); //устанавливает выход сигнала  имитация датчик холла 1 в низкий логический уровень
  hall_sensor0 = false; //устанавливает выход сигнала  имитация датчик холла 1 в низкий логический уровень
  digitalWrite(hall_2_pin, LOW); //устанавливает выход сигнала  имитация датчик холла 2 в низкий логический уровень
  hall_sensor1 = false; //устанавливает выход сигнала  имитация датчик холла 2 в низкий логический уровень
  //period = 50;  
}

void loop() {
  // put your main code here, to run repeatedly:
  but_state = digitalRead(butpin);

  if (but_state == 1) //проверка чтения кнопки если лог1 то читаем аналоговый вход, если лог0 не читаем
  {
    pot_val = analogRead(potpin); // чтение аналогового входа
    freq = map(pot_val, 0, 1023, -140, 140); // диапазон чтения 0,1023 положение резюка -250,250 частота в герцах
  
    period = 1000 / abs(freq); //расчет частоты
    if (freq > min_freq) 
    {
      delay(period/4);
      hall_sensor0 = !hall_sensor0; //логика работы выходов
      digitalWrite(hall_1_pin, hall_sensor0);

      delay(period/4);
      hall_sensor1 = !hall_sensor1;
      digitalWrite(hall_2_pin, hall_sensor1);
    } 
    else if ( freq < (-(min_freq))) //почему меньше чем -мин_частота?!
    {
      delay(period/4);
      hall_sensor1 = !hall_sensor1;
      digitalWrite(hall_2_pin, hall_sensor1);

      delay(period/4);
      hall_sensor0 = !hall_sensor0;
      digitalWrite(hall_1_pin, hall_sensor0);
      
    }
  
  }  

  Serial.print("Frequency: ");
  Serial.print(freq);
  Serial.print(" Pot_val: ");
  Serial.print(analogRead(potpin));
  Serial.print(" But_state: ");
  Serial.print(but_state);
  Serial.println(" ");
  
}

