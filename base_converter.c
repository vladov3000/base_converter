#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

int parse_base(char* base) {
  
#define invalid_base(fmt, ...) do {                               \
    printf("%d\n", __LINE__);                                     \
    printf("Invalid base '%s': " fmt "\n", base, ## __VA_ARGS__); \
    return -1;                                                    \
  } while (0)
  
  int res = 0;

  for (char c; (c = *base); base++) {
    res *= 10;
    
    if ('0' <= c && c <= '9')
      res += c - '0';
    else
      invalid_base("contains non-digit character '%c'", c);
  }

  if (res < 2 || res > 16)
    invalid_base("is not between 2-16");

  return res;
  
#undef invalid_base
}

int parse_num(char* num, int base) {

#define invalid_num(fmt, ...) do {                                      \
    printf("Invalid base '%d' number '%s': "                            \
           fmt "\n", base, num, ## __VA_ARGS__);                        \
    return -1;                                                          \
  } while(0);
  
  int res = 0;
  
  for (char c; (c = *num); num++) {
    res *= base;
    int to_add;
    
    if ('0' <= c && c <= '9')
      to_add = c - '0';
    if ('a' <= c && c <= 'f')
      to_add = c - 'a';
    if ('A' <= c && c <= 'F')
      to_add = c - 'A';

    if (to_add > base - 1)
      invalid_num("digit '%c' is too large", c);

    if (res > INT_MAX - to_add)
      invalid_num("number is too large. Max is %d", INT_MAX);
    
    res += to_add;
  }

  return res;
  
#undef invalid_num
}

void print_converted(int num, int base) {
  // the largest string is 8 bits * 32 bytes (num is an int) + 1 for null terminator
#define NUM_STR_LEN (32 * 8 + 1)
  
  char s[NUM_STR_LEN];
  int idx = NUM_STR_LEN - 1;
  s[idx--] = '\0';

  while (num > 0) {
    int digit = num % base;
    s[idx--] = digit + (digit <= 9 ? '0' : ('A' - 10));
    num /= base;
  }

  printf("%s\n", s + idx + 1);
}

int main(int argc, char** argv) {
  if (argc != 4) {
    printf(
           "Usage: base_converter BASE1 BASE2 NUM\n\n"
           "Converts NUM encoded in BASE1 to BASE2.\n"
           "BASEs are passed in as decimal numbers from 2 to 16\n"
           );

    return 1;
  }

  int base1 = parse_base(argv[1]);
  int base2 = parse_base(argv[2]);

  if (base1 == -1 || base2 == -1)
    return 1;
  
  int num = parse_num(argv[3], base1);

  print_converted(num, base2);

  return 0;
}
