#include "types.h"
#include "stat.h"
#include "user.h"
#include "param.h"
#include "fcntl.h"

int main(void) {
int size = 8192;
int fd = open("README", O_RDWR);
printf(1,"freemem now is %d\n",freemem());
char* text = (char*)mmap(0, size, PROT_READ|PROT_WRITE, MAP_POPULATE, fd, 0);
printf(1, "mmap return is: %d\n", text);
printf(1,"freemem now is %d\n",freemem());
printf(1, "parent text[110] is: %c\n", text[110]);
text[110] = '9';
printf(1, "parent text[110] is: %c\n", text[110]);

int fo;
if((fo=fork())==0){
  printf(1,"fork! child freemem now is %d\n",freemem());
    printf(1, "child text[110] is: %c\n", text[110]);
  text[110] = '7';
  printf(1, "child text[110] is: %c\n", text[110]);
  exit();
}
else{
  wait();
  printf(1,"parent freemem now is %d\n",freemem());
  printf(1, "parent text[110] is: %c\n", text[110]);
  exit();
}
}