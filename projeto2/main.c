#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#define FILES 50
#define MAX_GRAY_LEVEL 256
#define ILBP_MAX 512
#define METRICS 24

double ** images_process(char * type, double ** descriptor);
int ** read_image_file(char* fileName, int *line, int *colum);
int* func_ILBP(int **, int, int);
double* get_glcm_for_direction(int direction[2], int ** matriz, int line, int col);
double* func_GLCM(int ** matriz, int line, int col);
double * training(double ** training_descriptor);
double testing(double* asphalt_trained, double* grass_trained, double** descriptor_type, int is_grass);

int main(int argc, char const *argv[]) {
   char *type = "./DataSet/asphalt/asphalt_";
   char *type2 = "./DataSet/grass/grass_";
   double ** asphalt_descriptor;
   double ** grass_descriptor;
   double * asphalt_training;
   double * grass_training;
   double hits;

   srand(time(NULL));

   asphalt_descriptor = images_process(type, asphalt_descriptor);
   grass_descriptor = images_process(type2, asphalt_descriptor);

   asphalt_training = training(asphalt_descriptor);
   grass_training = training(grass_descriptor);

   hits = testing(asphalt_training, grass_training, asphalt_descriptor, 0);
   hits += testing(asphalt_training, grass_training, grass_descriptor, 1);

   hits = (hits * 100)/50.0;

   free(asphalt_descriptor);
   free(grass_descriptor);
   free(asphalt_training);
   free(grass_training);
   printf("Acertos: %.0lf%%\n", hits);


   return 0;
}

//=============FUNCTIONS=============


int ** read_image_file(char* fileName, int *line, int *colum) {
	FILE* file = fopen(fileName, "r");
  *line = 0;
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

	int ** matriz = (int **) malloc((*line)* sizeof(int*));

  for(int i = 0; i < (*line); i++){
    matriz[i] = (int*) malloc((*colum)*sizeof(int));
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
int* func_ILBP(int** matriz, int line, int colum) {

	int* ilbp = malloc(512* sizeof(int));

  int secundary_matriz[3][3];
  double avarage;
  unsigned short binary = 0;
  int x, y;
  int lowest_binary_num;
  unsigned short extracted;

	for (int i = 1; i < line-1; i++) {
		for (int j = 1; j < colum-1; j++) {

      secundary_matriz[0][0] = matriz[i-1][j-i];
      secundary_matriz[0][1] = matriz[i-1][j];
      secundary_matriz[0][2] = matriz[i-1][j+1];

      secundary_matriz[1][0] = matriz[i][j-1];
      secundary_matriz[1][1] = matriz[i][j];
      secundary_matriz[1][2] = matriz[i][j+1];

      secundary_matriz[2][0] = matriz[i+1][j-1];
      secundary_matriz[2][1] = matriz[i+1][j];
      secundary_matriz[2][2] = matriz[i+1][j+1];

			avarage = 0;
			for (int x = 0; x < 3; x++) {
				for (int y = 0; y < 3; y++) {
					avarage += secundary_matriz[x][y];
				}
			}

			avarage = avarage / 9;

			binary = 0;

			x = 0;
      y = 0;
			while (true) {
				binary = binary << 1;

				if (secundary_matriz[x][y] >= avarage) {
					binary = binary | 0x0001;
				}


				if (x == 1 && y == 1) {
					break;
				}

				if ((x == 0 || x == 1) && y != 2) {
					y++;
					continue;
				}

				if (y == 2 && x != 2) {
					x++;
					y = 2;
					continue;
				}

				if (x == 2 && y != 0) {
					x = 2;
					y--;
					continue;
				}
				else {
					x = 1;
					y = 0;
				}
			}

			lowest_binary_num =  (int) binary;

			for (int k = 0; k < 9; k++) {

				extracted = binary & 0x0100;
				extracted = extracted >> 8;

				binary = binary << 1;

				binary = binary | extracted;
				binary = binary & 0x01FF;

				if (binary < lowest_binary_num)
					lowest_binary_num = (int) binary;

			}

			*(ilbp + lowest_binary_num) += 1;
		}
	}

	return ilbp;
}

double* func_GLCM(int ** matriz, int line, int colum) {

	double *glcm = malloc(24 * sizeof(double));

	int k = 0;
	for (int direction_x = 0; direction_x < 3; direction_x++) {
		for (int direction_y = 0; direction_y < 3; direction_y++) {
			if (direction_x == 1 && direction_y == 1) {
				continue;
			}

			int vec[2] = { direction_x, direction_y };

			double *metrics = get_glcm_for_direction(vec, matriz, line, colum);

			for (int i = 0; i < 3; i++) {
				glcm[k] = metrics[i];
				k++;
			}

			free(metrics);
		}
	}

	return glcm;

}

double* get_glcm_for_direction(int direction[2], int ** matriz, int line, int colum) {

	direction[0] -= 1;
	direction[1] -= 1;

	double contrast = 0;
	double energy = 0;
	double homogeneity = 0;

	double* glcm_matrix = (double *) calloc(pow(MAX_GRAY_LEVEL, 2), sizeof(double));

	for (int x = 0; x < line; x++) {
		for (int y = 0; y < colum; y++) {

			int j = matriz[x][y];
			if (x + direction[0] < 0 || x + direction[0] >= line || y + direction[1] < 0 || y + direction[1] >= colum )
				continue;

			int i = matriz[x + direction[0]][y + direction[1]];

			glcm_matrix[(i*MAX_GRAY_LEVEL) + j] += 1;
		}
	}

	for (int i = 0; i < MAX_GRAY_LEVEL; i++) {
		for (int j = 0; j < MAX_GRAY_LEVEL; j++) {

			double current_value = glcm_matrix[(i*MAX_GRAY_LEVEL) + j];

			contrast += pow(abs(i - j), 2) * current_value;
			energy += pow(current_value, 2);
			homogeneity += current_value / (1 + abs(i - j));
		}
	}

	double *metrics = calloc(3, sizeof(double));

	metrics[0] = contrast;
	metrics[1] = energy;
	metrics[2] = homogeneity;

	free(glcm_matrix);

	return metrics;
}

double ** images_process(char * type, double ** descriptor){
  char *extension = ".txt";
  char fileName[strlen(type)+strlen(extension)+3];
  int random[FILES];
  int ** matrix[FILES];
  int matrix_line[FILES];
  int matrix_colum[FILES];
  int * ilbp[FILES];
  double * glcm[FILES];

  descriptor = malloc(FILES * sizeof(double *));

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

    matrix[i] = read_image_file(fileName, &matrix_line[i], &matrix_colum[i]);
    ilbp[i] = func_ILBP(matrix[i], matrix_line[i], matrix_colum[i]);
    glcm[i] = func_GLCM(matrix[i], matrix_line[i], matrix_colum[i]);

    descriptor[i] = calloc((ILBP_MAX  + METRICS), sizeof(double));

		for (int j = 0; j < (ILBP_MAX + 24); j++) {

			descriptor[i][j] = (j < ILBP_MAX) ? (double) ilbp[i][j] : (double) glcm[i][j - ILBP_MAX];
		}

    double max = descriptor[i][0];

    for(int b = 0; b < ILBP_MAX  + METRICS; b++){
        if(descriptor[i][b] > max){
            max = descriptor[i][b];
        }
    }

    double min = descriptor[i][0];

    for(int c = 0; c < ILBP_MAX  + METRICS; c++){
        if(descriptor[i][c] < min){
            min = descriptor[i][c];
        }
    }

    for(int k = 0; k < ILBP_MAX  + METRICS; k++){
        descriptor[i][k] = ( descriptor[i][k] - min) / (max - min);
    }
    free(matrix[i]);
    free(ilbp[i]);
    free(glcm[i]);
  }
  return descriptor;
}

double * training(double ** training_descriptor){
  double * vector = calloc(ILBP_MAX + METRICS, sizeof(double));

  for(int i = 0; i < ILBP_MAX  + METRICS; i++){
      for(int j = 0; j < FILES/2; j++){
          vector[i] += training_descriptor[j][i];
      }
      vector[i] /= FILES/2;
  }

  return vector;
}

double testing(double* asphalt_trained, double* grass_trained, double** descriptor_type, int is_grass){

    double distance_from_asphalt = 0;
    double distance_from_grass = 0;
    double hits = 0;
    double aux = 0;
    for(int i = 0; i < 25; i++){

        distance_from_grass = 0;
        distance_from_asphalt = 0;

        for(int j = 0; j < ILBP_MAX  + METRICS; j++){
            distance_from_asphalt  += pow(asphalt_trained[i] - descriptor_type[i+25][j], 2); //taking the last 25
            distance_from_grass  += pow(grass_trained[i] - descriptor_type[i+25][j], 2);
        }

        distance_from_asphalt = sqrt(distance_from_asphalt);
        distance_from_grass = sqrt(distance_from_grass);

        int test_result = (distance_from_grass < distance_from_asphalt) ? 1 : 0;

        if(test_result == is_grass){
          hits++;
        }else{
          aux++;
        }
    }
    is_grass ? printf("Falso negativo: %.0lf%%\n", aux) : printf("Falso positivo: %.0lf%%\n", (aux * 100)/50);
    return hits;
}
