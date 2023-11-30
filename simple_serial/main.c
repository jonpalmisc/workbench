#include <stdio.h>
#include <string.h>

#define SERIAL_SIZE 16

// Overly-verbose error set to allow for more interesting code paths.
typedef enum {
  SerialErrorNull = 0x10,
  SerialErrorTooShort,
  SerialErrorTooLong,
  SerialErrorBadCharacter,
  SerialErrorBadSum,
  SerialErrorOk,
} SerialError;

__attribute__((noinline)) SerialError validate_serial(char const *serial_str) {

  if (!serial_str)
    return SerialErrorNull;

  size_t serial_len = strlen(serial_str);
  if (serial_len > SERIAL_SIZE)
    return SerialErrorTooLong;
  else if (serial_len < SERIAL_SIZE)
    return SerialErrorTooShort;

  // Copy the serial while checking that it conforms to the Base32 alphabet.
  unsigned char serial[SERIAL_SIZE] = {0};
  for (size_t i = 0; i < serial_len; ++i) {
    switch (serial_str[i]) {
    case '1':
    case '8':
    case '9':
    case '0':
      return SerialErrorBadCharacter;
    default:
      serial[i] = serial_str[i];
    }
  }

  unsigned odd_sum = 0, even_sum = 0;
  for (int i = 0; i < SERIAL_SIZE; ++i) {
    if (i % 2 == 0)
      even_sum += serial[i];
    else
      odd_sum += serial[i];
  }

  if (odd_sum % 23 != 0)
    return SerialErrorBadSum;
  if (even_sum % 37 != 0)
    return SerialErrorBadSum;
  if ((even_sum + odd_sum) % 31 != 0)
    return SerialErrorBadSum;

  return SerialErrorOk;
}

int main(int argc, char const **argv) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <serial>\n", argv[0]);
    return 1;
  }

  SerialError error = validate_serial(argv[1]);
  if (error != SerialErrorOk) {
    printf("Invalid serial. (Error 0x%x)\n", error);
    return 1;
  }

  puts("Thank you for your purchase!");
  return 0;
}
