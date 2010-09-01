/* NSLOOKUP module */
module *nslookup = NULL;

/**
 * Object NSLOOKUP
 * Used to distinguish NSLOOKUP object passed from JavaScript
 */
typedef struct _object_nslookup {
   object       obj;
} object_nslookup;

/**
 * NSLOOKUP settings
 */
static struct {
   int query;
} settings = {0};

static bool
hasProperty(NPObject *obj, NPIdentifier identifier)
{
   if (identifier == browser->getstringidentifier("found")) {
      DEBUG_STR("plugin->nslookup->hasProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      return true;
   }
   if (identifier == browser->getstringidentifier("query")) {
      DEBUG_STR("plugin->ping->hasProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      return true;
   }
   DEBUG_STR("plugin->nslookup->hasProperty(%s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}

static bool
getProperty(NPObject *obj, NPIdentifier identifier, NPVariant *value)
{
   if (identifier == browser->getstringidentifier("found")) {
      DEBUG_STR("plugin->nslookup->getProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      BOOLEAN_TO_NPVARIANT(((shell_module *)nslookup)->found, *value);
      return true;
   }
   if (identifier == browser->getstringidentifier("query")) {
      DEBUG_STR("plugin->ping->hasProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      INT32_TO_NPVARIANT(settings.query, *value);
      return true;
   }
   DEBUG_STR("plugin->nslookup->getProperty(%s): false", DEBUG_IDENTIFIER(identifier));
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
   DEBUG_STR("plugin->nslookup->setProperty(%s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}

static void
destroy()
{
   DEBUG_STR("nslookup->destroy()");
   if (nslookup)
      shell->destroy_module((shell_module *)nslookup);
}

static NPObject *
allocate(NPP instance, NPClass *class)
{
   object_nslookup *obj;

   DEBUG_STR("plugin->nslookup->allocate()");

   obj = browser->memalloc(sizeof(*obj));
   ((object *)obj)->instance = instance;

   return (NPObject *)obj;
}

bool
init_module_nslookup()
{
   DEBUG_STR("nslookup->init()");
   nslookup = (module *)shell->init_module("nslookup");
   nslookup->destroy = destroy;
   nslookup->class = modules->class;
   nslookup->class.allocate = allocate;
   nslookup->class.invoke = invokeMethod;
   nslookup->class.hasProperty = hasProperty;
   nslookup->class.getProperty = getProperty;
   nslookup->class.setProperty = setProperty;

   return true;
}
