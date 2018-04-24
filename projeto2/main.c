#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILES 25

int main(int argc, char const *argv[]) {
   char *extension = ".txt";
   char *type = "asphalt_";
   char fileName[strlen(type)+strlen(extension)+3];
   int number[FILES];

   srand(time(NULL));

   for (size_t i = 0; i < FILES; i++) {
     do{
       number[i] = rand()%51;
       for (size_t j = 0; j < i; j++) {
         if (number[j] == number[i]) {
           number[i] = 0;
         }
       }
     }while(number[i] == 0);

     snprintf( fileName, sizeof( fileName ), "%s%.2d%s", type, number[i], extension );
     printf("%s",fileName);
   }
  return 0;
}
