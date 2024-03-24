#ifndef __CCTL_RBT_H__
#define __CCTL_RBT_H__

#include <memory.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "cctl.h"

#define CCTL_RBT_RED false
#define CCTL_RBT_BLACK true

#define rbt_node(TYPE) cctl_join(TYPE, rbt_node)
#define rbt_node_struct(TYPE) cctl_join(rbt_node(TYPE), struct)
#define rbt(TYPE) cctl_join(TYPE, rbt)
#define rbt_struct(TYPE) cctl_join(rbt(TYPE), struct)
#define rbt_func(FUNC, TYPE) cctl_join(rbt(TYPE), FUNC)

#define rbt_init(TYPE, p_r) rbt_func(init, TYPE)(p_r)
#define rbt_free(TYPE, p_r) rbt_func(free, TYPE)(p_r)
#define rbt_insert(TYPE, p_r, index, item) rbt_func(insert, TYPE)(p_r, index, item)
#define rbt_find(TYPE, p_r, index) rbt_func(find, TYPE)(p_r, index)
#define rbt_delete(TYPE, p_r, index) rbt_func(delete, TYPE)(p_r, index)

#define rbt_fd(TYPE) \
	typedef struct rbt_node_struct(TYPE) rbt_node(TYPE); \
	typedef struct rbt_struct(TYPE) rbt(TYPE); \

#define rbt_imp_h(TYPE) \
	struct rbt_node_struct(TYPE) { \
		rbt_node(TYPE)* p_right; \
		rbt_node(TYPE)* p_left; \
		rbt_node(TYPE)* p_parent; \
		size_t index; \
		bool color; \
		TYPE data; \
	}; \
	struct rbt_struct(TYPE) { \
		rbt_node(TYPE)* p_root; \
		rbt_node(TYPE)* p_nil; \
	}; \
	rbt_node(TYPE)* rbt_func(node_new, TYPE)(); \
	bool rbt_func(init, TYPE)(rbt(TYPE)* p_r); \
	void rbt_func(rotate_left, TYPE)(rbt(TYPE)* p_r, rbt_node(TYPE)* p_x); \
	void rbt_func(rotate_right, TYPE)(rbt(TYPE)* p_r, rbt_node(TYPE)* p_x); \
	void rbt_func(insertion_fixup, TYPE)(rbt(TYPE)* p_r, rbt_node(TYPE)* p_z); \
	void rbt_func(insert_node, TYPE)(rbt(TYPE)* p_r, rbt_node(TYPE)* p_z); \
	bool rbt_func(insert, TYPE)(rbt(TYPE)* p_r, size_t index, TYPE data); \
	void rbt_func(transplant, TYPE)(rbt(TYPE)* p_r, rbt_node(TYPE)* p_u, rbt_node(TYPE)* p_v); \
	rbt_node(TYPE)* rbt_func(minimum, TYPE)(rbt(TYPE)* p_r, rbt_node(TYPE)* p_x); \
	void rbt_func(deletion_fixup, TYPE)(rbt(TYPE)* p_r, rbt_node(TYPE)* p_x); \
	void rbt_func(delete_node, TYPE)(rbt(TYPE)* p_r, rbt_node(TYPE)* p_z); \
	void rbt_func(delete, TYPE)(rbt(TYPE)* p_r, size_t index); \
	rbt_node(TYPE)* rbt_func(find_node_recurse, TYPE)(rbt(TYPE)* p_r, rbt_node(TYPE)* p_n, size_t index); \
	rbt_node(TYPE)* rbt_func(find_node, TYPE)(rbt(TYPE)* p_r, size_t index); \
	TYPE* rbt_func(find, TYPE)(rbt(TYPE)* p_r, size_t index); \
	void rbt_func(free_recurse, TYPE)(rbt(TYPE)* p_r, rbt_node(TYPE)* p_n); \
	void rbt_func(free, TYPE)(rbt(TYPE)* p_r);

#define rbt_imp_c(TYPE) \
	rbt_node(TYPE)* rbt_func(node_new, TYPE)() { \
		rbt_node(TYPE)* p_n = (rbt_node(TYPE)*) malloc(sizeof(rbt_node(TYPE))); \
		if (!p_n) return NULL; \
		memset(p_n, 0, sizeof(rbt_node(TYPE))); \
		return p_n; \
	} \
	bool rbt_func(init, TYPE)(rbt(TYPE)* p_r) { \
		rbt_node(TYPE)* p_nil_node = rbt_func(node_new, TYPE)(); \
		if (!p_nil_node) return false; \
		p_nil_node->color = CCTL_RBT_BLACK; \
		p_r->p_nil = p_nil_node; \
		p_r->p_root = p_nil_node; \
		return true; \
	} \
	void rbt_func(rotate_left, TYPE)(rbt(TYPE)* p_r, rbt_node(TYPE)* p_x) { \
		rbt_node(TYPE)* p_y = p_x->p_right; \
		p_x->p_right = p_y->p_left; \
		if (p_y->p_left != p_r->p_nil) p_y->p_left->p_parent = p_x; \
		p_y->p_parent = p_x->p_parent; \
		if (p_x->p_parent == p_r->p_nil) { p_r->p_root = p_y; } \
		else if (p_x == p_x->p_parent->p_left) { p_x->p_parent->p_left = p_y; } \
		else { p_x->p_parent->p_right = p_y; } \
		p_y->p_left = p_x; \
		p_x->p_parent = p_y; \
	} \
	void rbt_func(rotate_right, TYPE)(rbt(TYPE)* p_r, rbt_node(TYPE)* p_x) { \
		rbt_node(TYPE)* p_y = p_x->p_left; \
		p_x->p_left = p_y->p_right; \
		if (p_y->p_right != p_r->p_nil) p_y->p_right->p_parent = p_x; \
		p_y->p_parent = p_x->p_parent; \
		if (p_x->p_parent == p_r->p_nil) { p_r->p_root = p_y; } \
		else if (p_x == p_x->p_parent->p_right) { p_x->p_parent->p_right = p_y; } \
		else { p_x->p_parent->p_left = p_y; } \
		p_y->p_right = p_x; \
		p_x->p_parent = p_y; \
	} \
	void rbt_func(insertion_fixup, TYPE)(rbt(TYPE)* p_r, rbt_node(TYPE)* p_z) { \
		while (p_z->p_parent->color == CCTL_RBT_RED) { \
			if (p_z->p_parent == p_z->p_parent->p_parent->p_left) { \
				rbt_node(TYPE)* p_y = p_z->p_parent->p_parent->p_right; \
				if (p_y->color == CCTL_RBT_RED) { \
					p_y->color = CCTL_RBT_BLACK; \
					p_z->p_parent->color = CCTL_RBT_BLACK; \
					p_z->p_parent->p_parent->color = CCTL_RBT_RED; \
					p_z = p_z->p_parent->p_parent; \
				} \
				else { \
					if (p_z == p_z->p_parent->p_right) { \
						p_z = p_z->p_parent; \
						rbt_func(rotate_left, TYPE)(p_r, p_z); \
					} \
					p_z->p_parent->color = CCTL_RBT_BLACK; \
					p_z->p_parent->p_parent->color = CCTL_RBT_RED; \
					rbt_func(rotate_right, TYPE)(p_r, p_z->p_parent->p_parent); \
				} \
			} \
			else { \
				rbt_node(TYPE)* p_y = p_z->p_parent->p_parent->p_left; \
				if (p_y->color == CCTL_RBT_RED) { \
					p_y->color = CCTL_RBT_BLACK; \
					p_z->p_parent->color = CCTL_RBT_BLACK; \
					p_z->p_parent->p_parent->color = CCTL_RBT_RED; \
					p_z = p_z->p_parent->p_parent; \
				} \
				else { \
					if (p_z == p_z->p_parent->p_left) { \
						p_z = p_z->p_parent; \
						rbt_func(rotate_right, TYPE)(p_r, p_z); \
					} \
					p_z->p_parent->color = CCTL_RBT_BLACK; \
					p_z->p_parent->p_parent->color = CCTL_RBT_RED; \
					rbt_func(rotate_left, TYPE)(p_r, p_z->p_parent->p_parent); \
				} \
			} \
		} \
		p_r->p_root->color = CCTL_RBT_BLACK; \
	} \
	void rbt_func(insert_node, TYPE)(rbt(TYPE)* p_r, rbt_node(TYPE)* p_z) { \
		rbt_node(TYPE)* p_y = p_r->p_nil; \
		rbt_node(TYPE)* p_temp = p_r->p_root; \
		while (p_temp != p_r->p_nil) { \
			p_y = p_temp; \
			if (p_z->index < p_temp->index) p_temp = p_temp->p_left; \
			else p_temp = p_temp->p_right; \
		} \
		p_z->p_parent = p_y; \
		if (p_y == p_r->p_nil) p_r->p_root = p_z; \
		else if (p_z->index < p_y->index) p_y->p_left = p_z; \
		else p_y->p_right = p_z; \
		p_z->p_right = p_r->p_nil; \
		p_z->p_left = p_r->p_nil; \
		rbt_func(insertion_fixup, TYPE)(p_r, p_z); \
	} \
	bool rbt_func(insert, TYPE)(rbt(TYPE)* p_r, size_t index, TYPE data) { \
		rbt_node(TYPE)* p_n = rbt_func(find_node, TYPE)(p_r, index); \
		if (p_n) { \
			p_n->data =data; \
			return true; \
		} \
		p_n = rbt_func(node_new, TYPE)(); \
		if (!p_n) return false; \
		p_n->index = index; \
		p_n->data = data; \
		rbt_func(insert_node, TYPE)(p_r, p_n); \
		return true; \
	} \
	void rbt_func(transplant, TYPE)(rbt(TYPE)* p_r, rbt_node(TYPE)* p_u, rbt_node(TYPE)* p_v) { \
		if (p_u->p_parent == p_r->p_nil) p_r->p_root = p_v; \
		else if (p_u == p_u->p_parent->p_left) p_u->p_parent->p_left = p_v; \
		else p_u->p_parent->p_right = p_v; \
		p_v->p_parent = p_u->p_parent; \
	} \
	rbt_node(TYPE)* rbt_func(minimum, TYPE)(rbt(TYPE)* p_r, rbt_node(TYPE)* p_x) { \
		while (p_x->p_left != p_r->p_nil) p_x = p_x->p_left; \
		return p_x; \
	} \
	void rbt_func(deletion_fixup, TYPE)(rbt(TYPE)* p_r, rbt_node(TYPE)* p_x) { \
		while (p_x != p_r->p_root && p_x->color == CCTL_RBT_BLACK) { \
			if (p_x == p_x->p_parent->p_left) { \
				rbt_node(TYPE)* p_w = p_x->p_parent->p_right; \
				if (p_w->color == CCTL_RBT_RED) { \
					p_w->color = CCTL_RBT_BLACK; \
					p_x->p_parent->color = CCTL_RBT_RED; \
					rbt_func(rotate_left, TYPE)(p_r, p_x->p_parent); \
					p_w = p_x->p_parent->p_right; \
				} \
				if (p_w->p_left->color == CCTL_RBT_BLACK && p_w->p_right->color == CCTL_RBT_BLACK) { \
					p_w->color = CCTL_RBT_RED; \
					p_x = p_x->p_parent; \
				} \
				else { \
					if (p_w->p_right->color == CCTL_RBT_BLACK) { \
						p_w->p_left->color = CCTL_RBT_BLACK; \
						p_w->color = CCTL_RBT_RED; \
						rbt_func(rotate_right, TYPE)(p_r, p_w); \
						p_w = p_x->p_parent->p_right; \
					} \
					p_w->color = p_x->p_parent->color; \
					p_x->p_parent->color = CCTL_RBT_BLACK; \
					p_w->p_right->color = CCTL_RBT_BLACK; \
					rbt_func(rotate_left, TYPE)(p_r, p_x->p_parent); \
					p_x = p_r->p_root; \
				} \
			} \
			else { \
				rbt_node(TYPE)* p_w = p_x->p_parent->p_left; \
				if (p_w->color == CCTL_RBT_RED) { \
					p_w->color = CCTL_RBT_BLACK; \
					p_x->p_parent->color = CCTL_RBT_RED; \
					rbt_func(rotate_right, TYPE)(p_r, p_x->p_parent); \
					p_w = p_x->p_parent->p_left; \
				} \
				if (p_w->p_right->color == CCTL_RBT_BLACK && p_w->p_left->color == CCTL_RBT_BLACK) { \
					p_w->color = CCTL_RBT_RED; \
					p_x = p_x->p_parent; \
				} \
				else { \
					if (p_w->p_left->color == CCTL_RBT_BLACK) { \
						p_w->p_right->color = CCTL_RBT_BLACK; \
						p_w->color = CCTL_RBT_RED; \
						rbt_func(rotate_left, TYPE)(p_r, p_w); \
						p_w = p_x->p_parent->p_left; \
					} \
					p_w->color = p_x->p_parent->color; \
					p_x->p_parent->color = CCTL_RBT_BLACK; \
					p_w->p_left->color = CCTL_RBT_BLACK; \
					rbt_func(rotate_right, TYPE)(p_r, p_x->p_parent); \
					p_x = p_r->p_root; \
				} \
			} \
		} \
		p_x->color = CCTL_RBT_BLACK; \
	} \
	void rbt_func(delete_node, TYPE)(rbt(TYPE)* p_r, rbt_node(TYPE)* p_z) { \
		rbt_node(TYPE)* p_y =  p_z; \
		rbt_node(TYPE)* p_x; \
		bool y_original_color = p_y->color; \
		if (p_z->p_left == p_r->p_nil) { \
			p_x = p_z->p_right; \
			rbt_func(transplant, TYPE)(p_r, p_z, p_z->p_right); \
		} \
		else if (p_z->p_right == p_r->p_nil) { \
			p_x = p_z->p_left; \
			rbt_func(transplant, TYPE)(p_r, p_z, p_z->p_left); \
		} \
		else { \
			p_y =  rbt_func(minimum, TYPE)(p_r, p_z->p_right); \
			y_original_color = p_y->color; \
			p_x = p_y->p_right; \
			if (p_y->p_parent == p_z) p_x->p_parent = p_z; \
			else { \
				rbt_func(transplant, TYPE)(p_r, p_y, p_y->p_right); \
				p_y->p_right = p_z->p_right; \
				p_y->p_right->p_parent = p_y; \
			} \
			rbt_func(transplant, TYPE)(p_r, p_z, p_y); \
			p_y->p_left = p_z->p_left; \
			p_y->p_left->p_parent = p_y; \
			p_y->color = p_z->color; \
		} \
		if (y_original_color == CCTL_RBT_BLACK) rbt_func(deletion_fixup, TYPE)(p_r, p_x); \
		free(p_z); \
	} \
	void rbt_func(delete, TYPE)(rbt(TYPE)* p_r, size_t index) { \
		rbt_node(TYPE)* p_n = rbt_func(find_node, TYPE)(p_r, index); \
		if (!p_n) return; \
		rbt_func(delete_node, TYPE)(p_r, p_n); \
	} \
	rbt_node(TYPE)* rbt_func(find_node_recurse, TYPE)(rbt(TYPE)* p_r, rbt_node(TYPE)* p_n, size_t index) { \
		rbt_node(TYPE)* p_result = NULL; \
		if (p_n != p_r->p_nil) { \
			if (p_n->index == index) return p_n; \
			p_result = rbt_func(find_node_recurse, TYPE)(p_r, p_n->p_left, index); \
			if (!p_result) p_result = rbt_func(find_node_recurse, TYPE)(p_r, p_n->p_right, index); \
		} \
		return p_result; \
	} \
	rbt_node(TYPE)* rbt_func(find_node, TYPE)(rbt(TYPE)* p_r, size_t index) { \
		return rbt_func(find_node_recurse, TYPE)(p_r, p_r->p_root, index); \
	} \
	TYPE* rbt_func(find, TYPE)(rbt(TYPE)* p_r, size_t index) { \
		rbt_node(TYPE)* node = rbt_func(find_node, TYPE)(p_r, index); \
		if (!node) return NULL; \
		return &(node->data); \
	} \
	void rbt_func(free_recurse, TYPE)(rbt(TYPE)* p_r, rbt_node(TYPE)* p_n) { \
		if (p_n != p_r->p_nil) { \
			rbt_func(free_recurse, TYPE)(p_r, p_n->p_left); \
			rbt_func(free_recurse, TYPE)(p_r, p_n->p_right); \
			free(p_n); \
		} \
	} \
	void rbt_func(free, TYPE)(rbt(TYPE)* p_r) { \
		rbt_func(free_recurse, TYPE)(p_r, p_r->p_root); \
		free(p_r->p_nil); \
		p_r->p_root = NULL; \
		p_r->p_nil = NULL; \
	}

#endif