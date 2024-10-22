#include <Arduino.h>

int butpin = 5;   // Кнопанька имитации остановки энкодера -- button to simulate brakes
int potpin = A3;   //потенциометр регулировки скорости вращения -- rpm control pot
int hall_1_pin = 2;  //выход датчик холла 1 -- hall effect sensor simulator 1
int hall_2_pin = 3;  //выход датчик холла 2 -- hall effect sensor simulator 2

int min_freq = 1; // установка минимальной частоты -- minimum frequency
int hysteresis = 20; //гистерезис -- hysteresis

bool hall_sensor0, hall_sensor1; //переменная датчиков холла -- hall effect signal flags
int but_state = 0;  //состояние кнопки -- button state

int freq; //частота -- frequency
int period; //длительность  -- period length
int pot_val; //данные с потенциометра -- raw pot value

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(hall_1_pin, OUTPUT); //выход сигнала  имитация датчик холла 1 -- hall effect sensor simulator output 1
  pinMode(hall_2_pin, OUTPUT); //выход сигнала  имитация датчик холла 2 -- hall effect sensor simulator output 2
  pinMode(butpin, INPUT_PULLUP); //вход кнопки имитации остановки энкодеар -- brake button input
  pinMode(potpin, INPUT); //подключение потенциометра -- pot input
  digitalWrite(hall_1_pin, LOW); //устанавливает выход сигнала  имитация датчик холла 1 в низкий логический уровень -- sets output level for sensor simulator
  hall_sensor0 = false; //устанавливает выход сигнала  имитация датчик холла 1 в низкий логический уровень -- sets flag for sensor simulator
  digitalWrite(hall_2_pin, LOW); //устанавливает выход сигнала  имитация датчик холла 2 в низкий логический уровень -- sets output level for sensor simulator
  hall_sensor1 = false; //устанавливает выход сигнала  имитация датчик холла 2 в низкий логический уровень -- sets flag for sensor simulator
  //period = 50;  
}

void loop() {
  // put your main code here, to run repeatedly:
  but_state = digitalRead(butpin);

  if (but_state == 1) //проверка чтения кнопки если лог1 то читаем аналоговый вход, если лог0 не читаем -- checking for button state, 1 - proceed to read pot, 0 - exit
  {
    pot_val = analogRead(potpin); // чтение аналогового входа -- reading analog input
    if (pot_val > (512+hysteresis)) {freq = map(pot_val, 512, 1023, 1, 140);} // диапазон чтения 1,1023 положение резюка -140, 140 частота в герцах -- remapping raw pot input into frequency in Hz
    else if (pot_val < (512-hysteresis)) {freq = map(pot_val, 1, 512, -140, 1);}
    else pot_val = 1;
    if (freq != 0){period = 1000 / abs(freq);} //расчет частоты -- calculating frequency
      else if (freq == 0) {freq = 1;}  //исключаем возможность деления на 0 -- attempting to mitigate division by 0
    if (freq > (min_freq+hysteresis)) 
    {
      delay(period/4);
      hall_sensor0 = !hall_sensor0; //логика работы выходов -- output switching logic
      digitalWrite(hall_1_pin, hall_sensor0);

      delay(period/4);
      hall_sensor1 = !hall_sensor1;
      digitalWrite(hall_2_pin, hall_sensor1);
    } 
    else if ( freq < (min_freq-hysteresis)) //почему меньше чем -мин_частота?! -- why less that -freq?! original "else if ( freq < (-(min_freq-hysteresis)))" was slowing down code
    {
      delay(period/4);
      hall_sensor1 = !hall_sensor1;
      digitalWrite(hall_2_pin, hall_sensor1);

      delay(period/4);
      hall_sensor0 = !hall_sensor0;
      digitalWrite(hall_1_pin, hall_sensor0);
      
    }
 
  }  

else if (but_state == 0) {pot_val = 0; freq = 0;}

  Serial.print("Frequency: ");
  Serial.print(freq);
  Serial.print(" Pot_val: ");
  Serial.print(pot_val);
  Serial.print(" But_state: ");
  Serial.print(but_state);
  Serial.println(" ");
  
}

