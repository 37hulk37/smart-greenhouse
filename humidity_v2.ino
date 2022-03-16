void Hum()
{
  // считываем текущее показания датчика влажности почвы
  int humidityNow_lev_H = analogRead(HUMIDITY_PIN_LEV_H);
  int humidityNow_lev_L = analogRead(HUMIDITY_PIN_LEV_L);
  int humidityNow_prav_H = analogRead(HUMIDITY_PIN_PRAV_H);
  int humidityNow_prav_L = analogRead(HUMIDITY_PIN_PRAV_L);

  int humidityNow_mid_lev = (humidityNow_lev_H + humidityNow_lev_L) / 2;
  int humidityNow_mid_prav = (humidityNow_prav_H + humidityNow_prav_L) / 2;

  //Serial.println(humidityNow_mid_lev);
  //Serial.println(humidityNow_mid_prav);

  if (byte(humidityNow_mid_lev)) mas[2] = byte(round(humidityNow_mid_lev));
  if (byte(humidityNow_mid_prav)) mas[3] = byte(round(humidityNow_mid_prav));
  
  Serial.println("lev_H");
  Serial.println(humidityNow_lev_H);
  Serial.println("lev_L");
  Serial.println(humidityNow_lev_L);
  Serial.println("prav_H");
  Serial.println(humidityNow_prav_H);
  Serial.println("prav_L");
  Serial.println(humidityNow_prav_L);

  // если показания текущей влажности почвы
  // не равняется предыдущему запросу

  if (humidityNow_mid_lev != humidity_mid_lev)
  {

    // сохраняем текущие показания влажности
    humidity_mid_lev = humidityNow_mid_lev;
  }


  if (humidityNow_mid_prav != humidity_mid_prav)
  {

    // сохраняем текущие показания влажности
    humidity_mid_prav = humidityNow_mid_prav;
  }

  // если прошёл заданный интервал времени
  // и значения датчика влажности меньше допустимой границы
  if (waitTime == 0 || (millis() - waitTime > INTERVAL))
  {
    if (humidity_mid_lev < HUMIDITY_MIN )
    {
      // включаем помпу
      digitalWrite(POMP_PIN_LEV, HIGH);
    }
    else if (humidity_mid_lev >= (HUMIDITY_MIN + (HUMIDITY_MIN + HUMIDITY_MAX) / 2))
    {
      // выключаем помпу
      digitalWrite(POMP_PIN_LEV, LOW);

    }
    if (humidity_mid_prav < HUMIDITY_MIN )
    {
      // включаем помпу
      digitalWrite(POMP_PIN_PRAV, HIGH);
    }
    else if (humidity_mid_prav >= (HUMIDITY_MIN + (HUMIDITY_MIN + HUMIDITY_MAX) / 2))
    {
      // выключаем помпу
      digitalWrite(POMP_PIN_PRAV, LOW);
    }

    // приравниваем переменной waitTime
    // значение текущего времени плюс 3 минуты
    waitTime = millis();
  }

  if (Serial1.available())
  {
    Serial1.write(3);
    Serial1.write(humidity_mid_lev / 1024 * 100);
    int cur_hum_R = int(Serial1.read());
    delay(100);

    Serial1.write(4);
    Serial1.write(humidity_mid_prav / 1024 * 100);
    int cur_hum_L = int(Serial1.read());
    delay(60000);

    if  (cur_hum_L * 1024 / 100 != humidity_mid_lev)
    {
      if  (cur_hum_L * 1024 / 100 > humidity_mid_lev)
      {
        digitalWrite(POMP_PIN_LEV, LOW);
      }
      else
      {
        digitalWrite(POMP_PIN_LEV, HIGH);
      }
    }

    if  (cur_hum_R * 1024 / 100 != humidity_mid_prav)
    {
      if  (cur_hum_R * 1024 / 100 > humidity_mid_prav)
      {
        digitalWrite(POMP_PIN_PRAV, LOW);
      }
      else
      {
        digitalWrite(POMP_PIN_PRAV, HIGH);
      }
    }


    int hum_now_prav = mas_now[2];
  }
}
