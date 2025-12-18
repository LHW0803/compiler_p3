int x;

int main () {
  int y;

  x = 1;
  y = 10;

  write_string("Initial x: 1, y: 10\n");

  write_string("x++: ");
  write_int(x++);
  write_string(" / then: ");
  write_int(x);
  write_string("\n");

  write_string("++y: ");
  write_int(++y);
  write_string("\n");

}