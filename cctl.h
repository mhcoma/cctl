#ifndef __CCTL_CCTL_H__
#define __CCTL_CCTL_H__

#define cctl_concat_(A, B) A##B
#define cctl_concat(A, B) cctl_concat_(A, B)
#define cctl_join(PREFIX, NAME) cctl_concat(PREFIX, cctl_concat(_, NAME))
#define cctl_join3(A, B, C) cctl_join(cctl_join(A, B), C)
#define cctl_join4(A, B, C, D) cctl_join(cctl_join3(A, B, C), D)
#define cctl_stringify_(TOKEN) #TOKEN
#define cctl_stringify(TOKEN) cctl_stringify_(TOKEN)

#define cctl_ptr(TYPE) cctl_join(TYPE, ptr)
#define cctl_ptr_def(TYPE) \
	typedef TYPE *cctl_ptr(TYPE)

#define cctl_num_args(...) (sizeof((int[]) { __VA_ARGS__ } ) / sizeof(int))

#endif