#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>


bool fCase = false;

int frobcmp(char const * a, char const * b){
  char uA;
  char uB;
  for(; *a != ' ' && *b != ' '; a++, b++){

    char xorA = *a ^ 42;
    char xorB = *b ^ 42; //decrypt using xor 42

    if(fCase){
      xorA = toupper((unsigned char)xorA);
      xorB = toupper((unsigned char)xorB);
    }

    if(xorA < xorB || *a == ' ')
      return -1;

    if(xorA > xorB || *b == ' ')
      return 1;
  }

  return 0; //both are same ie exited loop
  
}

int frobcmpVoid(const void * a, const void * b){
  
  return frobcmp(*(char **)a, *(char **)b);
  
}


void freeStuff(char * word, char ** arr, int len){
  free(word);
  for(int i = 0; i < len; i++){
    free(arr[i]);
  }
  free(arr);
}

int main(int argc, char* argv[]){

  if(argc > 2){
    write(STDERR_FILENO, "Too many args\n", 14);
    exit(1);
  }

  if(argc == 2 && strcmp(argv[1], "-f") == 0){
    fCase = true;
  }
  
  if(argc == 2 && strcmp(argv[1], "-f") != 0){
    write(STDERR_FILENO, "Wrong Arg\n", 10);
    exit(1);
  }

  struct stat buf;
  int ret = fstat(STDIN_FILENO, &buf);
  if(ret == -1){
    write(STDERR_FILENO, "File Stat Error\n", 16);
    exit(1);
  }

  int arrCount = 0;
  char ** fileArr;
  char * regFile;
  
  if(S_ISREG(buf.st_mode)){
    
    regFile = (char*)malloc((buf.st_size) * sizeof(char));
    if(read(STDIN_FILENO, regFile, buf.st_size) == -1){
      write(STDERR_FILENO, "Reading File Error\n", 19);
      exit(1);
    }
    
    int wordCount = 0;
    int i;
    
    if(regFile[0] != ' ')
        wordCount++;

    for(int i = 1; i < buf.st_size-1; i++){

      if(regFile[i] == ' ' && regFile[i+1] != ' ')
	wordCount++;
    }
    
    fileArr = (char**)malloc(wordCount * sizeof(char*));
    if(fileArr == NULL){
      write(STDERR_FILENO,"Memory Allocation Error\n",24);
      exit(1);
    }

    if(regFile[0] != ' '){
        fileArr[0] = &regFile[0];
        arrCount++;
      }

    for(int i = 0; i < buf.st_size; i++){
      
      if(regFile[i] == ' ' && regFile[i+1] != ' '){
	fileArr[arrCount] = &regFile[i+1];
	arrCount++;
      }
    }
  }
  else{
    fileArr = NULL;
  }

  char c[1];
  char * word = NULL;
  int wlen = 0;
  int alen = 0;
  
  while(true){
    int check = read(STDIN_FILENO, c, 1);
    if(check == 0 && word == NULL)
      break;
    if(check == -1){
      write(STDERR_FILENO,"Reading from STDIN Error\n",25); 
      exit(1);
    }
    
    if(c[0] != ' ' && c[0] != '\n' && check != 0){
      wlen++;
      if(word == NULL){
	word = (char*) malloc(sizeof(char));
	if(word == NULL){
	  write(STDERR_FILENO, "Allocating Memory Error", 24);
	  freeStuff(word, fileArr, alen);
	  exit(1);
	}
      }
      else{
	word = (char*) realloc(word, wlen * sizeof(char));
	if(word == NULL){
          write(STDERR_FILENO, "Reallocating Memory Error", 26);
	  freeStuff(word, fileArr, alen);
          exit(1);
        }
      }
      word[wlen-1] = c[0];
    }
   
    if((c[0] == ' ' || c[0] == '\n' || check == 0) && word != NULL){
      alen++;
      if(fileArr == NULL){
	fileArr = (char**) malloc(sizeof(char*));
	if(fileArr == NULL){
          write(STDERR_FILENO, "Allocating Memory Error\n", 24);
	  freeStuff(word, fileArr, alen);
          exit(1);
        }
      }

      else{
	fileArr = (char**) realloc(fileArr, alen * sizeof(char*));
	if(fileArr == NULL){
          write(STDERR_FILENO, "Reallocating Memory Error\n",26 );
	  freeStuff(word, fileArr, alen);
          exit(1);
        }
      }
      fileArr[alen - 1] = word;
      word = NULL;
      wlen = 0;
    }
  }

  int aCount = 0;
  if(S_ISREG(buf.st_mode))
    aCount = arrCount;
  else
    aCount = alen;
  
  qsort(fileArr, aCount, sizeof(char*), frobcmpVoid); 

 
  char out[1];
  for(int i = 0; i < aCount; i++){
    for(int j = 0; fileArr[i][j] != 0 && fileArr[i][j] != ' ' && fileArr[i][j] != '\n'; j++){
      out[0] = fileArr[i][j];
      if(write(STDOUT_FILENO, out, 1) < 0){
	write(STDERR_FILENO, "Error writing STDOUT\n",21);
	exit(1);
	}
    }
    write(STDOUT_FILENO, " ", 1);
  }

  freeStuff(word, fileArr, alen);


  return 0;
}
