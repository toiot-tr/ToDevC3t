# ToDevC3tf Başlangıç Rehberi

## İçindekiler
1. [Gereksinimler](#gereksinimler)
2. [Kurulum](#kurulum)
3. [Firmware Yükleme](#firmware-yükleme)
4. [İlk Çalıştırma](#i̇lk-çalıştırma)
5. [Sorun Giderme](#sorun-giderme)

## Gereksinimler
- Visual Studio Code
- PlatformIO IDE Eklentisi
- USB Type-A Bağlantı Noktası
- 6V-28V Güç Kaynağı (opsiyonel)

## Kurulum

### PlatformIO IDE Kurulumu
1. [Visual Studio Code](https://code.visualstudio.com/)'u indirin ve kurun
2. VS Code'u açın ve Extensions (Ctrl+Shift+X) menüsüne gidin
3. "PlatformIO IDE" eklentisini arayın ve kurun
4. VS Code'u yeniden başlatın

### Proje Kurulumu
1. PlatformIO ana sayfasından "Open Project" seçin
2. ToDevC3tf firmware klasörünü seçin
3. Proje açıldığında gerekli bağımlılıklar otomatik olarak yüklenecektir

## Firmware Yükleme

### USB Bağlantısı
1. USB adaptörü PCB'ye takın
2. USB kablosunu bilgisayara bağlayın
3. Cihaz otomatik olarak tanınacaktır

### Yükleme İşlemi
1. VS Code'da PlatformIO menüsünü açın
2. "Upload" butonuna tıklayın
3. Derleme ve yükleme işlemi otomatik olarak başlayacaktır

## İlk Çalıştırma

### WiFi Bağlantısı
1. Cihaz ilk açıldığında AP modunda başlar
2. WiFi ağlarından "ToDevC3tf" ağını bulun
3. Şifre: "12345678"
4. Bağlandıktan sonra 192.168.4.1 adresine gidin

### LED Test
1. Durum LED'i (IO5) yanıp sönecektir
2. Kontrol butonu (IO6) ile LED'i kontrol edebilirsiniz
3. Web arayüzünden RGB LED'leri test edebilirsiniz

## Sorun Giderme

### USB Bağlantı Sorunları
- USB adaptörünün doğru yönde takıldığından emin olun
- Farklı bir USB portu deneyin
- Sürücülerin güncel olduğunu kontrol edin

### Versiyon Sorunları
Eğer USB boot sorunu yaşarsanız:
1. `platformio.ini` dosyasındaki alternatif USB ayarlarını deneyin
2. Framework versiyonunu test edilmiş versiyona (2.0.9) sabitleyin
3. Yorum satırlarındaki açıklamaları takip edin

### Yardım ve Destek
- [GitHub Issues](https://github.com/kullaniciadi/ToDevC3tf/issues)
- [Discussions](https://github.com/kullaniciadi/ToDevC3tf/discussions)

## Güncelleme Geçmişi
- v1.0.0 - İlk sürüm
  - ESP32 Arduino framework 2.0.9
  - PlatformIO Core 6.3.2
