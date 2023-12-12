const FLAG_DATA = 1;
const FLAG_BREAK = 2;

function Value2Text(flag, value) {
  switch (flag) {
    case FLAG_DATA:
      return "0x" + value.toString(16).toUpperCase();
    case FLAG_BREAK:
      return "BRK";
  }
}
