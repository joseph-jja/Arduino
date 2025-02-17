// configuration
#include "Globals.h"

float* altAzToHADec(float lat, float alt, float az) {
    
    float H = atan2(-sin(az), tan(alt) * cos(lat) - cos(az) * sin(lat));
    if (H < 0) {
        H += M_PI * 2;
    }
    float dec = asin(sin(lat) * sin(alt) + cos(lat) * cos(alt) * cos(az));

    float results[2];
    results[0] = H;
    results[1] = dec;
    
    return results;
}
/*
float earthRotationAngle(int jd) {
    //IERS Technical Note No. 32

    int t = jd - 2451545.0;
    int f = jd % 1.0;

    float theta = 2 * M_PI * (f + 0.7790572732640 + 0.00273781191135448 * t); //eq 14
    //theta = 2 * M_PI % theta;
    if (theta < 0) {
        theta += 2 * M_PI;
    }

    return theta;

}
*/
/*

function greenwichMeanSiderealTime(jd) {
    //"Expressions for IAU 2000 precession quantities" N. Capitaine1,P.T.Wallace2, and J. Chapront
    const t = ((jd - 2451545.0)) / 36525.0;

    let gmst = this.earthRotationAngle(jd) + (0.014506 + 4612.156534 * t + 1.3915817 * t * t - 0.00000044 * t * t * t - 0.000029956 * t * t * t * t - 0.0000000368 * t * t * t * t * t) / 60.0 / 60.0 * Math.PI / 180.0; //eq 42
    gmst %= 2 * Math.PI;
    if (gmst < 0) gmst += 2 * Math.PI;

    return gmst;
}

function hourAngleToRa(hourAngle, lon, jd_ut) {

    const gmst = greenwichMeanSiderealTime(jd_ut);
    let localSiderealTime = (gmst + lon) % (2 * Math.PI);

    
    //let H=(localSiderealTime - ra);
    //if(H<0){H+=2*Math.PI;}
    //if(H>Math.PI){H=H-2*Math.PI;}
    
    const ra = H + localSiderealTime;
    return ra;
}
*/
