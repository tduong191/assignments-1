#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node {
  float data;
  struct node *next;
  struct restaurant *rest;
};

struct node* insert_sorted(struct node* head, float data, struct restaurant *rest) {
  struct node* new_node = malloc(sizeof(struct node));
  new_node->data = data;
  new_node->next = NULL;
  new_node->rest = rest;

  if (head == NULL) {
    return new_node;
  }

  // insert front
  if (data > head->data) {
    new_node->next = head;
    return new_node;
  }

  // find insertion point
  struct node* current = head;
  while (current->next != NULL && current->next->data > data) {
    current = current->next;
  }
  new_node->next = current->next;
  current->next = new_node;
  return head;
};

void clear(struct node* head) {
  struct node* current = head;
  while (current != NULL) {
    struct node* next = current->next;
    free(current);
    current = next;
  }
}

struct restaurant {
    char name[32];
    int open_time;
    int close_time;
    float rating;
};

int main() {
  printf("How many restaurants do you want to enter? ");
  int num_restaurants;
  scanf("%d", &num_restaurants);

  struct node* head = NULL;
  struct restaurant* res = malloc(sizeof(struct restaurant)*num_restaurants);

  for (int i = 0; i < num_restaurants; i++) {
    printf("Enter a name: ");
    scanf(" %[^\n]%*c", res[i].name);
    printf("Open time: ");
    scanf("%d", &res[i].open_time);
    printf("Close time: ");
    scanf("%d", &res[i].close_time);
    printf("Stars: ");
    scanf("%f", &res[i].rating);
    head = insert_sorted(head, res[i].rating, &res[i]);

  }

  printf("Welcome to Sorted Sally's Restaurant Directory.\n\n");

  struct node* current = head;
  int i = 0;
  while (current != NULL) {
    printf("%d)", i);
    printf("   %3s  ", current->rest->name);
    printf("open: %3d   ", current->rest->open_time);
    printf("close: %3d   ", current->rest->close_time);
    printf("stars: %.1f   \n", current->rest->rating);
    current = current->next;
    i++;
  }

  clear(head);
  free(res);

  return 0;
}
