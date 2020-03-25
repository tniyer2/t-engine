
#ifndef UTILITY_IF_DEBUG_H
#define UTILITY_IF_DEBUG_H

#ifdef _DEBUG
	#define IF_DEBUG(x) x
#else
	#define IF_DEBUG(x)
#endif

#define UNUSED(x) (void)x;

#endif
