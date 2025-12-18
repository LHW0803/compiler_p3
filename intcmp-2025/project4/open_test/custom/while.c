int main() {
  int x;
  int z;

  x = 0;
  z = 0;

  while (x <= 10) {
    if (x==5) { x++; continue; }
    if (x==8) { break; }

    z = z + x++;
  }
  
  write_int(z);       /* 1 + 2 + 3 + 4 + 6 + 7 = 23 */
  write_string("\n");
}