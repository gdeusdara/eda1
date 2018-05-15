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

void free_element(List *element);
void create_list(List *list, Contact new_register);
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
// =========== FUNCTIONS ==========

void free_element(List *element)
{
    free(element->info.name);
    free(element->info.tel);
    free(element->info.adress);
    free(element->info.birthday);
    free(element->next);

    element->previous->next = NULL;
    free(element);

}
void create_list(List *list, Contact new_register)
{
  List *element = list->next;
  while (element != NULL)
  {
      element = element->next;
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
        List *element = (List *) malloc(sizeof(List));
        element = list;
        element->previous = NULL;
        bool first_element = true;

        do{
            List *new_element = (List *) malloc(sizeof(List));


            new_element->info.name = (char *) malloc(sizeof(char *));
            new_element->info.tel = (char *) malloc(sizeof(char *));
            new_element->info.adress = (char *) malloc(sizeof(char *));
            new_element->info.birthday = (char *) malloc(sizeof(char *));


            element->next = new_element;

            new_element->previous = element;
            new_element->next = NULL;

            element = element->next;
        } while (fscanf(file, "%[^\n]%*c%[^\n]%*c%[^\n]%*c%d%*c%[^\n]%*c$\n", element->info.name, element->info.tel, element->info.adress, &element->info.cep, element->info.birthday) != EOF);

        free_element(element);
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
    printf("\nLILSTA:\n\n");
    while( element != NULL){
        printf("Nome: %s\nTelefone: %s\nEndereço: %s\nCEP: %d\nData de Nascimento: %s\n\n", element->info.name, element->info.tel, element->info.adress, element->info.cep, element->info.birthday);
        element = element->next;
    }

printf("\n\n");
}
