#ifndef ASSERT_H_
#define ASSERT_H_

#include "Kernel.h"

#define KASSERT(expr) \
   ({ \
     int __res=(int)(expr); \
     if (! __res) \
       Kernel::FATAL_ERROR("%s@%s:%d Assertion " # expr " failed", \
			       __PRETTY_FUNCTION__, __FILE__, __LINE__); \
   })


#endif /*ASSERT_H_*/
