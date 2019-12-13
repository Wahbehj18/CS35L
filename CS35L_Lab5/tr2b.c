#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


int main(int argc, char *argv[]){

  if(argc != 3){
    fprintf(stderr,"Too many arguments\n");
    exit(1);
  }
  
  int fromLen = strlen(argv[1]);
  int toLen = strlen(argv[2]);
  if(fromLen != toLen){
    fprintf(stderr, "from and to are not the same length\n");
    exit(1);
  }

  for(int i = 0; i < fromLen; i++){
    for(int j = i+1; j < fromLen; j++){
      if(argv[1][i] == argv[1][j]){
	fprintf(stderr, "duplicate chars in from\n");
	exit(1);
      }	
    }
  }

  bool contains = false;
  while(!feof(stdin)){
    char c = getchar();

    if(ferror(stdin)){
      fprintf(stderr, "Standard input Error");
      exit(1);
    }
    contains = false;

    if(c == EOF)
      break;
    


    for(int i = 0; i < fromLen; i++){
      if(c == argv[1][i]){
	putchar(argv[2][i]);
	if(ferror(stdout)){
	  fprintf(stderr,"Standard ouput Error");
	  exit(1);
	}
	contains = true;
      }
    }

    if(!contains){
      putchar(c);
         if(ferror(stdout)){
          fprintf(stderr,"Standard ouput Error");
          exit(1);
        }
    }
    
  }

  
  return 0;
}


  
