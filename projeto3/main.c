#include <stdio.h>
#include <stdlib.h>

#define FILE_NAME "contatos.txt"
#define MAX_STRING 100
#define MAX_TEL 10
#define MAX_BIRTHDAY 10

typedef struct contact
{
    char *name;
    char *tel;
    char *adress;
    int cep;
    char *birthday;
} Contact;

typedef struct list
{
    struct list *previous;
    Contact info;
    struct list *after;
} List;

void new_register();
void remove_register(char *name);
void show_register(char *name);
void show_all();

int main ()
{
    new_register();
    return 0;
}

void new_register()
{
    Contact new;
    FILE *file;

    new.name = (char *) malloc(sizeof(char *));
    printf ("Digite o nome: ");
    scanf("%[^\n]%*c", new.name);

    new.tel = (char *) malloc(sizeof(char *));
    printf("Digite o telefone: ");
    scanf("%[^\n]%*c", new.tel);

    new.adress = (char *) malloc(sizeof(char *));
    printf("Digite o Endereço: ");
    scanf("%[^\n]%*c", new.adress);


    printf("Digite o CEP: ");
    scanf("%d%*c", &new.cep);

    new.birthday = (char *) malloc(sizeof(char *));
    printf("Digite a data de nascimento: ");
    scanf("%[^\n]%*c", new.birthday);


    file = fopen(FILE_NAME, "a");
    if (file == NULL)
        exit(1);

    if(!fprintf(file, "%s\n%s\n%s\n%d\n%s\n$\n", new.name, new.tel, new.adress, new.cep, new.birthday))
      printf("Erro ao gravar no arquivo\n");
    if(fclose(file))
      printf("Erro ao fechar Arquivo\n");
}
