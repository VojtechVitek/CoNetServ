/* TRACEROUTE and TRACEROUTE6 modules */
module *traceroute = NULL;
module *traceroute6 = NULL;

/**
 * Object TRACEROUTE - abstraction of TRACEROUTE and TRACEROUTE6
 * Used to distinguish TRACEROUTE and TRACEROUTE6 objects passed from JavaScript
 */
typedef struct _object_traceroute {
   object       obj;
   shell_module *program;
} object_traceroute;

/**
 * TRACEROUTE settings
 * The same settings for both TRACEROUTE and TRACEROUTE6
 */
static struct {
   int  maxhops;
   int  waittime;
   bool iptohostname;
} settings = {0};

static bool
hasProperty(NPObject *obj, NPIdentifier identifier)
{
   if (identifier == browser->getstringidentifier("found")) {
      DEBUG_STR("plugin->traceroute->hasProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      return true;
   }
   if (identifier == browser->getstringidentifier("maxhops")) {
      DEBUG_STR("plugin->traceroute->hasProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      return true;
   }
   if (identifier == browser->getstringidentifier("waittime")) {
      DEBUG_STR("plugin->traceroute->hasProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      return true;
   }
   if (identifier == browser->getstringidentifier("iptohostname")) {
      DEBUG_STR("plugin->traceroute->hasProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      return true;
   }
   DEBUG_STR("plugin->traceroute->hasProperty(%s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}

static bool
getProperty(NPObject *obj, NPIdentifier identifier, NPVariant *value)
{
   if (identifier == browser->getstringidentifier("found")) {
      DEBUG_STR("plugin->traceroute->getProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      BOOLEAN_TO_NPVARIANT(((object_traceroute *)obj)->program->found, *value);
      return true;
   }
   if (identifier == browser->getstringidentifier("maxhops")) {
      DEBUG_STR("plugin->traceroute->hasProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      INT32_TO_NPVARIANT(settings.maxhops, *value);
      return true;
   }
   if (identifier == browser->getstringidentifier("waittime")) {
      DEBUG_STR("plugin->traceroute->hasProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      INT32_TO_NPVARIANT(settings.waittime, *value);
      return true;
   }
   if (identifier == browser->getstringidentifier("iptohostname")) {
      DEBUG_STR("plugin->traceroute->hasProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      BOOLEAN_TO_NPVARIANT(settings.iptohostname, *value);
      return true;
   }
   DEBUG_STR("plugin->traceroute->getProperty(%s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}

static bool
setProperty(NPObject *obj, NPIdentifier identifier, const NPVariant *value)
{
   if (identifier == browser->getstringidentifier("maxhops")) {
      DEBUG_STR("plugin->traceroute->hasProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      settings.maxhops = NPVARIANT_TO_INT32(*value);
      return true;
   }
   if (identifier == browser->getstringidentifier("waittime")) {
      DEBUG_STR("plugin->traceroute->hasProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      settings.waittime = NPVARIANT_TO_INT32(*value);
      return true;
   }
   if (identifier == browser->getstringidentifier("iptohostname")) {
      DEBUG_STR("plugin->traceroute->hasProperty(%s): true", DEBUG_IDENTIFIER(identifier));
      settings.iptohostname = NPVARIANT_TO_BOOLEAN(*value);
      return true;
   }
   DEBUG_STR("plugin->traceroute->setProperty(%s): false", DEBUG_IDENTIFIER(identifier));
   return false;
}

static void
destroy_traceroute()
{
   DEBUG_STR("traceroute->destroy()");
   if (traceroute)
      shell->destroy_module((shell_module *)traceroute);
}

static void
destroy_traceroute6()
{
   DEBUG_STR("traceroute6->destroy()");
   if (traceroute6)
      shell->destroy_module((shell_module *)traceroute6);
}

static NPObject *
allocate_traceroute(NPP instance, NPClass *class)
{
   object_traceroute *obj;

   DEBUG_STR("plugin->traceroute->allocate()");

   obj = browser->memalloc(sizeof(*obj));
   ((object *)obj)->instance = instance;
   obj->program = (shell_module *)traceroute;

   return (NPObject *)obj;
}

static NPObject *
allocate_traceroute6(NPP instance, NPClass *class)
{
   object_traceroute *obj;

   DEBUG_STR("plugin->traceroute6->allocate()");

   obj = browser->memalloc(sizeof(*obj));
   ((object *)obj)->instance = instance;
   obj->program = (shell_module *)traceroute6;

   return (NPObject *)obj;
}

bool
init_module_traceroute()
{
   DEBUG_STR("traceroute->init()");
   traceroute = (module *)shell->init_module("traceroute");
   traceroute->destroy = destroy_traceroute;
   traceroute->class = modules->class;
   traceroute->class.allocate = allocate_traceroute;
   traceroute->class.invoke = invokeMethod;
   traceroute->class.hasProperty = hasProperty;
   traceroute->class.getProperty = getProperty;
   traceroute->class.setProperty = setProperty;

   return true;
}

bool
init_module_traceroute6()
{
   DEBUG_STR("traceroute6->init()");
   traceroute6 = (module *)shell->init_module("traceroute6");
   traceroute6->destroy = destroy_traceroute6;
   traceroute6->class = modules->class;
   traceroute6->class.allocate = allocate_traceroute6;
   traceroute6->class.invoke = invokeMethod;
   traceroute6->class.hasProperty = hasProperty;
   traceroute6->class.getProperty = getProperty;
   traceroute6->class.setProperty = setProperty;

   return true;
}
