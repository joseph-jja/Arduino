#pragma once

#include "Location.h"

typedef struct {
    char sitename[128]; 
    SiteLocation siteLocation;
} SiteInfo;

bool addSite(char* siteName, char* latitude, char* longitude, char* utcOffset, int siteNumber);
