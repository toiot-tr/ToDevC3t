#include "header.h"
#ifndef WIFI_GLOBAL_H
#define WIFI_GLOBAL_H // #define _wifi_h

#include "WiFi.h"
#include <DNSServer.h>
#include <ESPmDNS.h>
#include <ArduinoOTA.h>
#include <ArduinoJson.h>


#include "web/dokumanHtml.h"
#include "web/captiveHtml.h"
#include "web/girisHtml.h"
#include "web/ayarlarHtml.h"
#include "web/portalHtml.h"

#define _HTML "text/html"                  //: HTML içeriği.
#define _JSCRIPT "application/javascript"  //: JavaScript kodu içeriği.
#define _JSON "application/json"           //: JSON veri formatında içerik.
#define _CSS "text/css"                    //: CSS stil sayfası içeriği.
#define _TEXT "text/plain"                 //: Düz metin içeriği.
#define _XML "application/xml"             //: XML veri formatında içerik.
#define _PDF "application/pdf"             //: PDF belge içeriği.
#define _JPG "image/jpeg"                  //: JPEG görüntü içeriği.
#define _PNG "image/png"                   //: PNG görüntü içeriği.
#define _GIF "image/gif"                   //: GIF görüntü içeriği.
#define _STREAM "application/octet-stream" //: Belirli bir MIME türüne sahip olmayan genel ikili veri içeriği.
#define _ICON "image/x-icon"               //: İkon içeriği.
#define _EVENT "text/event-stream"         //: Server-Sent Events (SSE) içeriği.

#define _HTTP200 "HTTP/1.1 200 OK"                    //:  200 OK: İstek başarıyla gerçekleştirildi ve cevap başarılı bir şekilde döndü.
#define _HTTP204 "HTTP/1.1 204 No Content"            //:  204 No Content: İstek başarılı oldu, ancak yanıtta içerik bulunmuyor.
#define _HTTP400 "HTTP/1.1 400 Bad Request"           //:  400 Bad Request: İstek hatalı veya geçersiz.
#define _HTTP401 "HTTP/1.1 401 Unauthorized"          //:  401 Unauthorized: Kimlik doğrulaması gerekiyor veya yetkilendirme başarısız.
#define _HTTP404 "HTTP/1.1 404 Not Found"             //:  404 Not Found: İstek yapılan kaynak bulunamadı.
#define _HTTP500 "HTTP/1.1 500 Internal Server Error" //:  500 Internal Server Error: Sunucu hatası, genel bir hata oluştu.
#define _HTTP505 "HTTP/1.1 505 HTTP Version Not Supported"

// dns_ayarları *****************************************************
const uint8_t DNS_PORT = 53; // ok
IPAddress ApIP(3, 3, 3, 3);  // ok
DNSServer DnsServer;         // ok
WiFiServer WebServer(80);    // ok




extern bool wifiPowerSet();
extern void portalStart();
extern void portalStop();
extern void wifiOta();
extern void httpSend(WiFiClient _client, String _durum, String _content, String _icerik);
extern void jsonPrepare(WiFiClient _client, uint8_t _istek_No);
extern void serverLoop();
extern void serverGET(WiFiClient client);
extern void serverPOST(WiFiClient client);

#elif !defined(WIFI_LOCAL_H)
#define WIFI_LOCAL_H
#pragma once

bool wifiPowerSet()
{

  return true;
}

void portalStart()
{
  vPortYield();
  WiFi.mode(WIFI_AP_STA);

    WiFi.softAP("toiot");


  WiFi.softAPConfig(ApIP, ApIP, IPAddress(255, 255, 255, 0));
  wifiOta();
  DnsServer.start(DNS_PORT, "*", ApIP);
  WebServer.begin();
  BLedSondur = 50;
}

void portalStop()
{
  WebServer.close();
  DnsServer.stop();
  WiFi.mode(WIFI_STA);
  BLedSondur = 0;
  vTaskDelay(1000);
}

void wifiOta()
{
  ArduinoOTA.end();

  ArduinoOTA

      .onStart([]()
               {
 
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else 
        type = "filesystem"; })
      .onEnd([]() {

      })
      .onProgress([](unsigned int progress, unsigned int total)
                  {
            static bool kalan = false;
                        if (kalan == false)
                        {
                            ledcWrite(BLedChannel_, 0);
                            kalan = true;
                        }
                        else
                        {
                            ledcWrite(BLedChannel_, 255);
                            kalan = false;
                        } })
      .onError([](ota_error_t error)
               {
                 if (error == OTA_AUTH_ERROR) serialWrite("Auth Failed");
                 else if (error == OTA_BEGIN_ERROR) serialWrite("Begin Failed");
                 else if (error == OTA_CONNECT_ERROR) serialWrite("Connect Failed");
                 else if (error == OTA_RECEIVE_ERROR) serialWrite("Receive Failed");
                 else if (error == OTA_END_ERROR) serialWrite("End Failed"); });

  ArduinoOTA.begin();
}

void httpSend(WiFiClient _client, String _durum = _HTTP204, String _content = _HTML, String _icerik = "")
{
  _client.println(_durum);
  _client.println("X-Captive-Portal: true");
  _client.println("Content-Type: " + _content);
  _client.println("Content-Length: " + String(_icerik.length()));
  _client.println("Connection: keep-alive");
  // CORS başlığı ekleniyor
  _client.println("Access-Control-Allow-Origin: *");                             // Tüm domainlere izin vermek için '*' kullanabilirsiniz.
  _client.println("Access-Control-Allow-Methods: GET, OKU, YAZ, BTN, SET, KMT"); // İzin verilen HTTP yöntemleri
  _client.println("Access-Control-Allow-Headers: Content-Type");                 // İzin verilen başlıklar

  _client.println();
  _client.println(_icerik);
  _client.stop();
}

void jsonPrepare(WiFiClient _client, uint8_t _istek_No)
{

  struct tm timeinfo;
  time_t now = time(nullptr);
  localtime_r(&now, &timeinfo);
  char zaman[32];
  strftime(zaman, sizeof(zaman), "%d/%m/%Y %H:%M", &timeinfo);

  JsonDocument doc;
  JsonArray ayarlarr = doc["ayarlar"].to<JsonArray>();
  JsonArray infoo = doc["infoo"].to<JsonArray>();
  // JsonArray pinDurum = doc["pinDurum"].to<JsonArray>();

  // if (_istek_No == AyarSyfYukle_)
  // {


  // }
  // else if (_istek_No == PortalSyfYukle_)
  // {
  // }

  // infoo.add(_VERSION_Project);
  infoo.add(Donanim + " " + DonanimVer);
  // infoo.add(_VERSION_String);
  infoo.add(String(__DATE__) + " " + String(__TIME__));
  infoo.add(WiFi.macAddress());
  infoo.add(zaman);




  String gonder;
  serializeJson(doc, gonder);
  serialWrite("  ::JSON > ", gonder);

  httpSend(_client, _HTTP200, _JSON, gonder);
}

void serverLoop()
{
  WiFiClient clientt = WebServer.available();
  if (!clientt)
  {
    return;
  }
  // clientt.setTimeout(50); // Zaman aşımı süresini optimize et

  while (clientt.connected() && !clientt.available())
  {
    taskYIELD(); // Diğer işlemlerin çalışmasına izin ver
  }

  size_t availableBytes = clientt.available();
  if (availableBytes == 0 || availableBytes > 1024)
  {
    clientt.read(); // Tüm veriyi hızlıca okuyun ve yok sayın
    return;
  }

  // İlk 4 karakteri kontrol ederek GET isteğini yakalayın
  char buffer[4] = {0};                          // Buffer sadece "GET " için ayarlandı
  clientt.readBytes(buffer, sizeof(buffer) - 1); // 3 karakter oku (GET)

  // "GET" isteğini yakala ve hızlıca yönlendir
  if (strcmp(buffer, "GET") == 0)
  {
    serverGET(clientt); // Yönlendirme veya sayfa döndürme işlemleri burada yapılır
  }
  else
  {

  }
}

void serverGET(WiFiClient client)
{
  // Bufferlar ve temel ayarlar
  char buffer[10] = {0};       // GET kısmı için buffer
  char istenenSayfa[10] = {0}; // Sayfa istekleri için buffer

  // İstekleri hızlıca oku
  client.readBytesUntil('/', buffer, sizeof(buffer) - 1);             // "GET " kısmını al
  client.readBytesUntil('.', istenenSayfa, sizeof(istenenSayfa) - 1); // Sayfa adını al
  // size_t bytesRead = client.readBytesUntil('.', istenenSayfa, sizeof(istenenSayfa) - 1); // Sayfa adını al
  // buffer[bytesRead] = '\0';                                                              // Elle son byte'tan sonra ekle
  client.read(); // Tüm veriyi okuyun ve yok sayın

  // "toiot" sayfası mı kontrol et
  if (strcmp(istenenSayfa, "toiot") == 0)
  {

      serialWrite("----->>>> send Giris_Html");
      httpSend(client, _HTTP200, _HTML, Giris_Html);

  }
  // "giris" sayfası mı kontrol et
  else if (strcmp(istenenSayfa, "giris") == 0)
  {
    serialWrite("----->>>> send Giris_Html");
    httpSend(client, _HTTP200, _HTML, Giris_Html);
  }
  // "ayarlar" sayfası mı kontrol et
  else if (strcmp(istenenSayfa, "ayarlar") == 0)
  {
    serialWrite("----->>>> send Ayarlar_Html");
    httpSend(client, _HTTP200, _HTML, Ayarlar_Html);
    serialWrite("ayarlarhtml send");
  }
  // "portal" sayfası mı kontrol et
  else if (strcmp(istenenSayfa, "portal") == 0)
  {
    serialWrite("----->>>> send Portal_Html");
    httpSend(client, _HTTP200, _HTML, Portal_Html);
  }
  // Eğer tanınmayan bir istekse Captive Portal göster
  else
  {
    serialWrite("----->>>> send captive");
    httpSend(client, _HTTP200, _HTML, Captive_Html);
  }
}

void serverPOST(WiFiClient client)
{
  client.read(); // Tüm veriyi hızlıca okuyun ve yok sayın
  httpSend(client, _HTTP400);
  serialWrite("POST --> _HTTP400");
}

#endif // _wifi_h