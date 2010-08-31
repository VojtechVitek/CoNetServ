#pragma once
#ifndef _IDENTIFIER_H_
#define _IDENTIFIER_H_

#include "npapi.h"
#include "init_modules.h"

typedef struct _identifier {
   NPIdentifier   read;
   NPIdentifier   start;
   NPIdentifier   stop;

   NPIdentifier   running;
   NPIdentifier   version;
   NPIdentifier   found;

#ifdef MODULE_PING
   NPIdentifier   ping;
#endif

#ifdef MODULE_PING6
   NPIdentifier   ping6;
#endif

#if defined(MODULE_PING) || defined(MODULE_PING6)
   NPIdentifier   count;
   NPIdentifier   timeout;
   NPIdentifier   packetsize;
   NPIdentifier   ttl;
#endif

#ifdef MODULE_TRACEROUTE
   NPIdentifier   tracert;
   NPIdentifier   traceroute;
#endif

#ifdef MODULE_TRACEROUTE6
   NPIdentifier   tracert6;
   NPIdentifier   traceroute6;
#endif

#if defined(MODULE_TRACEROUTE) || defined(MODULE_TRACEROUTE6)
   NPIdentifier   maxhops;
   NPIdentifier   waittime;
   NPIdentifier   iptohostname;
#endif

#ifdef MODULE_NSLOOKUP
   NPIdentifier   nslookup;
   NPIdentifier   query;
#endif

#ifdef MODULE_WHOIS
   NPIdentifier   whois;
#endif

#ifdef MODULE_DIG
   NPIdentifier   dig;
#endif

#ifdef MODULE_NMAP
   NPIdentifier   nmap;
#endif

   void           (*destroy)();
} identifier;

bool init_identifiers();

extern identifier *identifiers;

#endif /*_IDENTIFIER_H_*/
