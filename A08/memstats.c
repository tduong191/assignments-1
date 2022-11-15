#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include "rand.h"

#define ROUNDS 3
#define BUFFER 5
#define LOOP 10

struct chunk {
  int size;
  int used;
  struct chunk *next;
};

void memstats(struct chunk* freelist, void* buffer[], int len) {
  // get the current size and memory of the free list
  int free_block = 0;
  int free_memory = 0;
  struct chunk* next = freelist;
  while (next != NULL) {
    free_memory += next->size;
    free_block++;
    next = next->next;
  }
  // get the current size of the buffer
  int used_memory = 0;
  int used_block = 0;
  int unused_memory = 0;
  for (int i = 0; i < len; i++) {
    if (buffer[i] != NULL) {
      struct chunk* cnk = (struct chunk*)((struct chunk*)buffer[i] - 1);
      used_memory += cnk->size;
      unused_memory += cnk->size - cnk->used;
      used_block++;
    }
  }
  // print the current block usage
  printf("Total Blocks: %d, Free Blocks: %d, Used Blocks: %d\n", free_block + used_block, free_block, used_block);
  // print the current memory usage
  printf("Total Memory: %d, Free Memory: %d, Used Memory: %d\n", free_memory + used_memory, free_memory, used_memory);
  // print underutilized 
  printf("Underutilized: %.2f\n", (float)unused_memory/used_memory);
}

int main ( int argc, char* argv[]) {

  printf("Starting test..\n");

  srand(100);

  double timer;
  struct timeval tstart, tend;

  gettimeofday(&tstart, NULL);

  void *buffer[BUFFER];
  for (int i = 0; i < BUFFER; i++) {
    buffer[i] = NULL;
  }

  void *init = sbrk(0);
  void *current;
  printf("The initial top of the heap is %p.\n", init);
  for (int j = 0 ; j < ROUNDS; j++) {
    printf("---------------\n%d\n" , j);

    for (int i= 0 ; i < LOOP ; i++) {
      int index = rand() % BUFFER;
      if (buffer[index] != NULL) {
        free(buffer[index]);
        buffer[index] = NULL;
        printf("Freeing index %d\n", index);
      } 
      else {
        size_t size = (size_t) randExp(8, 4000); 
        int *memory = NULL;
        memory = malloc(size);

        if (memory == NULL) {
          fprintf(stderr, "malloc failed\n");
          return(1);
        } 
        *memory = 123;
        buffer[index] = memory;
        printf("Allocating %d bytes at index %d\n", (int) size, index);
      }
    }
    extern struct chunk* flist;
    current = sbrk(0);
    int allocated = current - init;
    init = current;

    printf("The new top of the heap is %p.\n", current);
    printf("Increased by %d (0x%x) bytes\n", allocated, allocated);
    memstats(flist, buffer, BUFFER);
  }

  for (int i = 0; i < BUFFER; i++) {
    free(buffer[i]); 
  }
  gettimeofday(&tend, NULL);
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
  printf("Time is %g\n", timer);

  return 0 ;
}
