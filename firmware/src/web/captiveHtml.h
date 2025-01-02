#include "header.h"
const char Captive_Html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="tr">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width,initial-scale=1.0">
    <title>ToioT</title>
    <style>
        .spinner {
            position: fixed;
            width: 100%;
            height: 100%;
            display: flex;
            justify-content: center;
            align-items: center;
        }
        .spinnerSvg {
            width: 200px;
            height: 200px;
            animation: spinnerRotate 3s infinite linear;
        }
        @keyframes spinnerRotate {
            from {
                transform: rotate(0deg);
            }
            to {
                transform: rotate(360deg);
            }
        }
    </style>
</head>
<body>
    <div id="spinnerid" class="spinner">
        <svg class="spinnerSvg" version="1.1" viewBox="0 0 20 20">
            <path d="M7,2.8 A9,11,0,0,1,15,4" fill="none" stroke="green" stroke-width="2" />
            <path d="M17.7,11 A8,8,0,0,1,11,17.7" fill="none" stroke="red" stroke-width="2" />
            <path d="M3,13.4 A6,7,0,0,1,3,6.8" fill="none" stroke="blue" stroke-width="2" />
        </svg>
    </div>
    <script>
        setTimeout(function () {
            window.location.href = "toiot.html";
        }, 200);
    </script>
</body>
</html>
)rawliteral";