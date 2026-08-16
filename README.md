# CCTL : C Template Library

## How to use

* `test.h`

```c
#include "cctl/vector.h"

vector_imp_h(int);
```

* `main.c`

```c
#include "test.h"

vector_imp_c(int);

int main(void) {
	vector(int) v;
	vector_init(int, &v);

	vector_push_back(int, &v, 10);
	vector_push_back(int, &v, 20);
	vector_push_back(int, &v, 30);

	for (int i = 0; i < v.size; i++) {
		printf("%d\n", vector_at(int, &v, i));
	}

	vector_free(int, &v);
}
```

### Data Structures

### vector(T)

Dynamic array

* `cctl/vector.h`
* `size_t size`
* `size_t capacity`
* `void vector_init(T, vector(T)* p_v)`
* `void vector_free(T, vector(T)* p_v)`
* `void vector_clear(T, vector(T)* p_v)`
* `bool vector_reserve(T, vector(T)* p_v, size_t capacity)`
* `bool vector_resize(T, vector(T)* p_v, size_t size)`
* `bool vector_push_back(T, vector(T)* p_v, T item)`
* `bool vector_pop_back(T, vector(T)* p_v)`
* `T* vector_at(T, vector(T)* p_v, size_t index)`
* `T* vector_front(T, vector(T)* p_v)`
* `T* vector_back(T, vector(T)* p_v)`

### deque(T)

Dynamic double-ended queue

* `cctl/deque.h`
* `size_t size`
* `void deque_init(T, deque(T)* p_v)`
* `void deque_free(T, deque(T)* p_v)`
* `void deque_clear(T, deque(T)* p_v)`
* `bool deque_resize(T, deque(T)* p_v, size_t size)`
* `bool deque_push_front(T, deque(T)* p_v, T item)`
* `bool deque_pop_front(T, deque(T)* p_v)`
* `bool deque_push_back(T, deque(T)* p_v, T item)`
* `bool deque_pop_back(T, deque(T)* p_v)`
* `T* deque_at(T, deque(T)* p_v, size_t index)`
* `T* deque_front(T, deque(T)* p_v)`
* `T* deque_back(T, deque(T)* p_v)`

### list(T)

Doubly linked list

* `cctl/list.h`
* `size_t size`
* `void list_init(T, list(T)* p_v)`
* `void list_free(T, list(T)* p_v)`
* `bool list_push_front(T, list(T)* p_v, T item)`
* `bool list_pop_front(T, list(T)* p_v)`
* `bool list_push_back(T, list(T)* p_v, T item)`
* `bool list_pop_back(T, list(T)* p_v)`
* `T* list_at(T, list(T)* p_v, size_t index)`
* `T* list_front(T, list(T)* p_v)`
* `T* list_back(T, list(T)* p_v)`
* `node(T)* list_node_at(T, list(T)* p_v, size_t index)`
* `node(T)* list_node_front(T, list(T)* p_v)`
* `node(T)* list_node_back(T, list(T)* p_v)`

### trie256(T)

Radix-256 Trie

* `cctl/trie256.h`
* `void trie256_init(T, trie(T)* p_t)`
* `void trie256_free(T, trie(T)* p_t)`
* `T* trie256_insert(T, trie(T)* p_t, const char* p_key, T item)`
* `void trie256_remove(T, trie(T)* p_t, const char* p_key)`
* `T* trie256_find(T, trie(T)* p_t, const char* p_key)`

### rbt(T)

Red-black tree

* `cctl/rbt.h`
* `bool rbt_init(T, rbt(T)* p_r)`
* `void rbt_free(T, rbt(T)* p_r)`
* `bool rbt_insert(T, rbt(T)* p_r, size_t index, T item)`
* `T* rbt_find(T, rbt(T)* p_r, size_t index)`
* `void rbt_delete(T, rbt(T)* p_r, size_t index)`

### pair(KEY, VALUE)

Key-value pair structure

* `cctl/pair.h`
* `KEY key`
* `VALUE value`

### hasher

Hashing object for various data types and algorithms

* `cctl/hasher.h`
* `uint32_t (*fp_hash)(KEY)`
* `bool (*fp_comp)(KEY, KEY)`

* Supported `KIND`:
  * `cstr`: Null-terminated string. Evaluates length based on `CHAR_TYPE` until `\0` is found.
  * `mem`: Raw memory block. Evaluates fixed memory block using `sizeof(KEY)`.

* Supported `ALGORITHM`:
  * `fnv1a`: Simple and fast. Recommended for short keys.
  * `murmur3`: Excellent distribution and performance. Recommended for general use.
  * `siphash`: Provides protection against hash flooding attacks. Slightly slower but secure.

* C-String (`cstr`) Macros:
  * `void hasher_cstr_[ALGORITHM]_init(CHAR_TYPE, hasher* p_hasher)`
  * `uint32_t hasher_cstr_[ALGORITHM]_hash(CHAR_TYPE, KEY key)`
  * `bool hasher_cstr_[ALGORITHM]_comp(CHAR_TYPE, KEY a, KEY b)`

* Raw Memory (`mem`) Macros:
  * `void hasher_mem_[ALGORITHM]_init(KEY, hasher* p_hasher)`
  * `uint32_t hasher_mem_[ALGORITHM]_hash(KEY, KEY key)`
  * `bool hasher_mem_[ALGORITHM]_comp(KEY, KEY a, KEY b)`

### hashmap(KEY, VALUE)

Open addressing hash table

* `cctl/hashmap.h`
* `size_t size`
* `size_t capacity`
* `void hashmap_init(KEY, VALUE, hashmap(KEY, VALUE)* p_hm, Hasher* p_hasher)`
* `void hashmap_free(KEY, VALUE, hashmap(KEY, VALUE)* p_hm)`
* `void hashmap_clear(KEY, VALUE, hashmap(KEY, VALUE)* p_hm)`
* `bool hashmap_reserve(KEY, VALUE, hashmap(KEY, VALUE)* p_hm, size_t capacity)`
* `bool hashmap_insert(KEY, VALUE, hashmap(KEY, VALUE)* p_hm, KEY key, VALUE value)`
* `bool hashmap_remove(KEY, VALUE, hashmap(KEY, VALUE)* p_hm, KEY key)`
* `VALUE* hashmap_find(KEY, VALUE, hashmap(KEY, VALUE)* p_hm, KEY key)`

## Utils

Utility macros for template generation and type manipulation

* `cctl/cctl.h`
* `cctl_concat(A, B)` => `AB`
* `cctl_join(A, B)` => `A_B`
* `cctl_join3(A, B, C)` => `A_B_C`
* `cctl_join4(A, B, C, D)` => `A_B_C_D`
* `cctl_stringify(TOKEN)` => `"TOKEN"`
* `cctl_ptr_def(T)` => `typedef T *cctl_ptr(T)`
* `cctl_ptr(T)` => `T_ptr`
* `cctl_num_args(...)` => Return count of `...`