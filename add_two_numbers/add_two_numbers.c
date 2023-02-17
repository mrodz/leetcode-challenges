#include <stdlib.h>

#define DEBUG

#ifdef DEBUG
#include <stdio.h>
#endif

struct ListNode {
    int val;
    struct ListNode *next;
};

struct ListNode* from_array(int *array, int length) {
	struct ListNode *result = (struct ListNode*) malloc(sizeof(struct ListNode));
	struct ListNode *p = result;

	for (int i = 0; i < length - 1; i++) {
		p->val = array[i];
		struct ListNode *next = (struct ListNode*) malloc(sizeof(struct ListNode));
		p->next = next;
		p = next;
	}

	p->val = array[length - 1];
	p->next = NULL;
	return result;
}

/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */
struct ListNode* addTwoNumbers(struct ListNode* l1, struct ListNode* l2) {
	#define TOP 0b100
	#define BOTTOM 0b010

    struct ListNode *result = (struct ListNode*) malloc(sizeof(struct ListNode));
	struct ListNode *p = result;

	struct ListNode *bottom = l1, *top = l2;

	char flags = 0b000;

	int add = 0;

	while (1) {
		if (top == NULL) flags |= TOP;
		if (bottom == NULL) flags |= BOTTOM;
		
		int sum = add;

		if (flags ^ TOP) sum += top->val;
		if (flags ^ BOTTOM) sum += bottom->val;	
		
		if (sum > 9) {
			add = 1;
		} else {
			add = 0;
		}

		sum %= 10;
		p->val = sum;

		if (flags ^ TOP) top = top->next;
		if (flags ^ BOTTOM) bottom = bottom->next;

		if (bottom == NULL && top == NULL) {
			p->next = NULL;
			break;
		}

		p->next = malloc(sizeof(struct ListNode));
		p = p->next;
	}

	if (add) { // edge case
		p->next = malloc(sizeof(struct ListNode));
		p->next->val = 1;
		p->next->next = NULL;
	}

	return result;
}

void cleanup(struct ListNode *node) {
	free(node);
}

void print(struct ListNode *list) {
#ifdef DEBUG
	printf("%d ", list->val);	
#endif
}

void traverse(struct ListNode *list, void(*callback)(struct ListNode *node)) {
	struct ListNode *p = list, *next;
	do {
		next = p->next;
		(*callback)(p);
	} while ((p = next) != NULL);
}

void main(void) {
	int top[] = {2,4,3};
	int bottom[] = {5,6,4};

	struct ListNode *l1 = from_array(top, sizeof(top) / sizeof(int));
	struct ListNode *l2 = from_array(bottom, sizeof(bottom) / sizeof(int));

	struct ListNode *l3 = addTwoNumbers(l1, l2);

	traverse(l3, &print);
	traverse(l3, &cleanup);
}