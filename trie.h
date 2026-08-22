#ifndef __CCTL_TRIE_H__
#define __CCTL_TRIE_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "cctl.h"

#define trie_iterator(TYPE) cctl_join(TYPE, trie_iterator)
#define trie_iterator_func(FUNC, TYPE) cctl_join(trie_iterator(TYPE), FUNC)
#define trie_iterator_struct(TYPE) cctl_join(trie_iterator(TYPE), struct)

#define trie_iterator_get(TYPE, p_it) ((p_it)->p_node ? &((p_it)->p_node->data) : NULL)
#define trie_iterator_is_valid(TYPE, p_it) ((p_it)->p_node != NULL)
#define trie_iterator_next(TYPE, p_it) trie_iterator_func(next, TYPE)(p_it)
#define trie_iterator_prev(TYPE, p_it) trie_iterator_func(prev, TYPE)(p_it)
#define trie_iterator_get_key(TYPE, p_it) trie_func(get_key, TYPE)((p_it)->p_node)

#define trie_iterator_insert_after(TYPE, p_it, item) (false)
#define trie_iterator_insert_before(TYPE, p_it, item) (false)
#define trie_iterator_remove(TYPE, p_it) (false)

#define trie_iterator_imp_h(TYPE) \
	typedef struct trie_iterator_struct(TYPE) trie_iterator(TYPE); \
	\
	struct trie_iterator_struct(TYPE) { \
		trie(TYPE)* p_node; \
	}; \
	\
	void trie_iterator_func(next, TYPE)(trie_iterator(TYPE)* p_it); \
	void trie_iterator_func(prev, TYPE)(trie_iterator(TYPE)* p_it);

#define trie_iterator_imp_c(TYPE) \
	void trie_iterator_func(next, TYPE)(trie_iterator(TYPE)* p_it) { \
		if (!p_it || !(p_it->p_node)) return; \
		trie(TYPE)* p_current = p_it->p_node; \
		size_t next_child_index = 0; \
		while (true) { \
			bool found_child = false; \
			for (size_t i = next_child_index; i < 256; i++) { \
				if (p_current->children[i]) { \
					p_current = p_current->children[i]; \
					if (p_current->existence) { \
						p_it->p_node = p_current; \
						return; \
					} \
					next_child_index = 0; \
					found_child = true; \
					break; \
				} \
			} \
			if (found_child) continue; \
			if (p_current->p_parent == NULL) { \
				p_it->p_node = NULL; \
				return; \
			} \
			next_child_index = p_current->current_character + 1; \
			p_current = p_current->p_parent; \
		} \
	} \
	\
	void trie_iterator_func(prev, TYPE)(trie_iterator(TYPE)* p_it) { \
		if (!p_it || !(p_it->p_node)) return; \
		trie(TYPE)* p_current = p_it->p_node; \
		while (p_current->p_parent != NULL) { \
			uint8_t current_character = p_current->current_character; \
			trie(TYPE)* p_parent = p_current->p_parent; \
			bool found_sibling = false; \
			for (size_t i = current_character; i > 0; i--) { \
				if (p_parent->children[i - 1]) { \
					p_current = p_parent->children[i - 1]; \
					found_sibling = true; \
					break; \
				} \
			} \
			if (found_sibling) { \
				while (true) { \
					bool found_child = false; \
					for (size_t i = 256; i > 0; i--) { \
						if (p_current->children[i - 1]) { \
							p_current = p_current->children[i - 1]; \
							found_child = true; \
							break; \
						} \
					} \
					if (!found_child) { \
						p_it->p_node = p_current; \
						return; \
					} \
				} \
			} \
			p_current = p_parent; \
			if (p_current->existence) { \
				p_it->p_node = p_current; \
				return; \
			} \
		} \
		p_it->p_node = NULL; \
	}

#define trie(TYPE) cctl_join(TYPE, trie)
#define trie_func(FUNC, TYPE) cctl_join(trie(TYPE), FUNC)
#define trie_struct(TYPE) cctl_join(trie(TYPE), struct)

#define trie_init(TYPE, p_t) trie_func(init, TYPE)(p_t)
#define trie_free(TYPE, p_t) trie_func(free, TYPE)(p_t)
#define trie_insert(TYPE, p_t, p_key, item) trie_func(insert, TYPE)(p_t, p_key, item)
#define trie_remove(TYPE, p_t, p_key) trie_func(remove, TYPE)(p_t, p_key)
#define trie_find(TYPE, p_t, p_key) trie_func(find, TYPE)(p_t, p_key)
#define trie_get_key(TYPE, p_node) trie_func(get_key, TYPE)(p_node)

#define trie_begin(TYPE, p_t) trie_func(begin, TYPE)(p_t)
#define trie_rbegin(TYPE, p_t) trie_func(rbegin, TYPE)(p_t)
#define trie_foreach(TYPE, p_t, it) for (trie_iterator(TYPE) it = trie_begin(TYPE, p_t); trie_iterator_is_valid(TYPE, &it); trie_iterator_next(TYPE, &it))
#define trie_rforeach(TYPE, p_t, it) for (trie_iterator(TYPE) it = trie_rbegin(TYPE, p_t); trie_iterator_is_valid(TYPE, &it); trie_iterator_prev(TYPE, &it))

#define trie_fd(TYPE) \
	typedef struct trie_struct(TYPE) trie(TYPE);

#define trie_imp_h(TYPE) \
	trie_iterator_imp_h(TYPE); \
	\
	struct trie_struct(TYPE) { \
		TYPE data; \
		bool existence; \
		uint8_t current_character; \
		trie(TYPE)* p_parent; \
		trie(TYPE)* ap_children[256]; \
	}; \
	\
	void trie_func(init, TYPE)(trie(TYPE)* p_t); \
	void trie_func(free, TYPE)(trie(TYPE)* p_t); \
	TYPE* trie_func(insert, TYPE)(trie(TYPE)* p_t, const char* p_key, TYPE item); \
	void trie_func(remove, TYPE)(trie(TYPE)* p_t, const char* p_key); \
	TYPE* trie_func(find, TYPE)(trie(TYPE)* p_t, const char* p_key); \
	char* trie_func(get_key, TYPE)(trie(TYPE)* p_node); \
	trie_iterator(TYPE) trie_func(begin, TYPE)(trie(TYPE)* p_t); \
	trie_iterator(TYPE) trie_func(rbegin, TYPE)(trie(TYPE)* p_t);

#define trie_imp_c(TYPE) \
	trie_iterator_imp_c(TYPE) \
	\
	void trie_func(init, TYPE)(trie(TYPE)* p_t) { \
		memset(p_t, 0, sizeof(trie(TYPE))); \
	} \
	\
	void trie_func(free, TYPE)(trie(TYPE)* p_t) { \
		for (size_t i = 0; i < 256; i++) { \
			if (p_t->ap_children[i]) { \
				trie_func(free, TYPE)(p_t->ap_children[i]); \
				free(p_t->ap_children[i]); \
			} \
		} \
	} \
	\
	TYPE* trie_func(insert, TYPE)(trie(TYPE)* p_t, const char* p_key, TYPE item) { \
		trie(TYPE)* p_current = p_t; \
		while (*p_key) { \
			uint8_t index = (uint8_t) *p_key; \
			if (!(p_current->ap_children[index])) { \
				p_current->ap_children[index] = (trie(TYPE)*) malloc(sizeof(trie(TYPE))); \
				if (!(p_current->ap_children[index])) return NULL; \
				trie_func(init, TYPE)(p_current->ap_children[index]); \
				p_current->ap_children[index]->p_parent = p_current; \
				p_current->ap_children[index]->current_character = index; \
			} \
			p_current = p_current->ap_children[index]; \
			p_key++; \
		} \
		p_current->data = item; \
		p_current->existence = true; \
		return &(p_current->data); \
	} \
	\
	void trie_func(remove, TYPE)(trie(TYPE)* p_t, const char* p_key) { \
		trie(TYPE)* p_current = p_t; \
		while (*p_key) { \
			uint8_t index = (uint8_t) *p_key; \
			if (!(p_current->ap_children[index])) return; \
			p_current = p_current->ap_children[index]; \
			p_key++; \
		} \
		if (!p_current->existence) return; \
		p_current->existence = false; \
		while (p_current != p_t) { \
			if (p_current->existence) break; \
			bool has_children = false; \
			for (size_t i = 0; i < 256; i++) { \
				if (p_current->ap_children[i]) { \
					has_children = true; \
					break; \
				} \
			} \
			if (has_children) break; \
			trie(TYPE)* p_parent = p_current->p_parent; \
			p_parent->ap_children[p_current->current_character] = NULL; \
			free(p_current); \
			p_current = p_parent; \
		} \
	} \
	\
	TYPE* trie_func(find, TYPE)(trie(TYPE)* p_t, const char* p_key) { \
		trie(TYPE)* p_current = p_t; \
		while (*p_key) { \
			uint8_t index = (uint8_t) *p_key; \
			if (!(p_current->ap_children[index])) return NULL; \
			p_current = p_current->ap_children[index]; \
			p_key++; \
		} \
		return p_current->existence ? &(p_current->data) : NULL; \
	} \
	\
	char* trie_func(get_key, TYPE)(trie(TYPE)* p_node) { \
		if (!p_node) return NULL; \
		size_t length = 0; \
		trie(TYPE)* p_current = p_node; \
		while (p_current->p_parent != NULL) { \
			length++; \
			p_current = p_current->p_parent; \
		} \
		char* p_key = (char*) malloc(length + 1); \
		if (!p_key) return NULL; \
		p_key[length] = '\0'; \
		p_current = p_node; \
		while (p_current->p_parent != NULL) { \
			length--; \
			p_key[length] = (char) p_current->current_character; \
			p_current = p_current->p_parent; \
		} \
		return p_key; \
	} \
	\
	trie_iterator(TYPE) trie_func(begin, TYPE)(trie(TYPE)* p_t) { \
		trie_iterator(TYPE) it = {}; \
		if (!p_t) return it; \
		it.p_node = p_t; \
		if (!p_t->existence) { \
			trie_iterator_func(next, TYPE)(&it); \
		} \
		return it; \
	} \
	\
	trie_iterator(TYPE) trie_func(rbegin, TYPE)(trie(TYPE)* p_t) { \
		trie_iterator(TYPE) it = {}; \
		if (!p_t) return it; \
		trie(TYPE)* p_current = p_t; \
		while (true) { \
			bool found_child = false; \
			for (size_t i = 256; i > 0; i--) { \
				if (p_current->children[i - 1]) { \
					p_current = p_current->children[i - 1]; \
					found_child = true; \
					break; \
				} \
			} \
			if (!found_child) break; \
		} \
		if (p_current->existence) it.p_node = p_current; \
		else it.p_node = NULL; \
		return it; \
	}

#endif