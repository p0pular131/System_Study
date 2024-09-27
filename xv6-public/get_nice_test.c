#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char **argv)
{
  if(argc != 2){
    printf(2, "usage: getnice pid...\n");
    exit();
  }
  int num = getnice(atoi(argv[1]));  
  if(num < 0) printf(1, "Fail to get nice value ...\n");
  printf(1, "%d\n", num);
  exit();
}
