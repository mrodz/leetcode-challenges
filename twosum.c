#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct number_t {
	int key;
    int value;
    struct number_t *next;
    bool empty;
} number_t;

number_t num(int key, int value) {
    return (number_t) { .key = key, .value = value, .next = NULL, .empty = true };
}

void print_number(number_t *p) {
    printf("{ %d: %d }", p->key, p->value);
    
    number_t*next = p->next;
    
    while (next != NULL) {
        printf(" & { %d: %d }", next->key, next->value);
        next = next->next;
    }
    
    printf("\n");
}

unsigned int hash_num(unsigned int x, size_t length) {
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x % length;
}

typedef struct {
    number_t *numbers;
    size_t length;
} hashtable_t;

hashtable_t hashtable(size_t length) {
    number_t *numbers = (number_t*) calloc(length, sizeof(number_t));

    if (numbers == NULL) {
        printf("not enough space");
        exit(1);
    }
    
    for (int i = 0; i < length; i++) {
        numbers[i].empty = true;
    }
    
    return (hashtable_t) { .numbers = numbers, .length = length };
}

void add_number(hashtable_t *table, number_t *p) {
    number_t *copy = (number_t*) malloc(sizeof(number_t));
    
    *copy = (number_t) { .key = p->key, .value = p->value, .next = p->next, .empty = false };

    unsigned int hash = hash_num(copy->key, table->length);
    
    printf("hash for %d is %u\n", copy->key, hash);

    if (table->numbers[hash].empty) {
        printf("\tfound instantly\n");
        table->numbers[hash] = *copy;
        // table->numbers[hash].key = copy.key;
        // (*table->numbers[hash]).value = copy.value;
        // (*table->numbers[hash]).next = copy.next;
        return;
    }

    printf("\toccupied...\n");
    number_t *next = &(table->numbers[hash]);
    while (1) {
        if (next->key == copy->key) {
            printf("\t\tduplicate keys (%d == %d), overriding %d\n", next->key, copy->key, next->value);
            next->value = copy->value;
            return;
        }

        if (next->next == NULL) {
            printf("\t\tfound slot\n");
            next->next = copy;
            // (*next).next->key = copy.key;
            // next->next->value = copy.value;
            // next->next->next = copy.next;
            return;
        }

        next = next->next;
    }
    
    exit(1); // never reached
}

bool from_key(hashtable_t *table, int key, int *dest) {
    unsigned int hash = hash_num(key, table->length);

    if (table->numbers[hash].empty) {
        return false;
    }

    if (table->numbers[hash].key == key) {
        *dest = table->numbers[hash].value;
        return true;
    }

    number_t *next = table->numbers[hash].next;

    while (next != NULL) {
        if (next->key == key) {
            *dest = next->value;
            return true;
        }

        next = next->next;
    }

    return false;
} 

void print_table(hashtable_t *table) {
    printf("{\n");
    for (int i = 0; i < table->length; i++) {
        printf("\t[%d]: ", i);
        if (!table->numbers[i].empty) {
            print_number(&(table->numbers[i]));
        } else {
            printf("---\n");
        }
    }
    printf("}\n");
}

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* twoSum(int* nums, int numsSize, int target, int* returnSize){
    hashtable_t mapping = hashtable(numsSize);

    for (int i = 0; i < numsSize; i++) {
        int difference = target - nums[i];

        int found;

        if (from_key(&mapping, nums[i], &found)) {
            int *result = (int*) calloc(2, sizeof(int));

            if (result == NULL) goto FAIL;

            print_table(&mapping);
            clean_hashtable(&mapping);
            result[0] = found;
            result[1] = i;
            *returnSize = 2;
            return result;
        }

        number_t *n = (number_t*) malloc(sizeof(number_t));
        *n = num(difference, i);
        add_number(&mapping, n);
    }

FAIL:
    print_table(&mapping);
    printf("1\n");
    *returnSize = 0;
    return NULL;
}

// int main() {
//     int nums[4] = { 0, 4, 3, 0 };
//     int target = 0;
//     int returnSize;

//     int *result = twoSum(nums, 4, target, &returnSize);

//     printf("len(%d) : ", returnSize);
//     for (int i = 0; i < returnSize; i++) {
//         printf("%d ", result[i]);
//     }

//     free(result);

//     // hashtable_t table = hashtable(5);
//     // number_t p1 = num(3, 1);
//     // number_t p2 = num(3, 2);

//     // add_number(&table, &p1);
//     // add_number(&table, &p2);

//     // print_table(&table);

//     /*number_t p1 = num(16, 1);
//     number_t p2 = num(3, 2);
//     number_t p3 = num(20, 3);
//     number_t p4 = num(23, 4);
//     number_t p5 = num(60, 5);
//     number_t p6 = num(95, 6);
//     number_t p9 = num(77, 7);
//     number_t p10 = num(15, 8);
//     number_t p11 = num(11, 9);
//     number_t p13 = num(6, 10);
//     number_t p14 = num(17, 11);
//     number_t p16 = num(14, 12);
//     number_t p17 = num(13, 13);
//     number_t p18 = num(18, 14);
    
//     hashtable_t table = hashtable(20);
    
//     add_number(&table, &p1);
//     add_number(&table, &p2);
//     add_number(&table, &p3);
//     add_number(&table, &p4);
//     add_number(&table, &p5);
//     add_number(&table, &p6);
//     add_number(&table, &p9);
//     add_number(&table, &p10);
//     add_number(&table, &p11);
//     add_number(&table, &p13);
//     add_number(&table, &p14);
//     add_number(&table, &p16);
//     add_number(&table, &p17);
//     add_number(&table, &p18);
    
//     print_table(&table);

//     int result;
//     from_key(&table, 6, &result);
//     printf("%d", result);


//     return 0;*/
// }