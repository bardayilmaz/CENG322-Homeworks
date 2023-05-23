#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

unsigned countNumOfElements(char* filename);

unsigned* readNumbers(char* filename, unsigned num_element);

pthread_mutex_t lock;

typedef struct node {
    struct node* r_node;
    unsigned value;
    pthread_mutex_t mutex; // -> ????
} node;

typedef struct hash_table {
    node** list;
    unsigned nof_element; // sum of each row's element count
} hash_table;

// for casting thread arguments to (void *)
typedef struct parameterPass {
    unsigned thread_id;
    unsigned num_threads;
    unsigned* numbers;
    unsigned num_elements;
    hash_table* table;
} parameterPass;

hash_table* initializeHashTable(unsigned numOfThread, unsigned numElements);

void* insertionFunction(void* parameters);

void swap(node* a, node* b);

void bubbleSort(node* start);

unsigned countNumOfElements(char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening the file: %s\n", filename);
        return 0;
    }
    
    unsigned count = 0;
    char buffer[256];
    
    // Skip the header line
    if (fgets(buffer, sizeof(buffer), file) == NULL) {
        fclose(file);
        return 0;
    }
    
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        count++;
    }
    
    fclose(file);
    return count;
}

unsigned* readNumbers(char* filename, unsigned num_elements) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening the file: %s\n", filename);
        return NULL;
    }
    
    // Skip the header line
    char buffer[256];
    if (fgets(buffer, sizeof(buffer), file) == NULL) {
        fclose(file);
        return NULL;
    }
    
    unsigned* numbers = malloc(num_elements * sizeof(unsigned));

    if (numbers == NULL) {
        fclose(file);
        printf("Memory allocation failed\n");
        return NULL;
    }
    
    unsigned count = 0;
    while (fgets(buffer, sizeof(buffer), file) != NULL && count < num_elements) {
        sscanf(buffer, "%*[^,],%u", &numbers[count]);
        count++;
    }
    
    fclose(file);
    return numbers;
}

// thread sayısı
hash_table* initializeHashTable(unsigned numOfThread, unsigned numElements) {
    hash_table* table = malloc(sizeof(hash_table));
    if (table == NULL) {
        printf("Memory allocation failed\n");
        return NULL;
    }
    
    table->list = malloc(numOfThread * (numOfThread + 1) / 2 * sizeof(node*));
    if (table->list == NULL) {
        free(table);
        printf("Memory allocation failed\n");
        return NULL;
    }
    
    table->nof_element = 0;
    
    // Initialize the linked list pointers to NULL
    for (unsigned i = 0; i < numOfThread * (numOfThread + 1) / 2; i++) {
        table->list[i] = NULL;
    }
    
    return table;
}

void* insertionFunction(void* parameters) {
    parameterPass* params = (parameterPass*)parameters;
    unsigned thread_id = params->thread_id;
    unsigned num_threads = params->num_threads;
    unsigned* numbers = params->numbers;
    unsigned num_elements = params->num_elements;
    hash_table* table = params->table;

    unsigned interval = num_elements / num_threads + 1;
    unsigned offset = interval * thread_id;
    unsigned last_element = offset + interval;
    if (last_element > num_elements) {
        last_element = num_elements;
    }

    for (unsigned i = offset; i < last_element; i++) {
        unsigned number = numbers[i];
        // printf("number: %i\n", number);
        
        // Calculate the index in the hash table using the hashing function
        unsigned index = numbers[i] % 6;
        // printf("index: %i, thread_id:\n\n\n", index);
        // Create a new node to store the number
        node* new_node = malloc(sizeof(node*));
        if (new_node == NULL) {
            printf("Memory allocation failed\n");
            return NULL;
        }
        new_node->value = number;
        new_node->r_node = NULL;
        
        // pthread_mutex_init(&(new_node->mutex), NULL); // bu burda olmcuak
        
        // // Acquire a lock to modify the linked list in the hash table
        pthread_mutex_lock(&lock);
        // // Insert the node at the beginning of the linked list
        new_node->r_node = table->list[index];
        table->list[index] = new_node;
        
        // // Update the total number of elements in the hash table
        pthread_mutex_unlock(&lock);
        table->nof_element++;
        
        // // Release the lock
        // pthread_mutex_unlock(&table->list[index]->mutex);
    }
    
    pthread_exit(NULL);
}

void swap(node* a, node* b) {
    unsigned temp = a->value;
    a->value = b->value;
    b->value = temp;
}

void bubbleSort(node* start) {
    int swapped;
    node* ptr1;
    node* lptr = NULL;

    /* Checking for empty list */
    if (start == NULL)
        return;

    do {
        swapped = 0;
        ptr1 = start;

        while (ptr1->r_node != lptr) {
            if (ptr1->value > ptr1->r_node->value) {
                swap(ptr1, ptr1->r_node);
                swapped = 1;
            }
            ptr1 = ptr1->r_node;
        }
        lptr = ptr1;
    } while (swapped);
}

parameterPass* createParameters(hash_table* table, unsigned* numbers, unsigned numElements, unsigned numOfThreads, unsigned threadId) {
    parameterPass* params = malloc(sizeof(parameterPass));
    if (params == NULL) {
        perror("Failed to allocate memory for parameters");
        exit(1);
    }

    params->table = table;
    params->thread_id = threadId;
    params->numbers = numbers;
    params->num_elements = numElements;
    params->num_threads = numOfThreads;
    return params;
}

void printLinkedList(node* head) {
    node* current = head;
    while (current != NULL) {
        printf("%u ", current->value);
        current = current->r_node;
    }
    printf("\n");
}

void freeHashTable(hash_table* table) {
    if (table == NULL) {
        return;
    }

    // Free each linked list in the hash table
    for (unsigned i = 0; i < table->nof_element; i++) {
        node* current = table->list[i];
        while (current != NULL) {
            node* next = current->r_node;
            free(current);
            current = next;
        }
    }

    // Free the array of linked lists
    free(table->list);

    // Free the hash table struct
    free(table);
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage: ./program <filename> <numOfThreads>\n");
        return 1;
    }

    char* filename = argv[1];
    unsigned numOfThreads = atoi(argv[2]);

    // Step 1: Count the number of elements in the file
    unsigned numElements = countNumOfElements(filename);
    printf("Number of elements: %u\n", numElements);

    // Step 2: Read the numbers from the file
    unsigned* numbers = readNumbers(filename, numElements);

    // Step 3: Initialize the hash table
    hash_table* table = initializeHashTable(numOfThreads, numElements);

    // Step 4: Create the parameter structs for threads
    parameterPass** threadParams = malloc(numOfThreads * sizeof(parameterPass*));

    pthread_t* threads = (pthread_t*) malloc(numOfThreads * sizeof(pthread_t));

    if (pthread_mutex_init(&lock, NULL) != 0) {
		printf("\n mutex init has failed\n");
		return 1;
	}
    for(int i = 0; i < numOfThreads; i++) {
        threadParams[i] = createParameters(table, numbers, numElements, numOfThreads, i);
        int thread_creation_result = pthread_create(&threads[i], NULL, insertionFunction, (void*) threadParams[i]);
        if(thread_creation_result != 0) {
            printf("Error creating thread\n");
            return 1;
        }
    }

    for (int i = 0; i < numOfThreads; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("aye %i\n", table->list[5]->r_node->value);

    for(int i = 0; i<numOfThreads; i++) {
        free(threadParams[i]);
    }

    int sortingThreadCount = (numOfThreads * (numOfThreads + 1)) / 2;
    pthread_t* sortingThreads = (pthread_t*) malloc(sortingThreadCount * sizeof(pthread_t));

    // for(int i = 0; i < sortingThreadCount; i++) {
    //     int thread_creation_result = pthread_create(&sortingThreads[i], NULL, bubbleSort, (void*) table->list[i]);
    //     if(thread_creation_result != 0) {
    //         printf("Error creating thread\n");
    //         return 1;
    //     }
    // }

    // for (int i = 0; i < sortingThreadCount; i++) {
    //     pthread_join(sortingThreads[i], NULL);
    // }
/*
    // Step 7: Sort the linked lists within the hash table
    for (unsigned i = 0; i < numOfThreads; i++) {
        bubbleSort(table->list[i]);
    }
printf("here\n");
    // Step 8: Print the sorted numbers
    printf("Sorted numbers:\n");
    for (unsigned i = 0; i < numOfThreads; i++) {
        printLinkedList(table->list[i]);
    }
printf("here\n");
    // Step 9: Cleanup - free memory
    for (unsigned i = 0; i < numOfThreads; i++) {
        free(threadParams[i]);
    }
    free(threadParams);
    free(threads);
    free(numbers);
    freeHashTable(table);
*/
    return 0;
}