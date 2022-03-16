void temperatura()
{
  //блок температуры
  int temp_mid_L = 0;
  int temp_mid_R = 0;

  float temp_L_l = 0;  // считывание температуры
  float temp_L_h = 0;
  float temp_R_l = 0;
  float temp_R_h = 0;

  int temp_Now_L = 0;
  int temp_Now_R = 0;

  // блок считывания температуры
  if (millis() - timer_t >= 3000)
  {
    temp_L_l = dht_L_n.readTemperature();  // считывание температуры
    temp_L_h = dht_L_v.readTemperature();
    temp_R_l = dht_R_n.readTemperature();
    temp_R_h = dht_R_v.readTemperature();

    //вычисление средних температур в блоках
    temp_mid_L = (temp_L_l + temp_L_h) / 2;
    temp_mid_R = (temp_R_l + temp_R_h) / 2;

    mas_temp[0] = temp_mid_L;
    mas_temp[1] = temp_mid_R;

#if OTLADKA_temp

    Serial.print("temp_L_l");
    Serial.print(temp_L_l);
    Serial.print(" temp_L_h");
    Serial.print(temp_L_h);

    Serial.print("  temp_R_l");
    Serial.print(temp_R_l);
    Serial.print(" temp_R_h");
    Serial.println(temp_R_h);

    Serial.print("  temp_mid_L");
    Serial.print(temp_mid_L);
    Serial.print(" temp_mid_R");
    Serial.println(temp_mid_R);

#endif

    timer_t = millis();
  }
  if (!byte(temp_L_l))
  {
    digitalWrite(temp_L_l, LOW); //Выставляем низкий уровень значения на порту
    delay(1500);
    digitalWrite(temp_L_l, HIGH); //Выставляем высокий уровень значения на порту
    delay(500);
    dht_L_n.begin(); //Запускаем инициализацию датчика bme280
  }

  if (!byte(temp_L_h))
  {
    digitalWrite(temp_L_h, LOW); //Выставляем низкий уровень значения на порту
    delay(1500);
    digitalWrite(temp_L_h, HIGH); //Выставляем высокий уровень значения на порту
    delay(500);
    dht_L_v.begin(); //Запускаем инициализацию датчика bme280
  }

  if (!byte(temp_L_l))
  {
    digitalWrite(temp_R_l, LOW); //Выставляем низкий уровень значения на порту
    delay(1500);
    digitalWrite(temp_R_l, HIGH); //Выставляем высокий уровень значения на порту
    delay(500);
    dht_R_n.begin(); //Запускаем инициализацию датчика bme280
  }

  if (!byte(temp_R_h))
  {
    digitalWrite(temp_R_v, LOW); //Выставляем низкий уровень значения на порту
    delay(1500);
    digitalWrite(temp_R_v, HIGH); //Выставляем высокий уровень значения на порту
    delay(500);
    dht_R_v.begin(); //Запускаем инициализацию датчика bme280
  }

  if (temp_mid_L < sr_temp)
  {
    digitalWrite(Ven_L, HIGH); //выкл левый вентилятор
    digitalWrite(nag_L, LOW); // вкл левый нагреватель
  }
  else
  {
    digitalWrite(nag_L, HIGH ); //выкл
    digitalWrite(Ven_L, LOW); //вкл левый вентилятор
  }

  if (temp_mid_R < sr_temp)
  {
    digitalWrite(Ven_R, HIGH); //выкл правый вентилятор
    digitalWrite(nag_R, LOW); // вкл правый нагреватель
  }
  else
  {
    digitalWrite(nag_R, HIGH); //выкл
    digitalWrite(Ven_R, LOW); //вкл правый вентилятор
  }

  temp_Now_L = int(mas_received[0]);
  temp_Now_R = int(mas_received[1]);

  //блок настройки параметров отсеков

  //настройка параметров левого отсека
  if (temp_Now_L < sr_temp)
  {
    digitalWrite(Ven_L, HIGH); //выкл левый вентилятор
    digitalWrite(nag_L, LOW); // вкл левый нагреватель
  }
  else
  {
    digitalWrite(nag_L, HIGH ); //выкл
    digitalWrite(Ven_L, LOW); //вкл левый вентилятор
  }

  //настройка параметров правого отсека
  if (temp_Now_R < sr_temp)
  {
    digitalWrite(Ven_R, HIGH); //выкл правый вентилятор
    digitalWrite(nag_R, LOW); // вкл правый нагреватель
  }
  else
  {
    digitalWrite(nag_R, HIGH); //выкл
    digitalWrite(Ven_R, LOW); //вкл правый вентилятор
  }
  
}
