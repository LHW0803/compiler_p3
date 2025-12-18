int g;

int* test1(int x, int y) {
  g = x + y;
  return &g;
}

int main() {
  int x;
  int y;
  int z;
  int *a;

  x = 10;
  y = 15;
  z = *test1(x, y);

  write_string("z: ");
  write_int(z);
  write_string("\n");

  x = 20;
  y = 40;
  a = test1(x, y);

  write_string("*a: ");
  write_int(*a);
  write_string("\n");
}