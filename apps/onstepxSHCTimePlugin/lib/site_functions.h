#pragma once

#include "location_functions.h"

#define SITE_NAME_LENGTH 50
#define MAX_SITES 4

typedef struct {
    char sitename[SITE_NAME_LENGTH]; 
    Location latitude;
    Location longitude;
    Offset utc_offset;
} SiteInfo;

bool addSite(SiteInfo* siteInfo, char* siteName, char* latitude, char* longitude, char* utcOffset, int siteNumber);
