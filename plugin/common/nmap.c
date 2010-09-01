/* NMAP module */
module *nmap = NULL;

/**
 * Object NMAP
 * Used to distinguish NMAP object passed from JavaScript
 */
typedef struct _object_nmap {
   object       obj;
} object_nmap;

/**
 * NMAP settings
 */
static struct {
   int query;
} settings = {0};

static bool
hasProperty(NPObject *obj, NPIdentifier identifier)
{
   if (identifier == browser->getstringidentifier("found")) {
      DEBUG_STR("plugin->nmap->hasProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      return true;
   }
   if (identifier == browser->getstringidentifier("query")) {
      DEBUG_STR("plugin->ping->hasProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      return true;
   }
   DEBUG_STR("plugin->nmap->hasProperty(%s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}

static bool
getProperty(NPObject *obj, NPIdentifier identifier, NPVariant *value)
{
   if (identifier == browser->getstringidentifier("found")) {
      DEBUG_STR("plugin->nmap->getProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      BOOLEAN_TO_NPVARIANT(((shell_module *)nmap)->found, *value);
      return true;
   }
   if (identifier == browser->getstringidentifier("query")) {
      DEBUG_STR("plugin->ping->hasProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      INT32_TO_NPVARIANT(settings.query, *value);
      return true;
   }
   DEBUG_STR("plugin->nmap->getProperty(%s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}

static bool
setProperty(NPObject *obj, NPIdentifier identifier, const NPVariant *value)
{
   if (identifier == browser->getstringidentifier("query")) {
      DEBUG_STR("plugin->ping->hasProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      settings.query = NPVARIANT_TO_INT32(*value);
      return true;
   }
   DEBUG_STR("plugin->nmap->setProperty(%s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}

static void
destroy()
{
   DEBUG_STR("nmap->destroy()");
   if (nmap)
      shell->destroy_module((shell_module *)nmap);
}

static NPObject *
allocate(NPP instance, NPClass *class)
{
   object_nmap *obj;

   DEBUG_STR("plugin->nmap->allocate()");

   obj = browser->memalloc(sizeof(*obj));
   ((object *)obj)->instance = instance;

   return (NPObject *)obj;
}

bool
init_module_nmap()
{
   DEBUG_STR("nmap->init()");
   nmap = (module *)shell->init_module("nmap");
   nmap->destroy = destroy;
   nmap->class = modules->class;
   nmap->class.allocate = allocate;
   nmap->class.invoke = invokeMethod;
   nmap->class.hasProperty = hasProperty;
   nmap->class.getProperty = getProperty;
   nmap->class.setProperty = setProperty;

   return true;
}
