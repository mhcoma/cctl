#ifndef __CCTL_DEQUE_H__
#define __CCTL_DEQUE_H__

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "cctl.h"

#define cctl_deque_chunk_max 512

#define chunk(TYPE) cctl_join(TYPE, chunk)
#define chunk_func(FUNC, TYPE) cctl_join(chunk(TYPE), FUNC)
#define chunk_struct(TYPE) cctl_join(chunk(TYPE), struct)

#define chunk_front(T, p_c) chunk_func(at, T)(p_c, 0)
#define chunk_back(T, p_c) chunk_func(at, T)(p_c, (p_c)->size - 1)

#define chunk_imp_h(TYPE) \
	typedef struct chunk_struct(TYPE) chunk(TYPE); \
	\
	struct chunk_struct(TYPE) { \
		TYPE* p_data; \
		size_t begin; \
		size_t size; \
	}; \
	\
	void chunk_func(init, TYPE)(chunk(TYPE)* p_c); \
	void chunk_func(clear, TYPE)(chunk(TYPE)* p_c); \
	void chunk_func(free, TYPE)(chunk(TYPE)* p_c); \
	bool chunk_func(push_front, TYPE)(chunk(TYPE)* p_c, TYPE item); \
	bool chunk_func(push_back, TYPE)(chunk(TYPE)* p_c, TYPE item); \
	bool chunk_func(pop_front, TYPE)(chunk(TYPE)* p_c); \
	bool chunk_func(pop_back, TYPE)(chunk(TYPE)* p_c); \
	TYPE* chunk_func(at, TYPE)(chunk(TYPE)* p_c, size_t index);

#define chunk_imp_c(TYPE) \
	void chunk_func(init, TYPE)(chunk(TYPE)* p_c) { \
		memset(p_c, 0, sizeof(chunk(TYPE))); \
	} \
	\
	void chunk_func(clear, TYPE)(chunk(TYPE)* p_c) { \
		p_c->begin = 0; \
		p_c->size = 0; \
	} \
	\
	void chunk_func(free, TYPE)(chunk(TYPE)* p_c) { \
		if (p_c->p_data) free(p_c->p_data); \
		chunk_func(init, TYPE)(p_c); \
	} \
	\
	bool chunk_func(push_front, TYPE)(chunk(TYPE)* p_c, TYPE item) { \
		if (p_c->size == cctl_deque_chunk_max) return false; \
		if (!p_c->p_data) { \
			if (!(p_c->p_data = (TYPE*) malloc(cctl_deque_chunk_max * sizeof(TYPE)))) return false; \
		} \
		p_c->begin = (p_c->begin + cctl_deque_chunk_max - 1) % cctl_deque_chunk_max; \
		*(p_c->p_data + p_c->begin) = item; \
		p_c->size++; \
		return true; \
	} \
	\
	bool chunk_func(push_back, TYPE)(chunk(TYPE)* p_c, TYPE item) { \
		if (p_c->size == cctl_deque_chunk_max) return false; \
		if (!p_c->p_data) { \
			if (!(p_c->p_data = (TYPE*) malloc(cctl_deque_chunk_max * sizeof(TYPE)))) return false; \
		} \
		size_t end_idx = (p_c->begin + p_c->size) % cctl_deque_chunk_max; \
		*(p_c->p_data + end_idx) = item; \
		p_c->size++; \
		return true; \
	} \
	\
	bool chunk_func(pop_front, TYPE)(chunk(TYPE)* p_c) { \
		if (p_c->size == 0) return false; \
		p_c->begin = (p_c->begin + 1) % cctl_deque_chunk_max; \
		p_c->size--; \
		return true; \
	} \
	\
	bool chunk_func(pop_back, TYPE)(chunk(TYPE)* p_c) { \
		if (p_c->size == 0) return false; \
		p_c->size--; \
		return true; \
	} \
	\
	TYPE* chunk_func(at, TYPE)(chunk(TYPE)* p_c, size_t index) { \
		return p_c->p_data + ((p_c->begin + index) % cctl_deque_chunk_max); \
	}

#define deque_iterator(TYPE) cctl_join(TYPE, deque_iterator)
#define deque_iterator_func(FUNC, TYPE) cctl_join(deque_iterator(TYPE), FUNC)
#define deque_iterator_struct(TYPE) cctl_join(deque_iterator(TYPE), struct)

#define deque_iterator_get(TYPE, p_it) deque_func(at, TYPE)((p_it)->p_d, (p_it)->index)
#define deque_iterator_is_valid(TYPE, p_it) ((p_it)->p_d && (p_it)->index < (p_it)->p_d->size)
#define deque_iterator_next(TYPE, p_it) ((p_it)->index++)
#define deque_iterator_prev(TYPE, p_it) ((p_it)->index--)

#define deque_iterator_insert_after(TYPE, p_it, item) deque_iterator_func(insert_after, TYPE)(p_it, item)
#define deque_iterator_insert_before(TYPE, p_it, item) deque_iterator_func(insert_before, TYPE)(p_it, item)
#define deque_iterator_remove(TYPE, p_it) deque_iterator_func(remove, TYPE)(p_it)

#define deque_iterator_imp_h(TYPE) \
	typedef struct deque_iterator_struct(TYPE) deque_iterator(TYPE); \
	struct deque_iterator_struct(TYPE) { \
		deque(TYPE)* p_d; \
		size_t index; \
	}; \
	\
	bool deque_iterator_func(insert_after, TYPE)(deque_iterator(TYPE)* p_it, TYPE item); \
	bool deque_iterator_func(insert_before, TYPE)(deque_iterator(TYPE)* p_it, TYPE item); \
	bool deque_iterator_func(remove, TYPE)(deque_iterator(TYPE)* p_it);

#define deque_iterator_imp_c(TYPE) \
	bool deque_iterator_func(insert_after, TYPE)(deque_iterator(TYPE)* p_it, TYPE item) { \
		if (!p_it || !deque_iterator_is_valid(TYPE, p_it)) return false; \
		TYPE none; \
		memset(&none, 0, sizeof(TYPE)); \
		if (!deque_func(push_back, TYPE)(p_it->p_d, none)) return false; \
		for (size_t i = p_it->p_d->size - 1; i > p_it->index + 1; i--) { \
			TYPE* p_current = deque_func(at, TYPE)(p_it->p_d, i); \
			TYPE* p_previous = deque_func(at, TYPE)(p_it->p_d, i - 1); \
			*p_current = *p_previous; \
		} \
		*deque_func(at, TYPE)(p_it->p_d, p_it->index + 1) = item; \
		return true; \
	} \
	\
	bool deque_iterator_func(insert_before, TYPE)(deque_iterator(TYPE)* p_it, TYPE item) { \
		if (!p_it || !deque_iterator_is_valid(TYPE, p_it)) return false; \
		TYPE none; \
		memset(&none, 0, sizeof(TYPE)); \
		if (!deque_func(push_back, TYPE)(p_it->p_d, none)) return false; \
		for (size_t i = p_it->p_d->size - 1; i > p_it->index; i--) { \
			TYPE* p_current = deque_func(at, TYPE)(p_it->p_d, i); \
			TYPE* p_previous = deque_func(at, TYPE)(p_it->p_d, i - 1); \
			*p_current = *p_previous; \
		} \
		*deque_func(at, TYPE)(p_it->p_d, p_it->index) = item; \
		p_it->index++;\
		return true; \
	} \
	\
	bool deque_iterator_func(remove, TYPE)(deque_iterator(TYPE)* p_it) { \
		if (!p_it || !deque_iterator_is_valid(TYPE, p_it)) return false; \
		for (size_t i = p_it->index; i < p_it->p_d->size - 1; i++) { \
			TYPE* p_current = deque_func(at, TYPE)(p_it->p_d, i); \
			TYPE* p_next = deque_func(at, TYPE)(p_it->p_d, i + 1); \
			*p_current = *p_next; \
		} \
		deque_func(pop_back, TYPE)(p_it->p_d); \
		return true; \
	}

#define deque(TYPE) cctl_join(TYPE, deque)
#define deque_chunk_func(FUNC, TYPE) cctl_join(cctl_join(deque(TYPE), chunk), FUNC)
#define deque_func(FUNC, TYPE) cctl_join(deque(TYPE), FUNC)
#define deque_struct(TYPE) cctl_join(deque(TYPE), struct)

#define deque_init(TYPE, p_d) deque_func(init, TYPE)(p_d)
#define deque_resize(TYPE, p_d, size) deque_func(resize, TYPE)(p_d, size)
#define deque_clear(TYPE, p_d) deque_func(clear, TYPE)(p_d)
#define deque_free(TYPE, p_d) deque_func(free, TYPE)(p_d)
#define deque_push_back(TYPE, p_d, item) deque_func(push_back, TYPE)(p_d, item)
#define deque_pop_back(TYPE, p_d) deque_func(pop_back, TYPE)(p_d)
#define deque_at(TYPE, p_d, index) deque_func(at, TYPE)(p_d, index)

#define deque_chunk_front(TYPE, p_d) deque_chunk_func(at, TYPE)(p_d, 0)
#define deque_chunk_back(TYPE, p_d) deque_chunk_func(at, TYPE)(p_d, (p_d)->chunk_count - 1)

#define deque_front(TYPE, p_d) ((p_d)->size ? chunk_front(TYPE, deque_chunk_front(TYPE, p_d)) : NULL)
#define deque_back(TYPE, p_d) ((p_d)->size ? chunk_back(TYPE, deque_chunk_back(TYPE, p_d)) : NULL)

#define deque_begin(TYPE, p_d) deque_func(begin, TYPE)(p_d)
#define deque_rbegin(TYPE, p_d) deque_func(rbegin, TYPE)(p_d)
#define deque_seek(TYPE, p_d, index) deque_func(seek, TYPE)(p_d, index)
#define deque_foreach(TYPE, p_d, it) for (deque_iterator(TYPE) it = deque_begin(TYPE, p_d); deque_iterator_is_valid(TYPE, &it); deque_iterator_next(TYPE, &it))
#define deque_rforeach(TYPE, p_d, it) for (deque_iterator(TYPE) it = deque_rbegin(TYPE, p_d); deque_iterator_is_valid(TYPE, &it); deque_iterator_prev(TYPE, &it))

#define deque_fd(TYPE) \
	typedef struct deque_struct(TYPE) deque(TYPE);

#define deque_imp_h(TYPE) \
	chunk_imp_h(TYPE); \
	deque_iterator_imp_h(TYPE); \
	\
	struct deque_struct(TYPE) { \
		chunk(TYPE)* p_data; \
		size_t chunk_begin; \
		size_t chunk_count; \
		size_t chunk_capacity; \
		size_t size; \
	}; \
	\
	bool deque_chunk_func(reserve, TYPE)(deque(TYPE)* p_d, size_t size); \
	bool deque_chunk_func(push_front, TYPE)(deque(TYPE)* p_d); \
	bool deque_chunk_func(push_back, TYPE)(deque(TYPE)* p_d); \
	bool deque_chunk_func(pop_front, TYPE)(deque(TYPE)* p_d); \
	bool deque_chunk_func(pop_back, TYPE)(deque(TYPE)* p_d); \
	chunk(TYPE)* deque_chunk_func(at, TYPE)(deque(TYPE)* p_d, size_t index); \
	\
	bool deque_func(resize, TYPE)(deque(TYPE)* p_d, size_t size); \
	void deque_func(init, TYPE)(deque(TYPE)* p_d); \
	void deque_func(clear, TYPE)(deque(TYPE)* p_d); \
	void deque_func(free, TYPE)(deque(TYPE)* p_d); \
	bool deque_func(push_front, TYPE)(deque(TYPE)* p_d, TYPE item); \
	bool deque_func(push_back, TYPE)(deque(TYPE)* p_d, TYPE item); \
	bool deque_func(pop_front, TYPE)(deque(TYPE)* p_d); \
	bool deque_func(pop_back, TYPE)(deque(TYPE)* p_d); \
	TYPE* deque_func(at, TYPE)(deque(TYPE)* p_d, size_t index); \
	deque_iterator(TYPE) deque_func(begin, TYPE)(deque(TYPE)* p_d); \
	deque_iterator(TYPE) deque_func(rbegin, TYPE)(deque(TYPE)* p_d); \
	deque_iterator(TYPE) deque_func(seek, TYPE)(deque(TYPE)* p_d, size_t index);

#define deque_imp_c(TYPE) \
	chunk_imp_c(TYPE); \
	deque_iterator_imp_c(TYPE) \
	\
	bool deque_chunk_func(reserve, TYPE)(deque(TYPE)* p_d, size_t size) { \
		if (p_d->chunk_capacity >= size) return true; \
		size_t capacity = p_d->chunk_capacity == 0 ? 4 : p_d->chunk_capacity; \
		while (capacity < size) capacity <<= 1; \
		chunk(TYPE)* p_new_data = (chunk(TYPE)*)calloc(capacity, sizeof(chunk(TYPE))); \
		if (!p_new_data) return false; \
		for (size_t i = 0; i < p_d->chunk_count; i++) { \
			size_t old_index = (p_d->chunk_begin + i) % p_d->chunk_capacity; \
			p_new_data[i] = p_d->p_data[old_index]; \
			p_d->p_data[old_index].p_data = NULL; \
		} \
		for (size_t i = 0; i < p_d->chunk_capacity; i++) { \
			if (p_d->p_data[i].p_data) free(p_d->p_data[i].p_data); \
		} \
		free(p_d->p_data); \
		p_d->p_data = p_new_data; \
		p_d->chunk_begin = 0; \
		p_d->chunk_capacity = capacity; \
		return true; \
	} \
	\
	chunk(TYPE)* deque_chunk_func(at, TYPE)(deque(TYPE)* p_d, size_t index) { \
		return &p_d->p_data[(p_d->chunk_begin + index) % p_d->chunk_capacity]; \
	} \
	\
	bool deque_chunk_func(push_front, TYPE)(deque(TYPE)* p_d) { \
		if (!deque_chunk_func(reserve, TYPE)(p_d, p_d->chunk_count + 1)) return false; \
		p_d->chunk_begin = (p_d->chunk_begin + p_d->chunk_capacity - 1) % p_d->chunk_capacity; \
		p_d->chunk_count++; \
		chunk(TYPE)* p_target = deque_chunk_front(TYPE, p_d); \
		p_target->begin = 0; \
		p_target->size = 0; \
		return true; \
	} \
	\
	bool deque_chunk_func(push_back, TYPE)(deque(TYPE)* p_d) { \
		if (!deque_chunk_func(reserve, TYPE)(p_d, p_d->chunk_count + 1)) return false; \
		p_d->chunk_count++; \
		chunk(TYPE)* p_target = deque_chunk_back(TYPE, p_d); \
		p_target->begin = 0; \
		p_target->size = 0; \
		return true; \
	} \
	\
	bool deque_chunk_func(pop_front, TYPE)(deque(TYPE)* p_d) { \
		if (p_d->chunk_count == 0) return false; \
		p_d->chunk_begin = (p_d->chunk_begin + 1) % p_d->chunk_capacity; \
		p_d->chunk_count--; \
		return true; \
	} \
	\
	bool deque_chunk_func(pop_back, TYPE)(deque(TYPE)* p_d) { \
		if (p_d->chunk_count == 0) return false; \
		p_d->chunk_count--; \
		return true; \
	} \
	\
	void deque_func(init, TYPE)(deque(TYPE)* p_d) { \
		memset(p_d, 0, sizeof(deque(TYPE))); \
	} \
	\
	void deque_func(clear, TYPE)(deque(TYPE)* p_d) { \
		p_d->chunk_count = 0; \
		p_d->chunk_begin = 0; \
		p_d->size = 0; \
	} \
	\
	void deque_func(free, TYPE)(deque(TYPE)* p_d) { \
		if (p_d->p_data) { \
			for (size_t i = 0; i < p_d->chunk_capacity; i++) { \
				if (p_d->p_data[i].p_data) free(p_d->p_data[i].p_data); \
			} \
			free(p_d->p_data); \
		} \
		deque_func(init, TYPE)(p_d); \
	} \
	\
	bool deque_func(push_front, TYPE)(deque(TYPE)* p_d, TYPE item) { \
		if (p_d->chunk_count == 0 || deque_chunk_front(TYPE, p_d)->size == cctl_deque_chunk_max) { \
			if (!deque_chunk_func(push_front, TYPE)(p_d)) return false; \
		} \
		if (!chunk_func(push_front, TYPE)(deque_chunk_front(TYPE, p_d), item)) return false; \
		p_d->size++; \
		return true; \
	} \
	\
	bool deque_func(push_back, TYPE)(deque(TYPE)* p_d, TYPE item) { \
		if (p_d->chunk_count == 0 || deque_chunk_back(TYPE, p_d)->size == cctl_deque_chunk_max) { \
			if (!deque_chunk_func(push_back, TYPE)(p_d)) return false; \
		} \
		if (!chunk_func(push_back, TYPE)(deque_chunk_back(TYPE, p_d), item)) return false; \
		p_d->size++; \
		return true; \
	} \
	\
	bool deque_func(pop_front, TYPE)(deque(TYPE)* p_d) { \
		if (p_d->size == 0) return false; \
		chunk_func(pop_front, TYPE)(deque_chunk_front(TYPE, p_d)); \
		p_d->size--; \
		if (deque_chunk_front(TYPE, p_d)->size == 0) { \
			deque_chunk_func(pop_front, TYPE)(p_d); \
		} \
		return true; \
	} \
	\
	bool deque_func(pop_back, TYPE)(deque(TYPE)* p_d) { \
		if (p_d->size == 0) return false; \
		chunk_func(pop_back, TYPE)(deque_chunk_back(TYPE, p_d)); \
		p_d->size--; \
		if (deque_chunk_back(TYPE, p_d)->size == 0) { \
			deque_chunk_func(pop_back, TYPE)(p_d); \
		} \
		return true; \
	} \
	\
	TYPE* deque_func(at, TYPE)(deque(TYPE)* p_d, size_t index) { \
		size_t front_size = deque_chunk_front(TYPE, p_d)->size; \
		if (index < front_size) \
			return chunk_func(at, TYPE)(deque_chunk_front(TYPE, p_d), index); \
		index -= front_size; \
		size_t chunk_idx = index / cctl_deque_chunk_max + 1; \
		index %= cctl_deque_chunk_max; \
		return chunk_func(at, TYPE)(deque_chunk_func(at, TYPE)(p_d, chunk_idx), index); \
	} \
	\
	bool deque_func(resize, TYPE)(deque(TYPE)* p_d, size_t size) { \
		if (size > p_d->size) { \
			TYPE none; \
			memset(&none, 0, sizeof(TYPE)); \
			for (size_t i = p_d->size; i < size; i++) { \
				if (!deque_func(push_back, TYPE)(p_d, none)) return false; \
			} \
		} \
		else if (size < p_d->size) { \
			size_t temp_size = p_d->size; \
			for (size_t i = size; i < temp_size; i++) { \
				deque_func(pop_back, TYPE)(p_d); \
			} \
		} \
		return true; \
	} \
	\
	deque_iterator(TYPE) deque_func(begin, TYPE)(deque(TYPE)* p_d) { \
		deque_iterator(TYPE) it = { p_d, 0 }; \
		return it; \
	} \
	\
	deque_iterator(TYPE) deque_func(rbegin, TYPE)(deque(TYPE)* p_d) { \
		deque_iterator(TYPE) it = { p_d, p_d->size > 0 ? p_d->size - 1 : (size_t)-1 }; \
		return it; \
	} \
	\
	deque_iterator(TYPE) deque_func(seek, TYPE)(deque(TYPE)* p_d, size_t index) { \
		deque_iterator(TYPE) it = { p_d, index < p_d->size ? index : (size_t)-1 }; \
		return it; \
	}

#endif