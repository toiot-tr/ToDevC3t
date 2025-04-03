#include "header.h"

const char Portal_Html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="tr">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Ledfly Akıllı Aydınlatma</title>
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
        .sayfaBaslik {
            display: flex;
            flex-direction: row;
            justify-content: space-between;
            padding: 10px;
        }
        .sayfaGovde {
            box-shadow: 0 2px 8px 0 rgba(0, 0, 0, 0.2), 0px 5px 10px 0 rgba(0, 0, 0, 0.19);
            border-radius: 10px;
            height: 40px;
            overflow: hidden;
            transition: height 500ms;
            margin-bottom: 10px;
            margin-top: 10px;
            border: 1px solid #ef4b25;
        }
        .sayfa {
            display: flex;
            flex-direction: column;
            align-items: center;
            height: 600px;
        }
        .ayarsayfa {
            height: 100%;
            width: 100%;
            margin-bottom: 5px;
            margin-top: 5px;
            overflow-y: auto;
            display: flex;
            flex-direction: row;
            flex-wrap: wrap;
            justify-content: center;
            align-items: center;
        }
        .arrow {
            width: 30px;
            height: 30px;
            display: flex;
            transition: transform 500ms;
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
        .verigosteccs {
            width: 80px;
            height: 30px;
            min-width: 70px;
            outline: 0;
            border-width: 2px 0 2px;
            border-color: grey;
            background-color: transparent;
            text-align: center;
            justify-content: center;
            font-size: 18px;
            font-weight: 800;
        }
        .gucbar {
            margin-top: 5px;
            margin-bottom: 5px;
            border-radius: 50px;
            height: 40px;
            width: 80%;
            background-color: var(--bg-secondary);
            border: 1px solid var(--border-color);
            justify-content: center;
        }
        .slider {
            -webkit-appearance: none;
            appearance: none;
            width: 95%;
            height: 20px;
            background-color: transparent;
            opacity: 0.7;
            -webkit-transition: .2s;
            transition: opacity .2s;
            margin-top: 10px;
            border: none;
        }
        .slider::-webkit-slider-thumb {
            -webkit-appearance: none;
            appearance: none;
            width: 30px;
            height: 60px;
            background-color: var(--text-primary);
            cursor: pointer;
            border-radius: 15px;
            border: none;
        }
    </style>
</head>
<body onload="yuklendi()">
    <div id="spinnerid" class="spinnerCss">
        <div class="loading-spinner"></div>
    </div>
    <div class="main" id="ustdivid">
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
    <div class="main" id="mesajdivid" style="display: none;">
    </div>
    <div class="main" id="mainDivid" style="display: none;">
        <div class="sayfaGovde">
            <div class="sayfaBaslik">
                <div>İnfo</div>
                <svg class="arrow"></svg>
            </div>
            <div class="sayfa">
                <div class="ayarsayfa" id="infoSayfaid">
                    <div class="formGrup">
                        <form action="home.html" method="GET"
                            onsubmit="document.getElementById('gBtnid').innerHTML='Yükleniyor...'"><button id="gBtnid"
                                type="submit" style="height: 40px; width: 200px;">Giriş</button></form>
                        </form>
                    </div>
                </div>
            </div>
        </div>
        <div class="formGrup">
        </div>
        <div>
            <div class="formGrup">
                <input class="verigosteccs" disabled id="setVeriid">
                <input class="verigosteccs" disabled id="gucVeriid" type="button">
                <input class="verigosteccs" disabled id="sensVeriid">
            </div>
        </div>
        <div class="formGrup">
        </div>
        <div>
            <div id="gucBarDivid">
                <div class="formGrup">
                    <div class="gucbar">
                        <input class="slider" id="gucBarid" type="range" min="0" max="100" step="5"
                            onchange="gucGonder()">
                    </div>
                </div>
            </div>
        </div>
        <div class="formGrup">
        </div>
        <div class="formGrup">
            <button class="buton" style="background-color: orange;" onclick="komutbtn('reset')">Reset</button>
            <button class="buton" style="background-color: rgb(0, 255, 213);" onclick="veriiste()">Yenile</button>
        </div>
    </div>
<script>
    let mesajDiv = document.getElementById("mesajdivid");
    let mainContent = document.getElementById('mainDivid');
    let spinner = document.getElementById('spinnerid');
    let setVeri = document.getElementById('setVeriid');
    let gucVeri = document.getElementById('gucVeriid');
    let sensVeri = document.getElementById('sensVeriid');
    let gucBar = document.getElementById('gucBarid');
    let okicon = `<svg version="1.1" viewBox="1 0 18 20" style="width: 30px ; height: 30px;" id="XMLID_225_">
<line x1="0" y1="5" x2="10.75" y2="15" stroke="black" stroke-width="2" />
<line x1="9.32" y1="15" x2="20" y2="5" stroke="black" stroke-width="2" />
</svg>`;
    let arrowElements = document.querySelectorAll('.arrow');
    arrowElements.forEach((arrowElement) => {
        arrowElement.innerHTML = okicon;
    });
    const sayfaBasliklar = document.querySelectorAll('.sayfaBaslik');
    const sayfaGovdeler = document.querySelectorAll('.sayfaGovde');
    const sayfaArrow = document.querySelectorAll('.arrow');
    sayfaBasliklar.forEach((sayfaBaslik, index) => {
        sayfaBaslik.addEventListener('click', () => {
            const currentHeight = parseInt(sayfaGovdeler[index].style.height);
            sayfaGovdeler.forEach((sayfaGovde, idx) => {
                if (idx === index) {
                    if (currentHeight === 40) {
                        sayfaGovde.style.height = '650px';
                        sayfaArrow[idx].style.transform = 'rotate(180deg)';
                    } else {
                        sayfaGovde.style.height = '40px';
                        sayfaArrow[idx].style.transform = 'rotate(0deg)';
                    }
                } else {
                    sayfaGovde.style.height = '40px';
                    sayfaArrow[idx].style.transform = 'rotate(0deg)';
                }
            });
        });
    });
    function formGrupOlustur(etiketMetni, selectEnabled, visible, hedefId, bileşenTipi = 'sel', secenekler = []) {
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
            formGirisDiv.appendChild(input);
        } else if (bileşenTipi === 'num') {
            const input = document.createElement('input');
            input.type = 'number';
            input.className = 'veri';
            input.disabled = !selectEnabled;
            formGirisDiv.appendChild(input);
        } else if (bileşenTipi === 'info') {
            const input = document.createElement('input');
            input.type = 'text';
            input.className = 'infoo';
            input.disabled = true;
            formGirisDiv.appendChild(input);
        }
        formGrupDiv.appendChild(formGirisDiv);
        const hedefDiv = document.getElementById(hedefId);
        hedefDiv.classList.add('sayfa');
        hedefDiv.appendChild(formGrupDiv);
    }
    function formuBaslat() {
        let sNo = 'infoSayfaid';
        formGrupOlustur('Cihaz Modeli', 1, 1, sNo, 'info');
        formGrupOlustur('Donanım', 1, 1, sNo, 'info');
        formGrupOlustur('Yazılım', 1, 1, sNo, 'info');
        formGrupOlustur('Yükleme Zamanı', 1, 1, sNo, 'info');
        formGrupOlustur('Mac', 1, 1, sNo, 'info');
        formGrupOlustur('Zaman', 1, 1, sNo, 'info');
        formGrupOlustur('AKIM', 1, 1, sNo, 'info');
        formGrupOlustur('300V', 1, 1, sNo, 'info');
        formGrupOlustur('24V', 1, 1, sNo, 'info');
        formGrupOlustur('NTC', 1, 1, sNo, 'info');
        formGrupOlustur('Gönderi (ok/fail)', 1, 1, sNo, 'info');
        formGrupOlustur('Gelen Mesajlar', 1, 1, sNo, 'info');
    }
    function SpinnerGoster() {
        spinner.style.display = 'flex';
        mainContent.classList.add('blur');
    }
    function SpinnerGizle() {
        spinner.style.display = 'none';
        mainContent.classList.remove('blur');
    }
    function yuklendi() {
        if (window.innerWidth < 340) {
            document.body.style.zoom = "0.6";
        } else if (window.innerWidth < 400) {
            document.body.style.zoom = "0.8";
        }
        mainContent.style.display = 'block';
        SpinnerGoster();
        formuBaslat();
        setTimeout(function () {
            let xmlhttp = new XMLHttpRequest();
            xmlhttp.timeout = 15000;
            xmlhttp.ontimeout = function () {
                location.reload();
            };
            xmlhttp.onreadystatechange = function () {
                if (this.readyState == 4) {
                    if (this.status == 200) {
                        SpinnerGizle();
                        let jsonData = JSON.parse(this.responseText);
                        const selectInfo = document.querySelectorAll('.infoo');
                        selectInfo.forEach((selectInfo, index) => {
                            if (jsonData.infoo && jsonData.infoo[index] !== undefined) {
                                selectInfo.value = jsonData.infoo[index];
                            }
                        });
                        setVeri.value = jsonData.SetVeri;
                        gucVeri.value = jsonData.GucVeri;
                        sensVeri.value = jsonData.SensVeri;
                        gucBar.value = jsonData.GucBar;
                        setInterval(veriiste, 30000);
                    } else {
                        SpinnerGizle();
                        return;
                        mainContent.style.display = 'none';
                        mesajDiv.style.display = 'flex';
                        mesajDiv.innerHTML = "Yanıt alınamadı sayfa yeniden yükleniyor..";
                        setTimeout(function () {
                            location.reload();
                        }, 5000);
                    }
                }
            };
            xmlhttp.open("OKU", "/portalyuklendi.yukle");
            xmlhttp.send();
        }, 500);
    }
    function komutbtn(hangiBtn) {
        mainContent.style.display = 'none';
        mesajDiv.style.display = 'flex';
        mesajDiv.innerHTML = "Komut işleniyor..";
        SpinnerGoster();
        window.scrollTo(0, 0);
        let veri;
        if (hangiBtn == "reset") {
            veri = "reset.buton";
        } else if (hangiBtn == "sifirla") {
            veri = "sifirla.buton";
        }
        let xmlhttp = new XMLHttpRequest();
        xmlhttp.timeout = 10000;
        xmlhttp.ontimeout = function () {
            location.reload();
        };
        xmlhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                SpinnerGizle();
                mesajDiv.innerHTML = this.responseText;
            }
        };
        xmlhttp.open("BTN", veri);
        xmlhttp.send();
    }
    function gucGonder() {
        let json = JSON.stringify({
            ["GucBar"]: gucBar.value
        });
        let xmlhttp = new XMLHttpRequest();
        xmlhttp.timeout = 10000;
        xmlhttp.ontimeout = function () {
            location.reload();
        };
        xmlhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
            }
            else if (this.readyState == 4 && this.status == 400) {
                mainContent.style.display = 'none';
                mesajDiv.style.display = 'flex';
                mesajDiv.innerHTML = this.responseText;
                location.reload();
            }
        };
        xmlhttp.open('SET', '/gucbar');
        xmlhttp.send("<<json|" + json + "|json>>");
    }
    function veriiste() {
        let xmlhttp = new XMLHttpRequest();
        xmlhttp.timeout = 10000;
        xmlhttp.ontimeout = function () {
        };
        xmlhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                let jsonData = JSON.parse(this.responseText);
                const selectInfo = document.querySelectorAll('.infoo');
                selectInfo.forEach((selectInfo, index) => {
                    selectInfo.value = jsonData.infoo[index];
                });
                setVeri.value = jsonData.SetVeri;
                gucVeri.value = jsonData.GucVeri;
                sensVeri.value = jsonData.SensVeri;
                gucBar.value = jsonData.GucBar;
            }
        };
        xmlhttp.open("OKU", "/portalyenile.yenile");
        xmlhttp.send();
    }
    /* Tema yönetimi */
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
    /* Bildirim gösterme fonksiyonu */
    function showNotification(message, type = 'info', duration = 3000) {
        const notification = document.createElement('div');
        notification.className = `notification ${type}`;
        notification.textContent = message;
        document.body.appendChild(notification);
        setTimeout(() => notification.classList.add('show'), 10);
        setTimeout(() => {
            notification.classList.remove('show');
            setTimeout(() => notification.remove(), 300);
        }, duration);
    }
    /* Status frame'e log ekleme fonksiyonu */
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
)rawliteral";