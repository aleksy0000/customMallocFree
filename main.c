#include <stdio.h>
#include <stdlib.h>

typedef struct BlockHeader {
    size_t size;
    int is_free;
    struct BlockHeader* next;
} BlockHeader;

BlockHeader* head = NULL;

void* my_alloc(size_t size) {
    BlockHeader* block = malloc(sizeof(BlockHeader) + size);
    if (!block) return NULL;

    block->size = size;
    block->is_free = 0;
    block->next = NULL;

    if (head == NULL) {
        head = block;
    } else {
        BlockHeader* current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = block;
    }

    return (void*)(block + 1);
}

BlockHeader* get_header(void* user_ptr) {
    return ((BlockHeader*)user_ptr) - 1;
}

int main() {
    void* ptr1 = my_alloc(100);
    void* ptr2 = my_alloc(200);

    BlockHeader* h1 = get_header(ptr1);
    BlockHeader* h2 = get_header(ptr2);

    printf("Block 1: size=%zu, next=%p\n", h1->size, (void*)h1->next);
    printf("Block 2: size=%zu, next=%p\n", h2->size, (void*)h2->next);

    // Free entire blocks starting from header
    free(h1);
    free(h2);

    return 0;
}

