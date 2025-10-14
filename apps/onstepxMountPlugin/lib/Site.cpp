#include "string.h"
#include "stdio.h"

#include "Site.h"

void Site::init() {
    // Initialize all site entries to empty
    for (int i = 0; i < 4; i++) {
        siteInfo[i].sitename[0] = '\0';
        sprintf(siteInfo[i].sitename, "Site %d", i + 1);
        siteInfo[i].siteLocation.latitude.sign = '+';
        siteInfo[i].siteLocation.latitude.hours = 0;
        siteInfo[i].siteLocation.latitude.minutes = 0;
        siteInfo[i].siteLocation.longitude.sign = '+';
        siteInfo[i].siteLocation.longitude.hours = 0;
        siteInfo[i].siteLocation.longitude.minutes = 0;
        siteInfo[i].siteLocation.utc_offset.sign = '+';
        siteInfo[i].siteLocation.utc_offset.hours = 0;
    }
};

bool Site::addSite(char* siteName, char* latitude, char* longitude, char* utcOffset, int siteNumber) {
    if (siteNumber < 0 || siteNumber >= 4) {
        return false; // Invalid site number
    }

    // Copy site name
    strncpy(siteInfo[siteNumber].sitename, siteName, sizeof(siteInfo[siteNumber].sitename) - 1);
    siteInfo[siteNumber].sitename[sizeof(siteInfo[siteNumber].sitename) - 1] = '\0'; // Ensure null-termination

    // Parse and set latitude
    if (!siteLocation.parse_location(latitude, &siteInfo[siteNumber].siteLocation.latitude)) {
        return false; // Invalid latitude format
    }

    // Parse and set longitude
    if (!siteLocation.parse_location(longitude, &siteInfo[siteNumber].siteLocation.longitude)) {
        return false; // Invalid longitude format
    }

    // Parse and set UTC offset
    if (!siteLocation.parse_offset(utcOffset, &siteInfo[siteNumber].siteLocation.utc_offset)) {
        return false; // Invalid UTC offset format
    }

    return true; // Successfully added the site
};

Site site;