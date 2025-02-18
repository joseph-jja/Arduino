const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body {
     font-family: Helvetica, Times, Arial;
     margin: 0px auto;
    }
    p { font-size: 1.5rem; }
    .labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>Motion Server</h2>
  <p>
    <span class="labels">Temperature</span> 
    <br><span id="temperature-data"></span>
  </p>
  <p>
    <span class="labels">GPS</span> 
    <br><span id="GPS-data"></span>
  </p>
  <p>
    <span class="labels">Motion</span> 
    <br><span id="motion-data"></span>    
  </p>
  <script type="text/html">
  window.addEventListener('DOMContentLoaded', async () => {
     const response = await fetch('/update');
     const buff = await response.text();
     const data = JSON.parse(buff);
     const gps = document.getElementById('GPS-data');
     if (gps) { gps.innerHTML = JSON.stringify(data.gps); }
     const motion = document.getElementById('motion-data');
     if (motion) { motion.innerHTML = JSON.stringify(data.Acc) + " " + JSON.stringify(data.Gyro); }
     const temperature = document.getElementById('temperature-data');
     if (temperature) { temperature.innerHTML = JSON.stringify(data.degC) + " " + JSON.stringify(degF); }
  });
  </script>
</body>
</html>)rawliteral";
