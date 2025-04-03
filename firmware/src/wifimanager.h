#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include "header.h"
#include "WiFi.h"
// #include "esp_wifi.h" // Eksik olan include'u ekleyin
#include <ESPmDNS.h>
#include <DNSServer.h>
#include <ArduinoOTA.h>
#include <ArduinoJson.h>

#include "utils.h"

// dns_ayarları *****************************************************
String MdnsName = "ToioT"; // mDNS isim

const uint8_t DNS_PORT = 53;
IPAddress ApIP(3, 3, 3, 3);
DNSServer DnsServer;
WiFiServer WifiWebServer(80);

// WiFi Sabitleri
#define WifiMaxTry_ 20

// WiFi Güç Seviyeleri
enum WifiPower
{
    WifiPwrMin_ = 1,
    WifiPwrLow_ = 2,
    WifiPwrMed_ = 3,
    WifiPwrHigh_ = 4,
    WifiPwrMax_ = 5
};

// WiFi Çalışma Modları
enum WifiMode
{
    WifiOff_ = 0,       // Kapalı
    WifiOn_ = 1,        // Açık
    WifiTimeUpdate_ = 2 // Saat Güncelle
};

// Portal Çalışma Modları
enum PortalMode
{
    PortalOff_ = 0,  // Kapalı
    PortalOn_ = 1,   // Açık
    PortalNoNet_ = 2 // Wifi yoksa aç
};

// WiFi ve Portal Durumları
enum WifiStatus
{
    WifiConnected_ = -1,
    WifiNoSsid_ = -2,     // Kayıtlı SSID yok
    WifiConnFailed_ = -3, // Bağlantı başarısız
    WifiLost_ = -4,       // Bağlantı koptu
    WifiNoNetwork_ = -5,  // Ağ taramasında ağ bulunamadı
    WifiDisconnected_ = -6
};

enum PortalStatus
{
    PortalOpen_ = -5,
    PortalClosed_ = -6,
    PortalError_ = -7,
    PortalBtnPressed_ = -8
};

// Kayıtlı ağ yapısı
struct WifiNetwork
{
    String ssid = "";
    String bssid = "";
    String password = "";
    int8_t lastRssi = -100; // Son sinyal gücü
    bool hasFailed = false; // Bağlantı başarısız oldu mu?
    uint8_t channel = 0;    // WiFi kanalı (1-13)
    bool connected = false; // Bağlantı durumu
};

// WiFi bağlantı modları için enum
enum ConnectMode
{
    SsidPass_ = -3,   // SSID ve şifre ile bağlan
    FromList_ = -2,   // Listedeki sıraya göre dene
    BestSignal_ = -1, // En iyi sinyalli ağa bağlan
    Index0_ = 0,      // Index ile belirtilen ağa bağlan (0-4 arası)
    Index1_ = 1,
    Index2_ = 2,
    Index3_ = 3,
    Index4_ = 4
};

class WifiManager
{
private:
public:
    // Tüm değişkenler public
    static const uint8_t MAX_NETWORKS = 5;
    WifiNetwork networks[MAX_NETWORKS];

    // WiFi değişkenleri
    int8_t wifiMode = WifiOff_;     // Wifi modu
    int8_t portalMode = PortalOff_; // Portal modu
    int8_t statusWifi = WifiNoSsid_;
    int8_t wifiPower = WifiPwrMed_;
    int8_t wifiChannel = 0;
    String lastIP = "Yok";

    // Portal değişkenleri
    String portalName = "ToioT";
    String PortalPsk = "";
    int8_t statusPortal = PortalClosed_;
    int8_t portalBtn = PortalClosed_;

    // WiFi gücünü ayarlayan fonksiyon - varsayılan olarak mevcut wifiPower'ı kullanır
    bool setWifiPower(int8_t level = -1)
    {
        if (level == -1)
        {
            level = wifiPower;
        }

        switch (level)
        {
        case WifiPwrMin_:
            WiFi.setTxPower(WIFI_POWER_2dBm);
            break;
        case WifiPwrLow_:
            WiFi.setTxPower(WIFI_POWER_5dBm);
            break;
        case WifiPwrMed_:
            WiFi.setTxPower(WIFI_POWER_11dBm);
            break;
        case WifiPwrHigh_:
            WiFi.setTxPower(WIFI_POWER_17dBm);
            break;
        case WifiPwrMax_:
            WiFi.setTxPower(WIFI_POWER_19_5dBm);
            break;
        default:
            return false;
        }

        utils.serialWrite("WifiPwr: ", String(level), " -> txPower: ", String(WiFi.getTxPower()));
        return true;
    }

    void setupOTA(String password = "")
    {
        ArduinoOTA.end();
        ArduinoOTA.setHostname(portalName.c_str());

        if (password != "")
        {
            ArduinoOTA.setPassword(password.c_str());
            utils.serialWrite("OTA şifresi ayarlandı");
        }

        ArduinoOTA
            .onStart([]()
                     {
                         String type = (ArduinoOTA.getCommand() == U_FLASH) ? "sketch" : "filesystem";
                         utils.serialWrite("OTA başladı: " + type); })
            .onEnd([]()
                   { utils.serialWrite("\nOTA bitti"); })
            .onProgress([](unsigned int progress, unsigned int total)
                        {
                           utils.serialWrite("OTA Progress: " + String((progress / (total / 100))) + "%\r");
                           // LED'i yanıp söndür
                           static bool ledState = false;
                           ledcWrite(BLedChannel_, ledState ? 255 : 0);
                           ledState = !ledState; })
            .onError([](ota_error_t error)
                     {
                         String errorStr;
                         switch (error)
                         {
                         case OTA_AUTH_ERROR:    errorStr = "Auth Failed"; break;
                         case OTA_BEGIN_ERROR:   errorStr = "Begin Failed"; break;
                         case OTA_CONNECT_ERROR: errorStr = "Connect Failed"; break;
                         case OTA_RECEIVE_ERROR: errorStr = "Receive Failed"; break;
                         case OTA_END_ERROR:     errorStr = "End Failed"; break;
                         }
                         utils.serialWrite("OTA Error[" + String(error) + "]: " + errorStr); });

        ArduinoOTA.begin();
        utils.serialWrite("OTA " + String(password != "" ? "şifreli" : "şifresiz") + " başlatıldı");
    }

    int8_t startPortal()
    {
        portalName = DevName;
        if (statusPortal == PortalOpen_)
        {
            utils.serialWrite("Portal zaten açık");
            return statusPortal;
        }

        // Her zaman AP+STA modunu kullan
        WiFi.mode(WIFI_AP_STA);
        utils.serialWrite("WiFi modu: AP+STA");
        WiFi.softAPsetHostname(portalName.c_str());

        // Portal şifresini kontrol et
        uint8_t passLength = utils.karakterSayisi(PortalPsk);
        if (passLength < 8)
        {
            utils.serialWrite("Portal şifresi çok kısa, şifresiz başlatılıyor");
            WiFi.softAP(portalName.c_str() + WiFi.macAddress().substring(11, 17));
        }
        else
        {
            WiFi.softAP(portalName.c_str() + WiFi.macAddress().substring(11, 17), PortalPsk.c_str());
        }
        WiFi.softAPConfig(ApIP, ApIP, IPAddress(255, 255, 255, 0));

        statusPortal = PortalOpen_;
        xTaskNotify(bLedTask_, 50, eSetValueWithOverwrite);
        utils.serialWrite("Portal başlatıldı: " + portalName);
        vTaskDelay(1000);
        startServices();

        return statusPortal;
    }

    void stopPortal()
    {
        // Sadece portal servisini durdur, WiFi modunu değiştirme
        WiFi.softAPdisconnect(false);

        statusPortal = PortalClosed_;
        utils.serialWrite("Portal kapatıldı");
    }

    void startServices()
    {
        static bool dnsStarted = false;
        static bool webStarted = false;
        static bool otaStarted = false;
        static bool mdnsStarted = false;

        // DNS Sunucusu - Sadece portal aktifse
        if (statusPortal == PortalOpen_ && !dnsStarted)
        {
            if (DnsServer.start(DNS_PORT, "*", ApIP))
            {
                utils.serialWrite("DNS Sunucusu başlatıldı");
                dnsStarted = true;
            }
            else
            {
                utils.serialWrite("DNS Sunucusu HATA!!!!!");
            }
        }

        // Web Sunucusu - Her durumda
        if (!webStarted)
        {
            WifiWebServer.begin();
            utils.serialWrite("Web sunucu başlatıldı");
            webStarted = true;
        }

        // OTA Servisi - Her durumda
        if (!otaStarted)
        {
            setupOTA();
            utils.serialWrite("OTA servisi başlatıldı");
            otaStarted = true;
        }

        // MDNS Servisi - Sadece WiFi bağlıysa
        if (statusWifi == WifiConnected_ && !mdnsStarted)
        {
            if (MDNS.begin(MdnsName.c_str()))
            {
                MDNS.addService("http", "tcp", 80);
                MDNS.addService("ToioT", "tcp", 80);
                MDNS.addService("wled", "tcp", 80); // WLED hizmeti

                // Cihaz hakkında ek bilgiler
                MDNS.addServiceTxt("ToioT", "tcp", "Project", _VERSION_Project);
                MDNS.addServiceTxt("ToioT", "tcp", "Version", _VERSION_String);
                MDNS.addServiceTxt("ToioT", "tcp", "Hardware", Hardware + " v" + HardwareVer);

                MDNS.addServiceTxt("wled", "tcp", "mac", WiFi.macAddress());



                utils.serialWrite("MDNS servisi başlatıldı");
                mdnsStarted = true;
            }
            else
            {
                utils.serialWrite("MDNS servisi başlatılamadı");
            }
        }

        utils.serialWrite("Tüm servisler kontrol edildi");
    }

    int8_t loadNetworks()
    {
        clearNetworks();

        uint8_t networkCount = 0;
        if (!ssidlist.begin("networkData", true))
        {
            utils.serialWrite("NVS açılamadı!");
            return -1;
        }

        // SSID listesini yükle
        for (uint8_t i = 0; i < MAX_NETWORKS; i++)
        {
            String ssid = ssidlist.getString(("ssid" + String(i)).c_str(), "");
            if (ssid != "")
            {
                networks[networkCount].ssid = ssid;
                networks[networkCount].password = ssidlist.getString(("pass" + String(i)).c_str(), "");
                networkCount++;
            }
            else
            {
                networks[networkCount].ssid = "";
                networks[networkCount].password = "";
                networks[networkCount].lastRssi = -100;
                networks[networkCount].hasFailed = false;
                networks[networkCount].channel = 0;
                networks[networkCount].bssid = "";
            }
        }

        ssidlist.end();
        utils.serialWrite("Ağ listesi yüklendi, " + String(networkCount) + " ağ mevcut");
        return networkCount;
    }

    void clearNetworks()
    {
        for (uint8_t i = 0; i < MAX_NETWORKS; i++)
        {
            networks[i].ssid = "";
            networks[i].password = "";
            networks[i].lastRssi = -100;
            networks[i].hasFailed = false;
            networks[i].channel = 0;
            networks[i].bssid = "";
            networks[i].connected = false;
        }
        utils.serialWrite("Ağ listesi temizlendi");
    }

    // Kayıtlı ağları JSON formatında döndürür (Ajax istekleri için)
    String getNetworksAsJson()
    {
        JsonDocument doc;
        JsonArray savedNetworks = doc["savedNetworks"].to<JsonArray>(); // Kayıtlı ağlar için yeni dizi

        for (uint8_t i = 0; i < MAX_NETWORKS; i++)
        {
            if (networks[i].ssid != "")
            {
                JsonObject network = savedNetworks.add<JsonObject>();
                network["ssid"] = networks[i].ssid;
                network["lastRssi"] = networks[i].lastRssi;
                if (WiFi.status() == WL_CONNECTED && WiFi.BSSIDstr() == networks[i].bssid)
                {
                    int8_t netChannel = WiFi.channel();
                    networks[i].channel = netChannel;
                    network["channel"] = netChannel;
                    network["msg"] = "ch:" + String(netChannel); // kanal numarası
                    networks[i].connected = true;
                    network["connected"] = true;
                }
                else
                {
                    networks[i].connected = false;
                    network["connected"] = false;
                    networks[i].channel = 0;
                    network["channel"] = 0;
                    network["msg"] = "";
                }
            }
        }

        String output;
        serializeJson(doc, output);
        utils.serialWrite("Networks JSON: ", output);
        return output;
    }

    // Wifi ağlarını tara ve JSON olarak döndür
    String scanNetworks()
    {
        // Mevcut WiFi bağlantı durumunu kontrol et ve sakla
        bool wasConnected = (WiFi.status() == WL_CONNECTED); // Şu an bağlı mıyız?
        String connectedSSID = WiFi.SSID();                  // Bağlı olduğumuz ağın adı
        String connectedBSSID = WiFi.BSSIDstr();             // Bağlı olduğumuz ağın MAC adresi
        // WiFi taramasını başlat - Ayarlardaki kanal seçimine göre
        if (wifiChannel > 0)
        {
            // Belirli bir kanalı tara (1-13 arası)
            utils.serialWrite("Wifi taraması başladı - Kanal:", String(wifiChannel));
            // async=true, hidden=false, passive=false, max_ms=300, channel=seçili kanal
            WiFi.scanNetworks(true, false, false, 300, wifiChannel);
        }
        else
        {
            // Tüm kanalları tara (kanal=0 ise)
            utils.serialWrite("Wifi taraması başladı - Tüm kanallar");
            WiFi.scanNetworks(true); // Sadece async=true parametresi ile çağır
        }

        // Tarama tamamlanana kadar bekle
        int16_t networkCount; // Bulunan ağ sayısı
        do
        {
            networkCount = WiFi.scanComplete(); // Tarama durumunu kontrol et
            vTaskDelay(100);                    // 100ms bekle ve tekrar kontrol et
        } while (networkCount < 0); // Tarama bitene kadar döngüde kal

        // JSON yanıtını hazırlamak için
        JsonDocument doc;                                               // JSON belgesi oluştur
        JsonArray foundNetworks = doc["foundNetworks"].to<JsonArray>(); // Bulunan ağlar dizisi

        if (networkCount == 0)
        {
            // Hiç ağ bulunamadıysa
            return "{\"message\":\"Ağ bulunamadı\","
                   "\"notification\":{\"message\":\"Ağ bulunamadı\",\"type\":\"error\"}}";
        }
        else
        {
            // Ağlar bulunduysa
            doc["message"] = String(networkCount) + " ağ bulundu";

            // Her bulunan ağ için bilgileri JSON'a ekle
            for (int i = 0; i < networkCount; ++i)
            {
                JsonObject network = foundNetworks.add<JsonObject>(); // Yeni ağ objesi

                String currentSSID = WiFi.SSID(i);      // Ağın adı
                String currentBSSID = WiFi.BSSIDstr(i); // Ağın MAC adresi
                int8_t currentRSSI = WiFi.RSSI(i);      // Sinyal gücü

                // Ağ bilgilerini JSON'a ekle
                network["ssid"] = currentSSID; // Ağ adı
                network["rssi"] = currentRSSI; // Sinyal gücü

                // Ağın durumunu kontrol et
                bool isSaved = false;     // Kayıtlı ağ mı?
                bool isConnected = false; // Bağlı olduğumuz ağ mı?

                // Bu ağ kayıtlı ağlar listemizde var mı?
                for (uint8_t j = 0; j < MAX_NETWORKS; j++)
                {
                    if (networks[j].ssid.isEmpty() == false && networks[j].ssid == currentSSID)
                    {
                        isSaved = true; // Ağ kayıtlı
                        // Kayıtlı ağın sinyal gücünü güncelle
                        networks[j].lastRssi = currentRSSI;
                        // break;
                    }
                }

                // Bu ağa şu an bağlı mıyız?
                if (wasConnected && currentBSSID == connectedBSSID)
                {
                    isConnected = true;
                }

                // Ağ durumunu JSON'a ekle
                network["msg"] = String(isSaved ? " 💾" : "") + // Kayıtlı ise disk ikonu ekle
                                 String(WiFi.encryptionType(i) != WIFI_AUTH_OPEN ? "🔒" : "") +
                                 "ch:" + String(WiFi.channel(i));

                /* Alternatif semboller (emoji'ler çalışmazsa):
                network["msg"] = String(isSaved ? "⬤ " : "") +  // Kayıtlı ise nokta (⬤) ekle
                                 String(WiFi.channel(i)) + " " +
                                 String(WiFi.encryptionType(i) != WIFI_AUTH_OPEN ? "⚿" : "⚪");  // Kilit yerine ⚿, açık için ⚪
                */

                network["connected"] = isConnected; // Bağlantı durumu
            }

            utils.serialWrite("Bulunan ağ sayısı: ", String(networkCount));
        }

        // Tarama sonuçlarını bellekten temizle
        WiFi.scanDelete();

        // JSON'ı string'e çevir ve gönder
        String jsonResponse;
        serializeJson(doc, jsonResponse);
        utils.serialWrite("scanNetworks JSON: ", jsonResponse);
        return jsonResponse;
    }

    String connectWifi(int8_t index = BestSignal_, String ssid = "", String pass = "")
    {
        // Bağlantı modları:
        // SsidPass_ (-3): Verilen SSID ve şifre ile bağlan
        // FromList_ (-2): Listedeki sıraya göre dene
        // BestSignal_ (-1): En iyi sinyal gücüne sahip ağa bağlan
        // Index0_ (0): Belirtilen indexteki ağa bağlan
        // Index1_ (1): Belirtilen indexteki ağa bağlan
        // Index2_ (2): Belirtilen indexteki ağa bağlan
        // Index3_ (3): Belirtilen indexteki ağa bağlan
        // Index4_ (4): Belirtilen indexteki ağa bağlan

        JsonDocument doc;
        String response;

        utils.serialWrite("  1. connectWifi başlatılıyor index: " + String(index) + " ssid: " + ssid + " pass: " + pass);

        utils.serialWrite("  2. WiFi modunu ayarla");
        // WiFi modunu ayarla
        if (statusPortal == PortalOpen_)
        {
            WiFi.mode(WIFI_AP_STA);
        }
        else
        {
            WiFi.mode(WIFI_STA);
        }

        utils.serialWrite("  3. index istek kontrolü");
        if (index == SsidPass_ && ssid.isEmpty()) // ssid yoksa hata dön
        {
            utils.serialWrite("4. sonuç: Ağ adı boş return");
            doc["message"] = "Ağ adı boş olamaz";
            doc["notification"]["message"] = "Ağ adı boş olamaz";
            doc["notification"]["type"] = "error";
            serializeJson(doc, response);
            return response;
        }
        else if (index == SsidPass_) // ssid ve pass ile bağlan ***************************************************
        {

            // WiFi bağlantısını başlat
            utils.serialWrite("4 ssid ve pass ile bağlanılıyor... SSID: " + ssid + " pass: " + pass);
            tryConnect(ssid, pass);
            JsonDocument scanDoc;
            deserializeJson(scanDoc, scanNetworks()); // ağları tara güncel bilgileri al
            doc["foundNetworks"] = scanDoc["foundNetworks"];
            JsonDocument networksDoc;
            deserializeJson(networksDoc, getNetworksAsJson()); // ağları tara güncel bilgileri al
            doc["savedNetworks"] = networksDoc["savedNetworks"];

            utils.serialWrite("5 Bağlantı sonucuna göre standart mesajları doldur ve değişkenleri güncelle");
            // Bağlantı sonucuna göre
            if (WiFi.status() == WL_CONNECTED)
            {
                utils.serialWrite("6 Bağlantı başarılı");
                doc["message"] = ssid + " ağına bağlanıldı";
                doc["notification"]["message"] = ssid + " ağına bağlanıldı";
                doc["notification"]["type"] = "success";
                doc["info"]["LastIP"] = WiFi.localIP().toString();
                DevConfig.begin("userData", false);
                DevConfig.putString("LastIP", WiFi.localIP().toString());
                DevConfig.end();
            }
            else
            {
                utils.serialWrite("6 Bağlantı başarısız");
                doc["message"] = "Bağlantı başarısız: " + ssid;
                doc["notification"]["message"] = "Bağlantı başarısız: " + ssid;
                doc["notification"]["type"] = "error";
            }

            serializeJson(doc, response);
            return response;
        }
        else if (index == FromList_)
        {
            utils.serialWrite("4. listenin başından başla");

            // Sırayla tüm ağları dene
            for (uint8_t i = 0; i < MAX_NETWORKS; i++)
            {
                // RSSI çok düşükse veya SSID boşsa atla
                if (networks[i].ssid == "")
                {
                    utils.serialWrite("5. index: ", String(i), " boş  continue; ");
                    continue;
                }

                utils.serialWrite("6. Bağlanılıyor: ", "index: ", String(i), " ssid: ", networks[i].ssid, " pass: ", networks[i].password);

                tryConnect(networks[i].ssid, networks[i].password);

                if (WiFi.status() == WL_CONNECTED)
                {
                    utils.serialWrite("7 Bağlantı başarılı");
                    networks[i].bssid = WiFi.BSSIDstr();
                    networks[i].channel = WiFi.channel();
                    networks[i].lastRssi = WiFi.RSSI();
                    networks[i].connected = true;
                    doc["message"] = networks[i].ssid + " ağına bağlanıldı";
                    doc["notification"]["message"] = networks[i].ssid + " ağına bağlanıldı";
                    doc["notification"]["type"] = "success";
                    doc["info"]["LastIP"] = WiFi.localIP().toString();
                    DevConfig.begin("userData", false);
                    DevConfig.putString("LastIP", WiFi.localIP().toString());
                    DevConfig.end();
                    break;
                }
                else
                {
                    utils.serialWrite("7. Bağlantı başarısız");
                    doc["message"] = "Bağlantı başarısız: " + networks[i].ssid;
                    doc["notification"]["message"] = "Bağlantı başarısız: " + networks[i].ssid;
                    doc["notification"]["type"] = "error";
                }
                vTaskDelay(1000);
            }
            JsonDocument networksDoc;
            deserializeJson(networksDoc, getNetworksAsJson()); // ağları tara güncel bilgileri al
            doc["savedNetworks"] = networksDoc["savedNetworks"];
            serializeJson(doc, response);
            return response;
        }
        else if (index == BestSignal_)
        {
            utils.serialWrite("  4. en iyi sinyale sahip ağa bağlan");
            utils.serialWrite("  5. rssi güncellemek için ağları tarama");
            scanNetworks();

            // ağların rssi değerlerini yazdır
            utils.serialWrite("  6. ağların rssi değerleri: ");
            for (uint8_t i = 0; i < MAX_NETWORKS; i++)
            {
                utils.serialWrite("   ", String(i), " ", String(networks[i].lastRssi));
            }

            // Ağları sinyal gücüne göre sırala
            uint8_t sortedIndexes[MAX_NETWORKS] = {0, 1, 2, 3, 4};

            // Sinyal gücüne göre sırala (bubble sort)
            for (uint8_t i = 0; i < MAX_NETWORKS - 1; i++)
            {
                for (uint8_t j = 0; j < MAX_NETWORKS - i - 1; j++)
                {
                    if (networks[sortedIndexes[j]].lastRssi < networks[sortedIndexes[j + 1]].lastRssi)
                    {
                        uint8_t temp = sortedIndexes[j];
                        sortedIndexes[j] = sortedIndexes[j + 1];
                        sortedIndexes[j + 1] = temp;
                    }
                }
            }
            // sıralama sonucu yazdır
            utils.serialWrite("   7.Sıralama sonucu: ");
            for (uint8_t i = 0; i < MAX_NETWORKS; i++)
            {
                utils.serialWrite("   ", String(sortedIndexes[i]), " ", networks[sortedIndexes[i]].ssid, " ", String(networks[sortedIndexes[i]].lastRssi));
            }

            // Sırayla en güçlü sinyalden başlayarak bağlanmayı dene
            for (uint8_t i = 0; i < MAX_NETWORKS; i++)
            {
                uint8_t connectIndex = sortedIndexes[i];

                // SSID boşsa atla
                if (networks[connectIndex].ssid == "")
                {
                    utils.serialWrite("index: ", String(connectIndex), " boş, sonraki ağa geç");
                    continue;
                }

                utils.serialWrite("  8.Bağlanılıyor: index: ", String(connectIndex), " ssid: ", networks[connectIndex].ssid, " pass: ", networks[connectIndex].password);

                tryConnect(networks[connectIndex].ssid, networks[connectIndex].password);

                if (WiFi.status() == WL_CONNECTED)
                {
                    utils.serialWrite("  9. Bağlantı başarılı");
                    networks[connectIndex].bssid = WiFi.BSSIDstr();
                    networks[connectIndex].channel = WiFi.channel();
                    networks[connectIndex].lastRssi = WiFi.RSSI();
                    networks[connectIndex].connected = true;
                    doc["message"] = networks[connectIndex].ssid + " ağına bağlanıldı";
                    doc["notification"]["message"] = networks[connectIndex].ssid + " ağına bağlanıldı";
                    doc["notification"]["type"] = "success";
                    doc["info"]["LastIP"] = WiFi.localIP().toString();
                    DevConfig.begin("userData", false);
                    DevConfig.putString("LastIP", WiFi.localIP().toString());
                    DevConfig.end();
                    break;
                }
                else
                {
                    utils.serialWrite("  10. Bağlantı başarısız");
                    doc["message"] = "Bağlantı başarısız: " + networks[connectIndex].ssid;
                    doc["notification"]["message"] = "Bağlantı başarısız: " + networks[connectIndex].ssid;
                    doc["notification"]["type"] = "error";
                }
                vTaskDelay(1000);
            }
            JsonDocument networksDoc;
            deserializeJson(networksDoc, getNetworksAsJson()); // ağları tara güncel bilgileri al
            doc["savedNetworks"] = networksDoc["savedNetworks"];
            serializeJson(doc, response);
            return response;
        }
        else // index numarasına göre ağa bağlan
        {
            utils.serialWrite("4. indexteki ağa bağlan: " + String(index));
            if (index >= 0 && index < MAX_NETWORKS)
            {
                tryConnect(networks[index].ssid, networks[index].password);

                if (WiFi.status() == WL_CONNECTED)
                {
                    utils.serialWrite("6. Bağlantı başarılı");
                    networks[index].bssid = WiFi.BSSIDstr();
                    networks[index].channel = WiFi.channel();
                    networks[index].lastRssi = WiFi.RSSI();
                    networks[index].connected = true;
                    doc["message"] = networks[index].ssid + " ağına bağlanıldı";
                    doc["notification"]["message"] = networks[index].ssid + " ağına bağlanıldı";
                    doc["notification"]["type"] = "success";
                    doc["info"]["LastIP"] = WiFi.localIP().toString();
                    DevConfig.begin("userData", false);
                    DevConfig.putString("LastIP", WiFi.localIP().toString());
                    DevConfig.end();
                }
                else
                {
                    utils.serialWrite("6. Bağlantı başarısız");
                    doc["message"] = "Bağlantı başarısız: " + networks[index].ssid;
                    doc["notification"]["message"] = "Bağlantı başarısız: " + networks[index].ssid;
                    doc["notification"]["type"] = "error";
                }
            }
            else
            {
                utils.serialWrite("5 index geçersiz, return");

                // kayıtlı ağların listesini al
                String networksJson = getNetworksAsJson();
                JsonDocument networksDoc;
                deserializeJson(networksDoc, networksJson);
                doc["savedNetworks"] = networksDoc["savedNetworks"];
                doc["message"] = "Hata: Geçersiz index";
                doc["notification"]["message"] = "Hata: Geçersiz index";
                doc["notification"]["type"] = "error";
            }
            JsonDocument networksDoc;
            deserializeJson(networksDoc, getNetworksAsJson()); // ağları tara güncel bilgileri al
            doc["savedNetworks"] = networksDoc["savedNetworks"];
            serializeJson(doc, response);
            return response;
        }
    }

    int8_t tryConnect(String ssid, String pass)
    {
        utils.serialWrite("tryConnect başlatılıyor");
        int8_t bLedMode = BLedWait_;
        xQueueSend(bledQueue, &bLedMode, 0);
        wifiDisconnect();
        vTaskDelay(500);

        for (uint8_t i = 0; i < 2; i++) // süreyi kısa tut (WifiMaxTry_) ama 2 kez zorla
        {

            uint8_t tryCount = 0;
            WiFi.begin(ssid.c_str(), pass.c_str());

            // Bağlantı için bekle
            while (tryCount < WifiMaxTry_)
            {
                ledcWrite(BLedChannel_, (tryCount % 2) ? 255 : 0); // Tek sayıda yak, çift sayıda söndür
                tryCount++;
                vTaskDelay(500);
                int8_t wStatus = WiFi.status();
                if (TestMode)
                {
                    Serial.print(String(wStatus));
                }

                if (wStatus == WL_CONNECTED)
                {
                    Serial.println();
                    utils.serialWrite("tryConnect bağlantı başarılı, IP: ", WiFi.localIP().toString());
                    statusWifi = WifiConnected_;
                    xTaskNotify(bLedTask_, 50, eSetValueWithOverwrite);
                    ledcWrite(BLedChannel_, 255); // bağlanınca yanık kalsın
                    lastIP = WiFi.localIP().toString();
                    startServices();
                    vTaskDelay(100);
                    return WifiConnected_;
                }
            }
        }
        Serial.println();
        wifiDisconnect();
        utils.serialWrite("tryConnect bağlantı başarısız");
        return WifiConnFailed_;
    }

    void wifiDisconnect(bool wifioff = false, bool eraseap = false)
    {
        utils.serialWrite("wifiDisconnect");
        WiFi.disconnect(wifioff, eraseap);
        statusWifi = WifiDisconnected_;
        for (uint8_t i = 0; i < MAX_NETWORKS; i++)
        {
            networks[i].connected = false;
        }
    }

    String saveWifi(String ssid, String password)
    {
        utils.serialWrite("saveWifi başlatılıyor", "ssid: ", ssid, "password: ", password);

        JsonDocument doc;
        String response;

        // NVS'yi aç
        if (!ssidlist.begin("networkData", false))
        {
            utils.serialWrite("NVS açılamadı!");
            doc["message"] = "Hata: NVS açılamadı!";
            doc["notification"]["message"] = "Hata: NVS açılamadı!";
            doc["notification"]["type"] = "error";
            serializeJson(doc, response);
            return response;
        }

        // Mevcut ağ bilgilerini geçici olarak sakla
        WifiNetwork tempNetworks[MAX_NETWORKS];
        for (uint8_t i = 0; i < MAX_NETWORKS; i++)
        {
            tempNetworks[i] = networks[i]; // Tüm bilgileri kopyala
        }

        // Yeni ağı ilk sıraya ekle
        networks[0].ssid = ssid;
        networks[0].password = password;
        networks[0].lastRssi = -100;
        networks[0].channel = 0;
        networks[0].bssid = "";
        networks[0].connected = false;
        networks[0].hasFailed = false;

        // Diğer ağları bir sıra kaydır ve bilgilerini koru
        for (uint8_t i = 0; i < MAX_NETWORKS - 1; i++)
        {
            networks[i + 1] = tempNetworks[i];
        }

        // NVS'yi temizle ve yeni listeyi kaydet
        ssidlist.clear();
        for (uint8_t i = 0; i < MAX_NETWORKS; i++)
        {
            if (networks[i].ssid != "")
            {
                ssidlist.putString(("ssid" + String(i)).c_str(), networks[i].ssid);
                ssidlist.putString(("pass" + String(i)).c_str(), networks[i].password);
            }
        }

        ssidlist.end();
        utils.serialWrite("Yeni ağ kaydedildi ve mevcut bilgiler korundu");

        // RSSI değerlerini güncelle
        scanNetworks();

        return getNetworksAsJson();
    }

    String deleteWifi(int8_t index)
    {
        utils.serialWrite("deleteWifi başlatılıyor", "index: ", String(index));

        JsonDocument doc;
        String response;

        // Index kontrolü
        if (index < 0 || index >= MAX_NETWORKS)
        {
            utils.serialWrite("Hata: Geçersiz index");
            doc["message"] = "Hata: Geçersiz index";
            doc["notification"]["message"] = "Hata: Geçersiz index";
            doc["notification"]["type"] = "error";
            serializeJson(doc, response);
            return response;
        }

        // NVS'yi aç
        if (!ssidlist.begin("networkData", false))
        {
            utils.serialWrite("NVS açılamadı!");
            doc["message"] = "Hata: NVS açılamadı!";
            doc["notification"]["message"] = "Hata: NVS açılamadı!";
            doc["notification"]["type"] = "error";
            serializeJson(doc, response);
            return response;
        }

        // Silinecek ağın bilgilerini geçici sakla (log için)
        String deletedSsid = networks[index].ssid;

        // Silinen ağdan sonraki ağları bir üst indexe kaydır
        for (uint8_t i = index; i < MAX_NETWORKS - 1; i++)
        {
            networks[i] = networks[i + 1];
        }

        // Son indexi temizle
        networks[MAX_NETWORKS - 1].ssid = "";
        networks[MAX_NETWORKS - 1].password = "";
        networks[MAX_NETWORKS - 1].lastRssi = -100;
        networks[MAX_NETWORKS - 1].channel = 0;
        networks[MAX_NETWORKS - 1].bssid = "";
        networks[MAX_NETWORKS - 1].connected = false;
        networks[MAX_NETWORKS - 1].hasFailed = false;

        // NVS'yi temizle ve güncel listeyi kaydet
        ssidlist.clear();
        for (uint8_t i = 0; i < MAX_NETWORKS; i++)
        {
            if (networks[i].ssid != "")
            {
                ssidlist.putString(("ssid" + String(i)).c_str(), networks[i].ssid);
                ssidlist.putString(("pass" + String(i)).c_str(), networks[i].password);
            }
        }

        ssidlist.end();
        utils.serialWrite("Ağ silindi: ", deletedSsid);

        // Başarılı mesajı hazırla
        doc["message"] = deletedSsid + " ağı silindi";
        doc["notification"]["message"] = deletedSsid + " ağı silindi";
        doc["notification"]["type"] = "success";

        // Güncel ağ listesini al
        JsonDocument networksDoc;
        deserializeJson(networksDoc, getNetworksAsJson());
        doc["savedNetworks"] = networksDoc["savedNetworks"];

        serializeJson(doc, response);
        return response;
    }

    String updateWifi(int8_t index, String ssid, String password)
    {
        utils.serialWrite("updateWifi başlatılıyor", "index: ", String(index), "ssid: ", ssid, "password: ", password);

        JsonDocument doc;
        String response;

        // Index kontrolü
        if (index < 0 || index >= MAX_NETWORKS)
        {
            utils.serialWrite("Hata: Geçersiz index");
            doc["message"] = "Hata: Geçersiz index";
            doc["notification"]["message"] = "Hata: Geçersiz index";
            doc["notification"]["type"] = "error";
            serializeJson(doc, response);
            return response;
        }

        // SSID kontrolü
        if (ssid.isEmpty())
        {
            utils.serialWrite("Hata: SSID boş olamaz");
            doc["message"] = "Hata: SSID boş olamaz";
            doc["notification"]["message"] = "Hata: SSID boş olamaz";
            doc["notification"]["type"] = "error";
            serializeJson(doc, response);
            return response;
        }

        // NVS'yi aç
        if (!ssidlist.begin("networkData", false))
        {
            utils.serialWrite("NVS açılamadı!");
            doc["message"] = "Hata: NVS açılamadı!";
            doc["notification"]["message"] = "Hata: NVS açılamadı!";
            doc["notification"]["type"] = "error";
            serializeJson(doc, response);
            return response;
        }

        // Eski bilgileri geçici sakla
        String oldSsid = networks[index].ssid;

        // Ağ bilgilerini güncelle (RSSI ve diğer bilgileri koru)
        networks[index].ssid = ssid;
        networks[index].password = password;
        networks[index].hasFailed = false; // Yeni şifre ile başarısız sayısını sıfırla

        // NVS'yi güncelle
        ssidlist.putString(("ssid" + String(index)).c_str(), ssid);
        ssidlist.putString(("pass" + String(index)).c_str(), password);
        ssidlist.end();

        utils.serialWrite("Ağ güncellendi: ", oldSsid, " -> ", ssid);

        // Başarılı mesajı hazırla
        doc["message"] = oldSsid + " ağı güncellendi";
        doc["notification"]["message"] = oldSsid + " ağı güncellendi";
        doc["notification"]["type"] = "success";

        // Güncel ağ listesini al
        JsonDocument networksDoc;
        deserializeJson(networksDoc, getNetworksAsJson());
        doc["savedNetworks"] = networksDoc["savedNetworks"];

        serializeJson(doc, response);
        return response;
    }
};

#endif