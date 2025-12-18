int x[10];

int main () {
  int i;
  int y[10];
  int z[10];

  for (i=0; i<=10; i++) {
    x[i] = i;
  }

  for (i=0; i<=10; i++) {
    y[i] = 100 + 10*i;
  }

  for (i=0; i<10; i++) {
    z[i] = x[i] + y[i];
  }

  for (i=0; i<10; i++) {
    write_string("x: ");
    write_int(x[i]);

    write_string(" | y: ");
    write_int(y[i]);

    write_string(" | z: ");
    write_int(z[i]);
    write_string("\n");
  }
}