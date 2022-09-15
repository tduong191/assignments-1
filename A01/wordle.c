#include <stdio.h>
#include <string.h>
#include <time.h>
#include "words.h" // header that defined chooseWord()

int isIn(char input, const char *word, int numWord) {
    for (int i = 0; i < numWord; i++) {
      if (input == word[i]) {
          return 1;
      }
    }
    return 0;
}


int wordle(const char* word, char* input) {
  int i = 0;
  while (word[i] != '\0') {
    // if the word character is in the same spot as the input character
    if (word[i] == input[i]) {
      printf("%c is in the word AND in the correct spot!\n", word[i]);
    } else if (isIn(input[i], word, 5)) {
        printf("%c is in the word (but not in the correct spot)!\n", input[i]);
    } else {
      printf("%c is NOT in the word!\n", input[i]);
    }
    i++;
  }
  int res = strcmp(word, input);
  if (res == 0) {
    return 1;
  } else {
    return 0;
  }
}

int checkInput(char* input) {
  if (strlen(input) != 5) {
    printf("Input must be 5 characters long.\n\n");
    return 0;
  }
  for (int i = 0; i < 5; i++) {
    if (input[i] < 'a' || input[i] > 'z') {
      printf("Input must be lowercase letters.\n\n");
      return 0;
    }
  }
  return 1;
}

int main() {
  srand(time(0));
  const char* word = chooseWord();
  int guesses_left = 6;
  // printf("cheat: %s\n", word);
  while (guesses_left > 0) {
    printf("You have %d guesses left.\n", guesses_left);
    printf("Please enter a 5-letter word:\n");
    char input[50];
    scanf("%s", input);
    // check if input is 5 letters
    if (!checkInput(input)) {
      continue;
    }
    int result = wordle(word, input);
    if (result == 1) {
      printf("You win!\n");
      return 0;
    }
    guesses_left--;
  }
  printf("You lose! The word was %s\n", word);
  return 0;
}
