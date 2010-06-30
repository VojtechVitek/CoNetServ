#pragma once
#ifndef _DEBUG_H_
#define _DEBUG_H_

#if defined(DEBUG)

void debug(const char *args, ...);

#else

#define debug()

#endif

#endif /*_DEBUG_H_*/
