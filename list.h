#ifndef __CCTL_LIST_H__
#define __CCTL_LIST_H__

#include <stdbool.h>
#include <stdlib.h>

#include "cctl.h"

#define node(TYPE) cctl_join(TYPE, node)
#define node_func(FUNC, TYPE) cctl_join(node(TYPE), FUNC)
#define node_struct(TYPE) cctl_join(node(TYPE), struct)

#define node_imp_h(TYPE) \
	typedef struct node_struct(TYPE) node(TYPE); \
	\
	struct node_struct(TYPE) { \
		TYPE data; \
		node(TYPE)* p_prev; \
		node(TYPE)* p_next; \
	}; \
	\
	void node_func(init, TYPE)(node(TYPE)* p_node);

#define node_imp_c(TYPE) \
	void node_func(init, TYPE)(node(TYPE)* p_node) { \
		p_node->p_prev = p_node->p_next = NULL; \
	}

#define list_iterator(TYPE) cctl_join(TYPE, list_iterator)
#define list_iterator_func(FUNC, TYPE) cctl_join(list_iterator(TYPE), FUNC)
#define list_iterator_struct(TYPE) cctl_join(list_iterator(TYPE), struct)

#define list_iterator_get(TYPE, p_it) ((p_it)->p_node ? &((p_it)->p_node->data) : NULL)
#define list_iterator_is_valid(TYPE, p_it) ((p_it)->p_node != NULL)
#define list_iterator_next(TYPE, p_it) ((p_it)->p_node = (p_it)->p_node->p_next)
#define list_iterator_prev(TYPE, p_it) ((p_it)->p_node = (p_it)->p_node->p_prev)

#define list_iterator_insert_after(TYPE, p_it, item) list_iterator_func(insert_after, TYPE)(p_it, item)
#define list_iterator_insert_before(TYPE, p_it, item) list_iterator_func(insert_before, TYPE)(p_it, item)
#define list_iterator_remove(TYPE, p_it) list_iterator_func(remove, TYPE)(p_it)

#define list_iterator_imp_h(TYPE) \
	typedef struct list_iterator_struct(TYPE) list_iterator(TYPE); \
	struct list_iterator_struct(TYPE) { \
		list(TYPE)* p_list; \
		node(TYPE)* p_node; \
	}; \
	\
	bool list_iterator_func(insert_after, TYPE)(list_iterator(TYPE)* p_it, TYPE item); \
	bool list_iterator_func(insert_before, TYPE)(list_iterator(TYPE)* p_it, TYPE item); \
	bool list_iterator_func(remove, TYPE)(list_iterator(TYPE)* p_it);

#define list_iterator_imp_c(TYPE) \
	bool list_iterator_func(insert_after, TYPE)(list_iterator(TYPE)* p_it, TYPE item) { \
		if (!p_it || !p_it->p_node) return false; \
		node(TYPE)* p_target_node = p_it->p_node; \
		node(TYPE)* p_new_node = (node(TYPE)*) malloc(sizeof(node(TYPE))); \
		if (!p_new_node) return false; \
		p_new_node->data = item; \
		p_new_node->p_prev = p_target_node; \
		p_new_node->p_next = p_target_node->p_next; \
		if (p_target_node->p_next) p_target_node->p_next->p_prev = p_new_node; \
		else p_it->p_list->p_back = p_new_node; \
		p_target_node->p_next = p_new_node; \
		p_it->p_list->size++; \
		return true; \
	} \
	\
	bool list_iterator_func(insert_before, TYPE)(list_iterator(TYPE)* p_it, TYPE item) { \
		if (!p_it || !p_it->p_node) return false; \
		node(TYPE)* p_target_node = p_it->p_node; \
		node(TYPE)* p_new_node = (node(TYPE)*) malloc(sizeof(node(TYPE))); \
		if (!p_new_node) return false; \
		p_new_node->data = item; \
		p_new_node->p_next = p_target_node; \
		p_new_node->p_prev = p_target_node->p_prev; \
		if (p_target_node->p_prev) p_target_node->p_prev->p_next = p_new_node; \
		else p_it->p_list->p_front = p_new_node; \
		p_target_node->p_prev = p_new_node; \
		p_it->p_list->size++; \
		return true; \
	} \
	\
	bool list_iterator_func(remove, TYPE)(list_iterator(TYPE)* p_it) { \
		if (!p_it || !p_it->p_node || p_it->p_list->size == 0) return false; \
		node(TYPE)* p_target_node = p_it->p_node; \
		if (p_target_node == p_it->p_list->p_front) { \
			p_it->p_node = p_target_node->p_next; \
			return list_func(pop_front, TYPE)(p_it->p_list); \
		} \
		if (p_target_node == p_it->p_list->p_back) { \
			p_it->p_node = NULL; \
			return list_func(pop_back, TYPE)(p_it->p_list); \
		} \
		p_target_node->p_prev->p_next = p_target_node->p_next; \
		p_target_node->p_next->p_prev = p_target_node->p_prev; \
		p_it->p_node = p_target_node->p_next; \
		free(p_target_node); \
		p_it->p_list->size--; \
		return true; \
	}

#define list(TYPE) cctl_join(TYPE, list)
#define list_func(FUNC, TYPE) cctl_join(list(TYPE), FUNC)
#define list_struct(TYPE) cctl_join(list(TYPE), struct)

#define list_init(TYPE, p_l) list_func(init, TYPE)(p_l)
#define list_free(TYPE, p_l) list_func(free, TYPE)(p_l)
#define list_clear(TYPE, p_l) list_func(clear, TYPE)(p_l)
#define list_push_front(TYPE, p_l, item) list_func(push_front, TYPE)(p_l, item)
#define list_push_back(TYPE, p_l, item) list_func(push_back, TYPE)(p_l, item)
#define list_pop_front(TYPE, p_l) list_func(pop_front, TYPE)(p_l)
#define list_pop_back(TYPE, p_l) list_func(pop_back, TYPE)(p_l)

#define list_node_front(TYPE, p_l) ((p_l)->p_front)
#define list_node_back(TYPE, p_l) ((p_l)->p_back)
#define list_node_at(TYPE, p_l, index) list_func(node_at, TYPE)(p_l, index)

#define list_front(TYPE, p_l) ((p_l)->p_front ? &((p_l)->p_front->data) : NULL)
#define list_back(TYPE, p_l) ((p_l)->p_back ? &((p_l)->p_back->data) : NULL)

#define list_begin(TYPE, p_l) list_func(begin, TYPE)(p_l)
#define list_rbegin(TYPE, p_l) list_func(rbegin, TYPE)(p_l)
#define list_end(TYPE, p_l) list_func(end, TYPE)(p_l)
#define list_seek(TYPE, p_l, index) list_func(seek, TYPE)(p_l, index)
#define list_foreach(TYPE, p_l, it) for(list_iterator(TYPE) it = list_begin(TYPE, p_l); list_iterator_is_valid(TYPE, &it); list_iterator_next(TYPE, &it))
#define list_rforeach(TYPE, p_l, it) for (list_iterator(TYPE) it = list_rbegin(TYPE, p_l); list_iterator_is_valid(TYPE, &it); list_iterator_prev(TYPE, &it))

#define list_fd(TYPE) \
	typedef struct list_struct(TYPE) list(TYPE);

#define list_imp_h(TYPE) \
	node_imp_h(TYPE); \
	list_iterator_imp_h(TYPE); \
	\
	struct list_struct(TYPE) { \
		node(TYPE)* p_front; \
		node(TYPE)* p_back; \
		size_t size; \
	}; \
	\
	void list_func(init, TYPE)(list(TYPE)* p_l); \
	void list_func(free, TYPE)(list(TYPE)* p_l); \
	void list_func(clear, TYPE)(list(TYPE)* p_l); \
	bool list_func(push_front, TYPE)(list(TYPE)* p_l, TYPE item); \
	bool list_func(push_back, TYPE)(list(TYPE)* p_l, TYPE item); \
	bool list_func(pop_back, TYPE)(list(TYPE)* p_l); \
	bool list_func(pop_front, TYPE)(list(TYPE)* p_l); \
	node(TYPE)* list_func(node_at, TYPE)(list(TYPE)* p_l, size_t index); \
	list_iterator(TYPE) list_func(begin, TYPE)(list(TYPE)* p_l); \
	list_iterator(TYPE) list_func(end, TYPE)(list(TYPE)* p_l); \
	list_iterator(TYPE) list_func(seek, TYPE)(list(TYPE)* p_l, size_t index); \

#define list_imp_c(TYPE) \
	node_imp_c(TYPE) \
	list_iterator_imp_c(TYPE) \
	\
	void list_func(init, TYPE)(list(TYPE)* p_l) { \
		p_l->p_front = p_l->p_back = NULL; \
		p_l->size = 0; \
	} \
	\
	void list_func(clear, TYPE)(list(TYPE)* p_l) { \
		while (p_l->size > 0) list_func(pop_back, TYPE)(p_l); \
	} \
	\
	void list_func(free, TYPE)(list(TYPE)* p_l) { \
		list_func(clear, TYPE)(p_l); \
	} \
	\
	bool list_func(push_front, TYPE)(list(TYPE)* p_l, TYPE item) { \
		node(TYPE)* p_new_node = (node(TYPE)*) malloc(sizeof(node(TYPE))); \
		if (!p_new_node) return NULL; \
		p_new_node->data = item; \
		p_new_node->p_prev = NULL; \
		p_new_node->p_next = p_l->p_front; \
		if (p_l->p_front) p_l->p_front->p_prev = p_new_node; \
		else p_l->p_back = p_new_node; \
		p_l->p_front = p_new_node; \
		p_l->size++; \
		return p_new_node; \
	} \
	\
	bool list_func(push_back, TYPE)(list(TYPE)* p_l, TYPE item) { \
		node(TYPE)* p_new_node = (node(TYPE)*) malloc(sizeof(node(TYPE))); \
		if (!p_new_node) return NULL; \
		p_new_node->data = item; \
		p_new_node->p_next = NULL; \
		p_new_node->p_prev = p_l->p_back; \
		if (p_l->p_back) p_l->p_back->p_next = p_new_node; \
		else p_l->p_front = p_new_node; \
		p_l->p_back = p_new_node; \
		p_l->size++; \
		return p_new_node; \
	} \
	\
	bool list_func(pop_front, TYPE)(list(TYPE)* p_l) { \
		if (p_l->size == 0) return false; \
		node(TYPE)* p_node_node = p_l->p_front; \
		p_l->p_front = p_node_node->p_next; \
		if (p_l->p_front) p_l->p_front->p_prev = NULL; \
		else p_l->p_back = NULL; \
		p_l->size--; \
		free(p_node_node); \
		return true; \
	} \
	\
	bool list_func(pop_back, TYPE)(list(TYPE)* p_l) { \
		if (p_l->size == 0) return false; \
		node(TYPE)* p_node_node = p_l->p_back; \
		p_l->p_back = p_node_node->p_prev; \
		if (p_l->p_back) p_l->p_back->p_next = NULL; \
		else p_l->p_front = NULL; \
		p_l->size--; \
		free(p_node_node); \
		return true; \
	} \
	node(TYPE)* list_func(node_at, TYPE)(list(TYPE)* p_l, size_t index) { \
		if (index >= p_l->size) return NULL; \
		bool is_reverse = (index > (p_l->size / 2)); \
		node(TYPE)* p_temp_node = is_reverse ? list_node_back(TYPE, p_l) : list_node_front(TYPE, p_l); \
		if (index == 0 || index == (p_l->size - 1)) return p_temp_node; \
		size_t temp_index = is_reverse ? (p_l->size - 1) : 0; \
		while (temp_index != index) { \
			if (is_reverse) { \
				temp_index--; \
				p_temp_node = p_temp_node->p_prev; \
			} \
			else { \
				temp_index++; \
				p_temp_node = p_temp_node->p_next; \
			} \
		} \
		return p_temp_node; \
	} \
	\
	TYPE* list_func(at, TYPE)(list(TYPE)* p_l, size_t index) { \
		if (index >= p_l->size) return NULL; \
		node(TYPE)* p_node_node = list_func(node_at, TYPE)(p_l, index); \
		if (!p_node_node) return NULL; \
		return &(p_node_node->data); \
	} \
	\
	list_iterator(TYPE) list_func(begin, TYPE)(list(TYPE)* p_l) { \
		list_iterator(TYPE) it = { p_l, p_l->p_front }; \
		return it; \
	} \
	\
	list_iterator(TYPE) list_func(rbegin, TYPE)(list(TYPE)* p_l) { \
		list_iterator(TYPE) it = { p_l, p_l->p_back }; \
		return it; \
	} \
	\
	list_iterator(TYPE) list_func(end, TYPE)(list(TYPE)* p_l) { \
		list_iterator(TYPE) it = { p_l, NULL }; \
		return it; \
	} \
	\
	list_iterator(TYPE) list_func(seek, TYPE)(list(TYPE)* p_l, size_t index) { \
		if (index >= p_l->size) return list_func(end, TYPE)(p_l); \
		list_iterator(TYPE) it = { p_l, list_func(node_at, TYPE)(p_l, index) }; \
		return it; \
	}

#endif