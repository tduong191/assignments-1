#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

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
  struct restaurant *my_restaurants = malloc(num_restaurants * sizeof(struct restaurant));
  for (int i = 0; i < num_restaurants; i++) {
    printf("Enter a name: ");
    scanf(" %[^\n]%*c", my_restaurants[i].name);
    printf("Open time: ");
    scanf("%d", &my_restaurants[i].open_time);
    printf("Close time: ");
    scanf("%d", &my_restaurants[i].close_time);
    printf("Stars: ");
    scanf("%f", &my_restaurants[i].rating);
  }
  printf("Welcome to Dynamic Donna's Restaurant Directory.\n\n");

  int i = 0;
  while (i < num_restaurants) {
    printf("%d)", i);
    printf("   %3s  ", my_restaurants[i].name);
    printf("open: %3d   ", my_restaurants[i].open_time);
    printf("close: %3d   ", my_restaurants[i].close_time);
    printf("stars: %3f   \n", my_restaurants[i].rating);
    i++;
  }
  free(my_restaurants);
  return 0;
}

