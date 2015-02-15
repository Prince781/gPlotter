#include "llist.h"
#include <stdio.h>
#include <stdlib.h>

struct llist *llist_new(void)
{
	struct llist *ll;

	if ((ll = malloc(sizeof(*ll))) == NULL) {
		fprintf(stderr, "%s: could not create list\n", __func__);
		return NULL;
	}
	ll->base = NULL;
	ll->head = &ll->base;
	ll->size = 0;

	return ll;
}

void llist_add(struct llist *ll, void *elem)
{
	struct llist_elem *lle;

	lle = malloc(sizeof(*lle));
	lle->data = elem;
	lle->next = NULL;
	*ll->head = lle;
	ll->head = &lle->next;
	++ll->size;
}

int llist_remove(struct llist *ll, void *elem)
{
	struct llist_elem **lleptr;
	struct llist_elem *llelem;

	for (lleptr = &ll->base; *lleptr != NULL; lleptr = &(*lleptr)->next)
		if ((*lleptr)->data == elem)
			break;
	
	if (*lleptr == NULL)
		return 0;

	if (&(*lleptr)->next == ll->head)
		ll->head = lleptr;
	llelem = *lleptr;
	*lleptr = (*lleptr)->next;
	free(llelem);
	--ll->size;

	return 1;
}

int llist_contains(struct llist *ll, void *elem)
{
	struct llist_elem *llelem;

	for (llelem = ll->base; llelem != NULL; llelem = llelem->next)
		if (llelem->data == elem)
			return 1;
	
	return 0;
}

struct llist_elem *llist_find(struct llist *ll, int (*f)(void *elem))
{
	struct llist_elem *llelem;

	llist_foreach(ll, llelem)
		if ((*f)(llelem->data))
			return llelem;
	return NULL;
}

void *llist_get(struct llist *ll, int index)
{
	struct llist_elem *llelem;
	int p;

	llelem = ll->base;
	for (p=0; p<index && llelem != NULL; ++p)
		llelem = llelem->next;
	
	return llelem;
}

void llist_destroy(struct llist *ll)
{
	while (ll->size > 0)
		llist_remove(ll, ll->base->data);
	
	free(ll);
}

void llist_destroy_data(struct llist *ll)
{
	void *data;
	while (ll->size > 0) {
		data = NULL;
		if (ll->base->data != NULL)
			data = ll->base->data;
		llist_remove(ll, ll->base->data);
		if (data != NULL)
			free(data);
	}
	free(ll);
}
