//управление датчиками
#include <DHT.h>

#define OTLADKA_temp 0
#define OTLADKA_hum 0

#define DHTTYPE DHT22   // DHT 11

#define temp_L_n 13 // Датчик температуры левый нижний
#define temp_L_v 12 //                    левый верхний
#define temp_R_n 11 //                    правый нижний
#define temp_R_v 10 //                    правый верхний

#define nag_L 3 // Нагреватель левый
#define nag_R 2 //             правый

#define Ven_L 6
#define Ven_R 5

#define POMP_PIN_LEV   7
#define POMP_PIN_PRAV  4

#define HUMIDITY_PIN_LEV_H A0 // подключёние датчиков влажности почвы
#define HUMIDITY_PIN_LEV_L A1
#define HUMIDITY_PIN_PRAV_H A2
#define HUMIDITY_PIN_PRAV_L A3

// Константы
#define HUMIDITY_MIN    200 // минимальный порог влажности почвы
#define HUMIDITY_MAX    700 // максимальный порог влажности почвы
#define INTERVAL        60000 // интервал между проверкой на полив растения
// переменная для хранения показания влажности почвы

byte mas_received[4];
byte mas_send[4];

unsigned int humidity_mid_lev = 0;  // переменная для хранения показания влажности почвы
unsigned int humidity_mid_prav = 0;

int mas_temp[2];
int mas_hum[2];

int sr_temp = 20;

unsigned long waitTime = 0;// статическая переменная для хранения времени

DHT dht_L_n(temp_L_n, DHTTYPE);
DHT dht_L_v(temp_L_v, DHTTYPE);
DHT dht_R_n(temp_R_n, DHTTYPE);
DHT dht_R_v(temp_R_v, DHTTYPE);

int timer_h = 0;
int timer_t = 0;

void setup()
{
  Serial.begin(9600);

  dht_L_n.begin();
  dht_L_v.begin();
  dht_R_n.begin();
  dht_R_v.begin();

  pinMode(temp_L_n, OUTPUT);  // для датчиков температуры
  pinMode(temp_L_v, OUTPUT);
  pinMode(temp_R_n, OUTPUT);
  pinMode(temp_R_v, OUTPUT);

  pinMode(nag_L, OUTPUT); // для нагревателей
  pinMode(nag_R, OUTPUT);

  pinMode(Ven_L, OUTPUT);  // для вентиляторов
  pinMode(Ven_R, OUTPUT);

  pinMode(POMP_PIN_LEV, OUTPUT);  // для солиноидов
  pinMode(POMP_PIN_PRAV, OUTPUT);
}

void loop()
{ 
  temperatura();
  Hum();

  mas_send[0] = byte(mas_temp[0]);
  mas_send[1] = byte(mas_temp[1]);
  mas_send[2] = byte(mas_hum[0] / 10);
  mas_send[3] = byte(mas_hum[1] / 10);

  if (Serial.available() > 0) 
  {
    int data = Serial.read();
    if (data == 1)
    {
      delay(1000);
      Serial.write(mas_send, 4);
    }
    if (data == 2)
    {
      Serial.readBytes(mas_received, 4);
    }
  }

}
