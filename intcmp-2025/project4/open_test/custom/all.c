struct st1 {
  int x;
  char y;

  struct st2 {
    int p;
    char q;

    struct st3 {
      int a;
    } st3e;
  } st2e;
};

struct st4 {
  int x[3];
};

/* Global Variables (Total size: 21) */
int g1;             /* size: 1 */
struct st1 st1e;    /* size: 5 */
struct st4 st4e[5]; /* size: 15 */

int test1(int x, int y) {
  return x + y;
}

int test2(int *x, int *y) {
  return *x + *y;
}

int *test3(int *x, int y) {
  g1 = *x + y;
  return &g1;
}

int main() {
  int x;
  int y;
  int o1;
  int o2;
  int o3;

  int i;
  int j;
  int k;

  /* 1. Basic Function Test */
  x = 1;
  y = 3;
  o1 = test1(x, y);

  write_string("## [1] Basic Function Test\n");
  write_string("o1: ");
  write_int(o1);
  write_string(" (answer is 4)\n");


  /* 2. Pointer Parameter Function Test */
  x = 3;
  y = 4;
  o2 = test2(&x, &y);

  write_string("## [2] Pointer Parameter Function Test\n");
  write_string("o2: ");
  write_int(o2);
  write_string(" (answer is 7)\n");

  /* 3. Complicated Function Test */
  o3 = *test3(&x, 5);

  write_string("## [3] Complicated Function Test\n");
  write_string("o3: ");
  write_int(o3);
  write_string(" (answer is 8)\n");

  /* 4. Struct Test */
  st1e.x = 6;
  st1e.y = 'A';
  st1e.st2e.p = 10;
  st1e.st2e.q = 'B';
  st1e.st2e.st3e.a = test1(st1e.x, st1e.st2e.p);

  write_string("## [4] Struct Test\n");
  write_string("st1e.x: ");
  write_int(st1e.x);
  write_string(" | st1e.y: ");
  write_char(st1e.y);
  write_string("\n");
  write_string("st1e.st2e.p: ");
  write_int(st1e.st2e.p);
  write_string(" | st1e.st2e.q: ");
  write_char(st1e.st2e.q);
  write_string("\n");
  write_string("st1e.st2e.st3e.a: ");
  write_int(st1e.st2e.st3e.a);
  write_string("\n");

  /* 5. INC/DEC Test */
  x = 10;
  y = 20;

  write_string("## [5] INC/DEC Test\n");
  write_string("x: ");
  write_int(x++);
  write_string(" (answer is 10)");
  write_string(" | now, x: ");
  write_int(x);
  write_string(" (answer is 11)");
  write_string(" | and this should be, x: ");
  write_int(++x);
  write_string(" (answer is 12)\n");

  write_string("y: ");
  write_int(y--);
  write_string(" (answer is 20)");
  write_string(" | now, y: ");
  write_int(y);
  write_string(" (answer is 19)");
  write_string(" | and this should be, y: ");
  write_int(--y);
  write_string(" (answer is 18)\n");

  write_string("x++ + y++: ");
  write_int(x++ + y++);
  write_string(" (answer is 30)");
  write_string(" | now, x + y: ");
  write_int(x + y);
  write_string(" (answer is 32)");
  write_string(" | and this should be, ++x + ++y: ");
  write_int(++x + ++y);
  write_string(" (answer is 34)\n");

  /* 6. IF-ELSE Test */
  x = 3;
  y = 6;

  write_string("## [6] IF-ELSE Test\n");
  write_string("x: ");    write_int(x);
  write_string(" | y: "); write_int(y);
  write_string("\n");
  write_string("Returns true.. when |");
  if (x == y) write_string(" x==y |");
  if (x >= y) write_string(" x>=y |");
  if (x <= y) write_string(" x<=y |");
  if (x > y) write_string(" x>y |");
  if (x < y) write_string(" x<y |");
  write_string("\n");

  write_string("Returns false.. when |");
  if (!(x == y)) write_string(" x==y |");
  if (!(x >= y)) write_string(" x>=y |");
  if (!(x <= y)) write_string(" x<=y |");
  if (!(x > y)) write_string(" x>y |");
  if (!(x < y)) write_string(" x<y |");
  write_string("\n");

  write_string("Checks for else staetements... ");
  if(x >= 5) {
    if (y > 5) write_string("(x >= 5) & (y > 5)\n");
    else write_string("(x >= 5) & (y <= 5)\n");
  }
  else {
    write_string("(x < 5)\n");
  }

  /* 7. For-Loop Test */
  write_string("## [7] For-Loop Test\n");

  for(i = 0; i < 5; i++) {
    for (j=0; j < 3; j++) {
      st4e[i].x[j] = i + j;
    }
  }

  for(i = 0; i < 5; i++) {
    for (j=0; j < 3; j++) {
      write_string("st4e["); write_int(i); write_string("]");
      write_string(".x["); write_int(j); write_string("]: ");
      write_int(st4e[i].x[j]); write_string("\n");
    }
  }

  /* 8. While-Loop Test */
  write_string("## [8] While-Loop Test\n");

  i = 0;
  j = 0;
  k = 0;

  while (i < 5) {
    while (j < 3) {
      write_string("i: "); write_int(i);
      write_string(" | j: "); write_int(j);
      write_string(" | k: "); write_int(k);
      write_string("\n");

      j++;
      k++;
    }
    i++;
    j = 0;
  }

  /* 9. Break/Continue Test */
  write_string("## [9-1] For-Loop Break Test\n");

  for(i = 0; i < 5; i++) {
    for (j=0; j < 5; j++) {
      if (j == 3) break;
      if (j >= 3) write_string("* Never Printed Line\n");
      else {
        write_string("st4e["); write_int(i); write_string("]");
        write_string(".x["); write_int(j); write_string("]: ");
        write_int(st4e[i].x[j]); write_string("\n");
      }
    }
  }

  write_string("## [9-2] For-Loop Continue Test\n");
  
  for(i = 0; i < 5; i++) {
    for (j=0; j < 5; j++) {
      if (j == 3) continue;
      if (j >= 3) write_string("* Should be Printed if j > 3\n");
      else {
        write_string("st4e["); write_int(i); write_string("]");
        write_string(".x["); write_int(j); write_string("]: ");
        write_int(st4e[i].x[j]); write_string("\n");
      }
    }
  }

  write_string("## [9-3] While-Loop Break Test1\n");

  i = 0;
  j = 0;
  k = 0;

  while (i < 5) {
    while (j < 5) {
      if (j == 3) break;
      if (j >= 3) write_string("* Never Printed Line\n");
      else {
        write_string("i: "); write_int(i);
        write_string(" | j: "); write_int(j);
        write_string(" | k: "); write_int(k);
        write_string("\n");
      }

      j++;
      k++;
    }
    i++;
    j = 0;
  }

  write_string("## [9-3] While-Loop Break Test2\n");

  i = 0;

  while(1) {
    if (i > 15) break;
    write_string("i: "); write_int(i); write_string(" | ");
    i = i+2;
  }
  write_string("\n");

  write_string("## [9-4] While-Loop Continue Test\n");

  i = 0;
  j = 0;
  k = 0;

  while (i < 5) {
    while (j < 5) {
      if (j == 3) { j = 5; continue; }
      if (j >= 3) write_string("* Never Printed Line\n");
      else {
        write_string("i: "); write_int(i);
        write_string(" | j: "); write_int(j);
        write_string(" | k: "); write_int(k);
        write_string("\n");
      }

      j++;
      k++;
    }
    i++;
    j = 0;
  }
}