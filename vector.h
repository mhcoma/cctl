#ifndef __CCTL_VECTOR_H__
#define __CCTL_VECTOR_H__

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "cctl.h"

#define vector_iterator(TYPE) cctl_join(TYPE, vector_iterator)
#define vector_iterator_func(FUNC, TYPE) cctl_join(vector_iterator(TYPE), FUNC)
#define vector_iterator_struct(TYPE) cctl_join(vector_iterator(TYPE), struct)

#define vector_iterator_get(TYPE, p_it) ((p_it)->p_v->p_data + (p_it)->index)
#define vector_iterator_is_valid(TYPE, p_it) ((p_it)->p_v && (p_it)->index < (p_it)->p_v->size)
#define vector_iterator_next(TYPE, p_it) ((p_it)->index++)
#define vector_iterator_prev(TYPE, p_it) ((p_it)->index--)

#define vector_iterator_insert_after(TYPE, p_it, item) vector_iterator_func(insert_after, TYPE)(p_it, item)
#define vector_iterator_insert_before(TYPE, p_it, item) vector_iterator_func(insert_before, TYPE)(p_it, item)
#define vector_iterator_remove(TYPE, p_it) vector_iterator_func(remove, TYPE)(p_it)

#define vector_iterator_imp_h(TYPE) \
	typedef struct vector_iterator_struct(TYPE) vector_iterator(TYPE); \
	struct vector_iterator_struct(TYPE) { \
		vector(TYPE)* p_v; \
		size_t index; \
	}; \
	\
	bool vector_iterator_func(insert_after, TYPE)(vector_iterator(TYPE)* p_it, TYPE item); \
	bool vector_iterator_func(insert_before, TYPE)(vector_iterator(TYPE)* p_it, TYPE item); \
	bool vector_iterator_func(remove, TYPE)(vector_iterator(TYPE)* p_it);

#define vector_iterator_imp_c(TYPE) \
	bool vector_iterator_func(insert_after, TYPE)(vector_iterator(TYPE)* p_it, TYPE item) { \
		if (!p_it || !vector_iterator_is_valid(TYPE, p_it)) return false; \
		if (!vector_func(resize, TYPE)(p_it->p_v, p_it->p_v->size + 1)) return false; \
		if (p_it->index + 1 < p_it->p_v->size - 1) { \
			memmove( \
				p_it->p_v->p_data + p_it->index + 2, \
				p_it->p_v->p_data + p_it->index + 1, \
				(p_it->p_v->size - 2 - p_it->index) * sizeof(TYPE) \
			); \
		} \
		*(p_it->p_v->p_data + p_it->index + 1) = item; \
		return true; \
	} \
	\
	bool vector_iterator_func(insert_before, TYPE)(vector_iterator(TYPE)* p_it, TYPE item) { \
		if (!p_it || !vector_iterator_is_valid(TYPE, p_it)) return false; \
		if (!vector_func(resize, TYPE)(p_it->p_v, p_it->p_v->size + 1)) return false; \
		memmove( \
			p_it->p_v->p_data + p_it->index + 1, \
			p_it->p_v->p_data + p_it->index, \
			(p_it->p_v->size - 1 - p_it->index) * sizeof(TYPE) \
		); \
		*(p_it->p_v->p_data + p_it->index) = item; \
		p_it->index++; \
		return true; \
	} \
	\
	bool vector_iterator_func(remove, TYPE)(vector_iterator(TYPE)* p_it) { \
		if (!p_it || !vector_iterator_is_valid(TYPE, p_it)) return false; \
		if (p_it->index < p_it->p_v->size - 1) { \
			memmove( \
				p_it->p_v->p_data + p_it->index, \
				p_it->p_v->p_data + p_it->index + 1, \
				(p_it->p_v->size - 1 - p_it->index) * sizeof(TYPE) \
			); \
		} \
		p_it->p_v->size--; \
		return true; \
	}

#define vector(TYPE) cctl_join(TYPE, vector)
#define vector_func(FUNC, TYPE) cctl_join(vector(TYPE), FUNC)
#define vector_struct(TYPE) cctl_join(vector(TYPE), struct)

#define vector_init(TYPE, p_v) vector_func(init, TYPE)(p_v)
#define vector_resize(TYPE, p_v, size) vector_func(resize, TYPE)(p_v, size)
#define vector_reserve(TYPE, p_v, size) vector_func(reserve, TYPE)(p_v, size)
#define vector_clear(TYPE, p_v) vector_func(clear, TYPE)(p_v)
#define vector_free(TYPE, p_v) vector_func(free, TYPE)(p_v)
#define vector_push_back(TYPE, p_v, item) vector_func(push_back, TYPE)(p_v, item)
#define vector_pop_back(TYPE, p_v) vector_func(pop_back, TYPE)(p_v)

#define vector_at(TYPE, p_v, index) ((p_v)->p_data + index)
#define vector_front(TYPE, p_v) ((p_v)->p_data)
#define vector_back(TYPE, p_v) ((p_v)->p_data + (p_v)->size - ((p_v)->size ? 1 : 0))

#define vector_begin(TYPE, p_v) vector_func(begin, TYPE)(p_v)
#define vector_rbegin(TYPE, p_v) vector_func(rbegin, TYPE)(p_v)
#define vector_seek(TYPE, p_v, index) vector_func(seek, TYPE)(p_v, index)
#define vector_foreach(TYPE, p_v, it) for (vector_iterator(TYPE) it = vector_begin(TYPE, p_v); vector_iterator_is_valid(TYPE, &it); vector_iterator_next(TYPE, &it))
#define vector_rforeach(TYPE, p_v, it) for (vector_iterator(TYPE) it = vector_rbegin(TYPE, p_v); vector_iterator_is_valid(TYPE, &it); vector_iterator_prev(TYPE, &it))

#define vector_fd(TYPE) \
	typedef struct vector_struct(TYPE) vector(TYPE);

#define vector_imp_h(TYPE) \
	vector_iterator_imp_h(TYPE); \
	struct vector_struct(TYPE) { \
		TYPE* p_data; \
		size_t size; \
		size_t capacity; \
	}; \
	bool vector_func(reserve, TYPE)(vector(TYPE)* p_v, size_t size); \
	bool vector_func(resize, TYPE)(vector(TYPE)* p_v, size_t size); \
	void vector_func(init, TYPE)(vector(TYPE)* p_v); \
	void vector_func(clear, TYPE)(vector(TYPE)* p_v); \
	void vector_func(free, TYPE)(vector(TYPE)* p_v); \
	bool vector_func(push_back, TYPE)(vector(TYPE)* p_v, TYPE item); \
	bool vector_func(pop_back, TYPE)(vector(TYPE)* p_v); \
	vector_iterator(TYPE) vector_func(begin, TYPE)(vector(TYPE)* p_v); \
 	vector_iterator(TYPE) vector_func(rbegin, TYPE)(vector(TYPE)* p_v); \
	vector_iterator(TYPE) vector_func(seek, TYPE)(vector(TYPE)* p_v, size_t index);

#define vector_imp_c(TYPE) \
	vector_iterator_imp_c(TYPE); \
	bool vector_func(reserve, TYPE)(vector(TYPE)* p_v, size_t size) { \
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
	bool vector_func(resize, TYPE)(vector(TYPE)* p_v, size_t size) { \
		if (vector_func(reserve, TYPE)(p_v, size)) { \
			p_v->size = size; \
			return true; \
		} \
		return false; \
	} \
	\
	void vector_func(init, TYPE)(vector(TYPE)* p_v) { \
		p_v->p_data = NULL; \
		p_v->size = 0; \
		p_v->capacity = 0; \
		return; \
	} \
	\
	void vector_func(clear, TYPE)(vector(TYPE)* p_v) { \
		memset(p_v->p_data, 0, p_v->size * sizeof(TYPE)); \
		p_v->size = 0; \
		return; \
	} \
	\
	void vector_func(free, TYPE)(vector(TYPE)* p_v) { \
		if (p_v) { \
			vector_func(clear, TYPE)(p_v); \
			free(p_v->p_data); \
			vector_func(init, TYPE)(p_v); \
			return; \
		} \
	} \
	\
	bool vector_func(push_back, TYPE)(vector(TYPE)* p_v, TYPE item) { \
		if (vector_func(resize, TYPE)(p_v, p_v->size + 1)) { \
			*(p_v->p_data + p_v->size - 1) = item; \
			return true; \
		} \
		return false; \
	} \
	\
	bool vector_func(pop_back, TYPE)(vector(TYPE)* p_v) { \
		if (p_v->size == 0) return false; \
		memset(p_v->p_data + p_v->size - 1, 0, sizeof(TYPE)); \
		p_v->size--; \
		return true; \
	} \
	\
    vector_iterator(TYPE) vector_func(begin, TYPE)(vector(TYPE)* p_v) { \
        vector_iterator(TYPE) it = { p_v, 0 }; \
        return it; \
    } \
    \
    vector_iterator(TYPE) vector_func(rbegin, TYPE)(vector(TYPE)* p_v) { \
        vector_iterator(TYPE) it = { p_v, p_v->size > 0 ? p_v->size - 1 : (size_t)-1 }; \
        return it; \
    } \
    \
    vector_iterator(TYPE) vector_func(seek, TYPE)(vector(TYPE)* p_v, size_t index) { \
        vector_iterator(TYPE) it = { p_v, index < p_v->size ? index : (size_t)-1 }; \
        return it; \
    }

#endif