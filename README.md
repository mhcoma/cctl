# CCTL : C Template Library

## How to use
* `test.h`
```c
#include "vector.h"

cctl_fd(int);
cctl_imp_h(int);
```

* `main.c`
```c
#include "test.h"

cctl_imp_c(int);

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
* `.size`
* `.capacity`
* `vector_init(T, vector(T)* p_v)`
* `vector_free(T, vector(T)* p_v)`
* `vector_clear(T, vector(T)* p_v)`
* `vector_reserve(T, vector(T)* p_v, size_t size)`
* `vector_resize(T, vector(T)* p_v, size_t size)`
* `vector_push_back(T, vector(T)* p_v, T item)`
* `vector_pop_back(T, vector(T)* p_v)`
* `vector_at(T, vector(T)* p_v, size_t index)`
* `vector_front(T, vector(T)* p_v)`
* `vector_back(T, vector(T)* p_v)`

## deque(T)
* `.size`
* `deque_init(T, vector(T)* p_v)`
* `deque_free(T, vector(T)* p_v)`
* `deque_clear(T, vector(T)* p_v)`
* `deque_resize(T, vector(T)* p_v, size_t size)`
* `deque_push_front(T, vector(T)* p_v, T item)`
* `deque_pop_front(T, vector(T)* p_v)`
* `deque_push_back(T, vector(T)* p_v, T item)`
* `deque_pop_back(T, vector(T)* p_v)`
* `deque_at(T, vector(T)* p_v, size_t index)`
* `deque_front(T, vector(T)* p_v)`
* `deque_back(T, vector(T)* p_v)`

## list(T)
* `.size`
* `deque_init(T, vector(T)* p_v)`
* `deque_free(T, vector(T)* p_v)`
* `deque_push_front(T, vector(T)* p_v, T item)`
* `deque_pop_front(T, vector(T)* p_v)`
* `deque_push_back(T, vector(T)* p_v, T item)`
* `deque_pop_back(T, vector(T)* p_v)`