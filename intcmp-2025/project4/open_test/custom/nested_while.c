int main() {
  int x;
  int y;
  int z;

  x = 0;
  y = 0;
  z = 0;

  while (x <= 10) {
    while (y < 5) {
      if (y == 3) { y=4; continue; }

      write_int(y);  /* 0 1 2 4 */
      write_string("\n");
      y++;
    }
    z = z + x++;
  }
  
  write_int(z);       /* 1 + 2 + 3 + 4 + 6 + 7 + 8 + 9 + 10 = 55 */
  write_string("\n");
}