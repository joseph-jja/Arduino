#pragma once

#include "Location.h"

typedef struct {
    char sitename[128]; 
    SiteLocation siteLocation;
} SiteInfo;

class Site {
public: 

    void init();

    SiteInfo siteInfo[4];

    bool addSite(char* siteName, char* latitude, char* longitude, char* utcOffset, int siteNumber);

};

extern Site site;