/* ROUTE module */
module *route = NULL;

/**
 * Object ROUTE
 * Used to distinguish ROUTE object passed from JavaScript
 */
typedef struct _object_route {
   object       obj;
} object_route;

/**
 * ROUTE settings
 */
/*
static struct {
} settings = {0};
*/

static bool
hasProperty(NPObject *obj, NPIdentifier identifier)
{
   if (identifier == identifiers->found) {
      DEBUG_STR("plugin->route->hasProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      return true;
   }
   DEBUG_STR("plugin->route->hasProperty(%s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}

static bool
getProperty(NPObject *obj, NPIdentifier identifier, NPVariant *value)
{
   if (identifier == identifiers->found) {
      DEBUG_STR("plugin->route->getProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      BOOLEAN_TO_NPVARIANT(((shell_module *)route)->found, *value);
      return true;
   }
   DEBUG_STR("plugin->route->getProperty(%s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}

static bool
setProperty(NPObject *obj, NPIdentifier identifier, const NPVariant *value)
{
   DEBUG_STR("plugin->route->setProperty(%s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}

static void
destroy()
{
   DEBUG_STR("route->destroy()");
   if (route)
      shell->destroy_module((shell_module *)route);
}

static NPObject *
allocate(NPP instance, NPClass *class)
{
   object_route *obj;

   DEBUG_STR("plugin->route->allocate()");

   obj = browser->memalloc(sizeof(*obj));
   ((object *)obj)->instance = instance;

   return (NPObject *)obj;
}
