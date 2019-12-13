#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


int main(int argc, char *argv[]){

  if(argc != 3){
    write(STDERR_FILENO,"Too many arguments\n", 19);
    exit(1);
  }
  
  int fromLen = strlen(argv[1]);
  int toLen = strlen(argv[2]);
  if(fromLen != toLen){
    write(STDERR_FILENO, "from and to are not the same length\n", 36);
    exit(1);
  }

  for(int i = 0; i < fromLen; i++){
    for(int j = i+1; j < fromLen; j++){
      if(argv[1][i] == argv[1][j]){
	write(STDERR_FILENO, "duplicate chars in from\n", 24);
	exit(1);
      }	
    }
  }

  char in[1];
  char out[1];

  bool contains = false;
  while(read(STDIN_FILENO, in, 1) == 1){
    contains = false;

    for(int i = 0; i < fromLen; i++){
      if(in[0] == argv[1][i]){
	out[0] = argv[2][i];
	if(write(STDOUT_FILENO, out, 1) != 1){
	  write(STDERR_FILENO,"Standard ouput Error\n", 21);
	  exit(1);
	}
	contains = true;
      }
    }

    if(!contains){
      if(write(STDOUT_FILENO, in, 1) != 1){
	write(STDERR_FILENO,"Standard ouput Error\n", 21);
	exit(1);
      }
    }
    
  }
  
  return 0;
}


  
