#ifndef _CONFIG_H
#define _CONFIG_H

#define strcasecmp stricmp
#define strncasecmp strnicmp

#define LOCALEDIR "."

#if _MSC_VER && !__cplusplus
# define inline __inline
#endif

#endif /* _CONFIG_H */
