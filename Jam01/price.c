#include <stdio.h>
#include <stdlib.h>
#include <time.h>  // include time

int main() {
  srand(time(0)); // seed the random number generator
  printf("Welcome to the price is right!\n");
  printf("Enter a guess to win a luxurious dishwasher!\n");
  int guess;
  scanf("%d", &guess);
  int ai1 = 3000 + (rand() % 2001);
  int ai2 = 3000 + (rand() % 2001);
  int true_cost = 3000 + (rand() % 2001);
  printf("AI contestant #1 guesses $%d\n", ai1);
  printf("AI contestant #2 guesses $%d\n", ai2);
  printf("The dishwasher cost is $%d\n", true_cost);

  int ai1_diff = ai1 - true_cost;
  int ai2_diff = ai2 - true_cost;
  int guess_diff = guess - true_cost;

  if (ai1_diff > 0 && ai1_diff < ai2_diff && ai1_diff < guess_diff) {
    printf("AI contestant #1 wins!\n");
  } 
  else if (ai2_diff > 0 && ai2_diff < ai1_diff && ai2_diff < guess_diff) {
    printf("AI contestant #2 wins!\n");
  } 
  else if (guess_diff > 0 && guess_diff < ai1_diff && guess_diff < ai2_diff) {
    printf("You win!\n");
  } 
  else {
    printf("No one wins!\n");
  }
  return 0;
}

