#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int frobcmp(char const * a, char const * b){

  for(; *a != ' ' && *b != ' '; a++, b++){

    char xorA = *a ^ 42;
    char xorB = *b ^ 42; //decrypt using xor 42

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

int main(){

  char c;
  char * word = NULL;
  char ** arr = NULL;
  int wlen = 0;
  int alen = 0;
  
  while(!feof(stdin)){
   
    c = getc(stdin);
    if (ferror(stdin)) {
      fprintf(stderr, "Error from Standard Input");
      exit(1);
    }

    if(c != ' ' && c != '\n' && c != EOF){
      wlen++;
      if(word == NULL){
	free(word);
	word = (char*) malloc(sizeof(char));
	if(word == NULL){
	  fprintf(stderr, "Allocating Memory Error");
	  freeStuff(word, arr, alen);
	  exit(1);
	}
      }
      else{
	word = (char*) realloc(word, wlen * sizeof(char));
	if(word == NULL){
          fprintf(stderr, "Reallocating Memory Error");
	  freeStuff(word, arr, alen);
          exit(1);
        }
      }
      word[wlen-1] = c;
    }
    
    if((c == ' ' || c == '\n' || c == 10 || c == EOF) && word != NULL){
      alen++;
      if(arr == NULL){
	arr = (char**) malloc(sizeof(char*));
	if(arr == NULL){
          fprintf(stderr, "Allocating Memory Error");
	  freeStuff(word, arr, alen);
          exit(1);
        }
      }

      else{
	arr = (char**) realloc(arr, alen * sizeof(char*));
	if(arr == NULL){
          fprintf(stderr, "Reallocating Memory Error");
	  freeStuff(word, arr, alen);
          exit(1);
        }
      }
      arr[alen - 1] = word;
      word = NULL;
      wlen = 0;
    }
  }

  
  qsort(arr, alen, sizeof(char*), frobcmpVoid); 
  
  
  for(int i = 0; i < alen; i++){
    for(int j = 0; arr[i][j] != 0; j++)
      printf("%c", arr[i][j]);
    printf(" ");
  }

  freeStuff(word, arr, alen);



  return 0;
}
