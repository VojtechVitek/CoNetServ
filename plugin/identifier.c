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

   identifiers->destroy = destroy;

  return true;
}