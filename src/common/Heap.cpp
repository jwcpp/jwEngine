#include "Heap.h"

using namespace Heap;

void CHeap::heap_init(struct heap* heap)
{
	heap->min = NULL;
	heap->nelts = 0;
}

struct heap_node* CHeap::heap_min(const struct heap* heap)
{
	return heap->min;
}

	/* Swap parent with child. Child moves closer to the root, parent moves away. */
void CHeap::heap_node_swap(struct heap* heap,
	struct heap_node* parent,
	struct heap_node* child) {
		struct heap_node* sibling;
		struct heap_node t;

		t = *parent;
		*parent = *child;
		*child = t;

		parent->parent = child;
		if (child->left == child) {
			child->left = parent;
			sibling = child->right;
		}
		else {
			child->right = parent;
			sibling = child->left;
		}
		if (sibling != NULL)
			sibling->parent = child;

		if (parent->left != NULL)
			parent->left->parent = parent;
		if (parent->right != NULL)
			parent->right->parent = parent;

		if (child->parent == NULL)
			heap->min = child;
		else if (child->parent->left == parent)
			child->parent->left = child;
		else
			child->parent->right = child;
	}

void CHeap::heap_insert(struct heap* heap,
	struct heap_node* newnode,
		heap_compare_fn less_than) {
		struct heap_node** parent;
		struct heap_node** child;
		unsigned int path;
		unsigned int n;
		unsigned int k;

		newnode->left = NULL;
		newnode->right = NULL;
		newnode->parent = NULL;

		/* Calculate the path from the root to the insertion point.  This is a min
		* heap so we always insert at the left-most free node of the bottom row.
		*/
		path = 0;
		for (k = 0, n = 1 + heap->nelts; n >= 2; k += 1, n /= 2)
			path = (path << 1) | (n & 1);

		/* Now traverse the heap using the path we calculated in the previous step. */
		parent = child = &heap->min;
		while (k > 0) {
			parent = child;
			if (path & 1)
				child = &(*child)->right;
			else
				child = &(*child)->left;
			path >>= 1;
			k -= 1;
		}

		/* Insert the new node. */
		newnode->parent = *parent;
		*child = newnode;
		heap->nelts += 1;

		/* Walk up the tree and check at each node if the heap property holds.
		* It's a min heap so parent < child must be true.
		*/
		while (newnode->parent != NULL && less_than(newnode, newnode->parent))
			heap_node_swap(heap, newnode->parent, newnode);
	}

void CHeap::heap_remove(struct heap* heap,
	struct heap_node* node,
		heap_compare_fn less_than) {
		struct heap_node* smallest;
		struct heap_node** max;
		struct heap_node* child;
		unsigned int path;
		unsigned int k;
		unsigned int n;

		if (heap->nelts == 0)
			return;

		/* Calculate the path from the min (the root) to the max, the left-most node
		* of the bottom row.
		*/
		path = 0;
		for (k = 0, n = heap->nelts; n >= 2; k += 1, n /= 2)
			path = (path << 1) | (n & 1);

		/* Now traverse the heap using the path we calculated in the previous step. */
		max = &heap->min;
		while (k > 0) {
			if (path & 1)
				max = &(*max)->right;
			else
				max = &(*max)->left;
			path >>= 1;
			k -= 1;
		}

		heap->nelts -= 1;

		/* Unlink the max node. */
		child = *max;
		*max = NULL;

		if (child == node) {
			/* We're removing either the max or the last node in the tree. */
			if (child == heap->min) {
				heap->min = NULL;
			}
			return;
		}

		/* Replace the to be deleted node with the max node. */
		child->left = node->left;
		child->right = node->right;
		child->parent = node->parent;

		if (child->left != NULL) {
			child->left->parent = child;
		}

		if (child->right != NULL) {
			child->right->parent = child;
		}

		if (node->parent == NULL) {
			heap->min = child;
		}
		else if (node->parent->left == node) {
			node->parent->left = child;
		}
		else {
			node->parent->right = child;
		}

		/* Walk down the subtree and check at each node if the heap property holds.
		* It's a min heap so parent < child must be true.  If the parent is bigger,
		* swap it with the smallest child.
		*/
		for (;;) {
			smallest = child;
			if (child->left != NULL && less_than(child->left, smallest))
				smallest = child->left;
			if (child->right != NULL && less_than(child->right, smallest))
				smallest = child->right;
			if (smallest == child)
				break;
			heap_node_swap(heap, child, smallest);
		}

		/* Walk up the subtree and check that each parent is less than the node
		* this is required, because `max` node is not guaranteed to be the
		* actual maximum in tree
		*/
		while (child->parent != NULL && less_than(child, child->parent))
			heap_node_swap(heap, child->parent, child);
	}

void CHeap::heap_dequeue(struct heap* heap, heap_compare_fn less_than) {
		heap_remove(heap, heap->min, less_than);
	}