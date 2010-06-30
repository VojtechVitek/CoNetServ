#pragma once
#ifndef _DEBUG_H_
#define _DEBUG_H_

#if defined(DEBUG)

void debug(const char *msg);

#else

#define debug()

#endif

#endif /*_DEBUG_H_*/
