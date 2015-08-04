#ifndef _DEBUG_NEW_H
#define _DEBUG_NEW_H

#ifndef NDEBUG	  	//如果不是调试模式，忽略

#include "Tracer.h"
#define new new(__FILE__,__LINE__)

#endif  			//NDEBUG

#endif  			// !_DEBUG_NEW_H
