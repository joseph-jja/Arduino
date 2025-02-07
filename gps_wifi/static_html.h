const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .dht-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>ESP8266 DHT Server</h2>
  <p>
    <span class="dht-labels">Temperature</span> 
    <span id="temperature">%TEMPERATURE_CELCIUS%</span>
    <sup class="units">&deg;C</sup>
    <span id="temperature">%TEMPERATURE_FARENHEIGHT%</span>
    <sup class="units">&deg;F</sup>
  </p>
  <p>
    <span class="dht-labels">GPS</span> 
    <span id="temperature">%GPS_DATA%</span>
    <sup class="units">&deg;C</sup>
  </p>
  <p>
    <span class="dht-labels">Motion</span> 
    <span id="temperature">%MOTION_DATA%</span>
    <sup class="units">&deg;C</sup>
  </p>
</body>
</html>)rawliteral";
