
#include "string.h"
#include "stdio.h"

#include "site_functions.h"

bool addSite(SiteInfo* siteInfo, char* siteName, char* latitude, char* longitude, char* utcOffset, int siteNumber) {
    if (siteNumber < 0 || siteNumber >= MAX_SITES) {
        return false; // Invalid site number
    }

    // Copy site name
    strncpy(siteInfo[siteNumber].sitename, siteName, sizeof(siteInfo[siteNumber].sitename) - 1);
    siteInfo[siteNumber].sitename[sizeof(siteInfo[siteNumber].sitename) - 1] = '\0'; // Ensure null-termination

    // Parse and set latitude
    if (!parse_location(latitude, &siteInfo[siteNumber].latitude, MAX_LATITUDE)) {
        return false; // Invalid latitude format
    }

    // Parse and set longitude
    if (!parse_location(longitude, &siteInfo[siteNumber].longitude, MAX_LONGITUDE)) {
        return false; // Invalid longitude format
    }

    // Parse and set UTC offset
    if (!parse_offset(utcOffset, &siteInfo[siteNumber].utc_offset)) {
        return false; // Invalid UTC offset format
    }

    return true; // Successfully added the site
};
