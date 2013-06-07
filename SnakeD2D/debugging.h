#ifndef SNAKEDEBUGGING
#define SNAKEDEBUGGING

#ifdef _DEBUG

#include <iostream>
#include <sstream>

 #define DBOUT( s )								\
{												\
   std::wostringstream os_;						\
   os_ << s;									\
   OutputDebugStringW( os_.str().c_str() );		\
}
#else 

#define DBOUT( s )

#endif

#endif
