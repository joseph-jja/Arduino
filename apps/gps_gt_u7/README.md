# GPS using TinyGPSPlus

## Tested with GT-U7 GPS and RYS352A

## This is basically a short hand version that writes a lot to serial print

## Usage
* You can pull this into a project and then call setup_gps() in your setup function
* Then call loop_gps() to poll the GPS data
* Then call GPS_LOCATION myGPS = getGPSData(); to get the actual data