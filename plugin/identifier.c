#include <stdio.h>

#include "debug.h"
#include "npapi.h"
#include "identifier.h"

identifier *identifiers = NULL;

void
destroy()
{
   browser->memfree(identifiers);
}

bool
init_identifiers()
{
  if ((identifiers = browser->memalloc(sizeof(*identifiers))) == NULL)
     return false;

  identifiers->read = browser->getstringidentifier("read");
  identifiers->running = browser->getstringidentifier("running");
  identifiers->start = browser->getstringidentifier("start");
  identifiers->stop = browser->getstringidentifier("stop");
  identifiers->version = browser->getstringidentifier("version");

#ifdef MODULE_PING
  identifiers->ping = browser->getstringidentifier("ping");
#endif

  identifiers->destroy = destroy;

  return true;
}