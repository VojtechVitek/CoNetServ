/* IPCONFIG module */
module *ipconfig = NULL;

/**
 * Object IPCONFIG
 * Used to distinguish IPCONFIG object passed from JavaScript
 */
typedef struct _object_ipconfig {
   object       obj;
} object_ipconfig;

/**
 * IPCONFIG settings
 */
/*
static struct {
} settings = {0};
*/

static bool
hasProperty(NPObject *obj, NPIdentifier identifier)
{
   if (identifier == identifiers->found) {
      DEBUG_STR("plugin->ipconfig->hasProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      return true;
   }
   DEBUG_STR("plugin->ipconfig->hasProperty(%s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}

static bool
getProperty(NPObject *obj, NPIdentifier identifier, NPVariant *value)
{
   if (identifier == identifiers->found) {
      DEBUG_STR("plugin->ipconfig->getProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      BOOLEAN_TO_NPVARIANT(((shell_module *)ipconfig)->found, *value);
      return true;
   }
   DEBUG_STR("plugin->ipconfig->getProperty(%s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}

static bool
setProperty(NPObject *obj, NPIdentifier identifier, const NPVariant *value)
{
   DEBUG_STR("plugin->ipconfig->setProperty(%s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}

static void
destroy()
{
   DEBUG_STR("ipconfig->destroy()");
   if (ipconfig)
      shell->destroy_module((shell_module *)ipconfig);
}

static NPObject *
allocate(NPP instance, NPClass *class)
{
   object_ipconfig *obj;

   DEBUG_STR("plugin->ipconfig->allocate()");

   obj = browser->memalloc(sizeof(*obj));
   ((object *)obj)->instance = instance;

   return (NPObject *)obj;
}
