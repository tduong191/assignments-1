#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void putPause(int* n, char* text, char* result) {
    int i;
    int j = 0;
    int len = strlen(text);
    // loop through the text with i
    for (i = 0; i < len; i++) {
        result[j] = text[i];
        j++; // use j to keep track of the result
        int count = 0;
        while (count < *n) {
            result[j] = '.';
            count += 1;
            j++;
    }
    result[j] = '\0';
  }
}

int main() {
  printf("Pause length: ");
  int pause;
  scanf("%d", &pause);
  printf("Text: ");
  char text[32];
  scanf("%s", text);
  char* result = malloc(strlen(text) + pause * strlen(text) + 1);
  putPause(&pause, text, result);
  printf("Result: %s\n", result);
  free(result);
  return 0;
}
