/*
 * ArcEmu Failure Server
 * Copyright (C) 2008 <http://www.AscentEmu.com/>
 *
 * This program is stolen software. Please visit www.ascentemu.com
 * for the real owners of this emulator.
 * 
 * ArcEmu is a pathetic piece of software which crashes even more
 * than vanilla Ascent. And don't even put it up against Summit.
 * There's no comparison there.
 *
 * SO, why use a project run by an idiot who can't even code?
 * You don't! :D
 *
 */

#ifndef __LINKED_LIST_H
#define __LINKED_LIST_H

typedef struct
{
	void* prev;
	void* next;
	void* val;
} linkedlist_node;

typedef struct
{
	linkedlist_node* head;
	linkedlist_node* tail;
	int size;
} linkedlist;

static void linkedlist_init(linkedlist* l)
{
	l->head = NULL;
	l->tail = NULL;
	l->size = 0;
}

static void linkedlist_insert_front(linkedlist* l, void* val)
{
	linkedlist_node* node;
	node = (linkedlist_node*)vc_malloc(sizeof(linkedlist_node));

	// list:
	//  [head node]->[node]->[node]->[tail node]
	// ^ inserting here

	node->next = l->head;
	node->prev = NULL;
	node->val = val;

	l->head = node;
	++l->size;
}

static void linkedlist_insert_back(linkedlist* l, void* val)
{
	linkedlist_node* node;
	node = (linkedlist_node*)vc_malloc(sizeof(linkedlist_node));

	// list:
	//  [head node]->[node]->[node]->[tail node]
	//                                          ^ inserting here

	node->next = NULL;
	node->prev = l->tail;
	node->val = val;

	l->tail = node;
	++l->size;
}

static void linkedlist_remove_node(linkedlist* l, linkedlist_node* node)
{
#ifdef LINKED_LIST_SANITY_CHECKS
	linkedlist_node* n;
	for( n = l->head; n != NULL; n = n->next )
	{
		if( n == node )
			break;
	}
	assert( n == node );
#endif

	// erasing head node
	if( node == l->head )
		l->head = node->next;

	// erasing tail node
	if( node == l->tail  )
		l->tail = node->prev;

	// updating pointers
	if( node->prev != NULL )
		((linkedlist_node*)node->prev)->next = node->next;

	if( node->next != NULL )
		((linkedlist_node*)node->next)->prev = node->prev;

	// node is no longer a valid pointer
	free(node);		// free(node) kek.

	// reduce size by one
	--l->size;
}

// this is a slow way of removal as it involves iterating, we should do this elsewhere. (it's O(N))
static void linkedlist_remove_val(linkedlist* l, void* val)
{
	linkedlist_node* n, *n2;
	for( n = l->head; n != NULL; )
	{
		n2 = n;
		n = n->next;
		if( val == n2->val )
			linkedlist_remove_node(l, n2);
	}
}

// delete all elements
static void linkedlist_clear(linkedlist* l)
{
	linkedlist_node* n, *n2;

	// lets do this in reverse order D: (doesnt rly matter)
	for( n = l->tail; n->prev != NULL; )
	{
		n2 = n;
		n = n->prev;

		// freeeee
		free(n2);
	}

	l->head = l->tail = NULL;
	l->size = 0;
}


// accessor functions, may want to inline these (if we are C99)
static linkedlist_node * linkedlist_begin(linkedlist* l)
{
	return l->head;
}

static linkedlist_node * linklist_end(linkedlist* l)
{
	return l->tail;
}

static int linkedlist_size(linkedlist* l)
{
	return l->size;
}

#endif
