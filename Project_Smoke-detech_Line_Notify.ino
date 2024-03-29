#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME ""
#define BLYNK_AUTH_TOKEN ""

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "";
char pass[] = "";

BlynkTimer timer;
WidgetLED alertLED(V2);


#include <TridentTD_LineNotify.h>
#define LINE_TOKEN  ""

int analogValue,limit,alert = 0;

BLYNK_CONNECTED()
{
  Blynk.syncVirtual(V1);
}

BLYNK_WRITE(V1)
{
  Serial.print("setup limit analog value to :");
  limit = param.asInt();
  Serial.println(limit);
}

void setup()
{
  Serial.begin(115200);
  delay(2000);

  Blynk.begin(auth, ssid, pass);
  timer.setInterval(5000L,updateAndCompare);

  LINE.setToken(LINE_TOKEN);
  LINE.notify("System Start...");
}

void loop()
{
   Blynk.run();
   timer.run();
}

void updateAndCompare()
{
  analogValue = 0;

  Serial.println("Update Analog Value...");
  for(int i=0;i<100;i++)
  {
    analogValue = analogValue+analogRead(A0);
  }
  analogValue = analogValue/100;

  Blynk.virtualWrite(V0,analogValue);

  Serial.print("analog value = ");
  Serial.print(analogValue);
  
  Serial.println();
  Serial.println("Start Compare...");
  if(analogValue >= limit)
  {
    Serial.println("Value above Limit!!");
    alertLED.on();
    LINE.notifySticker(789,10888);
    LINE.notify("ตรวจพบควันเกินกำหนด!!");
    LINE.notify(analogValue);
  }
  else
  {
    alertLED.off();
  }

  Serial.println();
}
