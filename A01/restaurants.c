#include <stdio.h>
#include <string.h>

struct restaurant {
    char name[100];
    int opening_hour;
    int closing_hour;
    float rating;
};

int main() {
  // define restaurants
  struct restaurant r1;
  struct restaurant r2;
  struct restaurant r3;

  // name restaurants
  strcpy(r1.name, "Sushi Atsumi ");
  strcpy(r2.name, "The Thirsty Horse");
  strcpy(r3.name, "Magic Bean Coffee");

  // set opening hours
  r1.opening_hour = 11;
  r2.opening_hour = 17;
  r3.opening_hour = 6;

  // set closing hours
  r1.closing_hour = 23;
  r2.closing_hour = 2;
  r3.closing_hour = 15;

  // set ratings
  r1.rating = 3.5;
  r2.rating = 4.5;
  r3.rating = 4.1;
  
  // store restaurants in array
  struct restaurant my_restaurants[3] = {r1, r2, r3};
  int number_of_restaurants = 3;

  int curr_hour;
  printf("Welcome to Steven Struct's Restarant List.\n\n");
  printf("What hour is it? ");
  scanf("%d", &curr_hour);
  // print all restaurants name, opening hour, closing hour, and rating
  int i = 0;
  while (i < number_of_restaurants) {
    printf("%d)   %s      open: %d      close: %d     rating: %f\n", i, my_restaurants[i].name, my_restaurants[i].opening_hour, my_restaurants[i].closing_hour, my_restaurants[i].rating);
    i++;
  }
  printf("Which restaurant would you like to visit?   [0,1,2] \n");
  // get user choice
  int choice;
  scanf("%d", &choice);

  // if user choice is invalid, print error message and exit
  if (choice < 0 || choice > 2) {
    printf("Invalid choice. Exiting.\n");
  }

  // if user choice is valid
  if (my_restaurants[choice].opening_hour < my_restaurants[choice].closing_hour) {
    // if restaurant is not open over night, check if current hour is between opening and closing hour
    if (curr_hour >= my_restaurants[choice].opening_hour && curr_hour <= my_restaurants[choice].closing_hour) {
      printf("Excellent choice. %s will be open for %d more hours.\n", my_restaurants[choice].name, my_restaurants[choice].closing_hour - curr_hour);
    } else {
      if (curr_hour < my_restaurants[choice].opening_hour) {
        printf("%s is not open isn't open until %d o'clock!!\n", my_restaurants[choice].name, my_restaurants[choice].opening_hour);
      } else {
        printf("%s closed at %d o'clock!!\n", my_restaurants[choice].name, my_restaurants[choice].closing_hour);
      }
    }
  } else {
    // if restaurant is open over night, check if current hour is between opening hour and midnight or between midnight and closing hour
    if (curr_hour >= my_restaurants[choice].opening_hour || curr_hour <= my_restaurants[choice].closing_hour) {
      if (curr_hour <= my_restaurants[choice].closing_hour) {
        printf("Excellent choice. %s will be open for %d more hours.\n", my_restaurants[choice].name, my_restaurants[choice].closing_hour - curr_hour);
      } else {
        printf("Excellent choice. %s will be open for %d more hours.\n", my_restaurants[choice].name, 24 - curr_hour + my_restaurants[choice].closing_hour);
      }
    } else {
      if (curr_hour - my_restaurants[choice].opening_hour > my_restaurants[choice].closing_hour - curr_hour) {
        printf("%s is not open until %d o'clock!!\n", my_restaurants[choice].name, my_restaurants[choice].opening_hour);
      } else {
        printf("%s closed at %d o'clock!!\n", my_restaurants[choice].name, my_restaurants[choice].closing_hour);
      }
    }
  }
  return 0;
}
