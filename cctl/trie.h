#ifndef __CCTL_TRIE_H__
#define __CCTL_TRIE_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "cctl.h"

#define trie(TYPE) cctl_join(TYPE, trie)
#define trie_func(FUNC, TYPE) cctl_join(trie(TYPE), FUNC)
#define trie_struct(TYPE) cctl_join(trie(TYPE), struct)

#define trie_init(TYPE, p_t) trie_func(init, TYPE)(p_t)
#define trie_free(TYPE, p_t) trie_func(free, TYPE)(p_t)
#define trie_insert(TYPE, p_t, key, item) trie_func(insert, TYPE)(p_t, key, item)
#define trie_find(TYPE, p_t, key) trie_func(find, TYPE)(p_t, key)

#define trie_fd(TYPE) \
	typedef struct trie_struct(TYPE) trie(TYPE); \

#define trie_imp_h(TYPE) \
	struct trie_struct(TYPE) { \
		TYPE data; \
		bool existence; \
		trie(TYPE)* children[256]; \
	}; \
	\
	void trie_func(init, TYPE)(trie(TYPE)* p_t); \
	void trie_func(free, TYPE)(trie(TYPE)* p_t); \
	TYPE* trie_func(insert, TYPE)(trie(TYPE)* p_t, const char* key, TYPE item); \
	TYPE* trie_func(find, TYPE)(trie(TYPE)* p_t, const char* key);

#define trie_imp_c(TYPE) \
	void trie_func(init, TYPE)(trie(TYPE)* p_t) { \
		memset(&(p_t->data), 0, sizeof(TYPE)); \
		p_t->existence = false; \
		for (size_t i = 0; i < 256; i++) { \
			p_t->children[i] = NULL; \
		} \
	} \
	\
	void trie_func(free, TYPE)(trie(TYPE)* p_t) { \
		for (size_t i = 0; i < 256; i++) { \
			if (p_t->children[i]) { \
				trie_func(free, TYPE)(p_t->children[i]); \
				free(p_t->children[i]); \
			} \
		} \
	} \
	\
	TYPE* trie_func(insert, TYPE)(trie(TYPE)* p_t, const char* key, TYPE item) { \
		if (!(*key)) { \
			p_t->data = item; \
			p_t->existence = true; \
			return &(p_t->data); \
		} \
		if (!(p_t->children[(uint8_t) *key])) { \
			p_t->children[(uint8_t) *key] = (trie(TYPE)*) malloc(sizeof(trie(TYPE))); \
			if (!(p_t->children[(uint8_t) *key])) { \
				return NULL; \
			} \
			trie_func(init, TYPE)(p_t->children[(uint8_t) *key]); \
		} \
		return trie_func(insert, TYPE)(p_t->children[(uint8_t) *key], key + 1, item); \
	} \
	\
	TYPE* trie_func(find, TYPE)(trie(TYPE)* p_t, const char* key) { \
		if (!(*key)) { \
			if (p_t->existence) return &(p_t->data); \
			else return NULL; \
		} \
		if (!(p_t->children[(uint8_t) *key])) { \
			return NULL; \
		} \
		return trie_func(find, TYPE)(p_t->children[(uint8_t) *key], key + 1); \
	}

#endif