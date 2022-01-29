#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

static HTTPClient http;
static int httpError;

static const char *id = "<your YT channel id>";
static const char *key = "<your YT API shared key"; 
static const char *ssid = "<your WIFI SSID>";
static const char *wifipass = "<your WIFI password";

static const char* rootca="-----BEGIN CERTIFICATE-----\n" \
"MIIFWjCCA0KgAwIBAgIQbkepxUtHDA3sM9CJuRz04TANBgkqhkiG9w0BAQwFADBH\n" \
"MQswCQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZpY2VzIExM\n" \
"QzEUMBIGA1UEAxMLR1RTIFJvb3QgUjEwHhcNMTYwNjIyMDAwMDAwWhcNMzYwNjIy\n" \
"MDAwMDAwWjBHMQswCQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNl\n" \
"cnZpY2VzIExMQzEUMBIGA1UEAxMLR1RTIFJvb3QgUjEwggIiMA0GCSqGSIb3DQEB\n" \
"AQUAA4ICDwAwggIKAoICAQC2EQKLHuOhd5s73L+UPreVp0A8of2C+X0yBoJx9vaM\n" \
"f/vo27xqLpeXo4xL+Sv2sfnOhB2x+cWX3u+58qPpvBKJXqeqUqv4IyfLpLGcY9vX\n" \
"mX7wCl7raKb0xlpHDU0QM+NOsROjyBhsS+z8CZDfnWQpJSMHobTSPS5g4M/SCYe7\n" \
"zUjwTcLCeoiKu7rPWRnWr4+wB7CeMfGCwcDfLqZtbBkOtdh+JhpFAz2weaSUKK0P\n" \
"fyblqAj+lug8aJRT7oM6iCsVlgmy4HqMLnXWnOunVmSPlk9orj2XwoSPwLxAwAtc\n" \
"vfaHszVsrBhQf4TgTM2S0yDpM7xSma8ytSmzJSq0SPly4cpk9+aCEI3oncKKiPo4\n" \
"Zor8Y/kB+Xj9e1x3+naH+uzfsQ55lVe0vSbv1gHR6xYKu44LtcXFilWr06zqkUsp\n" \
"zBmkMiVOKvFlRNACzqrOSbTqn3yDsEB750Orp2yjj32JgfpMpf/VjsPOS+C12LOO\n" \
"Rc92wO1AK/1TD7Cn1TsNsYqiA94xrcx36m97PtbfkSIS5r762DL8EGMUUXLeXdYW\n" \
"k70paDPvOmbsB4om3xPXV2V4J95eSRQAogB/mqghtqmxlbCluQ0WEdrHbEg8QOB+\n" \
"DVrNVjzRlwW5y0vtOUucxD/SVRNuJLDWcfr0wbrM7Rv1/oFB2ACYPTrIrnqYNxgF\n" \
"lQIDAQABo0IwQDAOBgNVHQ8BAf8EBAMCAQYwDwYDVR0TAQH/BAUwAwEB/zAdBgNV\n" \
"HQ4EFgQU5K8rJnEaK0gnhS9SZizv8IkTcT4wDQYJKoZIhvcNAQEMBQADggIBADiW\n" \
"Cu49tJYeX++dnAsznyvgyv3SjgofQXSlfKqE1OXyHuY3UjKcC9FhHb8owbZEKTV1\n" \
"d5iyfNm9dKyKaOOpMQkpAWBz40d8U6iQSifvS9efk+eCNs6aaAyC58/UEBZvXw6Z\n" \
"XPYfcX3v73svfuo21pdwCxXu11xWajOl40k4DLh9+42FpLFZXvRq4d2h9mREruZR\n" \
"gyFmxhE+885H7pwoHyXa/6xmld01D1zvICxi/ZG6qcz8WpyTgYMpl0p8WnK0OdC3\n" \
"d8t5/Wk6kjftbjhlRn7pYL15iJdfOBL07q9bgsiG1eGZbYwE8na6SfZu6W0eX6Dv\n" \
"J4J2QPim01hcDyxC2kLGe4g0x8HYRZvBPsVhHdljUEn2NIVq4BjFbkerQUIpm/Zg\n" \
"DdIx02OYI5NaAIFItO/Nis3Jz5nu2Z6qNuFoS3FJFDYoOj0dzpqPJeaAcWErtXvM\n" \
"+SUWgeExX6GjfhaknBZqlxi9dnKlC54dNuYvoS++cJEPqOba+MSSQGwlfnuzCdyy\n" \
"F62ARPBopY+Udf90WuioAnwMCeKpSwughQtiue+hMZL77/ZRBIls6Kl0obsXs7X9\n" \
"SQ98POyDGCBDTtWTurQ0sR8WNh8M5mQ5Fkzc4P4dyKliPUDqysU0ArSuiYgzNdws\n" \
"E3PYJ/HQcu51OyLemGhmW/HGY0dVHLqlCFF1pkgl\n" \
"-----END CERTIFICATE-----\n";



void setup(void) {
  Serial.begin(115200);
  tft.init();
  tft.setRotation(1);  
  tft.setTextSize(1);
  tft.fillScreen(TFT_BLACK);
  
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.drawString("Connecting  ", 0, 0, 1);  
  
  WiFi.begin(ssid, wifipass);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
  }  
  
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.drawString("Connected...", 0, 0, 1);  
}

void displayHTTPCode(TFT_eSPI *tft, int httpcode){
  tft->setTextSize(1);  
  tft->drawNumber(httpcode,0, 10, 1);
}

String getNrSubscriptions(HTTPClient *http, int *httpError, const char* id, const char *key, const char *rootca){
  
  String count = "";
  String request = "https://www.googleapis.com/youtube/v3/channels?part=statistics&id="+String(id)+"&key="+String(key);
  http->begin(request.c_str(), rootca);
  (*httpError) = http->GET();
  
  if ((*httpError) > 0){ 
    StaticJsonDocument<1024> doc;

    String res = http->getString();
    DeserializationError error = deserializeJson(doc, res.c_str());

    if (!error) count = doc["items"][0]["statistics"]["subscriberCount"].as<String>();

    return count;
  }
}

void loop() {
  //WiFi could've been disconnected in that case connect
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, wifipass);
  }

  //Wait for Wifi to be reconnected
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
  }  

  tft.setTextSize(9); 
  tft.setTextColor(TFT_RED, TFT_BLACK);
  String count = getNrSubscriptions(&http, &httpError, id, key, rootca);
  if (count != "") tft.drawString(getNrSubscriptions(&http, &httpError, id, key, rootca), 10, 45, 1);      
  
  tft.setTextColor( (httpError!=200)?TFT_RED:TFT_GREEN, TFT_BLACK);
  displayHTTPCode(&tft, httpError);
  http.end();
    
  delay(60*1000);  
}
