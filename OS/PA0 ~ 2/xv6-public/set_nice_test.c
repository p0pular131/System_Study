#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int
main(int argc, char **argv)
{
  if(argc != 3){
    printf(2, "usage: setnice pid nice...\n");
    exit();
  }
  if(setnice(atoi(argv[1]), atoi(argv[2])) == -1) {
    printf(1, "Fail to set nice value.\n");
  }
  exit();
}