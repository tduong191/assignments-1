#include <stdio.h>

void shift(char* s, int* n) {
  // use ascii table to shift
  int i = 0;
  while (s[i] != '\0') {
      if (s[i] + *n > 122) {
          s[i] = s[i] + *n - 26;
      } else if (s[i] + *n < 97) {
          s[i] = s[i] + *n + 26;
      } else {
          s[i] = s[i] + *n;
      }
      i++;
  }
  printf("Your cypher is %s\n",s);
}

int main() {
  char str[50];
  int n;
  printf( "Enter a word: ");
  scanf("%s", str);
  printf( "Enter a shift: "); 
  scanf("%d", &n);
  shift(str, &n);
  return 0;
}
