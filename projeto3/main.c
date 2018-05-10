#include <stdio.h>
#include <stdlib.h>

typedef struct contact
{
    char *name;
    char *tel;
    char *adress;
    unsigned long int cep;
    char *date;
} Contact;

void new_register();
void remove_register(char *name);
void show_register(char *name);
void show_all();

int main ()
{
    return 0;
}