#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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
    struct list *next;
} List;

void init_element(List *element, Contact new_register);
void init_list(List *list);
void new_register(List *list);
void remove_register(char *name);
void show_register(char *name);
void show_all(List *list);

int main ()
{
    List *list = (List *) malloc(sizeof(List));
    if(!list)
    {
		    printf("Sem memoria disponivel!\n");
		    exit(1);
	  }
    else
    {
        init_list(list);
        show_all(list);
        return 0;
    }
}

void init_element(List *element, Contact new_register)
{
    element->previous = NULL;
    element->next = NULL;
    element->info = new_register;

}

void init_list(List *list)
{
    FILE *file;

    file = fopen(FILE_NAME, "r");
    if (file == NULL)
    {
      printf("nao consegui LEr\n");
      list->next = NULL;
    } else
    {


        bool first_element = true;
        while (!feof(file))
        {
            Contact file_register;
            char *dolar = (char *) malloc(sizeof(char *));

            file_register.name = (char *) malloc(sizeof(char *));
            fscanf(file, "%[^\n]%*c", file_register.name);

            file_register.tel = (char *) malloc(sizeof(char *));
            fscanf(file, "%[^\n]%*c", file_register.tel);

            file_register.adress = (char *) malloc(sizeof(char *));

            fscanf(file, "%[^\n]%*c", file_register.adress);

            fscanf(file, "%d%*c", &file_register.cep);

            file_register.birthday = (char *) malloc(sizeof(char *));
            fscanf(file, "%[^\n]%*c", file_register.birthday);
            fgets(dolar, 3, file);

            printf("NOME: %s\n", file_register.name );
            printf("TEL: %s\n", file_register.tel );

            List *element = (List *) malloc(sizeof(List));
            List *next_element = (List *) malloc(sizeof(List));
            if (first_element) {
                init_element(element, file_register);
                list->next = element;
                first_element = false;
            } else
            {
                next_element->info = file_register;
                next_element->previous = element;
                next_element->next = NULL;
                element->next = next_element;
                element = next_element;
            }
        }
    }
}

void new_register(List *list)
{
    Contact new_register;
    FILE *file;
    List *new = (List *) malloc(sizeof(List));

    new_register.name = (char *) malloc(sizeof(char *));
    printf ("Digite o nome: ");
    scanf("%[^\n]%*c", new_register.name);


    new_register.tel = (char *) malloc(sizeof(char *));
    printf("Digite o telefone: ");
    scanf("%[^\n]%*c", new_register.tel);

    new_register.adress = (char *) malloc(sizeof(char *));
    printf("Digite o Endereço: ");
    scanf("%[^\n]%*c", new_register.adress);


    printf("Digite o CEP: ");
    scanf("%d%*c", &new_register.cep);

    new_register.birthday = (char *) malloc(sizeof(char *));
    printf("Digite a data de nascimento: ");
    scanf("%[^\n]%*c", new_register.birthday);

    init_element(new, new_register);

    if(list->next == NULL)
    {
        list->next = new;
    } else
    {
        List *element = list->next;
        printf("%s\n", element->info.name);

        bool finish = false;
        do
        {
            if(strcasecmp(new->info.name, element->info.name) < 0)
            {
                if (list->next == element)
                {
                    list->next = new;
                    new->next = element;
                    element->previous = new;
                } else
                {
                    new->previous = element->previous;
                    new->next = element;
                    element->previous->next = new;
                    element->previous = new;
                }
                finish = true;
            }
            else if (element->next == NULL)
            {
                element->next = new;
                new->previous = element;
                finish = true;
            }
            if (!finish) {
              element = element->next;
            }
        } while (!finish || element == NULL);

    }

    file = fopen(FILE_NAME, "w");
    if (file == NULL)
        exit(1);

    List *element = list->next;
    while (element != NULL)
    {
        if(!fprintf(file, "%s\n%s\n%s\n%d\n%s\n$\n", element->info.name, element->info.tel, element->info.adress, element->info.cep, element->info.birthday))
            printf("Erro ao gravar no arquivo\n");
        element = element->next;
    }

    if(fclose(file))
      printf("Erro ao fechar Arquivo\n");
}

void show_all(List *list)
{
    if(list->next == NULL){
      printf("Lista vazia!\n\n");
      return ;
    }

    List *element;
    element = list->next;
    printf("Lista:\n");
    while( element != NULL){
        printf("Nome: %s\nTelefone: %s\nEndereço: %s\nCEP: %d\nData de Nascimento: %s\n\n", element->info.name, element->info.tel, element->info.adress, element->info.cep, element->info.birthday);
        element = element->next;
    }

printf("\n\n");
}
