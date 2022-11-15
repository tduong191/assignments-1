#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>

struct chunk {
  int size;
  int used;
  struct chunk *next;
};
extern struct chunk* flist;

void check(int expr, const char* message) {
  if (!expr) {
    printf("%s: FAILED\n", message);
    exit(1);
  }
  else {
    printf("%s: PASSED\n", message);
  }
}

int main (int argc, char* argv[]) {

  printf("Running tests...\n");

  void *current;
  void *init = sbrk(0);

  free(0); // shouldn't crash
  void* empty = malloc(0); // should return null
  check(empty == 0, "test 1: size 0 returns NULL");
  check(flist == 0, "test 2: flist is NULL to start");

  void *request1 = malloc(sizeof(char)*32);
  current = sbrk(0);
  check(flist == 0, "test 3: flist is empty after first malloc");
  check((current-init) == 32+16, "test 4: correct amount allocated");

  struct chunk* header1 = (struct chunk*) ((struct chunk*) request1 - 1);
  check(header1->size == 32, "test 5: header size correct");
  check(header1->used == 32, "test 6: header used correct");

  free(request1);
  check(flist != 0, "test 7: flist is non-empty after free");

  request1 = malloc(sizeof(char) * 16);
  current = sbrk(0);
  check(flist == 0, "test 8: flist is empty");
  check((current-init) == 32+16, "test9: correct amount allocated");

  header1 = (struct chunk*) ((struct chunk*) request1 - 1);
  check(header1->size == 32, "test 10: header size correct");
  check(header1->used == 16, "test 11: header used correct");
  free(request1);
  
  void* request2 = malloc(sizeof(char) * 64);
  current = sbrk(0);
  check(flist != 0, "test 12: flist is not empty");
  check((current-init) == 32+2*16+64, "test 13: current-init correct size");
  
  struct chunk* header2 = (struct chunk*) ((struct chunk*) request2 - 1);
  check(header2->size == 64, "test 14: header size correct");
  check(header2->used == 64, "test 15: header used correct");

  free(request2);
  check(flist->size == 64, "test 16: flist's first node has correct size");
  check(flist->next != 0, "test 17: flist is non-empty");
  check(flist->next->size == 32, "test 18: flist's second node has correct size");

  return 0 ;
}
