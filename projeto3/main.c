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
void remove_register(List * list);
void show_register(List * list);
void show_all(List *list);
void save_to_file(List *list);
bool is_valid(char *input, char *form);

int main ()
{
    int option;
    List *list = (List *) malloc(sizeof(List));
    if(!list)
    {
		    printf("\t\tSem memoria disponivel!\n");
		    exit(1);
	  }
    else
    {init_list(list);
      do{
        printf("\n\n\n");
        printf("\t\t\t============MENU============\t\t\n");
        printf("\n\n");
        printf("\t\t\t[1] Inserir Novo Registro\n");
        printf("\t\t\t[2] Remover Registro\n");
        printf("\t\t\t[3] Busca de Registro por Nome\n");
        printf("\t\t\t[4] Lista de Registro\n");
        printf("\t\t\t[5] SAIR\n\n");

        printf("\t\tDigite uma opção: ");
        scanf("%d", &option);
        printf("\n");

        switch (option) {
          case 1:
            new_register(list);
            break;
          case 2:
            remove_register(list);
            break;
          case 3:
            show_register(list);
            //show_register(name);
            break;
          case 4:
            show_all(list);
            break;
          case 5:
            return 0;
          default:
            printf ("\tOPÇÃO INVÁLIDA, DIGITE NOVAMENTE!\n\n");
        }
      }while(option!=5);

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
      printf("\t\tNão consegui ler\n");
      list->next = NULL;
    } else
    {
        List *element = (List *) malloc(sizeof(List));
        element = list;
        element->previous = NULL;
        bool first_element = true;

        do{
            List *new_element = (List *) malloc(sizeof(List));


            new_element->info.name = (char *) malloc(101 * sizeof(char));
            new_element->info.tel = (char *) malloc(sizeof(char *));
            new_element->info.adress = (char *) malloc(101 * sizeof(char));
            new_element->info.birthday = (char *) malloc(sizeof(char *));


            element->next = new_element;

            new_element->previous = element;
            new_element->next = NULL;

            element = element->next;
        } while (fscanf(file, "%[^\n]%*c%[^\n]%*c%[^\n]%*c%d%*c%[^\n]%*c$\n", element->info.name, element->info.tel, element->info.adress, &element->info.cep, element->info.birthday) != EOF);
        element->previous->next = NULL;

        free_element(element);
    }
}

void new_register(List *list)
{
    Contact new_register;
    FILE *file;
    List *new = (List *) malloc(sizeof(List));

    new_register.name = (char *) malloc(101 * sizeof(char));
    printf ("\t\tDigite o nome: ");
    scanf(" %[^\n]%*c", new_register.name);

    do
    {
        new_register.tel = (char *) malloc(sizeof(char *));
        printf("\t\tDigite o telefone: ");
        scanf("%[^\n]%*c", new_register.tel);
    } while(!is_valid(new_register.tel, "#####-####"));

    new_register.adress = (char *) malloc(101 * sizeof(char));
    printf("\t\tDigite o Endereço: ");
    scanf(" %[^\n]%*c", new_register.adress);

    do
    {
        printf("\t\tDigite o CEP: ");
        scanf("%d%*c", &new_register.cep);
    } while(new_register.cep < 0);

    do {
        new_register.birthday = (char *) malloc(sizeof(char *));
        printf("\t\tDigite a data de nascimento: ");
        scanf("%[^\n]%*c", new_register.birthday);
    } while(!is_valid(new_register.birthday, "##/##/####"));

    init_element(new, new_register);

    if(list->next == NULL)
    {
        list->next = new;
    } else
    {
        List *element = list->next;

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

    save_to_file(list);
}

void show_all(List *list)
{
    if(list->next == NULL){
      printf("\t\tLista vazia!\n\n");
      return ;
    }

    List *element;
    element = list->next;
    printf("\n\t\tLISTA:\n\n");
    while( element != NULL){
        printf("\t\tNome: %s\n\t\tTelefone: %s\n\t\tEndereço: %s\n\t\tCEP: %d\n\t\tData de Nascimento: %s\n\n", element->info.name, element->info.tel, element->info.adress, element->info.cep, element->info.birthday);
        element = element->next;
    }

printf("\n\n");
}


void remove_register(List *list)
{
  char *name = (char *) malloc(sizeof(char *));
  bool find = false;
  printf("\n\t\tQual o nome do registro que deseja remover? ");
  scanf(" %[^\n]%*c", name);

  List *element = list->next;
  element->previous = list;
  while( element != NULL)
  {
      if(strcasecmp(name, element->info.name) == 0)
      {
            element->previous->next = element->next;

            if (element->next != NULL)
            {
                element->next->previous = element->previous;
            }
            free_element(element);
            printf("\t\tRegistro apagado com sucesso\n");
            find = true;
      }
      element = element->next;
  }
  if (!find) {
    printf("\t\tRegistro não encontrado!\n");
  }
  save_to_file(list);
}

void show_register(List *list)
{
  char *name = (char *) malloc(sizeof(char *));
  bool find = false;
  printf("\n\t\tQual o nome do registro que deseja buscar? ");
  scanf(" %[^\n]%*c", name);

  List *element = list->next;
  while( element != NULL)
  {
      if(strcasecmp(name, element->info.name) == 0)
      {
        printf("\t\tNome: %s\n\t\tTelefone: %s\n\t\tEndereço: %s\n\t\tCEP: %d\n\t\tData de Nascimento: %s\n\n", element->info.name, element->info.tel, element->info.adress, element->info.cep, element->info.birthday);
        find = true;
      }
      element = element->next;
  }
  if (!find) {
    printf("\t\tRegistro não encontrado!\n");
  }
}

void save_to_file(List *list)
{
  FILE *file = fopen(FILE_NAME, "w");
  if (file == NULL)
      exit(1);

  List *element = list->next;
  while (element != NULL)
  {
      if(!fprintf(file, "%s\n%s\n%s\n%d\n%s\n$\n", element->info.name, element->info.tel, element->info.adress, element->info.cep, element->info.birthday))
          printf("\t\tErro ao gravar no arquivo\n");
      element = element->next;

  }

  if(fclose(file))
    printf("\t\tErro ao fechar Arquivo\n");
}

bool is_valid(char *input, char *form)
{
     int i = 0;
     if (strlen(input) != 10) {
       printf("\n\t\tTamanho invalido! Siga este formato: %s\n\n", form);
       return false;
     }
     while(i <= 10)
     {
        if(form[i] != '#')
        {
           if (input[i] != form[i])
           {
             printf("\n\t\tInput invalido! Siga este formato %c %c: %s\n\n",form[i], input[i], form);
             return false;
           }
           i++;
        }
        else
        {
          if (!isdigit(input[i]))
          {
            printf("\n\t\tForma invalida! Siga este formato: %s\n\n", form);
            return false;
          }
        }
        i++;
     }
     return true;
}
