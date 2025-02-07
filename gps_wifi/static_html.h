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
    h2 { font-size: 1.5em; }
    p { font-size: 1.5rem; }
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
    <span id="temperature">%.2f</span>
    <sup class="units">&deg;C</sup>
    <span id="temperature">%.2f</span>
    <sup class="units">&deg;F</sup>
  </p>
  <p>
    <span class="dht-labels">GPS</span> 
    <span id="temperature">Latitude: %.2f \tLongitude: %.2f</span>
    <span id="temperature">Time: %2d:%2d</span>
  </p>
  <p>
    <span class="dht-labels">Motion</span> 
    <span id="temperature">Acceleration X: %.2f / Y: %.2f / Z: %.2f</span>
    <span id="temperature">Rotation X: %.2f / Y: %.2f / Z: %.2f</span>
  </p>
</body>
</html>)rawliteral";
