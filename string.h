#ifndef __CCTL_STRING_H__
#define __CCTL_STRING_H__

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "cctl.h"

#define string basic_string(char)
#define string_init(p_v) basic_string_init(char, p_v)
#define string_resize(p_v, size) basic_string_resize(char, p_v, size)
#define string_reserve(p_V, size) basic_string_reserve(char, p_v, size)
#define string_clear(p_v) basic_string_clear(char, p_v)
#define string_free(p_v) basic_string_clear(char, p_v)
#define string_push_back(p_v, item) basic_string_push_back(char, p_v, item)
#define string_pop_back(p_v) basic_string_pop_back(char, p_v)
#define string_at(p_v, index) basic_string_at(char, p_v, index)
#define string_back(p_v) basic_string_back(char, p_v)
#define string_front(p_v) basic_string_front(char, p_v)

#define string_fd() basic_string_fd(char)
#define string_imp_h() basic_string_imp_h(char)
#define string_imp_c() basic_string_imp_c(char)

#define basic_string(TYPE) cctl_join(TYPE, basic_string)
#define basic_string_func(FUNC, TYPE) cctl_join(basic_string(TYPE), FUNC)
#define basic_string_struct(TYPE) cctl_join(basic_string(TYPE), struct)

#define basic_string_init(TYPE, p_v) basic_string_func(init, TYPE)(p_v)
#define basic_string_resize(TYPE, p_v, size) basic_string_func(resize, TYPE)(p_v, size)
#define basic_string_reserve(TYPE, p_v, size) basic_string_func(reserve, TYPE)(p_v, size)
#define basic_string_clear(TYPE, p_v) basic_string_func(clear, TYPE)(p_v)
#define basic_string_free(TYPE, p_v) basic_string_func(free, TYPE)(p_v)
#define basic_string_push_back(TYPE, p_v, item) basic_string_func(push_back, TYPE)(p_v, item)
#define basic_string_pop_back(TYPE, p_v) basic_string_func(pop_back, TYPE)(p_v)

#define basic_string_copy(TYPE, p_v) basic_string_func(copy, TYPE)(p_v)

#define basic_string_at(TYPE, p_v, index) ((p_v)->p_data + index)
#define basic_string_front(TYPE, p_v) ((p_v)->p_data)
#define basic_string_back(TYPE, p_v) ((p_v)->p_data + (p_v)->size - ((p_v)->size ? 1 : 0))

#define basic_string_fd(TYPE) \
	typedef struct basic_string_struct(TYPE) basic_string(TYPE);

#define basic_string_imp_h(TYPE) \
	struct basic_string_struct(TYPE) { \
		TYPE* p_data; \
		size_t size; \
		size_t capacity; \
	}; \
	bool basic_string_func(reserve, TYPE)(basic_string(TYPE)* p_v, size_t size); \
	bool basic_string_func(resize, TYPE)(basic_string(TYPE)* p_v, size_t size); \
	void basic_string_func(init, TYPE)(basic_string(TYPE)* p_v); \
	void basic_string_func(clear, TYPE)(basic_string(TYPE)* p_v); \
	void basic_string_func(free, TYPE)(basic_string(TYPE)* p_v); \
	bool basic_string_func(push_back, TYPE)(basic_string(TYPE)* p_v, TYPE item); \
	bool basic_string_func(pop_back, TYPE)(basic_string(TYPE)* p_v); \
	basic_string(TYPE) basic_string_func(copy, TYPE)(basic_string(TYPE)* p_v);

#define basic_string_imp_c(TYPE) \
	bool basic_string_func(reserve, TYPE)(basic_string(TYPE)* p_v, size_t size) { \
		if (p_v->capacity >= size) return true; \
		size_t capacity = 1; \
		if (size > 0) while (capacity <= size) capacity <<= 1; \
		if (p_v->capacity < size) { \
			if (p_v->capacity) { \
				if (!(p_v->p_data = (TYPE*) realloc(p_v->p_data, capacity * sizeof(TYPE)))) return false; \
				memset(p_v->p_data + p_v->size, 0, (capacity - p_v->size) * sizeof(TYPE)); \
			} \
			else { \
				if (!(p_v->p_data = (TYPE*) calloc(capacity, sizeof(TYPE)))) return false; \
			} \
			p_v->capacity = capacity; \
		} \
		return true; \
	} \
	\
	bool basic_string_func(resize, TYPE)(basic_string(TYPE)* p_v, size_t size) { \
		if (basic_string_func(reserve, TYPE)(p_v, size)) { \
			p_v->size = size; \
			return true; \
		} \
		return false; \
	} \
	\
	void basic_string_func(init, TYPE)(basic_string(TYPE)* p_v) { \
		p_v->p_data = NULL; \
		p_v->size = 0; \
		p_v->capacity = 0; \
		return; \
	} \
	\
	void basic_string_func(clear, TYPE)(basic_string(TYPE)* p_v) { \
		memset(p_v->p_data, 0, p_v->size * sizeof(TYPE)); \
		p_v->size = 0; \
		return; \
	} \
	\
	void basic_string_func(free, TYPE)(basic_string(TYPE)* p_v) { \
		basic_string_func(clear, TYPE)(p_v); \
		free(p_v->p_data); \
		basic_string_func(init, TYPE)(p_v); \
		return; \
	} \
	\
	bool basic_string_func(push_back, TYPE)(basic_string(TYPE)* p_v, TYPE item) { \
		if (basic_string_func(resize, TYPE)(p_v, p_v->size + 1)) { \
			*(p_v->p_data + p_v->size - 1) = item; \
			return true; \
		} \
		return false; \
	} \
	\
	bool basic_string_func(pop_back, TYPE)(basic_string(TYPE)* p_v) { \
		if (p_v->size == 0) return false; \
		memset(p_v->p_data + p_v->size - 1, 0, sizeof(TYPE)); \
		p_v->size--; \
		return true; \
	} \
	\
	basic_string(TYPE) basic_string_func(copy, TYPE)(basic_string(TYPE)* p_v) { \
		basic_string(TYPE) new_str; \
		basic_string_init(TYPE, &new_str); \
		return true; \
	}

#endif