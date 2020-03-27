/* Copyright (c) 2013, Ben Noordhuis <info@bnoordhuis.nl>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef HEAP_H_
#define HEAP_H_

#include <stddef.h>  /* NULL */

namespace Heap
{
	struct heap_node {
		struct heap_node* left;
		struct heap_node* right;
		struct heap_node* parent;
	};

	/* A binary min heap.  The usual properties hold: the root is the lowest
	 * element in the set, the height of the tree is at most log2(nodes) and
	 * it's always a complete binary tree.
	 *
	 * The heap function try hard to detect corrupted tree nodes at the cost
	 * of a minor reduction in performance.  Compile with -DNDEBUG to disable.
	 */
	struct heap {
		struct heap_node* min;
		unsigned int nelts;
	};

	/* Return non-zero if a < b. */
	typedef int(*heap_compare_fn)(const struct heap_node* a, const struct heap_node* b);


	class CHeap{
	public:
		/* Public functions. */
		static void heap_init(struct heap* heap);
		static struct heap_node* heap_min(const struct heap* heap);
		static void heap_insert(struct heap* heap,
		struct heap_node* newnode, heap_compare_fn less_than);
		static void heap_remove(struct heap* heap, struct heap_node* node, heap_compare_fn less_than);
		static void heap_dequeue(struct heap* heap, heap_compare_fn less_than);

	private:
		static void heap_node_swap(struct heap* heap,
		struct heap_node* parent,
		struct heap_node* child);
	};
};

#endif  /* UV_SRC_HEAP_H_ */
