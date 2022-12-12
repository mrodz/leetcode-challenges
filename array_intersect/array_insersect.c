#include <stdlib.h>
#include <stdio.h>

#if 0
#	define TEST_MEMORY_LEAKS
#	define LEAK_MEMORY
#endif

typedef struct hash_map_node_t {
	struct hash_map_node_t *next;
	int key;
	int value;
} hash_map_node_t;

typedef struct hash_map_t {
	hash_map_node_t **raw;
	size_t capacity;
	size_t length;
} hash_map_t;

size_t hash(unsigned int x) {
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}

hash_map_t init(size_t max_capacity) {
	hash_map_t result;
	
	hash_map_node_t **node_base = malloc(sizeof(hash_map_node_t*) * max_capacity); // MUST cleanup.

	result.raw = node_base;
	result.capacity = max_capacity;
	result.length = 0;

	for (int i = 0; i < max_capacity; i++) {
		result.raw[i] = NULL;
	}

	return result;
}

hash_map_node_t *pair(int key, int value) {
	hash_map_node_t *result = malloc(sizeof(hash_map_node_t)); // MUST cleanup.

	result->key = key;
	result->value = value;
	result->next = NULL;

	return result;
}

int get(hash_map_t *map, int key, int *dest) {
	size_t maybe_position = hash(key) % map->capacity;

	if (map->raw[maybe_position] == NULL) {
		return 0;
	}

	hash_map_node_t *child = map->raw[maybe_position];

	while (1) {
		if (child == NULL) {
			return 0;
		}
	
		if (child->key == key) {
			*dest = child->value;
			return 1;
		}

		child = child->next;
	}
}

int insert(hash_map_t *map, int key, int value, int *swap) {
	size_t maybe_position = hash(key) % map->capacity;
	map->length += 1;

	if (map->raw[maybe_position] == NULL) {
		map->raw[maybe_position] = pair(key, value);
		return 0;
	}

	hash_map_node_t *child = map->raw[maybe_position];

	while (1) {
		if (child->key == key) {
			if (swap != NULL) {
				*swap = child->value;
			}

			child->value = value;
			return 0;
		}

		if (child->next == NULL) {
			child->next = pair(key, value);
			return 0;
		}

		child = child->next;
	}
}

void print_hashmap(hash_map_t *map) {
	for (int i = 0; i < map->capacity; i++) {
		if (map->raw[i] != NULL) {

			int key = map->raw[i]->key;
			int value = map->raw[i]->value;

			printf("[%d] { %d -> %d }", i, key, value);

			hash_map_node_t *child = map->raw[i];

			while (child->next != NULL) {
				int key = child->next->key;
				int value = child->next->value;

				printf(" + { %d -> %d }", key, value);
				child = child->next;
			}

			printf("\n"); 

		} else {
			printf("[%d] ---\n", i);
		}
	}
}

typedef void(*foreach_callback)(int index, int key, int value);

int index_checked(int *array, size_t i, size_t length, int *dest) {
	if (i >= length) return 0;

	*dest = array[i];

	return 1;
}

void foreach(hash_map_t *map, foreach_callback cb) {
	for (int i = 0, c = 0; i < map->length; i++) {
		if (map->raw[i] == NULL) continue;

		hash_map_node_t *child = map->raw[i];

		while (child != NULL) {
			(*cb)(c++, child->key, child->value);
			child = child->next;
		}
	}
}

void cleanup(hash_map_t *map) {
	for (int i = 0; i < map->capacity; i++) {
		if (map->raw[i] == NULL) continue;

		hash_map_node_t *child = map->raw[i];

		while (child != NULL) {
			hash_map_node_t *temp = child->next;
			free(child);
			child = temp;
		}
	}

	free(map->raw);
}

int *array_intersect(
	int *array1, size_t length1, 
	int *array2, size_t length2, 
	size_t *destination_length
) {
	size_t max_size = length1 > length2 ? length1 : length2;

	hash_map_t map = init(max_size);

	for (int i = 0; i < max_size; i++) {
		int this_element;
		if (index_checked(array1, i, length1, &this_element)) {
			int found = 0;
			get(&map, this_element, &found);
			insert(&map, this_element, found + 1, NULL);
		}

		if (index_checked(array2, i, length2, &this_element)) {
			int found = 0;
			get(&map, this_element, &found);
			insert(&map, this_element, found + 1, NULL);
		}	
	}

	int *result = malloc(sizeof(int) * map.length);
	int final_length = 0;

	for (int i = 0; i < map.capacity; i++) {
		if (map.raw[i] == NULL) continue;

		hash_map_node_t *child = map.raw[i];

		while (child != NULL) {
			if (child->value > 1) {
				result[final_length++] = child->key;
			}

			child = child->next;
		}
	}

	*destination_length = final_length;

	cleanup(&map);

	return result;
}

void test_memory_leaks() {
	int i = 100000;

	while (i > 0) {
		hash_map_t map = init(20);

		insert(&map, 10, 5, NULL);
		insert(&map, 7, 2, NULL);
		insert(&map, 14, 5, NULL);
		insert(&map, 88, -50, NULL);
		insert(&map, 6, 47, NULL);
		insert(&map, 3, 33, NULL);
		insert(&map, -1001, 1, NULL);

		insert(&map, 900, -1, NULL);
		insert(&map, 805, -2, NULL);
		insert(&map, 444, -3, NULL);
		insert(&map, 404, -4, NULL);
		insert(&map, 20, -5, NULL);

		insert(&map, 7, 3, NULL);

#ifndef LEAK_MEMORY
		cleanup(&map);
#endif
	}
}

int main(void) {
#ifdef TEST_MEMORY_LEAKS
	test_memory_leaks();
#endif
	int num1[] = { 1, 2, 3, 4 };
	int num2[] = { 4, 3, 6, 7 };

	size_t dest_len;

	int *result = array_intersect(num1, 4, num2, 4, &dest_len);

	for (int i = 0; i < dest_len; i++) {
		printf("%d ", result[i]);
	}

	return 0;
}