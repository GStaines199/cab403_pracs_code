#include <stdio.h>

int main(void)
{
  char input[128];
  (void)scanf("%128s", input);
  (void)printf("%s", input); /* Non-compliant */
}
