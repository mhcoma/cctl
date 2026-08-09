#ifndef __CCTL_TRIE256_H__
#define __CCTL_TRIE256_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

#include "cctl.h"

#define trie256(TYPE) cctl_join(TYPE, trie256)
#define trie256_func(FUNC, TYPE) cctl_join(trie256(TYPE), FUNC)
#define trie256_struct(TYPE) cctl_join(trie256(TYPE), struct)

#define trie256_init(TYPE, p_t) trie256_func(init, TYPE)(p_t)
#define trie256_free(TYPE, p_t) trie256_func(free, TYPE)(p_t)
#define trie256_insert(TYPE, p_t, key, item) trie256_func(insert, TYPE)(p_t, key, item)
#define trie256_remove(TYPE, p_t, key) trie256_func(remove, TYPE)(p_t, key)
#define trie256_find(TYPE, p_t, key) trie256_func(find, TYPE)(p_t, key)

#define trie256_fd(TYPE) \
	typedef struct trie256_struct(TYPE) trie256(TYPE); \

#define trie256_imp_h(TYPE) \
	struct trie256_struct(TYPE) { \
		TYPE data; \
		bool existence; \
		trie256(TYPE)* children[256]; \
	}; \
	\
	void trie256_func(init, TYPE)(trie256(TYPE)* p_t); \
	void trie256_func(free, TYPE)(trie256(TYPE)* p_t); \
	TYPE* trie256_func(insert, TYPE)(trie256(TYPE)* p_t, const char* key, TYPE item); \
	bool trie256_func(remove_recurse, TYPE)(trie256(TYPE)* p_t, const char* key); \
	void trie256_func(remove, TYPE)(trie256(TYPE)* p_t, const char* key); \
	TYPE* trie256_func(find, TYPE)(trie256(TYPE)* p_t, const char* key);

#define trie256_imp_c(TYPE) \
	void trie256_func(init, TYPE)(trie256(TYPE)* p_t) { \
		memset(p_t, 0, sizeof(trie(TYPE))); \
	} \
	\
	void trie256_func(free, TYPE)(trie256(TYPE)* p_t) { \
		for (size_t i = 0; i < 256; i++) { \
			if (p_t->children[i]) { \
				trie256_func(free, TYPE)(p_t->children[i]); \
				free(p_t->children[i]); \
			} \
		} \
	} \
	\
	TYPE* trie256_func(insert, TYPE)(trie256(TYPE)* p_t, const char* key, TYPE item) { \
		if (!(*key)) { \
			p_t->data = item; \
			p_t->existence = true; \
			return &(p_t->data); \
		} \
		if (!(p_t->children[(uint8_t) *key])) { \
			p_t->children[(uint8_t) *key] = (trie256(TYPE)*) malloc(sizeof(trie256(TYPE))); \
			if (!(p_t->children[(uint8_t) *key])) { \
				return NULL; \
			} \
			trie256_func(init, TYPE)(p_t->children[(uint8_t) *key]); \
		} \
		return trie256_func(insert, TYPE)(p_t->children[(uint8_t) *key], key + 1, item); \
	} \
	\
	bool trie256_func(remove_recurse, TYPE)(trie256(TYPE)* p_t, const char* key) { \
		if (*key) { \
			if (p_t && p_t->children[(uint8_t) *key]) { \
				bool result = trie256_func(remove_recurse, TYPE)(p_t->children[(uint8_t) *key], key + 1); \
				if (result) { \
					free(p_t->children[(uint8_t) *key]); \
					p_t->children[(uint8_t) *key] = NULL; \
					if (!p_t->existence) { \
						bool flag = false; \
						for (size_t i = 0; i < 256; i++) { \
							if (p_t->children[i]) { \
								flag = true; \
								break; \
							} \
						} \
						if (flag) return false; \
						else return true; \
					} \
				} \
			} \
		} \
		else if (p_t->existence) { \
			bool flag = false; \
			for (size_t i = 0; i < 256; i++) { \
				if (p_t->children[i]) { \
					flag = true; \
					break; \
				} \
			} \
			if (flag) { \
				p_t->existence = false; \
				return false; \
			} \
			else return true; \
		} \
		return false; \
	} \
	\
	void trie256_func(remove, TYPE)(trie256(TYPE)* p_t, const char* key) { \
		trie256_func(remove_recurse, TYPE)(p_t, key); \
	} \
	TYPE* trie256_func(find, TYPE)(trie256(TYPE)* p_t, const char* key) { \
		if (!(*key)) { \
			if (p_t->existence) return &(p_t->data); \
			else return NULL; \
		} \
		if (!(p_t->children[(uint8_t) *key])) { \
			return NULL; \
		} \
		return trie256_func(find, TYPE)(p_t->children[(uint8_t) *key], key + 1); \
	}

#endif