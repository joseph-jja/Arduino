
#include "string.h"
#include "stdio.h"

#include "site_functions.h"
#include "location_functions.h"

bool addSite(SiteInfo* siteInfo, char* siteName, char* latitude, char* longitude, char* utcOffset, int siteNumber) {

    if (siteName == nullptr || latitude == nullptr || longitude == nullptr || utcOffset == nullptr) {
        return false;
    }
    
    if (siteNumber < 0 || siteNumber >= MAX_SITES) {
        return false; // Invalid site number
    }

    // Parse into temporaries so we don't partially modify siteInfo on failure
    Location latTmp;
    Location longTmp;
    Offset utcTmp;

    // Parse and validate latitude
    if (!parse_location((const char*)latitude, &latTmp, MAX_LATITUDE)) {
        return false; // Invalid latitude format
    }

    // Parse and validate longitude
    if (!parse_location((const char*)longitude, &longTmp, MAX_LONGITUDE)) {
        return false; // Invalid longitude format
    }

    // Parse and validate UTC offset
    if (!parse_offset((const char*)utcOffset, &utcTmp)) {
        return false; // Invalid UTC offset format
    }

    // All parsing succeeded — now commit to the siteInfo struct

    // Copy site name safely
    // use snprintf to guarantee null termination
    snprintf(siteInfo[siteNumber].sitename, sizeof(siteInfo[siteNumber].sitename), "%s", siteName);

    // Copy parsed latitude/longitude into the struct, ensuring null-termination
    //strncpy(siteInfo[siteNumber].latitude, latTmp, sizeof(siteInfo[siteNumber].latitude) - 1);
    //siteInfo[siteNumber].latitude[sizeof(siteInfo[siteNumber].latitude) - 1] = '\0';

    //strncpy(siteInfo[siteNumber].longitude, lonTmp, sizeof(siteInfo[siteNumber].longitude) - 1);
    //siteInfo[siteNumber].longitude[sizeof(siteInfo[siteNumber].longitude) - 1] = '\0';

    //siteInfo[siteNumber].utc_offset = utcTmp;

    return true; // Successfully added the site
}
