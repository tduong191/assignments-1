#include <stdio.h>

int main() {
  char message[32];
  printf("Please enter a phrase: ");
  fgets(message, 31, stdin);
  message[31] = '\0';
  // make a mask for the 3rd bit
  unsigned char mask3rd = 0b00000100;
  // make a mask for the 4th bit
  unsigned char mask4th = 0b00001000;
  // loop through the message
  for (int i = 0; i < 32; i ++) {
    if (message[i] == '\0') {
      break;
    }
    if (message[i] == ' ') {
      continue;
    }
    // get the 3rd bit
    unsigned char bit3rd = message[i] & mask3rd;
    printf("3rd bit: %d\n", bit3rd);
    // get the 4th bit
    unsigned char bit4th = message[i] & mask4th;
    printf("4th bit: %d\n", bit4th);
    // shift the 3rd bit to the 4th bit
    bit3rd = bit3rd << 1; 
    printf("3rd bit shifted: %d\n", bit3rd);
    // shift the 4th bit to the 3rd bit
    bit4th = bit4th >> 1;
    printf("4th bit shifted: %d\n", bit4th);
    // erase the 3rd and 4th bits
    message[i] = message[i] & 0b11110011;
    // get the new character
    unsigned char new_char = message[i] | bit3rd | bit4th;
    printf("new char: %c\n", new_char);
    message[i] = new_char;
  }
  // todo: modify message using bitwise operations
  printf("scramble: %s\n", message);
}
