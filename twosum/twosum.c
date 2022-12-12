#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct number_t
{
    int key;
    int value;
    struct number_t *next;
    bool empty;
} number_t;

number_t num(int key, int value)
{
    number_t result;

    result.key = key;
    result.value = value;
    result.next = NULL;
    result.empty = true;

    return result;
}

void print_number(number_t *p)
{
    number_t *next;

    printf("{ %d: %d }", p->key, p->value);

    next = p->next;

    while (next != NULL)
    {
        printf(" & { %d: %d }", next->key, next->value);
        next = next->next;
    }

    printf("\n");
}

unsigned int hash_num(unsigned int x, size_t length)
{
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x % length;
}

typedef struct
{
    number_t *numbers;
    size_t length;
} hashtable_t;

hashtable_t hashtable(size_t length)
{
    number_t *numbers;
    hashtable_t result;

    numbers = (number_t *)calloc(length, sizeof(number_t));

    if (numbers == NULL)
    {
        printf("not enough space");
        exit(1);
    }

    for (int i = 0; i < length; i++)
    {
        numbers[i].empty = true;
    }

    result.numbers = numbers;
    result.length = length;

    return result;
}

void add_number(hashtable_t *table, number_t *p)
{
    number_t *copy;
    number_t *next;
    unsigned int hash;

    copy = (number_t *)malloc(sizeof(number_t));

    copy->key = p->key;
    copy->value = p->value;
    copy->next = p->next;
    copy->empty = p->empty;

    hash = hash_num(copy->key, table->length);

    if (table->numbers[hash].empty)
    {
        table->numbers[hash] = *copy;
        return;
    }

    next = &(table->numbers[hash]);

    while (1)
    {
        if (next->key == copy->key)
        {
            next->value = copy->value;
            return;
        }

        if (next->next == NULL)
        {
            next->next = copy;
            return;
        }

        next = next->next;
    }

    exit(1); // never reached
}

bool from_key(hashtable_t *table, int key, int *dest)
{
    unsigned int hash;
    number_t *next;

    hash = hash_num(key, table->length);

    if (table->numbers[hash].empty)
    {
        return false;
    }

    if (table->numbers[hash].key == key)
    {
        *dest = table->numbers[hash].value;
        return true;
    }

    next = table->numbers[hash].next;

    while (next != NULL)
    {
        if (next->key == key)
        {
            *dest = next->value;
            return true;
        }

        next = next->next;
    }

    return false;
}

void print_table(hashtable_t *table)
{
    printf("{\n");
    for (int i = 0; i < table->length; i++)
    {
        printf("\t[%d]: ", i);
        if (!table->numbers[i].empty)
        {
            print_number(&(table->numbers[i]));
        }
        else
        {
            printf("---\n");
        }
    }
    printf("}\n");
}

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int *twoSum(int *nums, int numsSize, int target, int *returnSize)
{
    hashtable_t mapping = hashtable(numsSize);

    for (int i = 0; i < numsSize; i++)
    {
        int difference;
        int found;

        difference = target - nums[i];

        if (from_key(&mapping, nums[i], &found))
        {
            int *result;

            result = (int *)calloc(2, sizeof(int));

            if (result == NULL)
            {
                goto FAIL;
            }

            print_table(&mapping);
            clean_hashtable(&mapping);
            result[0] = found;
            result[1] = i;
            *returnSize = 2;
            return result;
        }
        else
        {
            number_t *n;

            n = (number_t *)malloc(sizeof(number_t));
            *n = num(difference, i);
            add_number(&mapping, n);
        }
    }

FAIL:
    print_table(&mapping);
    *returnSize = 0;
    return NULL;
}