#include <Wire.h> // must be included here so that Arduino library object file references work
#include <RtcDS3231.h>
RtcDS3231<TwoWire> Rtc(Wire);

// o Pino SQW deve ser ligado ao pino 19 no Mega. Isso varia de placa para placa
#define sqw_pin 2 // Mega. No UNO, mude para 19
#define int_number 0 // Mega. No UNO, mude para 4

volatile bool int_flag = false; // Variável que indica se hounve interrupção

void interrompeu() // Rotina chamada quando houver uma interrupção
{
  int_flag = true;
}

void setup ()
{
  Serial.begin(9600);

  // Pullup no pino de interrupção
  pinMode(sqw_pin, INPUT_PULLUP);

  // Configuração inicial do RTC
  Rtc.Begin();
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  if (!Rtc.IsDateTimeValid())
  {
    Rtc.SetDateTime(compiled);
  }
  if (!Rtc.GetIsRunning())
  {
    Rtc.SetIsRunning(true);
  }
  RtcDateTime now = Rtc.GetDateTime();
  if (now < compiled)
  {
    Rtc.SetDateTime(compiled);
  }
  Rtc.Enable32kHzPin(false);
  Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeAlarmBoth); //Habilta 2 alarmes

  // Alarme1 daqui a 30 segundos
  RtcDateTime alarmTime = now + 30;
  DS3231AlarmOne alarm1(
    alarmTime.Day(),
    alarmTime.Hour(),
    alarmTime.Minute(),
    alarmTime.Second(),
    DS3231AlarmOneControl_HoursMinutesSecondsMatch);
  Rtc.SetAlarmOne(alarm1);

  // Alarm 2 a cada 1 minuto
  DS3231AlarmTwo alarm2(
    0,
    0,
    0,
    DS3231AlarmTwoControl_OncePerMinute);
  Rtc.SetAlarmTwo(alarm2);

  // Efetiva os alarmes
  Rtc.LatchAlarmsTriggeredFlags();

  // Configura interrupção
  attachInterrupt(int_number, interrompeu, FALLING);
}

void loop ()
{
  RtcDateTime now = Rtc.GetDateTime();
  Serial.print(formatDate(now,"d/m/y") + " ");
  Serial.println(formatTime(now,"h:m:s"));
  Serial.println();

  if (int_flag)
  {
    int_flag = false; // Reseta flag
    DS3231AlarmFlag flag = Rtc.LatchAlarmsTriggeredFlags(); //Variável para testar qual alarme disparou
    if (flag & DS3231AlarmFlag_Alarm1)
    {
      Serial.println(">>> Alarm 1 disparou <<<");
    }
    if (flag & DS3231AlarmFlag_Alarm2)
    {
      Serial.println(">>> Alarme 2 disparou <<<");
    }
  }
  delay(5000);
}

// Funções para formatação dos dados
String formatDate(const RtcDateTime& dt, String format) 
{
  String d = dt.Day() < 10 ? "0" + String(dt.Day()) : String(dt.Day()) ; 
  String m = dt.Month() < 10 ? "0" + String(dt.Month()) : String(dt.Month()) ;
  String y = String(dt.Year()) ;
  format.replace("d",d);
  format.replace("m",m);
  format.replace("y",y);
  return format;
}

String formatTime(const RtcDateTime& dt, String format)
{
  String h = dt.Hour() < 10 ? "0" + String(dt.Hour()) : String(dt.Hour()) ;
  String m = dt.Minute() < 10 ? "0" + String(dt.Minute()) : String(dt.Minute()) ;
  String s = dt.Second() < 10 ? "0" + String(dt.Second()) : String(dt.Second()) ;
  format.replace("h",h);
  format.replace("m",m);
  format.replace("s",s);
  return format;
}


