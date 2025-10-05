#pragma once

static const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>Update Time</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body {
     font-family: Helvetica, Times, Arial;
     margin: 0px auto;
    }
  </style>
</head>
<body>
  <h1>Update Time</h1>
  <script type="text/javascript">
    document.addEventListener('DOMContentLoaded', () => {
      const now = new Date(), start = new Date(now.getFullYear(), 0, 0);
      const doty = Math.floor((now - start) / 86400000);
      const dateStamp = `year=${now.getFullYear()}&month=${now.getMonth() + 1}&day=${now.getDate()}`;
      const timeStamp = `hour=${now.getHours()}&minutes=${now.getMinutes() + 1}&seconds=${now.getSeconds()}`;
      fetch(`/update?${dateStamp}&${timeStamp}&dotw=${now.getDay()}&doty=${doty}`).then(resp => {
        alert('time updated');
      }).catch(e => {
        alert('error time not updated');
      });
    });
  </script>
</body>
</html>)rawliteral";

