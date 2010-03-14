#ifndef _CONVERT_UTF_H
#define _CONVERT_UTF_H

#if defined(_WINDOWS)
#include <ac-stdint.h>
#endif

uint32_t *utf8_to_utf32(uint8_t *utf8, int len);

#endif /*_CONVERT_UTF_H*/
