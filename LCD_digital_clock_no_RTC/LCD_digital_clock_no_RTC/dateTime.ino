//////////////////////////////////////////////////////////////////////////
// convert time_t data type to time string
// return time string in hh:nn:ss format (eg. 12:34:56)
//////////////////////////////////////////////////////////////////////////
String timeStr(time_t t) {
  String result = int2str(hour(t)) + (":") + int2str(minute(t)) + (":") + int2str(second(t));
  return result;
}

//////////////////////////////////////////////////////////////////////////
// convert time_t data type to date string
// return date string in yy-mm-dd format (eg. 16-12-31)
//////////////////////////////////////////////////////////////////////////
String dateStr(time_t t) {
  String result = int2str(year(t)) + ("-") + int2str(month(t)) + ("-") + int2str(day(t));
  return result;
}

//////////////////////////////////////////////////////////////////////////
// check the validity of the time
//////////////////////////////////////////////////////////////////////////
boolean check_time(byte hours, byte minutes, byte seconds) {
  if ((hours > 23) || (hours < 0)) return false;
  if ((minutes > 59) || (minutes < 0)) return false;
  if ((seconds > 59) || (seconds < 0)) return false;
  return true;
}

//////////////////////////////////////////////////////////////////////////
// check the validity of the date
//////////////////////////////////////////////////////////////////////////
boolean check_date(int years, byte months, byte days) {
  if ((years > 2099) || (years < 0)) return false;
  if ((months > 12) || (months < 1)) return false;
  byte lastday = get_lastday(years, months);
  if ((days > lastday) || (days < 1)) return false;
  return true;
}

//////////////////////////////////////////////////////////////////////////
// get last day of a month
//////////////////////////////////////////////////////////////////////////
byte get_lastday(int years, byte months) {
  byte daysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  byte lastday = daysInMonth[months - 1];
  if (((years % 4) == 0) && (months == 2)) lastday = 29; //leap year
  return lastday;
}

//////////////////////////////////////////////////////////////////////////
// convert compliled date and time to time_t
//////////////////////////////////////////////////////////////////////////
time_t compiled_datetime() {
  //the date when the source file was compiled (Mmm dd yyyy)
  char const *str_date = __DATE__;

  //the time when the source file was compiled (hh:mm:ss)
  char const *str_time = __TIME__;

  char s_month[5];
  int year;
  tmElements_t tm;
  static const char month_names[] = "JanFebMarAprMayJunJulAugSepOctNovDec";

  sscanf(str_date, "%s %hhd %d", s_month, &tm.Day, &year);
  sscanf(str_time, "%2hhd %*c %2hhd %*c %2hhd", &tm.Hour, &tm.Minute, &tm.Second);

  // Find where is s_month in month_names. Deduce month value.
  tm.Month = (strstr(month_names, s_month) - month_names) / 3 + 1;
  tm.Year = year1970or2000(year);
  return makeTime(tm); //convert tmElements_t to time_t & return its value
}

//////////////////////////////////////////////////////////////////////////
// year can be given as '2010' or '10'.  It is converted to years since 1970
//////////////////////////////////////////////////////////////////////////
int year1970or2000(int yy) {
  int result;
  if (yy > 99) result = yy - 1970;
  else result = yy + 30;
  return result;
}

