#include <stdio.h>
//#include "list.h"
 
struct list_head {
	struct list_head *next, *prev;
};
 
struct test_list
{
	unsigned char *name;
	struct list_head test;
	int value;
};
 
static struct test_list lists[] = {
	[0] = {
		.value = 0,
		.name = "hehe"
	},
	[1] = {
		.value = 1,
	},
	[2] = {
		.value = 2,
	},
	[3] = {
		.value = 3,
		.name = "test"
	},
	[4] = {
		.value = 4,
	}	
};
 
struct list_head head = {
	&(head), &(head)
};
 
static void __list_add_tail(struct list_head *new, struct list_head *prev, struct list_head *next)
{
	new->next = next;
	new->prev = prev;
	prev->next = new;
	next->prev = new;
}
 
void mlist_add_tail(struct list_head *new, struct list_head *head)
{
	__list_add_tail(new, head->prev, head);
}
 
#if 1
#define __list_entry(mod, type, member_name) \
    (type *)((unsigned int)(mod) - ((unsigned int)(&(((type *)(0))->member_name))))
#else
#define mlist_entry(list_head, type, member_name) \
			(type *)((unsigned int)list_head - (unsigned int)(&(((type*)(0))->member_name)))
#endif 

#if 1
#define mlist_for_each(pos, head) \
			for(pos = (head)->next; pos != head; pos = pos->next)
#else 
#define mlist_for_each(pos, head) \
			for(pos = (head)->prev; pos != head; pos = pos->prev)
#endif 

int main(int argv, char *argc[])
{
	struct list_head *mod;
	unsigned int i = 0;
	
	mlist_add_tail(&lists[0].test, &head);
        printf("head0:next:%p prev:%p\n",head.next,head.prev);
	mlist_add_tail(&lists[1].test, &head);
        printf("head0:next:%p prev:%p\n",head.next,head.prev);
	mlist_add_tail(&lists[2].test, &head);
        printf("head0:next:%p prev:%p\n",head.next,head.prev);
	mlist_add_tail(&lists[3].test, &head);
        printf("head0:next:%p prev:%p\n",head.next,head.prev);
	mlist_add_tail(&lists[4].test, &head);
        printf("head0:next:%p prev:%p\n",head.next,head.prev);
	
	mlist_for_each(mod, &head)
	{
		struct test_list *inode;
		
		inode = __list_entry(mod, struct test_list, test);
		printf("%d %s\n", inode->value, inode->name);
	}		
		
	getchar();
	
	return 0;
}
