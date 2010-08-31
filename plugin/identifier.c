#include <stdio.h>

#include "debug.h"
#include "npapi.h"
#include "identifier.h"

identifier *identifiers = NULL;

void
destroy()
{
   DEBUG_STR("identifiers->destroy()");
   browser->memfree(identifiers);
}

bool
init_identifiers()
{
   DEBUG_STR("identifiers->init()");

   if ((identifiers = browser->memalloc(sizeof(*identifiers))) == NULL)
      return false;

   identifiers->read = browser->getstringidentifier("read");
   identifiers->running = browser->getstringidentifier("running");
   identifiers->start = browser->getstringidentifier("start");
   identifiers->stop = browser->getstringidentifier("stop");
   identifiers->version = browser->getstringidentifier("version");
   identifiers->found = browser->getstringidentifier("found");

#ifdef MODULE_PING
   identifiers->ping = browser->getstringidentifier("ping");
#endif

#ifdef MODULE_PING6
   identifiers->ping6 = browser->getstringidentifier("ping6");
#endif

#if defined(MODULE_PING) || defined(MODULE_PING6)
   identifiers->count = browser->getstringidentifier("count");
   identifiers->timeout = browser->getstringidentifier("timeout");
   identifiers->packetsize = browser->getstringidentifier("packetsize");
   identifiers->ttl = browser->getstringidentifier("ttl");
#endif

#ifdef MODULE_TRACEROUTE
   identifiers->tracert = browser->getstringidentifier("tracert");
   identifiers->traceroute = browser->getstringidentifier("traceroute");
#endif

#ifdef MODULE_TRACEROUTE6
   identifiers->tracert6 = browser->getstringidentifier("tracert6");
   identifiers->traceroute6 = browser->getstringidentifier("traceroute6");
#endif

#if defined(MODULE_TRACEROUTE) || defined(MODULE_TRACEROUTE6)
   identifiers->maxhops = browser->getstringidentifier("maxhops");
   identifiers->waittime = browser->getstringidentifier("waittime");
   identifiers->iptohostname = browser->getstringidentifier("iptohostname");
#endif

#ifdef MODULE_NSLOOKUP
   identifiers->nslookup = browser->getstringidentifier("nslookup");
#endif

#if defined(MODULE_NSLOOKUP) || defined(MODULE_NMAP)
   identifiers->query = browser->getstringidentifier("query");
#endif

#ifdef MODULE_WHOIS
   identifiers->whois = browser->getstringidentifier("whois");
#endif

#ifdef MODULE_DIG
   identifiers->dig = browser->getstringidentifier("dig");
#endif

#ifdef MODULE_NMAP
   identifiers->nmap = browser->getstringidentifier("nmap");
#endif

#ifdef MODULE_IPCONFIG
   identifiers->ipconfig = browser->getstringidentifier("ipconfig");
   identifiers->ifconfig = browser->getstringidentifier("ifconfig");
#endif

   identifiers->destroy = destroy;

  return true;
}