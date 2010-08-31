/* PING and PING6 modules */
module *ping = NULL;
module *ping6 = NULL;

/**
 * Object PING - abstraction of PING and PING6
 * Used for distinguish PING and PING6 objects passed from JavaScript
 */
typedef struct _object_ping {
   object       obj;
   shell_module *program;
} object_ping;

/**
 * PING settings
 * The same settings for both PING and PING6
 */
static struct {
   int count;
   int timeout;
   int packetsize;
   int ttl;
} settings = {0};

static bool
hasProperty(NPObject *obj, NPIdentifier identifier)
{
   if (identifier == identifiers->found) {
      DEBUG_STR("plugin->ping->hasProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      return true;
   }
   if (identifier == identifiers->count) {
      DEBUG_STR("plugin->ping->hasProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      return true;
   }
   if (identifier == identifiers->timeout) {
      DEBUG_STR("plugin->ping->hasProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      return true;
   }
   if (identifier == identifiers->packetsize) {
      DEBUG_STR("plugin->ping->hasProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      return true;
   }
   if (identifier == identifiers->ttl) {
      DEBUG_STR("plugin->ping->hasProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      return true;
   }
   DEBUG_STR("plugin->ping->hasProperty(%s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}

static bool
getProperty(NPObject *obj, NPIdentifier identifier, NPVariant *value)
{
   if (identifier == identifiers->found) {
      DEBUG_STR("plugin->ping->getProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      BOOLEAN_TO_NPVARIANT(((object_ping *)obj)->program->found, *value);
      return true;
   }
   if (identifier == identifiers->count) {
      DEBUG_STR("plugin->ping->getProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      INT32_TO_NPVARIANT(settings.count, *value);
      return true;
   }
   if (identifier == identifiers->timeout) {
      DEBUG_STR("plugin->ping->getProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      INT32_TO_NPVARIANT(settings.timeout, *value);
      return true;
   }
   if (identifier == identifiers->packetsize) {
      DEBUG_STR("plugin->ping->getProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      INT32_TO_NPVARIANT(settings.packetsize, *value);
      return true;
   }
   if (identifier == identifiers->ttl) {
      DEBUG_STR("plugin->ping->getProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      INT32_TO_NPVARIANT(settings.ttl, *value);
      return true;
   }
   DEBUG_STR("plugin->ping->getProperty(%s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}

static bool
setProperty(NPObject *obj, NPIdentifier identifier, const NPVariant *value)
{
   if (identifier == identifiers->count) {
      DEBUG_STR("plugin->ping->setProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      settings.count = NPVARIANT_TO_INT32(*value);
      return true;
   }
   if (identifier == identifiers->timeout) {
      DEBUG_STR("plugin->ping->setProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      settings.timeout = NPVARIANT_TO_INT32(*value);
      return true;
   }
   if (identifier == identifiers->packetsize) {
      DEBUG_STR("plugin->ping->setProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      settings.packetsize = NPVARIANT_TO_INT32(*value);
      return true;
   }
   if (identifier == identifiers->ttl) {
      DEBUG_STR("plugin->ping->setProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      settings.ttl = NPVARIANT_TO_INT32(*value);
      return true;
   }
   DEBUG_STR("plugin->ping->setProperty(%s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}

static void
destroy_ping()
{
   DEBUG_STR("ping->destroy()");
   if (ping)
      shell->destroy_module((shell_module *)ping);
}

static void
destroy_ping6()
{
   DEBUG_STR("ping6->destroy()");
   if (ping6)
      shell->destroy_module((shell_module *)ping6);
}

static NPObject *
allocate_ping(NPP instance, NPClass *class)
{
   object_ping *obj;

   DEBUG_STR("plugin->ping->allocate()");

   obj = browser->memalloc(sizeof(*obj));
   ((object *)obj)->instance = instance;
   obj->program = (shell_module *)ping;

   return (NPObject *)obj;
}

static NPObject *
allocate_ping6(NPP instance, NPClass *class)
{
   object_ping *obj;

   DEBUG_STR("plugin->ping6->allocate()");

   obj = browser->memalloc(sizeof(*obj));
   ((object *)obj)->instance = instance;
   obj->program = (shell_module *)ping6;

   return (NPObject *)obj;
}

bool
init_module_ping()
{
   DEBUG_STR("ping->init()");
   ping = (module *)shell->init_module("ping");
   ping->destroy = destroy_ping;
   ping->class = modules->class;
   ping->class.allocate = allocate_ping;
   ping->class.invoke = invokeMethod;
   ping->class.hasProperty = hasProperty;
   ping->class.getProperty = getProperty;
   ping->class.setProperty = setProperty;

   return true;
}

bool
init_module_ping6()
{
   DEBUG_STR("ping6->init()");
   ping6 = (module *)shell->init_module("ping6");
   ping6->destroy = destroy_ping6;
   ping6->class = modules->class;
   ping6->class.allocate = allocate_ping6;
   ping6->class.invoke = invokeMethod;
   ping6->class.hasProperty = hasProperty;
   ping6->class.getProperty = getProperty;
   ping6->class.setProperty = setProperty;

   return true;
}
