# CCTL : C Template Library

## How to use
* `test.h`
```c
#include "cctl/vector.h"

cctl_fd(int);
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

## vector(T)
Dynamic array
* `cctl/vector.h`
* `size_t size`
* `size_t capacity`
* `void vector_init(T, vector(T)* p_v)`
* `void vector_free(T, vector(T)* p_v)`
* `void vector_clear(T, vector(T)* p_v)`
* `bool vector_reserve(T, vector(T)* p_v, size_t size)`
* `bool vector_resize(T, vector(T)* p_v, size_t size)`
* `bool vector_push_back(T, vector(T)* p_v, T item)`
* `bool vector_pop_back(T, vector(T)* p_v)`
* `T* vector_at(T, vector(T)* p_v, size_t index)`
* `T* vector_front(T, vector(T)* p_v)`
* `T* vector_back(T, vector(T)* p_v)`

## deque(T)
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

## list(T)
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

## trie(T)
Trie
* `cctl/trie.h`
* `void trie_init(T, trie(T)* p_t)`
* `void trie_free(T, trie(T)* p_t)`
* `T* trie_insert(T, trie(T)* p_t, const char* key, T item)`
* `void trie_remove(T, trie(T)* p_t, const char* key)`
* `T* trie_find(T, trie(T)* p_t, const char* key)`

## rbt(T)
Red-black tree
* `cctl/rbt.h`
* `void rbt_init(T, rbt(T)* p_r)`
* `void rbt_free(T, rbt(T)* p_r)`
* `bool rbt_insert(T, rbt(T)* p_r, size_t index, T item)`
* `T* rbt_find(T, rbt(T)* p_r, size_t index)`
* `void rbt_delete(T, rbt(T)* p_r, size_t index)`

## Utils
`cctl/cctl.h`
* `cctl_concat(A, B)` => `AB`
* `cctl_join(A, B)` => `A_B`
* `cctl_ptr_def(T)` => `typedef T *cctl_ptr(T)`
* `cctl_ptr(T)` => `T_ptr`
* `cctl_num_args(...)` => Return count of `...`