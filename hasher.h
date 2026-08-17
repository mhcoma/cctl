#ifndef __CCTL_HASHER_H__
#define __CCTL_HASHER_H__

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "cctl.h"

#define hasher(KEY, ALGORITHM, KIND) cctl_join4(KEY, ALGORITHM, KIND, hasher)
#define hasher_struct(KEY, ALGORITHM, KIND) cctl_join(hasher(KEY, ALGORITHM, KIND), struct)
#define hasher_func(FUNC, KEY, ALGORITHM, KIND) cctl_join(hasher(KEY, ALGORITHM, KIND), FUNC)

#define hasher_init(KEY, ALGORITHM, KIND, p_hasher) hasher_func(init, KEY, ALGORITHM, KIND)(p_hasher)
#define hasher_hash(KEY, ALGORITHM, KIND, key) hasher_func(hash, KEY, ALGORITHM, KIND)(key)
#define hasher_comp(KEY, ALGORITHM, KIND, a, b) hasher_func(comp, KEY, ALGORITHM, KIND)(a, b)

static inline size_t cctl_hasher_get_generic_string_byte_length(const void* p_data, size_t char_size) {
    const uint8_t* p_bytes = (const uint8_t*) p_data;
    size_t character_count = 0;
    
    while (true) {
        bool is_null_terminator = true;
        for (size_t index = 0; index < char_size; index++) {
            if (p_bytes[(character_count * char_size) + index] != 0) {
                is_null_terminator = false;
                break;
            }
        }
        if (is_null_terminator) break;
        character_count++;
    }
    return character_count * char_size;
}

static inline uint32_t cctl_hasher_fnv1a_mem(const void* p_data, size_t size) {
    const uint8_t* p_bytes = (const uint8_t*) p_data;
    uint32_t hash_value = 2166136261u;
    for (size_t index = 0; index < size; index++) {
        hash_value ^= p_bytes[index];
        hash_value *= 16777619u;
    }
    return hash_value;
}

static inline uint32_t cctl_hasher_murmur3_mem(const void* p_data, size_t size) {
    const uint8_t* p_bytes = (const uint8_t*) p_data;
    uint32_t hash_value = 3323198485ul;
    const uint32_t c1 = 0xcc9e2d51;
    const uint32_t c2 = 0x1b873593;
    
    size_t block_count = size / 4;
    const uint32_t* p_blocks = (const uint32_t*) p_data;
    for (size_t index = 0; index < block_count; index++) {
        uint32_t k = p_blocks[index];
        k *= c1;
        k = (k << 15) | (k >> 17);
        k *= c2;
        hash_value ^= k;
        hash_value = (hash_value << 13) | (hash_value >> 19);
        hash_value = hash_value * 5 + 0xe6546b64;
    }
    
    const uint8_t* p_tail = p_bytes + (block_count * 4);
    uint32_t tail_accumulator = 0;
    size_t tail_size = size & 3;
    if (tail_size >= 3) tail_accumulator ^= (uint32_t) p_tail[2] << 16;
    if (tail_size >= 2) tail_accumulator ^= (uint32_t) p_tail[1] << 8;
    if (tail_size >= 1) {
        tail_accumulator ^= (uint32_t) p_tail[0];
        tail_accumulator *= c1;
        tail_accumulator = (tail_accumulator << 15) | (tail_accumulator >> 17);
        tail_accumulator *= c2;
        hash_value ^= tail_accumulator;
    }
    
    hash_value ^= (uint32_t) size;
    hash_value ^= hash_value >> 16;
    hash_value *= 0x85ebca6b;
    hash_value ^= hash_value >> 13;
    hash_value *= 0xc2b2ae35;
    hash_value ^= hash_value >> 16;
    return hash_value;
}

static inline uint32_t cctl_hasher_siphash_mem(const void* p_data, size_t size) {
    const uint8_t* p_bytes = (const uint8_t*) p_data;
    uint32_t hash_value = 0x736f6d65ul ^ (uint32_t) size;
    for (size_t index = 0; index < size; index++) {
        hash_value ^= (uint32_t) p_bytes[index];
        hash_value *= 0x517cc1b7ul;
        hash_value = (hash_value << 11) | (hash_value >> 21);
    }
    hash_value ^= hash_value >> 15;
    hash_value *= 0x05092825ul;
    hash_value ^= hash_value >> 12;
    return hash_value;
}

static inline bool cctl_hasher_comp_default(const void* p_a, const void* p_b, size_t size) {
    return memcmp(p_a, p_b, size) == 0;
}

#define hasher_cstr_fd(KEY, ALGORITHM) \
    typedef struct hasher_struct(KEY, ALGORITHM, cstr) hasher(KEY, ALGORITHM, cstr);

#define hasher_cstr_imp_h(KEY, ALGORITHM) \
    struct hasher_struct(KEY, ALGORITHM, cstr) { \
        uint32_t (*fp_hash)(KEY); \
        bool (*fp_comp)(KEY, KEY); \
    }; \
    uint32_t hasher_func(hash, KEY, ALGORITHM, cstr)(KEY key); \
    bool hasher_func(comp, KEY, ALGORITHM, cstr)(KEY a, KEY b); \
    void hasher_func(init, KEY, ALGORITHM, cstr)(hasher(KEY, ALGORITHM, cstr)* p_hasher);

#define hasher_cstr_imp_c(KEY, ALGORITHM, SIZE) \
    uint32_t hasher_func(hash, KEY, ALGORITHM, cstr)(KEY key) { \
        size_t byte_length = cctl_hasher_get_generic_string_byte_length((const void*) (uintptr_t) key, SIZE); \
        return cctl_join3(cctl_hasher, ALGORITHM, mem)((const void*) (uintptr_t) key, byte_length); \
    } \
    bool hasher_func(comp, KEY, ALGORITHM, cstr)(KEY a, KEY b) { \
        size_t byte_length_a = cctl_hasher_get_generic_string_byte_length((const void*) (uintptr_t) a, SIZE); \
        size_t byte_length_b = cctl_hasher_get_generic_string_byte_length((const void*) (uintptr_t) b, SIZE); \
        if (byte_length_a != byte_length_b) return false; \
        return cctl_hasher_comp_default((const void*) (uintptr_t) a, (const void*) (uintptr_t) b, byte_length_a); \
    } \
    void hasher_func(init, KEY, ALGORITHM, cstr)(hasher(KEY, ALGORITHM, cstr)* p_hasher) { \
        p_hasher->fp_hash = hasher_func(hash, KEY, ALGORITHM, cstr); \
        p_hasher->fp_comp = hasher_func(comp, KEY, ALGORITHM, cstr); \
    }

#define hasher_mem_fd(KEY, ALGORITHM) \
    typedef struct hasher_struct(KEY, ALGORITHM, mem) hasher(KEY, ALGORITHM, mem);

#define hasher_mem_imp_h(KEY, ALGORITHM) \
    struct hasher_struct(KEY, ALGORITHM, mem) { \
        uint32_t (*fp_hash)(KEY); \
        bool (*fp_comp)(KEY, KEY); \
    }; \
    uint32_t hasher_func(hash, KEY, ALGORITHM, mem)(KEY key); \
    bool hasher_func(comp, KEY, ALGORITHM, mem)(KEY a, KEY b); \
    void hasher_func(init, KEY, ALGORITHM, mem)(hasher(KEY, ALGORITHM, mem)* p_hasher);

#define hasher_mem_imp_c(KEY, ALGORITHM, SIZE) \
    uint32_t hasher_func(hash, KEY, ALGORITHM, mem)(KEY key) { \
        return cctl_join3(cctl_hasher, ALGORITHM, mem)(&key, SIZE); \
    } \
    bool hasher_func(comp, KEY, ALGORITHM, mem)(KEY a, KEY b) { \
        return cctl_hasher_comp_default(&a, &b, SIZE); \
    } \
    void hasher_func(init, KEY, ALGORITHM, mem)(hasher(KEY, ALGORITHM, mem)* p_hasher) { \
        p_hasher->fp_hash = hasher_func(hash, KEY, ALGORITHM, mem); \
        p_hasher->fp_comp = hasher_func(comp, KEY, ALGORITHM, mem); \
    }

#define hasher_cstr_fnv1a(CHAR_TYPE) hasher(cctl_ptr(CHAR_TYPE), fnv1a, cstr)
#define hasher_cstr_fnv1a_init(CHAR_TYPE, p_hasher) hasher_init(cctl_ptr(CHAR_TYPE), fnv1a, cstr, p_hasher)
#define hasher_cstr_fnv1a_hash(CHAR_TYPE, key) hasher_hash(cctl_ptr(CHAR_TYPE), fnv1a, cstr, key)
#define hasher_cstr_fnv1a_comp(CHAR_TYPE, a, b) hasher_comp(cctl_ptr(CHAR_TYPE), fnv1a, cstr, a, b)
#define hasher_cstr_fnv1a_fd(CHAR_TYPE) cctl_ptr_def(CHAR_TYPE); hasher_cstr_fd(cctl_ptr(CHAR_TYPE), fnv1a)
#define hasher_cstr_fnv1a_imp_h(CHAR_TYPE) hasher_cstr_imp_h(cctl_ptr(CHAR_TYPE), fnv1a)
#define hasher_cstr_fnv1a_imp_c(CHAR_TYPE) hasher_cstr_imp_c(cctl_ptr(CHAR_TYPE), fnv1a, sizeof(CHAR_TYPE))

#define hasher_cstr_murmur3(CHAR_TYPE) hasher(cctl_ptr(CHAR_TYPE), murmur3, cstr)
#define hasher_cstr_murmur3_init(CHAR_TYPE, p_hasher) hasher_init(cctl_ptr(CHAR_TYPE), murmur3, cstr, p_hasher)
#define hasher_cstr_murmur3_hash(CHAR_TYPE, key) hasher_hash(cctl_ptr(CHAR_TYPE), murmur3, cstr, key)
#define hasher_cstr_murmur3_comp(CHAR_TYPE, a, b) hasher_comp(cctl_ptr(CHAR_TYPE), murmur3, cstr, a, b)
#define hasher_cstr_murmur3_fd(CHAR_TYPE) cctl_ptr_def(CHAR_TYPE); hasher_cstr_fd(cctl_ptr(CHAR_TYPE), murmur3)
#define hasher_cstr_murmur3_imp_h(CHAR_TYPE) hasher_cstr_imp_h(cctl_ptr(CHAR_TYPE), murmur3)
#define hasher_cstr_murmur3_imp_c(CHAR_TYPE) hasher_cstr_imp_c(cctl_ptr(CHAR_TYPE), murmur3, sizeof(CHAR_TYPE))

#define hasher_cstr_siphash(CHAR_TYPE) hasher(cctl_ptr(CHAR_TYPE), siphash, cstr)
#define hasher_cstr_siphash_init(CHAR_TYPE, p_hasher) hasher_init(cctl_ptr(CHAR_TYPE), siphash, cstr, p_hasher)
#define hasher_cstr_siphash_hash(CHAR_TYPE, key) hasher_hash(cctl_ptr(CHAR_TYPE), siphash, cstr, key)
#define hasher_cstr_siphash_comp(CHAR_TYPE, a, b) hasher_comp(cctl_ptr(CHAR_TYPE), siphash, cstr, a, b)
#define hasher_cstr_siphash_fd(CHAR_TYPE) cctl_ptr_def(CHAR_TYPE); hasher_cstr_fd(cctl_ptr(CHAR_TYPE), siphash)
#define hasher_cstr_siphash_imp_h(CHAR_TYPE) hasher_cstr_imp_h(cctl_ptr(CHAR_TYPE), siphash)
#define hasher_cstr_siphash_imp_c(CHAR_TYPE) hasher_cstr_imp_c(cctl_ptr(CHAR_TYPE), siphash, sizeof(CHAR_TYPE))

#define hasher_mem_fnv1a(KEY) hasher(KEY, fnv1a, mem)
#define hasher_mem_fnv1a_init(KEY, p_hasher) hasher_init(KEY, fnv1a, mem, p_hasher)
#define hasher_mem_fnv1a_hash(KEY, key) hasher_hash(KEY, fnv1a, mem, key)
#define hasher_mem_fnv1a_comp(KEY, a, b) hasher_comp(KEY, fnv1a, mem, a, b)
#define hasher_mem_fnv1a_fd(KEY) hasher_mem_fd(KEY, fnv1a)
#define hasher_mem_fnv1a_imp_h(KEY) hasher_mem_imp_h(KEY, fnv1a)
#define hasher_mem_fnv1a_imp_c(KEY) hasher_mem_imp_c(KEY, fnv1a, sizeof(KEY))

#define hasher_mem_murmur3(KEY) hasher(KEY, murmur3, mem)
#define hasher_mem_murmur3_init(KEY, p_hasher) hasher_init(KEY, murmur3, mem, p_hasher)
#define hasher_mem_murmur3_hash(KEY, key) hasher_hash(KEY, murmur3, mem, key)
#define hasher_mem_murmur3_comp(KEY, a, b) hasher_comp(KEY, murmur3, mem, a, b)
#define hasher_mem_murmur3_fd(KEY) hasher_mem_fd(KEY, murmur3)
#define hasher_mem_murmur3_imp_h(KEY) hasher_mem_imp_h(KEY, murmur3)
#define hasher_mem_murmur3_imp_c(KEY) hasher_mem_imp_c(KEY, murmur3, sizeof(KEY))

#define hasher_mem_siphash(KEY) hasher(KEY, siphash, mem)
#define hasher_mem_siphash_init(KEY, p_hasher) hasher_init(KEY, siphash, mem, p_hasher)
#define hasher_mem_siphash_hash(KEY, key) hasher_hash(KEY, siphash, mem, key)
#define hasher_mem_siphash_comp(KEY, a, b) hasher_comp(KEY, siphash, mem, a, b)
#define hasher_mem_siphash_fd(KEY) hasher_mem_fd(KEY, siphash)
#define hasher_mem_siphash_imp_h(KEY) hasher_mem_imp_h(KEY, siphash)
#define hasher_mem_siphash_imp_c(KEY) hasher_mem_imp_c(KEY, siphash, sizeof(KEY))

#endif