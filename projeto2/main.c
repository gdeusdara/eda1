#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILES 25

int main(int argc, char const *argv[]) {
   char *extension = ".txt";
   char *type = "asphalt_";
   char fileName[strlen(type)+strlen(extension)+3];
   int number;

   srand(time(0));

   for (size_t i = 0; i <= FILES; i++) {
     do{
       number = rand()%51;
     }while(number == 0);

     snprintf( fileName, sizeof( fileName ), "%s%.2d%s", type, number, extension );
     printf("%s",fileName);
   }
  return 0;
}
