#include "EEPROM.h"
boolean enable_alarm;
#define ALARM_AUTO_OFF 10 //turn off alarm after alarm ON for 10 seconds
#define ALARM_PIN   17 //Analog 3 
unsigned long alarm_startTime = 0;
const byte EEPROM_START_ADDRESS = 0;

//////////////////////////////////////////////////////////////////////////
//  read alarm settings from eeprom
//////////////////////////////////////////////////////////////////////////
tmElements_t alarm_read_eeprom() {
  tmElements_t tm_alarm;
  enable_alarm = EEPROM.read(EEPROM_START_ADDRESS);
  tm_alarm.Hour = EEPROM.read(EEPROM_START_ADDRESS + 1);
  tm_alarm.Minute = EEPROM.read(EEPROM_START_ADDRESS + 2);
  tm_alarm.Second = EEPROM.read(EEPROM_START_ADDRESS + 3);
  if (tm_alarm.Hour >= 24) { //alarm not set yet
    tm_alarm.Hour = 0;
    tm_alarm.Minute = 0;
    tm_alarm.Second = 0;
  }
  return  tm_alarm;
}

//////////////////////////////////////////////////////////////////////////
//  write alarm settings from eeprom
//////////////////////////////////////////////////////////////////////////
void alarm_write_eeprom(tmElements_t tm_alarm) {
  EEPROM.write(EEPROM_START_ADDRESS + 1, tm_alarm.Hour);
  EEPROM.write(EEPROM_START_ADDRESS + 2, tm_alarm.Minute);
  EEPROM.write(EEPROM_START_ADDRESS + 3, tm_alarm.Second);
}

//////////////////////////////////////////////////////////////////////////
// show alarm time on display (right corner)
//////////////////////////////////////////////////////////////////////////
void show_alarmtime() {
  if (enable_alarm) {
    byte hh = tm_alarm.Hour;
    if (hourFormat == 12) {
      if (hh > 12) hh = hh - 12;
    }
    lcd.setCursor(11, FIRST_LINE);
    lcd.print(int2str(hh));
    lcd.print(":");
    lcd.print(int2str(tm_alarm.Minute));
    lcd.setCursor(14, SECOND_LINE);
    lcd.print(int2str(tm_alarm.Second));
  }
}

//////////////////////////////////////////////////////////////////////////
// show alarm time on display
//////////////////////////////////////////////////////////////////////////
void show_alarmtime(byte pos_y) {
  lcd.setCursor(0, SECOND_LINE);
  lcd.print(int2str(tm_alarm.Hour));
  lcd.print(":");
  lcd.print(int2str(tm_alarm.Minute));
  lcd.print(":");
  lcd.print(int2str(tm_alarm.Second));
}

//////////////////////////////////////////////////////////////////////////
//  disable/enabled alarm
//////////////////////////////////////////////////////////////////////////
void toggle_alarm_mode() {
  lcd.clear();
  if (enable_alarm) {
    enable_alarm = false;
    DEBUG_PRINTLN("Alarm Disabled");
    lcd.print("Alarm Disabled");
  } else {
    enable_alarm = true;
    DEBUG_PRINTLN("Alarm Enabled");
    lcd.print("Alarm Enabled");
  }
  EEPROM.write(EEPROM_START_ADDRESS, enable_alarm);
  delay(2000);
  lcd.clear();
}

//////////////////////////////////////////////////////////////////////////
// trigger alarm if alarm time = current time
//////////////////////////////////////////////////////////////////////////
void alarm(time_t tm, tmElements_t tm_alarm) {
  if (enable_alarm) {
    // comparing tmElements_t is not allowed
    // tmElements_t must convert to time_t before comparing
    time_t t_time, t_alarm;
    t_time = tm; //convert tmElements_t element to time_t variable
    t_alarm = makeTime(tm_alarm);

    if (t_time == t_alarm) {
      alarm_startTime = millis();
      switch_alarm(HIGH);
    }
  }
}

//////////////////////////////////////////////////////////////////////////
// turn ON/OFF alarm
//////////////////////////////////////////////////////////////////////////
void switch_alarm(boolean state) {
  pinMode(ALARM_PIN, OUTPUT);
  digitalWrite(ALARM_PIN, state);
  show_alarm_status(state);
}

//////////////////////////////////////////////////////////////////////////
// show alarm status
//////////////////////////////////////////////////////////////////////////
void show_alarm_status(boolean state) {
  if (state == LOW) {
    DEBUG_PRINTLN("Alarm OFF");
    lcd.setCursor(11, FIRST_LINE);
    lcd.print("Alarm");
    lcd.setCursor(13, SECOND_LINE);
    lcd.print("OFF");
  } else {
    DEBUG_PRINTLN("Alarm ON ");
    lcd.setCursor(11, FIRST_LINE);
    lcd.print("Alarm");
    lcd.setCursor(13, SECOND_LINE);
    lcd.print(" ON");
  }

  delay(1000);
  lcd.setCursor(11, FIRST_LINE);
  lcd.print("     ");
  lcd.setCursor(13, SECOND_LINE);
  lcd.print("   ");
}

//////////////////////////////////////////////////////////////////////////
// alarm state
//////////////////////////////////////////////////////////////////////////
boolean alarm_state() {
  return digitalRead(ALARM_PIN);
}

//////////////////////////////////////////////////////////////////////////
// auto turn off alarm
//////////////////////////////////////////////////////////////////////////
void auto_off_alarm() {
  if (alarm_state() == HIGH) {
    if (millis() >= alarm_startTime + ALARM_AUTO_OFF * 1000) {
      switch_alarm(LOW);
    }
  }
}
