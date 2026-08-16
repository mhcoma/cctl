#ifndef __CCTL_PAIR_H__
#define __CCTL_PAIR_H__

#include "cctl.h"

#define pair(KEY, VALUE) cctl_join3(KEY, VALUE, pair)
#define pair_struct(KEY, VALUE) cctl_join(pair(KEY, VALUE), struct)

#define pair_fd(KEY, VALUE) \
	typedef struct pair_struct(KEY, VALUE) pair(KEY, VALUE);

#define pair_imp_h(KEY, VALUE) \
	struct pair_struct(KEY, VALUE) { \
		KEY key; \
		VALUE value; \
	};

#endif