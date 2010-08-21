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

#ifdef MODULE_PING
   identifiers->ping = browser->getstringidentifier("ping");
   identifiers->ping6 = browser->getstringidentifier("ping6");
#endif

   identifiers->destroy = destroy;

  return true;
}