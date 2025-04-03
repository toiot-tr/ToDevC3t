#include "header.h"
#ifndef WIFI_GLOBAL_H
#define WIFI_GLOBAL_H // #define _wifi_h

#include "wifimanager.h"
#include <ArduinoJson.h>

// #include "web/dokumanHtml.h"
#include "web/captiveHTML.h"
#include "web/ayarlarHTML.h"
#include "web/portalHTML.h"
#include "web/wifiHTML.h"
#include "web/homeHTML.h"
#include "web/infologHTML.h"

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

// Global WiFi yöneticisi
WifiManager wifiManager;

// Global fonksiyon tanımlamaları
extern void httpSend(WiFiClient _client, String _durum = _HTTP204, String _content = _HTML, String _icerik = "", String _data1 = "", String _data2 = "", String _data3 = "", String _data4 = "");
extern void jsonPrepare(WiFiClient _client, uint8_t _istek_No);
extern void serverLoop();
extern void serverGET(WiFiClient clientGet, String request);
extern void serverPOST(WiFiClient clientPost, String request);
extern void serverBTN(WiFiClient clientBtn, String request, String jsonData);
extern void serverDATA(WiFiClient clientData, String request);
extern void serverSET(WiFiClient clientSet, String request);
extern void sendWLEDJsonInfo(WiFiClient clientGet);
extern String getInfoAsJson();
extern String prepareJSLists(bool settings = true, bool info = true, bool networks = true);

extern String disconnectWifi();

// Ui ayarlari
int8_t UiGiris = 1;

// Veri istekleri
#define HomeLoad_ 10
#define HomeRefresh_ 11
#define AyarlarLoad_ 20
#define AyarlarRefresh_ 21
#define PortalLoad_ 30
#define PortalRefresh_ 31
#define WifiLoad_ 40
#define WifiRefresh_ 41

// Ayar tipleri için enum
enum AyarTipi
{
  AYAR_INT8,
  AYAR_INT16,
  AYAR_INT32,
  AYAR_FLOAT,
  AYAR_STRING
};

// Ayar yapısı
struct AyarBilgi
{
  const char *ayarAdi; // Ayar adı (aynı zamanda NVS key'i)
  AyarTipi tip;        // Veri tipi
  float min;           // Min değer (string için min uzunluk)
  float max;           // Max değer (string için max uzunluk)
  void *degisken;      // Güncellenecek değişken pointer'ı
};

// Ayar listesi
const AyarBilgi AYARLAR[] = {
    //{Ayar adı,   tipi   ,min,max, değişken pointer'ı}
    {"DevName", AYAR_STRING, 0, 15, &DevName},
    // Wifi ayarları
    {"WifiMode", AYAR_INT8, 0, 2, &wifiManager.wifiMode},     // 0:Kapalı, 1:Açık, 2:Saat Güncelle
    {"WifiPower", AYAR_INT8, 0, 5, &wifiManager.wifiPower},   // dBm cinsinden güç limitleri
    {"WifiChan", AYAR_INT8, 0, 13, &wifiManager.wifiChannel}, // 0:Otomatik, 1-13:Sabit kanal

    // Portal ayarları
    {"PortalMode", AYAR_INT8, 0, 2, &wifiManager.portalMode},  // 0:Kapalı, 1:Açık, 2:WiFi yoksa aç
    {"PortalPsk", AYAR_STRING, 0, 15, &wifiManager.PortalPsk}, // WiFi şifre uzunluk limitleri

    // Diğer ayarlar
    {"UiGiris", AYAR_INT8, 0, 1, &UiGiris} // 0:Kapalı, 1:Açık
};

#elif !defined(WIFI_LOCAL_H)
#define WIFI_LOCAL_H
#pragma once

void httpSend(WiFiClient _client, String _durum, String _content, String _icerik,
              String _data1, String _data2, String _data3, String _data4)
{
  // HTML dışındaki içeriği birleştir
  String extraContent = "";
  size_t totalLength = _icerik.length(); // HTML uzunluğu

  // HTML için kapanış tag'lerini ekle
  if (_content == _HTML)
  {
    // Data string'lerini ekle
    const String *dataStrings[] = {&_data1, &_data2, &_data3, &_data4};
    for (const String *str : dataStrings)
    {
      if (str->length() > 0)
      {
        extraContent += *str + "\n";      // Her data'dan sonra satır atla
        totalLength += str->length() + 1; // +1 for newline
      }
    }
    static const char *HTML_CLOSE = "</script>\n</body>\n</html>";
    extraContent += HTML_CLOSE;
    totalLength += strlen(HTML_CLOSE);
  }

  // HTTP Headers
  _client.println(_durum);
  _client.println("X-Captive-Portal: true");
  _client.println("Content-Type: " + _content);
  _client.println("Content-Length: " + String(totalLength));
  _client.println("Connection: keep-alive");
  _client.println("Access-Control-Allow-Origin: *");
  _client.println("Access-Control-Allow-Methods: GET, OKU, YAZ, BTN, SET, KMT");
  _client.println("Access-Control-Allow-Headers: Content-Type");
  _client.println();

  // Önce HTML sayfasını gönder
  _client.println(_icerik);

  // Sonra birleştirilmiş extra içeriği gönder
  if (extraContent.length() > 0)
  {
    _client.print(extraContent);
  }
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

  switch (_istek_No)
  {
  case HomeLoad_: // 10
  {
    // Log dosyasını oku
  }
  case HomeRefresh_: // 11
  {
    // Ana sayfa yenileme
    // TODO: Ana sayfa güncel veriler
    break;
  }
  case AyarlarLoad_: // 20
  {
    // Ayarlar sayfası ilk yükleme
    // TODO: Ayarlar sayfası verileri
    break;
  }
  case AyarlarRefresh_: // 21
  {
    // Ayarlar sayfası yenileme
    // TODO: Ayarlar güncel veriler
    break;
  }
  case PortalLoad_: // 30
  {
    // Portal sayfası ilk yükleme
    // TODO: Portal ayarları eklenecek
    break;
  }
  case PortalRefresh_: // 31
  {
    // Portal sayfası yenileme
    // TODO: Portal güncel veriler
    break;
  }
  case WifiLoad_: // 40
  {
    // Wifi ayarları sırasıyla
    ayarlarr.add(wifiManager.wifiMode);
    ayarlarr.add(wifiManager.portalMode);
    ayarlarr.add(wifiManager.PortalPsk);
    ayarlarr.add(wifiManager.wifiPower);
    ayarlarr.add(wifiManager.wifiChannel);

    // Info alanları sırasıyla
    infoo.add(wifiManager.lastIP); // Son alınan IP
    // infoo.add("");                  // SSID alanı boş başlasın
    // infoo.add("");                  // PSK alanı boş başlasın

    // Kayıtlı ağları ekle - yeni syntax
    JsonArray savedNetworks = doc["savedNetworks"].to<JsonArray>();
    for (uint8_t i = 0; i < wifiManager.MAX_NETWORKS; i++)
    {
      if (wifiManager.networks[i].ssid != "")
      {
        JsonObject network = savedNetworks.add<JsonObject>();
        network["ssid"] = wifiManager.networks[i].ssid;
        network["password"] = "••••••••"; // Şifreyi gizle
        network["lastRssi"] = wifiManager.networks[i].lastRssi;
      }
    }

    break;
  }
  case WifiRefresh_: // 41
  {
    // Wifi sayfası yenileme - sadece değişen veriler
    infoo.add(wifiManager.lastIP); // Son alınan IP

    break;
  }
  }

  String gonder;
  serializeJson(doc, gonder);
  utils.serialWrite("  ::JSON > ", gonder);
  httpSend(_client, _HTTP200, _JSON, gonder);
}

void serverLoop()
{
  WiFiClient client = WifiWebServer.available();
  if (!client)
    return;

  while (client.connected() && !client.available())
  {
    taskYIELD(); // Diğer işlemlerin çalışmasına izin ver
  }

  size_t availableBytes = client.available();
  if (availableBytes == 0 || availableBytes > 1023)
  {
    client.println("HTTP/1.1 400 Bad Request"); // Hatalı istek yanıtı gönder
    client.stop();
    return;
  }

  int8_t bLedMode = BLedSendData_;
  xQueueSend(bledQueue, &bLedMode, 0);

  // HTTP isteğini parçalara ayır
  char method[6] = {0};        // İlk 5 karakter için (GET /, SET / vs.)
  char request[50] = {0};      // Boşluğa kadar olan kısım için (wifi, home vs.)
  char restBuffer[1024] = {0}; // Kalan veri için

  // 1. Method kısmını oku (ilk 5 karakter)
  client.readBytes(method, 5);

  // 2. İstek kısmını oku (boşluğa kadar)
  client.readBytesUntil(' ', request, sizeof(request) - 1);

  // 3. Kalan veriyi oku
  client.readBytes(restBuffer, sizeof(restBuffer) - 1);

  // Test modunda isteği yazdır
  if (TestMode)
  {
    Serial.println();
    Serial.println();
    Serial.println("<<<--------gelen istek----------------");
    Serial.print(method);
    Serial.print(request);
    Serial.print(" ");
    Serial.println(restBuffer);
    Serial.println("<<<-------------------------------****");
  }

  // İlk 5 karakteri kontrol et (örn: "GET /", "SET /", "BTN /")

  // Method ve "/" kontrolü
  if (strcmp(method, "GET /") == 0)
  {
    serverGET(client, request);
  }
  else if (strcmp(method, "DAT /") == 0)
  {
    serverDATA(client, request);
  }
  else if (strcmp(method, "BTN /") == 0)
  {
    // BTN metodu her zaman JSON içerir
    serverBTN(client, request, restBuffer); // JSON verisi restBuffer'da
  }
  else if (strcmp(method, "SET /") == 0)
  {
    serverSET(client, request);
  }
  else if (strcmp(method, "POST ") == 0)
  {
    serverPOST(client, request);
  }
  else
  {
    utils.serialWrite("Geçersiz istek: ", method);
    client.println("HTTP/1.1 400 Bad Request");
    client.stop();
    return;
  }

  // Keep-Alive kontrolü
  if (strstr(restBuffer, "Connection: keep-alive") == NULL)
  {
    client.stop(); // Keep-Alive istenmemişse bağlantıyı kapat
    utils.serialWrite("client.stop()");
  }
  else
  {
    // utils.serialWrite("keep-alive");
  }
  // utils.serialWrite("");
}

void serverGET(WiFiClient clientGet, String request)
{
  // "toiot" veya "home" sayfası mı kontrol et
  if (strstr(request.c_str(), "toiot.html") ||
      strstr(request.c_str(), "home.html"))
  {
    utils.serialWrite("----->>>> send Home_Html");
    httpSend(clientGet, _HTTP200, _HTML, Home_Html);
  }
  else if (strstr(request.c_str(), "infolog.html"))
  {
    utils.serialWrite("----->>>> send Infolog_Html");
    // Log dosyasını oku
    String logContent = "";
    File file = LittleFS.open("/log.txt", "r");
    if (file)
    {
      logContent = file.readString();
      file.close();
    }
    else
    {
      utils.serialWrite("Log dosyası okuma hatası");
      logContent = "Log dosyası okunamadı!";
    }

    // JavaScript değişkeni olarak gönder
    httpSend(clientGet, _HTTP200, _HTML, Infolog_Html, "logData = `" + logContent + "`;");
  }
  // "ayarlar" sayfası mı kontrol et
  else if (strstr(request.c_str(), "ayarlar.html"))
  {
    utils.serialWrite("----->>>> send Ayarlar_Html");
    httpSend(clientGet, _HTTP200, _HTML, Ayarlar_Html, prepareJSLists());
    utils.serialWrite("ayarlarhtml send");
  }
  // "portal" sayfası mı kontrol et
  else if (strstr(request.c_str(), "portal.html"))
  {
    utils.serialWrite("----->>>> send Portal_Html");
    httpSend(clientGet, _HTTP200, _HTML, Portal_Html, prepareJSLists());
  }
  else if (strstr(request.c_str(), "wifi.html"))
  {
    utils.serialWrite("----->>>> send Wifi_Html with data");

    // HTML ve data'yı ayrı ayrı gönder
    httpSend(clientGet, _HTTP200, _HTML, Wifi_Html, prepareJSLists());
  }
  // WLED uygulaması için /json/info endpoint'i
  else if (strstr(request.c_str(), "json/si"))
  {
    sendWLEDJsonInfo(clientGet);
  }
  // else if (strstr(request.c_str(), "favicon.png")) {
  //   utils.serialWrite("----->>>> send favicon.png");

  //   // HTTP başlıklarını gönder
  //   clientGet.println(_HTTP200);
  //   clientGet.println("Content-Type: image/png");
  //   clientGet.println("Content-Length: " + String(sizeof(Favicon_Base64)));
  //   clientGet.println("Cache-Control: max-age=31536000"); // 1 yıl önbellek
  //   clientGet.println();

  //   // Binary veriyi gönder
  //   clientGet.write_P(Favicon_Base64, sizeof(Favicon_Base64));
  // }
  // Eğer tanınmayan bir istekse Captive Portal göster
  else
  {
    utils.serialWrite("----->>>> send captive");
    httpSend(clientGet, _HTTP200, _HTML, Captive_Html);
  }
}

void serverPOST(WiFiClient clientPost, String request)
{

  clientPost.println("HTTP/1.1 400 Bad Request");
  utils.serialWrite("POST --> _HTTP400");
}

void serverBTN(WiFiClient clientBtn, String request, String jsonData)
{
  utils.serialWrite("BTN komutu -> ", request);
  JsonDocument responseDoc;
  String response;
  // json içermeyecek butonlar jsonu işlemeye gerek yok
  if (strcmp(request.c_str(), "reset.btn") == 0)
  {
    utils.serialWrite("Reset komutu alındı");
    responseDoc["message"] = "Cihaz resetleniyor...";
    responseDoc["notification"]["message"] = "Cihaz resetleniyor...";
    responseDoc["notification"]["type"] = "info";
    serializeJson(responseDoc, response);
    httpSend(clientBtn, _HTTP200, _JSON, response);
    espRestart();
    return;
  }
  else if (strcmp(request.c_str(), "restore.btn") == 0)
  {
    utils.serialWrite("Restore komutu alındı");
    responseDoc["message"] = "Ayarlar siliniyor...";
    responseDoc["notification"]["message"] = "Ayarlar siliniyor...";
    responseDoc["notification"]["type"] = "info";
    serializeJson(responseDoc, response);
    httpSend(clientBtn, _HTTP200, _JSON, response);
    restoreDefaults();
    return;
  }
  else if (strcmp(request.c_str(), "refresh.btn") == 0)
  {
    utils.serialWrite("Refresh komutu alındı");
    responseDoc["message"] = "Bilgiler yenilendi...";
    responseDoc["notification"]["message"] = "Bilgiler yenilendi...";
    responseDoc["notification"]["type"] = "info";
    JsonDocument infoDoc;
    deserializeJson(infoDoc, getInfoAsJson()); // info'yu parse et
    responseDoc["info"] = infoDoc;
    JsonDocument networksDoc;
    deserializeJson(networksDoc, wifiManager.getNetworksAsJson()); // ağları tara güncel bilgileri al
    responseDoc["savedNetworks"] = networksDoc["savedNetworks"];
    serializeJson(responseDoc, response);
    httpSend(clientBtn, _HTTP200, _JSON, response);
    return;
  }
  else if (strcmp(request.c_str(), "wifiScan.btn") == 0)
  {
    utils.serialWrite("wifiScan komutu alındı");
    httpSend(clientBtn, _HTTP200, _JSON, wifiManager.scanNetworks());
    return;
  }
  else if (strcmp(request.c_str(), "wifiDisconnect.btn") == 0)
  {
    utils.serialWrite("wifiDisconnect komutu alındı");
    String response = disconnectWifi();
    if (clientBtn.connected())
    {
      httpSend(clientBtn, _HTTP200, _JSON, response);
    }
    return;
  }

  // JSON belirteçlerini ara
  const char *startMarker = "<<json|";
  const char *endMarker = "|json>>";

  // Başlangıç belirtecini bul
  const char *jsonStart = strstr(jsonData.c_str(), startMarker);
  if (!jsonStart)
  {
    utils.serialWrite("JSON başlangıç belirteci bulunamadı");
    responseDoc["message"] = "Hata: json bulunamadı";
    responseDoc["notification"]["message"] = "Hata: json bulunamadı";
    responseDoc["notification"]["type"] = "error";
    serializeJson(responseDoc, response);
    httpSend(clientBtn, _HTTP400, _TEXT, response);
    return;
  }

  // JSON başlangıcına git
  jsonStart += strlen(startMarker);

  // Bitiş belirtecini bul
  const char *jsonEnd = strstr(jsonStart, endMarker);
  if (!jsonEnd)
  {
    utils.serialWrite("JSON bitiş belirteci bulunamadı");
    responseDoc["message"] = "Hata: json bitiş belirteci bulunamadı";
    responseDoc["notification"]["message"] = "Hata: json bitiş belirteci bulunamadı";
    responseDoc["notification"]["type"] = "error";
    serializeJson(responseDoc, response);
    httpSend(clientBtn, _HTTP400, _TEXT, response);
    return;
  }

  // JSON'ı parse et
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, jsonStart, jsonEnd - jsonStart);

  if (error)
  {
    utils.serialWrite("JSON parse hatası");
    responseDoc["message"] = "Hata: JSON parse hatası";
    responseDoc["notification"]["message"] = "Hata: JSON parse hatası";
    responseDoc["notification"]["type"] = "error";
    serializeJson(responseDoc, response);
    httpSend(clientBtn, _HTTP400, _TEXT, response);
    return;
  }

  // JSON verilerini al ve String'e dönüştür
  String ssid = doc["ssid"] | "";
  String password = doc["password"] | "";
  int8_t index = doc["index"] | -1;

  utils.serialWrite("JSON data -> ssid:", ssid, " password:", password, " index:", String(index));

  // json içerecek butonlar
  if (strcmp(request.c_str(), "scanConnect.btn") == 0)
  {
    utils.serialWrite("scanConnect komutu alındı");
    if (clientBtn.connected())
    {
      httpSend(clientBtn, _HTTP200, _JSON, wifiManager.connectWifi(SsidPass_, ssid, password));
    }
    else
    {
      wifiManager.connectWifi(SsidPass_, ssid, password);
    }
    return;
  }
  else if (strcmp(request.c_str(), "scanSave.btn") == 0)
  {
    utils.serialWrite("scanSave komutu alındı");
    // Ağı kaydet
    httpSend(clientBtn, _HTTP200, _JSON, wifiManager.saveWifi(ssid, password));
    return;
  }
  else if (strcmp(request.c_str(), "wifiConnect.btn") == 0)
  {
    utils.serialWrite("wifiConnect komutu alındı");
    if (clientBtn.connected())
    {
      httpSend(clientBtn, _HTTP200, _JSON, wifiManager.connectWifi(index));
    }
    else
    {
      wifiManager.connectWifi(index);
    }
    return;
  }
  else if (strcmp(request.c_str(), "wifiUpdate.btn") == 0)
  {
    utils.serialWrite("wifiUpdate komutu alındı");
    httpSend(clientBtn, _HTTP200, _JSON, wifiManager.updateWifi(index, ssid, password));
    return;
  }
  else if (strcmp(request.c_str(), "wifiDelete.btn") == 0)
  {
    utils.serialWrite("wifiDelete komutu alındı");

    httpSend(clientBtn, _HTTP200, _JSON, wifiManager.deleteWifi(index));
    return;
  }
  else
  {
    utils.serialWrite("Geçersiz komut!");
    httpSend(clientBtn, _HTTP200, _JSON, "{\"message\":\"Geçersiz komut!\",\"notification\":{\"message\":\"Geçersiz komut!\",\"type\":\"error\"}}");
    return;
  }

  // httpSend(clientBtn, _HTTP200, _TEXT, mesaj); // Mesajı gönder
}

void serverDATA(WiFiClient clientData, String request)
{
  // utils.serialWrite("DATA istek -> Sayfa: ", request);
  JsonDocument responseDoc;
  String response;

  if (strcmp(request.c_str(), "home.html") == 0)
  {
    JsonDocument infoDoc;
    deserializeJson(infoDoc, getInfoAsJson()); // info'yu parse et
    responseDoc["info"] = infoDoc["info"];
    serializeJson(responseDoc, response);
    // utils.serialWrite("Wifi yenileme", response);
    httpSend(clientData, _HTTP200, _JSON, response);
  }
  else if (strcmp(request.c_str(), "ayarlar.html") == 0)
  {
    httpSend(clientData, _HTTP200, _JSON, getInfoAsJson());
  }
  else if (strcmp(request.c_str(), "infolog.html") == 0)
  {
    httpSend(clientData, _HTTP200, _JSON, getInfoAsJson());
  }
  else if (strcmp(request.c_str(), "wifi.html") == 0)
  {
    JsonDocument infoDoc;
    deserializeJson(infoDoc, getInfoAsJson()); // info'yu parse et
    responseDoc["info"] = infoDoc["info"];
    JsonDocument networksDoc;
    deserializeJson(networksDoc, wifiManager.getNetworksAsJson()); // ağları tara güncel bilgileri al
    responseDoc["savedNetworks"] = networksDoc["savedNetworks"];
    serializeJson(responseDoc, response);
    // utils.serialWrite("Wifi yenileme", response);
    httpSend(clientData, _HTTP200, _JSON, response);
  }
}

// SET metoduyla gelen istekleri işleyen fonksiyon
void serverSET(WiFiClient clientSet, String request)
{
  // Ayar adı ve değerini al (örn: "WifiMode=1")
  int equalPos = request.indexOf('=');
  if (equalPos == -1)
  {
    utils.serialWrite("Geçersiz SET formatı");
    httpSend(clientSet, _HTTP400, _TEXT, "Geçersiz format");
    return;
  }

  String ayarAdi = request.substring(0, equalPos);
  String ayarDeger = request.substring(equalPos + 1);

  utils.serialWrite("SET istek -> Ayar: ", ayarAdi, " Değer: ", ayarDeger); // Debug log

  int8_t gecerli = 0; // Ayar değerinin geçerli olup olmadığını kontrol için
  String cevap;       // Client'a gönderilecek cevap

  DevConfig.begin("userData", false); // NVS'yi aç (kalıcı depolama)

  for (const AyarBilgi &ayar : AYARLAR)
  { // Tüm ayarları döngüyle kontrol et
    if (strcmp(ayar.ayarAdi, ayarAdi.c_str()) == 0)
    { // İstenen ayar bulunduysa
      switch (ayar.tip)
      { // Ayarın tipine göre işlem yap
      case AYAR_INT8:
      {                                         // 8-bit integer (-128 to 127)
        int8_t deger = atol(ayarDeger.c_str()); // String'i int8_t'ye çevir
        if (deger >= ayar.min && deger <= ayar.max)
        {                                        // Değer sınırlar içinde mi?
          *(int8_t *)ayar.degisken = deger;      // Değişkene yeni değeri ata
          DevConfig.putInt(ayar.ayarAdi, deger); // NVS'ye kaydet
          gecerli = 1;                           // Geçerli bir değer atandı
          cevap = String(deger);                 // Cevap olarak yeni değeri gönder
        }
        else
        {
          gecerli = -1;                             // Geçerli bir değer atanmadı
          cevap = String(*(int8_t *)ayar.degisken); // Geçersizse mevcut değeri gönder
        }
        break;
      }
      case AYAR_INT16:
      {                                          // 16-bit integer (-32,768 to 32,767)
        int16_t deger = atol(ayarDeger.c_str()); // String'i int16_t'ye çevir
        if (deger >= ayar.min && deger <= ayar.max)
        {
          *(int16_t *)ayar.degisken = deger;
          DevConfig.putInt(ayar.ayarAdi, deger);
          gecerli = 1;
          cevap = String(deger);
        }
        else
        {
          gecerli = -1;
          cevap = String(*(int16_t *)ayar.degisken);
        }
        break;
      }
      case AYAR_INT32:
      {                                          // 32-bit integer (-2,147,483,648 to 2,147,483,647)
        int32_t deger = atol(ayarDeger.c_str()); // String'i int32_t'ye çevir
        if (deger >= ayar.min && deger <= ayar.max)
        {
          *(int32_t *)ayar.degisken = deger;
          DevConfig.putInt(ayar.ayarAdi, deger);
          gecerli = 1;
          cevap = String(deger);
        }
        else
        {
          gecerli = -1;
          cevap = String(*(int32_t *)ayar.degisken);
        }
        break;
      }
      case AYAR_FLOAT:
      {                                        // Ondalıklı sayı
        float deger = atof(ayarDeger.c_str()); // String'i float'a çevir
        if (deger >= ayar.min && deger <= ayar.max)
        {
          *(float *)ayar.degisken = deger;
          DevConfig.putFloat(ayar.ayarAdi, deger);
          gecerli = 1;
          cevap = String(deger, 2); // 2 ondalık basamakla göster
        }
        else
        {
          gecerli = -1;
          cevap = String(*(float *)ayar.degisken, 2);
        }
        break;
      }
      case AYAR_STRING:
      {                                                   // Metin
        size_t uzunluk = utils.karakterSayisi(ayarDeger); // UTF-8 karakter sayısını hesapla
        if (uzunluk >= ayar.min && uzunluk <= ayar.max)
        {
          gecerli = 1;

          if (ayarAdi == "PortalPsk" && uzunluk < 8) // PortalPsk 8 karakterden küçükse boş kaydet
          {
            utils.serialWrite("PortalPsk silindi 8 karakterden küçük");
            ayarDeger = ""; // String'i boş yap
            gecerli = -1;
          }

          *(String *)ayar.degisken = ayarDeger;
          DevConfig.putString(ayar.ayarAdi, ayarDeger);
          cevap = ayarDeger;
        }
        else
        {
          gecerli = -1;
          cevap = *(String *)ayar.degisken;
        }
        break;
      }
      }
      break; // Ayar bulundu, döngüden çık
    }
  }

  DevConfig.end(); // NVS'yi kapat

  if (strcmp(ayarAdi.c_str(), "WifiPower") == 0)
  {
    wifiManager.setWifiPower();
  }

  if (gecerli == 1)
  {
    utils.serialWrite("Ayar kaydedildi: ", ayarAdi, " = ", cevap);
    httpSend(clientSet, _HTTP200, _JSON, "{\"cevap\":\"" + cevap + "\",\"durum\":\"ok\"}");
  }
  else if (gecerli == -1)
  {
    utils.serialWrite("Geçersiz değer: ", ayarAdi, " = ", ayarDeger);
    httpSend(clientSet, _HTTP200, _JSON, "{\"cevap\":\"" + cevap + "\",\"durum\":\"error\"}");
  }
  else
  {
    utils.serialWrite("Ayar bulunamadı: ", ayarAdi);
    httpSend(clientSet, _HTTP200, _JSON, "{\"cevap\":\"Ayar bulunamadı: " + String(ayarAdi) + "\",\"durum\":\"error\"}");
  }
}

// Listeleri JavaScript formatında hazırlayan genel fonksiyon
String prepareJSLists(bool settings, bool info, bool networks)
{
  String output = "";

  // Ayarlar listesi
  if (settings)
  {
    output += "ayarlar = {";
    bool first = true;

    for (const AyarBilgi &ayar : AYARLAR)
    {
      if (!first)
        output += ",";
      first = false;
      output += String(ayar.ayarAdi) + ":";

      switch (ayar.tip)
      {
      case AYAR_INT8:
        output += String(*(int8_t *)ayar.degisken);
        break;
      case AYAR_INT16:
        output += String(*(int16_t *)ayar.degisken);
        break;
      case AYAR_INT32:
        output += String(*(int32_t *)ayar.degisken);
        break;
      case AYAR_FLOAT:
        output += String(*(float *)ayar.degisken, 2);
        break;
      case AYAR_STRING:
        output += "\"" + *(String *)ayar.degisken + "\"";
        break;
      }
    }
    output += "};";
  }

  // Info verileri
  if (info)
  {
    output += "\n";
    output += "infoo = {";
    output += "Version:\"" + String(_VERSION_String) + "\",";
    output += "Hardware:\"" + String(Hardware) + "\",";
    output += "BuildTime:\"" + String(_VERSION_Build) + "\",";
    output += "LastIP:\"" + wifiManager.lastIP + "\",";
    // WiFi durumunu kontrol et
    output += "WifiStatus:" + String(WiFi.status() == WL_CONNECTED ? "true" : "false") + ",";
    // Sadece WiFi bağlıysa SSID ve RSSI gönder
    output += "CurrentSSID:\"" + (WiFi.status() == WL_CONNECTED ? WiFi.SSID() : "") + "\",";
    // RSSI'yi sayısal değer olarak gönder
    output += "Rssi:" + (WiFi.status() == WL_CONNECTED ? String(WiFi.RSSI()) : "0") + ",";
    // Portal durumunu kontrol et
    output += "PortalStatus:" + String(wifiManager.statusPortal == PortalOpen_ ? "true" : "false") + ",";
    output += "CpuTemp:\"" + String(getTemperature()) + "°C\",";
    output += "MaxCpuTemp:\"" + String(MaxCpuTemp) + "°C\",";
    output += "Uptime:\"" + String(UptimeDay) + " gün\"";
    output += "};";
  }

  // kayıtlı ağlar listesi
  if (networks)
  {
    output += "\n"; // Ayarlar ve ağlar arasına boşluk ekle
    output += "savedNetworks = [";
    bool first = true;

    for (uint8_t i = 0; i < wifiManager.MAX_NETWORKS; i++)
    {
      if (wifiManager.networks[i].ssid != "")
      {
        if (!first)
          output += ",";
        first = false;
        output += "{";
        output += "ssid:\"" + wifiManager.networks[i].ssid + "\",";
        output += "lastRssi:" + String(wifiManager.networks[i].lastRssi) + ",";
        output += "msg:\"ch:" + String(wifiManager.networks[i].channel) + "\",";
        if (WiFi.status() == WL_CONNECTED)
          output += "connected:" + String(wifiManager.networks[i].connected);
        else
          output += "connected:false";
        output += "}";
      }
    }
    output += "];";
  }

  utils.serialWrite("JS listeler: ", output);
  return output;
}

// Info verilerini JSON formatında döndürür
String getInfoAsJson()
{
  JsonDocument doc;
  JsonObject info = doc["info"].to<JsonObject>(); // info bir alt obje olarak oluşturulmalı

  info["Version"] = _VERSION_String;
  info["Hardware"] = Hardware;
  info["BuildTime"] = _VERSION_Build;
  info["LastIP"] = wifiManager.lastIP;
  info["WifiStatus"] = (WiFi.status() == WL_CONNECTED);
  info["CurrentSSID"] = WiFi.status() == WL_CONNECTED ? WiFi.SSID() : "";
  info["Rssi"] = WiFi.status() == WL_CONNECTED ? WiFi.RSSI() : 0;
  info["PortalStatus"] = (wifiManager.statusPortal == PortalOpen_);
  info["CpuTemp"] = String(getTemperature()) + "°C";
  info["MaxCpuTemp"] = String(MaxCpuTemp) + "°C";
  info["Uptime"] = String(UptimeDay) + " gün";
  info["DevMac"] = WiFi.macAddress().c_str();

  // cihzın zamanını gün ay yıl saat dakika şeklinde al
  info["DevTime"] = printLocalTime();

  String output;
  serializeJson(doc, output);
  // utils.serialWrite("Info JSON: ", output);
  return output;
}

// WiFi bağlantısını koparma fonksiyonu
String disconnectWifi()
{
  utils.serialWrite("WiFi bağlantısı koparılıyor...");

  JsonDocument doc;

  // Önce bağlantı durumunu kontrol et
  if (WiFi.status() != WL_CONNECTED)
  {
    doc["message"] = "WiFi bağlantısı yok";
    doc["notification"]["message"] = "WiFi bağlantısı yok";
    doc["notification"]["type"] = "info";
  }
  else
  {
    wifiManager.wifiDisconnect();
    doc["message"] = "WiFi bağlantısı koparıldı";
    doc["notification"]["message"] = "WiFi bağlantısı koparıldı";
    doc["notification"]["type"] = "info";
  }
  JsonDocument networksDoc;
  deserializeJson(networksDoc, wifiManager.getNetworksAsJson()); // ağları tara güncel bilgileri al
  doc["savedNetworks"] = networksDoc["savedNetworks"];
  String response;
  serializeJson(doc, response);
  return response;
}

void sendWLEDJsonInfo(WiFiClient clientGet)
{
  utils.serialWrite("----->>>> send WLED json info");
  JsonDocument doc;
  JsonObject state = doc["state"].to<JsonObject>();
  state["on"] = true;              // LED'lerin açık/kapalı durumu
  state["bri"] = 0;                // Genel parlaklık seviyesi (0-255)
  state["transition"] = 7;         // Efektler arası geçiş süresi (ms)
  state["bs"] = 0;                // Yedek durum slotu
  state["ps"] = -1;               // Etkin önayar slotu (-1: hiçbiri)
  state["pl"] = -1;               // Oynatılacak oynatma listesi
  state["ledmap"] = 0;            // LED haritalaması

  JsonObject nl = state["nl"].to<JsonObject>();
  nl["on"] = false;               // Gece ışığı modu aktif/pasif
  nl["dur"] = 60;                // Gece modu süresi (dakika)
  nl["mode"] = 1;                // Gece modu tipi
  nl["tbri"] = 0;               // Gece modu parlaklığı
  nl["rem"] = -1;               // Kalan süre

  JsonObject udpn = state["udpn"].to<JsonObject>();
  udpn["send"] = false;          // UDP senkronizasyon gönderimi
  udpn["recv"] = false;           // UDP senkronizasyon alımı
  udpn["sgrp"] = 1;             // Gönderim grubu
  udpn["rgrp"] = 1;             // Alım grubu

  state["lor"] = 0;              // Live override aktif/pasif
  state["mainseg"] = 0;          // Ana segment indeksi

  JsonArray seg = state["seg"].to<JsonArray>();
  JsonObject seg0 = seg.add<JsonObject>();
  seg0["id"] = 0;                // Segment ID
  seg0["start"] = 0;             // Başlangıç LED'i
  seg0["stop"] = 1;              // Bitiş LED'i
  seg0["len"] = 1;               // Segment uzunluğu
  seg0["grp"] = 1;               // Grup boyutu
  seg0["spc"] = 0;               // LED'ler arası boşluk
  seg0["of"] = 0;                // LED offset
  seg0["on"] = true;             // Segment aktif/pasif
  seg0["frz"] = false;           // Segment dondurulmuş
  seg0["bri"] = 255;             // Segment parlaklığı
  seg0["cct"] = 127;             // Renk sıcaklığı
  seg0["set"] = 0;               // Segment ayarları

  JsonArray col = seg0["col"].to<JsonArray>();
  JsonArray col1 = col.add<JsonArray>();
  col1.add(255);                 // Birincil renk R değeri
  col1.add(98);                  // Birincil renk G değeri
  col1.add(0);                   // Birincil renk B değeri

  JsonArray col2 = col.add<JsonArray>();
  col2.add(0);
  col2.add(0);
  col2.add(0);

  JsonArray col3 = col.add<JsonArray>();
  col3.add(0);
  col3.add(0);
  col3.add(0);

  seg0["fx"] = 0;               // Efekt ID (0-255 arası efekt numarası)
  seg0["sx"] = 128;             // Efekt özel ayarı 1 - Efekte özgü hız/boyut ayarı
  seg0["ix"] = 128;             // Efekt özel ayarı 2 - Efekte özgü yoğunluk ayarı
  seg0["pal"] = 0;              // Renk paleti ID (0-255 arası palet numarası)
  seg0["c1"] = 128;             // Özel renk 1 - Bazı efektlerde kullanılır
  seg0["c2"] = 128;             // Özel renk 2 - Bazı efektlerde kullanılır
  seg0["c3"] = 16;              // Özel renk 3 - Bazı efektlerde kullanılır
  seg0["sel"] = true;           // Segmentin seçili olup olmadığı
  seg0["rev"] = false;          // Efektin ters yönde çalışması 
  seg0["mi"] = false;           // Efektin aynalama modu
  seg0["o1"] = false;           // Seçenek 1 - Efekte özgü bayrak
  seg0["o2"] = false;           // Seçenek 2 - Efekte özgü bayrak  
  seg0["o3"] = false;           // Seçenek 3 - Efekte özgü bayrak
  seg0["si"] = 0;               // Segment başına düşen LED sayısı
  seg0["m12"] = 0;              // LED grubu başına mod ayarı

  JsonObject info = doc["info"].to<JsonObject>();
  info["ver"] = "0.15.0";  // WLED firmware versiyonu
  info["vid"] = 2412100;         // Build ID
  info["cn"] = "Kōsen";           // Cihaz ismi
  info["release"] = "ESP32-C3";  // Donanım versiyonu

  JsonObject leds = info["leds"].to<JsonObject>();
  leds["count"] = 1;             // Toplam LED sayısı
  leds["pwr"] = 120;             // Tahmini güç tüketimi (mA)
  leds["fps"] = 2;               // Gerçek FPS değeri
  leds["maxpwr"] = 0;            // Maksimum güç limiti
  leds["maxseg"] = 32;           // Maksimum segment sayısı
  leds["bootps"] = 0;            // Açılış önayar numarası

  JsonArray seglc = leds["seglc"].to<JsonArray>();
  seglc.add(1);                  // Segment başına LED sayacı

  leds["lc"] = 1;               // LED kontrolcü sayısı
  leds["rgbw"] = false;         // RGBW LED desteği
  leds["wv"] = 0;               // Beyaz kanal değeri
  leds["cct"] = 0;              // Renk sıcaklığı kontrolü

  info["str"] = false;          // LED verisi akış modunda mı?
  info["name"] = "ToioT";        // Cihazın adı
  info["udpport"] = 21324;      // UDP sync portu
  info["simplifiedui"] = false; // Basitleştirilmiş UI modu
  info["live"] = false;         // Live modu aktif mi?
  info["liveseg"] = -1;         // Hangi segment live modda (-1: hiçbiri)
  info["lm"] = "";              // Live mod tipi ("" veya "Color")
  info["lip"] = "";             // Live veri kaynağı IP'si
  info["ws"] = 0;               // WebSocket bağlantı sayısı
  info["fxcount"] = 215;        // Toplam efekt sayısı
  info["palcount"] = 72;        // Toplam palet sayısı
  info["cpalcount"] = 0;        // Özel palet sayısı

  JsonObject wifi = info["wifi"].to<JsonObject>();
  wifi["bssid"] = WiFi.BSSIDstr(); // Bağlı AP'nin MAC adresi
  wifi["rssi"] = WiFi.RSSI();                   // Sinyal gücü
  wifi["signal"] = 78;                  // Sinyal kalitesi (%)
  wifi["channel"] = WiFi.channel();                  // WiFi kanalı
  wifi["ap"] = true;                    // AP modu aktif/pasif

  JsonObject fs = info["fs"].to<JsonObject>();
  fs["u"] = 12;                 // Dosya sistemi kullanımı (KB)
  fs["t"] = 983;                // Toplam dosya sistemi alanı (KB)
  fs["pmt"] = 1743618189;       // Son preset değişikliği timestamp'i

  info["ndc"] = 0;              // DMA kanalı sayısı
  info["arch"] = "ESP32-C3";    // Donanım mimarisi
  info["core"] = "4.4.4.20230310"; // ESP-IDF sürümü
  info["clock"] = 160;          // İşlemci hızı (MHz)
  info["flash"] = 4;            // Flash bellek boyutu (MB)
  info["lwip"] = 0;             // LWIP sürümü
  info["freeheap"] = 162892;    // Boş heap bellek (bytes)
  info["uptime"] = 45481;       // Çalışma süresi (saniye)
  info["time"] = "2025-4-3, 06:09:33"; // Sistem zamanı
  info["opt"] = 79;             // Optimizasyon bayrakları:
                               // bit 0: Parlaklık düzeltme
                               // bit 1: Durum kaydı
                               // bit 2: Açılışta LED'leri aç
                               // bit 3: Son durumu yükle
                               // bit 4: Analog LED'ler
                               // bit 5: PWM gamma düzeltme
                               // bit 6: LED sırası tersine
                               // bit 7: Skip ilk LED

  info["brand"] = "WLED";       // Marka adı
  info["product"] = "FOSS";     // Ürün tipi
  info["mac"] = WiFi.macAddress(); // MAC adresi
  info["ip"] = WiFi.localIP().toString(); // IP adresi

  String jsonResponse;
  serializeJson(doc, jsonResponse);

  httpSend(clientGet, _HTTP200, _JSON, jsonResponse);
}

#endif // _wifi_h
