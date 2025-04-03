#include "header.h"

const char Wifi_Html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="tr">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ToioT</title>
    <link rel="icon" sizes="any" href="data:image/svg+xml,
        %3Csvg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 94 48'%3E
            %3Cpath fill='%23003366' d='M0 0h36v7h-36z'/%3E
            %3Cpath fill='%23003366' d='M16 0h8v50h-8z'/%3E
            %3Ccircle cx='36' cy='20' r='8' fill='none' stroke='%23003366' stroke-width='5'/%3E
            %3Cpath fill='%2366CCFF' d='M48 6h6v6h-6z'/%3E
            %3Cpath fill='%2366CCFF' d='M48 14h6v16h-6z'/%3E
            %3Ccircle cx='66' cy='20' r='8' fill='none' stroke='%2366CCFF' stroke-width='5'/%3E
            %3Cpath fill='%2366CCFF' d='M74 6h20v5h-20z'/%3E
            %3Cpath fill='%2366CCFF' d='M82 10h6v20h-6z'/%3E
        %3C/svg%3E">
    <style>
        :root {
            --bg-primary: #f6f8fa;
            --bg-secondary: #e9ecef;
            --bg-hover: #dee2e6;
            --text-primary: #24292e;
            --text-secondary: #586069;
            --border-color: #d1d5da;
            --shadow-color: rgba(0, 0, 0, 0.15);
            --status-success: #90EE90;
            --status-error: #ffcccb;
            --status-pending: #c8c803;
        }
        [data-theme="dark"] {
            --bg-primary: #191f27;
            --bg-secondary: #1b2634;
            --bg-hover: #2d3239;
            --text-primary: antiquewhite;
            --text-secondary: #8b949e;
            --border-color: #30363d;
            --shadow-color: rgba(0, 0, 0, 0.5);
            --status-success: #1a472a;
            --status-error: #5c1e1e;
            --status-pending: #919101;
        }
        body {
            color: var(--text-primary);
            background-color: var(--bg-primary);
            font-family: 'Arial';
            font-size: 18px;
            font-weight: 400;
            margin-top: 50px;
            max-width: 400px;
            margin: auto;
        }
        ::-webkit-scrollbar {
            display: none;
        }
        .spinnerCss {
            position: fixed;
            top: 0;
            left: 0;
            width: 100%;
            height: 100%;
            display: none;
            justify-content: center;
            align-items: center;
            z-index: 1;
            background-color: rgba(0, 0, 0, 0.2);
        }
        .loading-spinner {
            position: absolute;
            top: 50%;
            left: 50%;
            transform: translate(-50%, -50%);
            width: 80px;
            height: 80px;
            border: 8px solid var(--bg-secondary);
            border-top: 8px solid var(--text-primary);
            border-radius: 50%;
            animation: spin 0.8s linear infinite;
        }
        @keyframes spin {
            0% {
                transform: translate(-50%, -50%) rotate(0deg);
            }
            100% {
                transform: translate(-50%, -50%) rotate(360deg);
            }
        }
        .blur {
            filter: blur(5px);
        }
        .ustbar {
            width: 100%;
            display: flex;
            justify-content: space-between;
            align-items: center;
        }
        .ustbar .statusframe {
            width: 80%;
            background: var(--bg-secondary);
            border: 1px solid var(--border-color);
            border-radius: 10px 20px 10px 20px;
            margin-right: 15px;
            margin-left: 15px;
            display: flex;
            flex-direction: column;
            justify-content: flex-start;
            align-items: flex-start;
            box-shadow: 0 4px 12px var(--shadow-color);
            height: auto;
            min-height: 40px;
            max-height: 60px;
            padding: 8px;
            overflow: auto;
        }
        .log-entry {
            width: 100%;
            padding: 2px 5px;
            margin: 2px 0;
            font-size: 0.65em;
            opacity: 0.9;
            transition: opacity 0.3s;
        }
        .log-entry:last-child {
            opacity: 1;
            font-weight: 500;
        }
        .ustbar .themebuton {
            width: 20%;
            display: flex;
            justify-content: flex-end;
        }
        .ustbar .themebuton button {
            min-width: unset;
            width: 40px;
            height: 40px;
            font-size: 20px;
            border-radius: 10px 20px 10px 20px;
            background: var(--bg-secondary);
            border: 1px solid var(--border-color);
            cursor: pointer;
            box-shadow: 0 4px 12px var(--shadow-color);
            transition: transform 0.2s;
            display: flex;
            align-items: center;
            justify-content: center;
        }
        .ustbar .themebuton button:hover {
            transform: scale(1.1);
        }
        .signal-container {
            display: flex;
            flex-direction: column;
            justify-content: flex-end;
            align-items: center;
            min-width: 50px;
            height: 50px;
            margin-right: 4px;
        }
        .signal-status {
            min-width: 20px;
            margin: 3px;
            margin-top: 5px;
            font-size: 0.60em;
            color: #888;
            line-height: 1;
            max-width: 50px;
            overflow: hidden;
            text-overflow: ellipsis;
            white-space: nowrap;
        }
        .wifi-signal {
            width: 24px;
            height: 16px;
            position: relative;
            margin-left: 3px;
        }
        .wifi-signal .bar {
            width: 3px;
            position: absolute;
            bottom: 0;
            background-color: var(--text-secondary);
            border-radius: 1px;
            transition: height 0.2s;
        }
        .wifi-signal .bar-1 {
            left: 00px;
            height: 4px;
        }
        .wifi-signal .bar-2 {
            left: 5px;
            height: 8px;
        }
        .wifi-signal .bar-3 {
            left: 10px;
            height: 12px;
        }
        .wifi-signal .bar-4 {
            left: 15px;
            height: 16px;
        }
        .bar-check {
            position: absolute;
            left: 25px;
            bottom: 0;
            height: 16px;
            font-size: 12px;
            color: var(--text-secondary);
        }
        .signal-weak .bar-2,
        .signal-weak .bar-3,
        .signal-weak .bar-4 {
            opacity: 0.2;
        }
        .signal-fair .bar-3,
        .signal-fair .bar-4 {
            opacity: 0.2;
        }
        .signal-good .bar-4 {
            opacity: 0.2;
        }
        .signal-full .bar {
            opacity: 1;
        }
        .notification {
            position: fixed;
            top: 20px;
            left: 50%;
            transform: translateX(-50%);
            background: var(--bg-secondary);
            color: var(--text-primary);
            padding: 12px 24px;
            border-radius: 8px;
            box-shadow: 0 2px 8px var(--shadow-color);
            z-index: 1000;
            opacity: 0;
            transition: opacity 0.3s;
            max-width: 80%;
            text-align: center;
            border: 1px solid var(--border-color);
            margin: 0 auto;
        }
        .notification.show {
            opacity: 1;
        }
        .notification.success {
            border-left: 4px solid #28a745;
        }
        .notification.error {
            border-left: 4px solid #dc3545;
        }
        .notification.info {
            border-left: 4px solid #17a2b8;
        }
        button {
            width: 80%;
            min-width: 100px;
            height: 50px;
            margin: 10px;
            align-items: center;
            cursor: pointer;
            font-size: 18px;
            font-weight: 700;
            text-align: center;
            border-radius: 10px;
            background-color: aquamarine;
            color: black;
            border: none;
        }
        .buton:hover {
            transform: scale(1.05);
        }
        .btnMid {
            height: 40px;
            min-width: 50px;
            font-size: 12px;
            font-weight: normal;
        }
        .btnB {
            background-color: aqua;
            color: black;
        }
        .btnG {
            background-color: greenyellow;
            color: black;
        }
        .btnY {
            background-color: yellow;
            color: black;
        }
        .btnO {
            background-color: orange;
            color: white;
        }
        .btnR {
            background-color: #ff4444;
            color: white;
        }
        .main {
            width: 100%;
            margin: auto;
            display: flex;
            flex-direction: column;
            text-align: center;
            margin-top: 10px;
            align-items: center;
        }
        hr {
            width: 200px;
            opacity: 0, 4;
        }
        .sayfa {
            background-color: var(--bg-primary);
            border-radius: 15px;
            box-shadow: 0 4px 12px var(--shadow-color);
            width: 100%;
            height: 100%;
            margin-bottom: 25px;
            margin-top: 25px;
            flex-direction: column;
            align-items: center;
        }
        .sayfaLabel {
            width: 100%;
            height: 30px;
            display: flex;
            justify-content: center;
            align-items: center;
            font-size: 18px;
            font-weight: 600;
        }
        label {
            width: 100%;
            margin: auto;
            margin-left: 2%;
            margin-right: 2%;
            display: flex;
            border-radius: 10px;
            font-size: 18px;
            font-weight: 400;
        }
        .formGrup {
            width: 90%;
            margin: auto;
            height: 50px;
            display: flex;
            flex-direction: row;
            justify-content: space-evenly;
            align-items: center;
            background-color: transparent;
        }
        .formIsim {
            width: 55%;
            display: flex;
            flex-direction: row;
            justify-content: left;
        }
        .formGiris {
            width: 45%;
            display: flex;
            flex-direction: row;
            justify-content: right;
        }
        input,
        select {
            width: 100%;
            margin: auto;
            margin-left: 2%;
            margin-right: 2%;
            text-align: center;
            border-radius: 10px;
            background-color: var(--bg-secondary);
            font-size: 18px;
            font-weight: 400;
            color: var(--text-primary);
            border: 1px solid var(--border-color);
        }
        select {
            text-align-last: center;
            display: flex;
        }
        input:disabled {
            background-color: var(--bg-hover);
            color: var(--text-secondary);
            opacity: 0.7;
        }
        input.veri {
            background-color: var(--bg-primary);
            border: 1px solid var(--border-color);
            color: var(--text-primary);
        }
        .veri {
            border: none;
        }
        .infoo {
            min-width: 200px;
            height: 35px;
            pointer-events: none;
            background-color: #eee;
            font-size: 16px;
            font-weight: 600;
            border: 1px solid grey;
        }
        .wifiListesi {
            width: 80%;
            margin: auto;
            max-height: 300px;
            overflow-y: auto;
            margin-top: 10px;
        }
        .wifiEleman {
            position: relative;
            overflow: hidden;
            background-color: var(--bg-secondary);
            color: var(--text-primary);
            border-radius: 12px;
            margin: 8px 0;
            cursor: pointer;
            transition: all 0.15s ease;
        }
        .wifiEleman:active {
            transform: scale(0.98);
            background-color: var(--bg-hover);
        }
        .wifiEleman::after {
            content: '';
            position: absolute;
            width: 100%;
            height: 100%;
            top: 0;
            left: 0;
            pointer-events: none;
            background-image: radial-gradient(circle, var(--text-secondary) 10%, transparent 10.01%);
            background-repeat: no-repeat;
            background-position: 50%;
            transform: scale(10, 10);
            opacity: 0;
            transition: transform .3s, opacity .5s;
        }
        .wifiEleman:active::after {
            transform: scale(0, 0);
            opacity: 0.3;
            transition: 0s;
        }
        .wifiTitle {
            display: flex;
            min-height: 50px;
            flex-direction: row;
            align-items: center;
        }
        .wifi-name {
            flex: 1;
            white-space: nowrap;
            overflow: hidden;
            text-overflow: ellipsis;
        }
        .wifi-index {
            min-width: 25px;
            text-align: center;
            opacity: 0.7;
        }
        [data-theme="dark"] .wifiEleman:hover {
            background-color: #4d4d4d;
        }
        .kayitli-ag {
            position: relative;
            background-color: var(--bg-secondary);
            color: var(--text-primary);
            border-radius: 12px;
            margin: 8px 0;
            cursor: pointer;
            transition: all 0.3s ease;
        }
        .kayitli-ag .ag-detay {
            display: none;
        }
        .kayitli-ag.active .ag-detay {
            display: block;
        }
        .ag-input {
            width: 90%;
            margin: 3px 0;
            font-size: 14px;
            height: 32px;
            border: 1px solid var(--border-color);
            border-radius: 8px;
            background: var(--bg-primary);
            color: var(--text-primary);
        }
        .contNetBtn {
            display: flex;
        }
    </style>
</head>
<body onload="loadData()">
    <div id="spinnerid" class="spinnerCss">
        <div class="loading-spinner"></div>
    </div>
    <div class="main">
        <div class="ustbar">
            <div class="signal-container">
                <div class="wifi-signal">
                    <div class="bar bar-1"></div>
                    <div class="bar bar-2"></div>
                    <div class="bar bar-3"></div>
                    <div class="bar bar-4"></div>
                    <div id="portalIcon" class="bar-check" style="display: none;">🅿️</div>
                </div>
                <div class="signal-status" id="currentSsid"></div>
            </div>
            <div class="statusframe">
            </div>
            <div class="themebuton">
                <button onclick="toggleTheme()">🌓</button>
            </div>
        </div>
    </div>
    <div class="main" id="mesajdivid" style="display: none; margin-top: 150px;">
    </div>
    <div class="main" id="mainDivid" style="display: none;">
        <div class="formGrup">
            <form action="home.html" method="GET"
                onsubmit="document.getElementById('gBtnid').innerHTML='Yükleniyor...'">
                <button id="gBtnid" type="submit">Giriş</button>
            </form>
            </form>
            <form action="portal.html" method="GET"
                onsubmit="document.getElementById('pBtnid').innerHTML='Yükleniyor...'">
                <button id="pBtnid" type="submit">Portal</button>
            </form>
        </div>
        <br>
        <div class="formGrup">
            <small>
                • * işaretli ayarlar resetten sonra aktif olur<br>
                • ** işaretli ayarlar resetten önce kısmen aktif olur<br>
            </small>
        </div>
        <div class="sayfa">
            <div class="formGrup">
                <label class="sayfaLabel">**Kayıtlı Ağlar</label>
            </div>
            <div id="kayitliAglar" class="wifiListesi">
            </div>
        </div>
        <div class="formGrup">
            <small>
                • Sadece 2.4Ghz WiFi ağları görüntülenir<br>
                • Tüm kanalları taramak için WiFi kanalı = "Oto"<br>
            </small>
        </div>
        <div class="sayfa">
            <div class="formGrup">
                <label class="sayfaLabel">Ağ Ekleme</label>
            </div>
            <div class="formGrup">
                <label style="width: 30%;">Ağ (SSID)</label>
                <input style="width: 70%;" type="text" id="scanSsidInput">
            </div>
            <div class="formGrup">
                <label style="width: 30%;">Şifre (PSK)</label>
                <input style="width: 70%;" type="text" id="scanPskInput">
            </div>
            <div id="wifiListesi" class="wifiListesi">
            </div>
            <div class="formGrup">
            </div>
            <div class="formGrup">
                <button class="btnB btnMid" onclick="komutbtn('wifiScan')">Ağ Ara</button>
                <button class="btnG btnMid" onclick="komutbtn('scanConnect')">Bağlan</button>
                <button class="btnY btnMid" onclick="komutbtn('wifiDisconnect')">Kopar</button>
                <button class="btnO btnMid" onclick="komutbtn('scanSave')">Kaydet</button>
            </div>
        </div>
        <div class="formGrup">
        </div>
        <div class="formGrup">
            <button class="btnY" onclick="komutbtn('reset')">Reset</button>
            <button class="btnB" onclick="veriiste()">Yenile</button>
        </div>
        <div class="formGrup">
        </div>
    </div>
    <script>
        /* --------- Temel Değişkenler --------- */
        const mesajDiv = document.getElementById("mesajdivid");
        const mainContent = document.getElementById('mainDivid');
        const spinner = document.getElementById('spinnerid');
        const wifiSignal = document.querySelector('.wifi-signal');
        const portalIcon = document.getElementById('portalIcon');
        const currentSsid = document.getElementById('currentSsid');
        const scanPskInput = document.getElementById('scanPskInput');
        const scanSsidInput = document.getElementById('scanSsidInput');
        let ayarlar = {};
        let infoo = {};
        let savedNetworks = [];
        /* --------- Tema Yönetimi --------- */
        if (!window.matchMedia || !window.matchMedia('(prefers-color-scheme: light)').matches) {
            document.documentElement.setAttribute('data-theme', 'dark');
        }
        function toggleTheme() {
            const currentTheme = document.documentElement.getAttribute('data-theme');
            const newTheme = currentTheme === 'dark' ? 'light' : 'dark';
            document.documentElement.setAttribute('data-theme', newTheme);
            localStorage.setItem('theme', newTheme);
        }
        document.addEventListener('DOMContentLoaded', () => {
            const savedTheme = localStorage.getItem('theme');
            if (savedTheme) {
                document.documentElement.setAttribute('data-theme', savedTheme);
            }
        });
        function getSignalClass(rssi) {
            if (rssi >= -50) return 'signal-full';
            if (rssi >= -65) return 'signal-good';
            if (rssi >= -75) return 'signal-fair';
            return 'signal-weak';
        }
        /* --------- Spinner İşlemleri --------- */
        function SpinnerGoster() {
            spinner.style.display = 'flex';
            mainContent.classList.add('blur');
        }
        function SpinnerGizle() {
            spinner.style.display = 'none';
            mainContent.classList.remove('blur');
        }
        /* --------- Sayfa Yükleme ve Form İşlemleri --------- */
        function loadData() {
            if (window.innerWidth < 340) {
                document.body.style.zoom = "0.6";
            }
            else if (window.innerWidth < 400) {
                document.body.style.zoom = "0.8";
            }
            formuBaslat();
            ayrGonderInit();
            if (Object.keys(ayarlar).length === 0) {
                console.log('Test ortamında çalışıyor...');
                ayarlar.WifiMode = 2;
                ayarlar.PortalMode = 2;
                ayarlar.PortalPsk = "test ortamı";
                ayarlar.WifiChan = 1;
                ayarlar.WifiPower = 2;
                infoo.LastIP = "localhost";
                infoo.WifiStatus = true;
                infoo.Rssi = -65;
                infoo.CurrentSSID = "Test_WiFi_1";
                infoo.PortalStatus = true;
                savedNetworks.push({ ssid: "Test_WiFi_1", lastRssi: -65, msg: "ch:1", connected: true });
                savedNetworks.push({ ssid: "Test_WiFi_2", lastRssi: -75, msg: "ch:6", connected: false });
                wifiListesiniGoster(savedNetworks);
            }
            const selectAyarlar = document.querySelectorAll('.veri');
            selectAyarlar.forEach(element => {
                const ayarAdi = element.dataset.ayarAdi;
                if (ayarAdi && ayarlar[ayarAdi] !== undefined) {
                    element.value = ayarlar[ayarAdi];
                }
            });
            document.querySelectorAll('.infoo').forEach(element => {
                const infoAdi = element.dataset.ayarAdi;
                element.value = infoo[infoAdi] || '';
            });
            if (infoo.WifiStatus) {
                const signalClass = getSignalClass(infoo.Rssi);
                wifiSignal.className = `wifi-signal ${signalClass}`;
                currentSsid.textContent = infoo.CurrentSSID;
            }
            portalIcon.style.display = infoo.PortalStatus ? 'block' : 'none';
            kayitliAglariGoster(savedNetworks);
            mainContent.style.display = 'block';
            setInterval(veriiste, 60000);
        }
        async function veriiste() {
            try {
                const response = await fetchWithTimeout('/wifi.html', {
                    method: 'DAT',
                    timeout: 5000
                });
                const jsonData = await response.json();
                if (jsonData.info) {
                    const infoElements = document.querySelectorAll('.infoo');
                    infoElements.forEach(element => {
                        const infoAdi = element.dataset.ayarAdi;
                        if (jsonData.info[infoAdi]) {
                            element.value = jsonData.info[infoAdi];
                        }
                    });
                    if (jsonData.info.WifiStatus) {
                        const signalClass = getSignalClass(jsonData.info.Rssi);
                        wifiSignal.className = `wifi-signal ${signalClass}`;
                        currentSsid.textContent = jsonData.info.CurrentSSID;
                    } else {
                        wifiSignal.className = 'wifi-signal signal-weak';
                        currentSsid.textContent = 'Bağlı değil';
                    }
                    portalIcon.style.display = jsonData.info.PortalStatus ? 'block' : 'none';
                }
                if (jsonData.notification) {
                    showNotification(jsonData.notification.message, jsonData.notification.type);
                }
                if (jsonData.message) {
                    addStatusLog(jsonData.message);
                }
                if (jsonData.savedNetworks) {
                    kayitliAglariGoster(jsonData.savedNetworks);
                }
            } catch (error) {
            }
        }
        /* --------- ESP32 İletişim Fonksiyonları --------- */
        async function fetchWithTimeout(resource, options = {}) {
            const { timeout = 8000 } = options;
            const controller = new AbortController();
            const id = setTimeout(() => controller.abort(), timeout);
            try {
                const response = await fetch(resource, {
                    ...options,
                    signal: controller.signal
                });
                clearTimeout(id);
                return response;
            } catch (error) {
                clearTimeout(id);
                if (error.name === 'AbortError') {
                    throw new Error('Bağlantı zaman aşımına uğradı');
                }
                throw error;
            }
        }
        function showNotification(message, type = 'info', duration = 3000) {
            const notification = document.createElement('div');
            notification.className = `notification ${type}`;
            notification.textContent = message;
            document.body.appendChild(notification);
            setTimeout(() => {
                notification.classList.add('show');
            }, 10);
            setTimeout(() => {
                notification.classList.remove('show');
                setTimeout(() => {
                    notification.remove();
                }, 300);
            }, duration);
        }
        function addStatusLog(message) {
            const statusFrame = document.querySelector('.statusframe');
            const logEntry = document.createElement('div');
            logEntry.className = 'log-entry';
            logEntry.textContent = message;
            statusFrame.appendChild(logEntry);
            statusFrame.scrollTop = statusFrame.scrollHeight;
            while (statusFrame.children.length > 5) {
                statusFrame.removeChild(statusFrame.firstChild);
            }
        }
        /* --------- Sayfa Ekleme ve Form Oluşturma --------- */
        function sayfaEkle(baslikMetni, sayfaNumarasi) {
            const sayfaId = 'sayfa' + sayfaNumarasi;
            const sayfaDiv = document.createElement('div');
            sayfaDiv.id = sayfaId;
            sayfaDiv.classList.add('sayfa');
            const formGrupDiv = document.createElement('div');
            formGrupDiv.className = 'formGrup';
            const etiket = document.createElement('label');
            etiket.className = 'sayfaLabel';
            etiket.textContent = baslikMetni;
            formGrupDiv.appendChild(etiket);
            sayfaDiv.appendChild(formGrupDiv);
            const mainDiv = document.getElementById('mainDivid');
            const hedefChild = mainDiv.children[sayfaNumarasi];
            mainDiv.insertBefore(sayfaDiv, hedefChild);
        }
        function formGrupOlustur(etiketMetni, ayarAdi = '', selectEnabled, visible, sayfaNumarasi, bileşenTipi = 'sel', secenekler = []) {
            const formGrupDiv = document.createElement('div');
            formGrupDiv.className = 'formGrup';
            formGrupDiv.style.display = visible ? 'flex' : 'none';
            const formIsimDiv = document.createElement('div');
            formIsimDiv.className = 'formIsim';
            const etiket = document.createElement('label');
            etiket.textContent = etiketMetni;
            formIsimDiv.appendChild(etiket);
            formGrupDiv.appendChild(formIsimDiv);
            const formGirisDiv = document.createElement('div');
            formGirisDiv.className = 'formGiris';
            if (bileşenTipi === 'sel') {
                secenekler.forEach(secenekList => {
                    const select = document.createElement('select');
                    select.className = 'veri';
                    select.disabled = !selectEnabled;
                    select.dataset.ayarAdi = ayarAdi;
                    secenekList.forEach(secenek => {
                        const opt = document.createElement('option');
                        opt.value = secenek[0];
                        opt.textContent = secenek[1];
                        opt.disabled = secenek[2] === 0;
                        select.appendChild(opt);
                    });
                    formGirisDiv.appendChild(select);
                });
            } else if (bileşenTipi === 'txt') {
                const input = document.createElement('input');
                input.type = 'text';
                input.className = 'veri';
                input.disabled = !selectEnabled;
                input.dataset.ayarAdi = ayarAdi;
                formGirisDiv.appendChild(input);
            } else if (bileşenTipi === 'num') {
                const input = document.createElement('input');
                input.type = 'number';
                input.className = 'veri';
                input.disabled = !selectEnabled;
                input.dataset.ayarAdi = ayarAdi;
                formGirisDiv.appendChild(input);
            } else if (bileşenTipi === 'info') {
                const input = document.createElement('input');
                input.type = 'text';
                input.className = 'infoo';
                input.disabled = true;
                input.dataset.ayarAdi = ayarAdi;
                formGirisDiv.appendChild(input);
            }
            formGrupDiv.appendChild(formGirisDiv);
            const hedefDiv = document.getElementById(`sayfa${sayfaNumarasi}`);
            hedefDiv.classList.add('sayfa');
            hedefDiv.appendChild(formGrupDiv);
        }
        function formuBaslat() {
            let sNo = 2;
            sNo++;
            sayfaEkle('Cihaz Ayarları', sNo)
            formGrupOlustur('*Wifi Ağ Bağlantısı', 'WifiMode', 1, 1, sNo, 'sel', [
                [['0', 'Kapalı', 1],
                ['1', 'Açık', 1],
                ['2', 'Saat Güncelle', 1]]
            ]);
            formGrupOlustur('*Portal', 'PortalMode', 1, 1, sNo, 'sel', [
                [['0', 'Kapalı', 1],
                ['1', 'Açık', 1],
                ['2', 'Wifi yoksa aç', 1]]
            ]);
            formGrupOlustur('*Portal Şifresi', 'PortalPsk', 1, 1, sNo, 'txt');
            formGrupOlustur('Wifi Sinyal Gücü', 'WifiPower', 1, 1, sNo, 'sel', [
                [['1', 'Minimum', 1],
                ['2', 'Düşük', 1],
                ['3', 'Orta', 1],
                ['4', 'Yüksek', 1],
                ['5', 'Maksimum', 1]]
            ]);
            formGrupOlustur('**WiFi Kanalı', 'WifiChan', 1, 1, sNo, 'sel', [
                [['0', 'Oto', 1],
                ['1', '1', 1],
                ['2', '2', 1],
                ['3', '3', 1],
                ['4', '4', 1],
                ['5', '5', 1],
                ['6', '6', 1],
                ['7', '7', 1],
                ['8', '8', 1],
                ['9', '9', 1],
                ['10', '10', 1],
                ['11', '11', 1],
                ['12', '12', 1],
                ['13', '13', 1]]
            ]);
            formGrupOlustur('Son Alınan Ip', 'LastIP', 1, 1, sNo, 'info');
            sNo++;
        }
        async function guncelleAyar(selectElement) {
            selectElement.style.backgroundColor = 'var(--status-pending)';
            try {
                const response = await fetchWithTimeout(
                    `/${selectElement.dataset.ayarAdi}=${selectElement.value}`,
                    {
                        method: 'SET',
                        timeout: 10000
                    }
                );
                const responseData = await response.json();
                selectElement.value = responseData.cevap;
                if (responseData.durum === 'ok') {
                    selectElement.style.backgroundColor = 'var(--status-success)';
                } else {
                    selectElement.style.backgroundColor = 'var(--status-error)';
                }
            } catch (error) {
                selectElement.style.backgroundColor = 'red';
                mainContent.style.display = 'none';
                mesajDiv.style.display = 'flex';
                mesajDiv.style.fontSize = '20px';
                mesajDiv.style.color = '#ff3333';
                mesajDiv.innerHTML = "İşlemci yanıt vermedi, sayfa yeniden yükleniyor...";
                setTimeout(() => {
                    location.reload();
                }, 5000);
            }
        }
        function ayrGonderInit() {
            const selectElements = document.querySelectorAll('.veri');
            selectElements.forEach(selectElement => {
                selectElement.addEventListener('change', () => guncelleAyar(selectElement));
            });
        }
        async function komutbtn(komut, data = {}) {
            let timeout = 8000;
            let requestData = {
                ssid: '',
                password: '',
                index: -1
            };
            switch (komut) {
                case 'wifiScan': {
                    timeout = 60000;
                    requestData.action = 'scan';
                    break;
                }
                case 'scanConnect': {
                    timeout = 60000;
                    if (!scanSsidInput.value) {
                        showNotification('Lütfen bir ağ seçin', 'error');
                        return;
                    }
                    requestData.ssid = scanSsidInput.value;
                    requestData.password = scanPskInput.value;
                    break;
                }
                case 'scanSave': {
                    if (!scanSsidInput.value) {
                        showNotification('Lütfen bir ağ seçin', 'error');
                        return;
                    }
                    timeout = 60000;
                    requestData.ssid = scanSsidInput.value;
                    requestData.password = scanPskInput.value;
                    break;
                }
                case 'wifiConnect': {
                    timeout = 20000;
                    requestData.index = data.index;
                    break;
                }
                case 'wifiDisconnect': {
                    timeout = 10000;
                    break;
                }
                case 'wifiUpdate': {
                    requestData.ssid = data.ssid;
                    requestData.password = data.password;
                    requestData.index = data.index;
                    break;
                }
                case 'wifiDelete': {
                    requestData.index = data.index;
                    break;
                }
            }
            try {
                SpinnerGoster();
                const response = await fetchWithTimeout("/" + komut + ".btn", {
                    method: 'BTN',
                    headers: { 'Content-Type': 'application/json' },
                    body: "<<json|" + JSON.stringify(requestData) + "|json>>",
                    timeout: timeout
                });
                const responseData = await response.json();
                if (responseData.foundNetworks) {
                    wifiListesiniGoster(responseData.foundNetworks);
                }
                if (responseData.savedNetworks) {
                    kayitliAglariGoster(responseData.savedNetworks);
                }
                if (responseData.info) {
                    const infoElements = document.querySelectorAll('.infoo');
                    infoElements.forEach(element => {
                        const infoAdi = element.dataset.ayarAdi;
                        if (responseData.info[infoAdi]) {
                            element.value = responseData.info[infoAdi];
                        }
                    });
                    if (jsonData.info.WifiStatus) {
                        const signalClass = getSignalClass(jsonData.info.Rssi);
                        wifiSignal.className = `wifi-signal ${signalClass}`;
                        currentSsid.textContent = jsonData.info.CurrentSSID;
                    } else {
                        wifiSignal.className = 'wifi-signal signal-weak';
                        currentSsid.textContent = 'Bağlı değil';
                    }
                    portalIcon.style.display = jsonData.info.PortalStatus ? 'block' : 'none';
                }
                if (responseData.message) {
                    addStatusLog(responseData.message);
                }
                if (responseData.notification) {
                    showNotification(responseData.notification.message, responseData.notification.type);
                }
                SpinnerGizle();
            } catch (error) {
                SpinnerGizle();
                showNotification(error.message, 'error');
                console.error('Hata:', error);
                setTimeout(() => {
                    window.location.reload();
                }, 2000);
            }
        }
        /* --------- WiFi İşlemleri --------- */
        function createWifiTitleHTML(ag, index, signalClass) {
            return `
                <div class="wifiTitle">
                    <span class="wifi-index">${index + 1}</span>
                    <div class="wifi-name">${ag.ssid}</div>
                    <div class="signal-container">
                        <div class="wifi-signal ${signalClass}">
                            <div class="bar bar-1"></div>
                            <div class="bar bar-2"></div>
                            <div class="bar bar-3"></div>
                            <div class="bar bar-4"></div>
                            ${ag.connected ? '<div class="bar-check">✓</div>' : ''}
                        </div>
                        <div class="signal-status">${ag.msg || ''}</div>
                    </div>
                </div>
            `;
        }
        function kayitliAglariGoster(aglar) {
            const container = document.getElementById('kayitliAglar');
            container.innerHTML = '';
            aglar.forEach((ag, index) => {
                const signalClass = getSignalClass(ag.lastRssi);
                const agDiv = document.createElement('div');
                agDiv.className = 'kayitli-ag';
                agDiv.innerHTML = `
                    ${createWifiTitleHTML(ag, index, signalClass)}
                    <div class="ag-detay">
                        <input type="text" class="ag-input" value="${ag.ssid}" placeholder="Ağ Adı">
                        <input type="text" class="ag-input" placeholder="Yeni Şifre">
                        <div class="contNetBtn">
                            ${ag.connected ?
                        `<button class="btnMid btnY" onclick="komutbtn('wifiDisconnect')">Kopar</button>` :
                        `<button class="btnMid btnG" onclick="komutbtn('wifiConnect', {index:${index}})">Bağlan</button>`
                    }
                            <button class="btnMid btnO" onclick="
                                const detay = this.closest('.ag-detay');
                                komutbtn('wifiUpdate', {
                                    index: ${index},
                                    ssid: detay.querySelector('input[type=\'text\']:first-child').value,
                                    password: detay.querySelector('input[type=\'text\']:last-child').value
                                });
                            ">Güncelle</button>
                            <button class="btnMid btnR" onclick="komutbtn('wifiDelete', {index:${index}})">Sil</button>
                        </div>
                    </div>
                `;
                agDiv.querySelector('.wifiTitle').addEventListener('click', () => {
                    document.querySelectorAll('.kayitli-ag').forEach(el => {
                        if (el !== agDiv) el.classList.remove('active');
                    });
                    agDiv.classList.toggle('active');
                });
                container.appendChild(agDiv);
            });
        }
        function wifiListesiniGoster(networks) {
            const wifiListesi = document.getElementById('wifiListesi');
            wifiListesi.innerHTML = '';
            networks.forEach((ag, index) => {
                const signalClass = getSignalClass(ag.rssi);
                const wifiEleman = document.createElement('div');
                wifiEleman.classList.add('wifiEleman');
                wifiEleman.innerHTML = createWifiTitleHTML(ag, index, signalClass);
                wifiEleman.addEventListener('click', () => {
                    scanSsidInput.value = ag.ssid;
                    scanPskInput.value = '';
                    scanPskInput.focus();
                });
                wifiListesi.appendChild(wifiEleman);
            });
        }
)rawliteral";