#ifndef __CCTL_HASHMAP_H__
#define __CCTL_HASHMAP_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "cctl.h"
#include "pair.h"

#define CCTL_HASHMAP_EMPTY 0
#define CCTL_HASHMAP_VALID 1
#define CCTL_HASHMAP_DELETED 2

#define hashmap(KEY, VALUE) cctl_join3(KEY, VALUE, hashmap)
#define hashmap_func(FUNC, KEY, VALUE) cctl_join(hashmap(KEY, VALUE), FUNC)
#define hashmap_struct(KEY, VALUE) cctl_join(hashmap(KEY, VALUE), struct)

#define hashmap_slot(KEY, VALUE) cctl_join3(KEY, VALUE, hashmap_slot)
#define hashmap_slot_struct(KEY, VALUE) cctl_join(hashmap_slot(KEY, VALUE), struct)

#define hashmap_init(KEY, VALUE, p_hm, p_hasher) hashmap_func(init, KEY, VALUE)(p_hm, (p_hasher)->fp_hash, (p_hasher)->fp_comp)
#define hashmap_free(KEY, VALUE, p_hm) hashmap_func(free, KEY, VALUE)(p_hm)
#define hashmap_clear(KEY, VALUE, p_hm) hashmap_func(clear, KEY, VALUE)(p_hm)
#define hashmap_reserve(KEY, VALUE, p_hm, size) hashmap_func(reserve, KEY, VALUE)(p_hm, size)
#define hashmap_insert(KEY, VALUE, p_hm, key, item) hashmap_func(insert, KEY, VALUE)(p_hm, key, item)
#define hashmap_remove(KEY, VALUE, p_hm, key) hashmap_func(remove, KEY, VALUE)(p_hm, key)
#define hashmap_find(KEY, VALUE, p_hm, key) hashmap_func(find, KEY, VALUE)(p_hm, key)

#define hashmap_fd(KEY, VALUE) \
	pair_fd(KEY, VALUE); \
	typedef struct hashmap_slot_struct(KEY, VALUE) hashmap_slot(KEY, VALUE); \
	typedef struct hashmap_struct(KEY, VALUE) hashmap(KEY, VALUE);

#define hashmap_imp_h(KEY, VALUE) \
	pair_imp_h(KEY, VALUE); \
	\
	struct hashmap_slot_struct(KEY, VALUE) { \
		pair(KEY, VALUE) data; \
		uint32_t hash; \
		uint8_t state; \
	}; \
	\
	struct hashmap_struct(KEY, VALUE) { \
		hashmap_slot(KEY, VALUE)* p_slots; \
		size_t capacity; \
		size_t size; \
		size_t tombstone_count; \
		uint32_t (*fp_hash_func)(KEY); \
		bool (*fp_comp_func)(KEY, KEY); \
	}; \
	\
	void hashmap_func(init, KEY, VALUE)(hashmap(KEY, VALUE)* p_hm, uint32_t (*fp_hash_func)(KEY), bool (*fp_comp_func)(KEY, KEY)); \
	void hashmap_func(free, KEY, VALUE)(hashmap(KEY, VALUE)* p_hm); \
	void hashmap_func(clear, KEY, VALUE)(hashmap(KEY, VALUE)* p_hm); \
	bool hashmap_func(reserve, KEY, VALUE)(hashmap(KEY, VALUE)* p_hm, size_t new_capacity); \
	bool hashmap_func(insert, KEY, VALUE)(hashmap(KEY, VALUE)* p_hm, KEY key, VALUE value); \
	bool hashmap_func(remove, KEY, VALUE)(hashmap(KEY, VALUE)* p_hm, KEY key); \
	VALUE* hashmap_func(find, KEY, VALUE)(hashmap(KEY, VALUE)* p_hm, KEY key);

#define hashmap_imp_c(KEY, VALUE) \
	void hashmap_func(init, KEY, VALUE)(hashmap(KEY, VALUE)* p_hm, uint32_t (*fp_hash_func)(KEY), bool (*fp_comp_func)(KEY, KEY)) { \
		memset(p_hm, 0, sizeof(hashmap(KEY, VALUE))); \
		p_hm->fp_hash_func = fp_hash_func; \
		p_hm->fp_comp_func = fp_comp_func; \
	} \
	\
	void hashmap_func(free, KEY, VALUE)(hashmap(KEY, VALUE)* p_hm) { \
		if (p_hm->p_slots) free(p_hm->p_slots); \
		hashmap_func(init, KEY, VALUE)(p_hm, p_hm->fp_hash_func, p_hm->fp_comp_func); \
	} \
	\
	void hashmap_func(clear, KEY, VALUE)(hashmap(KEY, VALUE)* p_hm) { \
		if (p_hm->p_slots) { \
			memset(p_hm->p_slots, 0, p_hm->capacity * sizeof(hashmap_slot(KEY, VALUE))); \
		} \
		p_hm->size = 0; \
		p_hm->tombstone_count = 0; \
	} \
	\
	bool hashmap_func(reserve, KEY, VALUE)(hashmap(KEY, VALUE)* p_hm, size_t new_capacity) { \
		if (new_capacity < p_hm->size) new_capacity = p_hm->size; \
		size_t capacity = 16; \
		if (new_capacity > 0) while (capacity < new_capacity) capacity <<= 1; \
		if (capacity == p_hm->capacity && p_hm->tombstone_count == 0) return true; \
		hashmap_slot(KEY, VALUE)* p_new_slots = (hashmap_slot(KEY, VALUE)*)calloc(capacity, sizeof(hashmap_slot(KEY, VALUE))); \
		if (!p_new_slots) return false; \
		for (size_t i = 0; i < p_hm->capacity; i++) { \
			if (p_hm->p_slots[i].state == CCTL_HASHMAP_VALID) { \
				size_t index = p_hm->p_slots[i].hash & (capacity - 1); \
				while (p_new_slots[index].state != CCTL_HASHMAP_EMPTY) { \
					index = (index + 1) & (capacity - 1); \
				} \
				p_new_slots[index] = p_hm->p_slots[i]; \
			} \
		} \
		if (p_hm->p_slots) free(p_hm->p_slots); \
		p_hm->p_slots = p_new_slots; \
		p_hm->capacity = capacity; \
		p_hm->tombstone_count = 0; \
		return true; \
	} \
	\
	bool hashmap_func(insert, KEY, VALUE)(hashmap(KEY, VALUE)* p_hm, KEY key, VALUE value) { \
		if ((p_hm->size + p_hm->tombstone_count) * 4 >= p_hm->capacity * 3) { \
			size_t next_capacity = p_hm->capacity; \
			if (p_hm->size * 2 < p_hm->capacity) next_capacity = p_hm->capacity; \
			else next_capacity = p_hm->capacity == 0 ? 16 : p_hm->capacity * 2; \
			if (!hashmap_func(reserve, KEY, VALUE)(p_hm, next_capacity)) return false; \
		} \
		uint32_t hash = p_hm->fp_hash_func(key); \
		size_t index = hash & (p_hm->capacity - 1); \
		size_t target_index = (size_t)-1; \
		while (p_hm->p_slots[index].state != CCTL_HASHMAP_EMPTY) { \
			if (p_hm->p_slots[index].state == CCTL_HASHMAP_VALID && p_hm->p_slots[index].hash == hash && p_hm->fp_comp_func(p_hm->p_slots[index].data.key, key)) { \
				p_hm->p_slots[index].data.value = value; \
				return true; \
			} \
			if (p_hm->p_slots[index].state == CCTL_HASHMAP_DELETED && target_index == (size_t)-1) { \
				target_index = index; \
			} \
			index = (index + 1) & (p_hm->capacity - 1); \
		} \
		if (target_index != (size_t)-1) index = target_index; \
		p_hm->p_slots[index].hash = hash; \
		p_hm->p_slots[index].data.key = key; \
		p_hm->p_slots[index].data.value = value; \
		if (p_hm->p_slots[index].state == CCTL_HASHMAP_DELETED) p_hm->tombstone_count--; \
		p_hm->p_slots[index].state = CCTL_HASHMAP_VALID; \
		p_hm->size++; \
		return true; \
	} \
	\
	bool hashmap_func(remove, KEY, VALUE)(hashmap(KEY, VALUE)* p_hm, KEY key) { \
		if (p_hm->size == 0) return false; \
		uint32_t hash = p_hm->fp_hash_func(key); \
		size_t index = hash & (p_hm->capacity - 1); \
		while (p_hm->p_slots[index].state != CCTL_HASHMAP_EMPTY) { \
			if (p_hm->p_slots[index].state == CCTL_HASHMAP_VALID && p_hm->p_slots[index].hash == hash && p_hm->fp_comp_func(p_hm->p_slots[index].data.key, key)) { \
				p_hm->p_slots[index].state = CCTL_HASHMAP_DELETED; \
				p_hm->size--; \
				p_hm->tombstone_count++; \
				return true; \
			} \
			index = (index + 1) & (p_hm->capacity - 1); \
		} \
		return false; \
	} \
	\
	VALUE* hashmap_func(find, KEY, VALUE)(hashmap(KEY, VALUE)* p_hm, KEY key) { \
		if (p_hm->size == 0) return NULL; \
		uint32_t hash = p_hm->fp_hash_func(key); \
		size_t index = hash & (p_hm->capacity - 1); \
		while (p_hm->p_slots[index].state != CCTL_HASHMAP_EMPTY) { \
			if (p_hm->p_slots[index].state == CCTL_HASHMAP_VALID && p_hm->p_slots[index].hash == hash && p_hm->fp_comp_func(p_hm->p_slots[index].data.key, key)) { \
				return &(p_hm->p_slots[index].data.value); \
			} \
			index = (index + 1) & (p_hm->capacity - 1); \
		} \
		return NULL; \
	}

#endif