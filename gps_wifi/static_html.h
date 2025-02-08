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
  async function fetcher( url, options ) {
    const response = await fetch( url, options );
    const buff = await response.text();
    return buff;
  }
  </script>
</body>
</html>)rawliteral";
