#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>


#define FILES 50

int *read_image_file(char* fileName);

int main(int argc, char const *argv[]) {
   char *extension = ".txt";
   char *type = "./DataSet/asphalt/asphalt_";
   char *type2 = "./DataSet/grass/grass_";
   char fileName[strlen(type)+strlen(extension)+3];
   int random[FILES];
   bool repeat = false;
   FILE *file;
   long size;
   srand(time(NULL));
   int *matriz;

   for (size_t i = 0; i < FILES; i++) {
     do{
       random[i] = rand()%51;
       for (size_t j = 0; j < i; j++) {
         if (random[j] == random[i]) {
           random[i] = 0;
         }
       }
     }while(random[i] == 0);

     snprintf( fileName, sizeof( fileName ), "%s%.2d%s", type, random[i], extension );
     printf("%s ",fileName);
   }

   printf("\n");

     for (size_t i = 0; i < FILES; i++) {
     do{
       random[i] = rand()%51;
       for (size_t j = 0; j < i; j++) {
         if (random[j] == random[i]) {
           random[i] = 0;
         }
       }
     }while(random[i] == 0);

     snprintf( fileName, sizeof( fileName ), "%s%.2d%s", type2, random[i], extension );
     printf("%s ",fileName);
   }

  file = fopen(fileName, "r");

  if (file != NULL) {
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    printf("O arquivo %s possui %ld bytes", fileName, size/sizeof(int));
  }else{
    printf("Arquivo inexistente");
  }

  matriz = read_image_file(fileName);

  return 0;
}

//=============FUNCTIONS=============


int *read_image_file(char* fileName) {
	FILE* file = fopen(fileName, "r");
  int line = 0, colum = 0;

	if (file == NULL) {
		printf("Cannot open file: %s", fileName);
	}

		char count = 0;

		bool final_colum = false;

		while (feof(file) == 0) {

			count = fgetc(file);

			if (!final_colum && count == ';')
				colum++;

			if (count == '\n') {
				line++;
				final_colum = true;
			}
		}

      printf("\nIMG:\nLin: %d\nCol: %d\n", line, colum);


	rewind(file);

	int* matriz = calloc(line*colum, sizeof(int));


	for (int i = 0; i < line; i++) {
		for (int j = 0; j < colum; j++) {
			fscanf(file, "%d;", matriz + (i*colum) + j);
      printf("%d\n", *(matriz + (i*colum) + j));
		}
	}

	fclose(file);
  printf("%s\n", fileName );
  
	return matriz;
}
