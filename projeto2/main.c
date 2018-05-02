#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>


#define FILES 50
int ** read_image_file(char* fileName, int *line, int *colum);
void print(int ** matriz, int line, int colum);

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
   int ** asphalt[FILES];
   int ** grass[FILES];
   int asphalt_line[FILES];
   int asphalt_colum[FILES];
   int grass_line[FILES];
   int grass_colum[FILES];


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

     asphalt[i] = read_image_file(fileName, &asphalt_line[i], &asphalt_colum[i]);

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
     grass[i] = read_image_file(fileName, &grass_line[i], &grass_colum[i]);

   }
   printf("%d linhas\n%d colunas\n%d valor do primeiro arquivo\n", asphalt_line[0], asphalt_colum[0], asphalt[0][0][0]);


  return 0;
}

//=============FUNCTIONS=============


int ** read_image_file(char* fileName, int *line, int *colum) {
	FILE* file = fopen(fileName, "r");

  *colum = 1;
	if (file == NULL) {
		printf("Cannot open file: %s", fileName);
	}

		char count = 0;

		bool final_colum = false;

		while (feof(file) == 0) {

			count = fgetc(file);

			if (!final_colum && count == ';')
        (*colum)++;

			if (count == '\n') {
				(*line)++;
				final_colum = true;
			}
		}



	rewind(file);

	int ** matriz = (int **) malloc(*line* sizeof(int *));

  for(int i = 0; i < (*line); i++){
    matriz[i] = (int*)malloc((*colum)*sizeof(int));
  }


	for (int i = 0; i < *line; i++) {
		for (int j = 0; j < *colum; j++) {
			fscanf(file, "%d;", &matriz[i][j]);
		}
	}

	fclose(file);
  printf("%s\n", fileName);
  return matriz;

}
