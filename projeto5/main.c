#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAXBETWEEN(a,b) (((a)>(b))?(a):(b))
#define MAX 10

typedef struct Node{
    struct Node *left;
    struct Node *right;
    int value;
}Node;

Node *loadTreeFromFile(char fileName[MAX]);
void imprime (Node* a);
// void searchValue(Node* tree, int valor);
void printInOrder(Node *pRoot);
void printPreOrder(Node *pRoot);
void printPosOrder(Node *pRoot);

//NOVAS FUNCÔES
void fillLevel(Node *nodes[], Node *node, int targetLevel, int level, int nr);
void printCentered(int width, char *str);
void printSlahes(int numElements, int widthForEachElement, Node *const *nodes);
void printNode(int widthForEachElement, int value);
void printNodesForLevel(Node *root, int level, int depth);
int maxDepth(Node *node);
Node *NewNode(int number);

Node *insert(Node *node,int number);

// The big deal
void showTree(Node * tree);

int main(int argc, char const *argv[]) {

  char fileName[MAX];
  Node * tree=NULL;

  printf("Arquivo que deseja ler: ");
  scanf("%s", fileName);

  tree = loadTreeFromFile(fileName);
  printf("%p\n", tree);

  imprime(tree);
  printf("\n\n\n\n\n");
  showTree(tree);

  //TRAVESSIAS
  printf("\n");
  printf("In Order: ");
  printInOrder(tree);
  printf("\n");
  printf("Pre Order: ");
  printPreOrder(tree);
  printf("\n");
  printf("Pos Order: ");
  printPosOrder(tree);
  printf("\n");


  // searchValue(tree,55);

  return 0;
}

void fillLevel(Node *nodes[], Node *node, int targetLevel, int level, int nr) {
    if (level == targetLevel) {
        nodes[nr] = node;
        return;
    }

    if (node->left != NULL) {
        fillLevel(nodes, node->left, targetLevel, level + 1, nr * 2);
    }
    if (node->right != NULL) {
        fillLevel(nodes, node->right, targetLevel, level + 1, nr * 2 + 1);
    }
}

void printCentered(int width, char *str) {
    int emptySpace = (int) (width - strlen(str)) / 2;
    printf("%*s%s%*s", emptySpace, "", str, width - (int) strlen(str) - emptySpace, "");
}

//omit slahes if there is no value in that subtree
void printSlahes(int numElements, int widthForEachElement, Node *const *nodes) {
    for (int i = 0; i < numElements; i += 2) {
        if (nodes[i] != NULL) {
            printf("%*s", widthForEachElement / 2, "");
            printCentered(widthForEachElement / 2, "/");
        } else {
            printf("%*s", widthForEachElement, "");
        }
        if (nodes[i + 1] != NULL) {
            printCentered(widthForEachElement / 2, "\\");
            printf("%*s", widthForEachElement / 2, "");
        } else {
            printf("%*s", widthForEachElement, "");
        }
    }
    printf("\n");
}

void printNode(int widthForEachElement, int value) {
    char buf[20];
    sprintf(buf, "%02d", value);
    printCentered(widthForEachElement, buf);
}

void printNodesForLevel(Node *root, int level, int depth) {
    int maxLengthPerEntry = 4;
    int width = (int) pow(2, depth) * maxLengthPerEntry;
    int numElements = (int) pow(2, level);
    int widthForEachElement = width / numElements;

    Node **nodes = (Node **) calloc(numElements, sizeof(Node *));
    fillLevel(nodes, root, level, 0, 0);

    if (level > 0) {
        printSlahes(numElements, widthForEachElement, nodes);
    }

    for (int i = 0; i < numElements; i++) {
        if (nodes[i] != NULL) {
            printNode(widthForEachElement, nodes[i]->value);
        } else {
            printf("%*s", widthForEachElement, "");
        }
    }

    free(nodes);
    printf("\n");
}

int maxDepth(Node *node) {
    if (node == NULL) {
        return -1;
    } else {
        return MAXBETWEEN(maxDepth(node->left), maxDepth(node->right)) + 1;
    }
}


Node *NewNode(int number){
  Node *temp = (Node*)malloc(sizeof(Node));
  temp->value = number;
  temp->left = temp->right = NULL;
  return temp;
}

Node *insert(Node *node,int number){

  if (node == NULL) return NewNode(number);

   if(number < node->value)
  {
    node->left = insert(node->left,number);
  }
  else if(number > node->value){
    node->right = insert(node->right,number);
  }

  return node;
}



// iniciar lista -
// acho q nao vai precisar dessa pq tu já fez a de pegar do arquivo
// Node * setupTree()
// {
//     Node * no = NewNode(37);
//     insert(no,16);
//     insert(no,26);
//     insert(no,76);
//     insert(no,58);
//     insert(no,9);
//     insert(no,74);
//     insert(no,35);
//     insert(no,81);
//     insert(no,55);
//     insert(no,38);
//     insert(no, 1);
//
//     return no;
//
//
// }

void showTree(Node * tree){
  int depth = maxDepth(tree);
  for(int i = 0; i <= depth; i++) {
      printNodesForLevel(tree, i, depth);
  }
}

Node *loadTreeFromFile(char fileName[MAX]){

  char right[20] = "./BSTs/";
  int tree[MAX], i = 0, *p;
  Node *root = NULL;
  Node *new;
  Node *aux;

  p = tree;

  strncat(right, fileName, 10);

  FILE *file = fopen (right, "r");

  if (file == NULL){
    printf("Falha de leitura\n");
    exit(1);
  } else {
      while( (fscanf(file,"%d \n", &tree[i])) != EOF)
         i++;
  }

  fclose(file);

  for (i = 0; i < MAX; i++) {

    new =(Node *)malloc(sizeof(Node));

    //INSERÇÃO DA ARVORE
    new->value = tree[i];
    new->right = NULL;
    new->left = NULL;

    aux = root;

    while (aux != NULL)
    {
      if(new->value < aux->value)
      {
        if(aux->left == NULL)
        {
          aux->left = new;
          break;
        }

        aux = aux->left;

      } else {
        if(aux->right == NULL)
        {
          aux->right = new;
          break;
        }
        aux = aux->right;
      }
    }

    aux = new;

    if (root == NULL)
    {
      root = aux;
    }
  }
  return root;
}

void imprime (Node* tree)
{
 if (tree != NULL){
 printf("%d \n", tree->value); /* mostra root */
 imprime(tree->left); /* mostra sae */
 imprime(tree->right); /* mostra sad */
 }
}

// void searchValue(Node* tree, int valor){
//   int nivelNo = 0;
//   if (tree != NULL) {
//     if (tree->value == valor){
//       printf("O número existe na tree!\n");
//     } else{
//       printf("Número inexistente\n");
//     }
//     searchValue(tree->left, valor);
//     searchValue(tree->right,valor);
//   }
//   nivelNo++;
// }

void printInOrder(Node *pRoot){
    if(pRoot != NULL){
        printInOrder(pRoot->left);
        printf("%d ", pRoot->value);
        printInOrder(pRoot->right);
    }
}
void printPreOrder(Node *pRoot){
    if(pRoot != NULL){
        printf("%d ", pRoot->value);
        printPreOrder(pRoot->left);
        printPreOrder(pRoot->right);
    }
}
void printPosOrder(Node *pRoot){
    if(pRoot != NULL){
        printPosOrder(pRoot->left);
        printPosOrder(pRoot->right);
        printf("%d ", pRoot->value);
    }
}
