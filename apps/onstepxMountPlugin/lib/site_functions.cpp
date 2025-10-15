
#include "string.h"
#include "stdio.h"

#include "Site.h"

bool addSite(char* siteName, char* latitude, char* longitude, char* utcOffset, int siteNumber) {
    if (siteNumber < 0 || siteNumber >= 4) {
        return false; // Invalid site number
    }

    // Copy site name
    strncpy(siteInfo[siteNumber].sitename, siteName, sizeof(siteInfo[siteNumber].sitename) - 1);
    siteInfo[siteNumber].sitename[sizeof(siteInfo[siteNumber].sitename) - 1] = '\0'; // Ensure null-termination

    // Parse and set latitude
    if (!siteLocation.parse_location(latitude, &siteInfo[siteNumber].siteLocation.latitude, MAX_LATITUDE)) {
        return false; // Invalid latitude format
    }

    // Parse and set longitude
    if (!siteLocation.parse_location(longitude, &siteInfo[siteNumber].siteLocation.longitude, MAX_LONGITUDE)) {
        return false; // Invalid longitude format
    }

    // Parse and set UTC offset
    if (!siteLocation.parse_offset(utcOffset, &siteInfo[siteNumber].siteLocation.utc_offset)) {
        return false; // Invalid UTC offset format
    }

    return true; // Successfully added the site
};
