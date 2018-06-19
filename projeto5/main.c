#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX 10
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAXBETWEEN(a,b) (((a)>(b))?(a):(b))
#define MAX_HEIGHT 1000
#define INFINITY (1<<20)

int left[MAX_HEIGHT];
int right[MAX_HEIGHT];
int space = 3;
int printNext;

typedef struct no {
  int value;
  struct no *left;
  struct no *right;
} Node;

typedef struct BST {
    struct BST *left, *right;
    int branch;
    int height;
    int element;
    int type;
    char label[11];
}BST;

void printInOrder(Node *pRoot);
void printPreOrder(Node *pRoot);
void printPosOrder(Node *pRoot);

Node* removeValue(Node* pRoot, int key);
Node *loadTreeFromFile();

bool isFull (Node* pRoot);
int getHeight(Node* node);

void showTree(Node *);
void printLevel(BST *, int , int);
BST *createsBSTRecursive(Node *t);
void fillBranch(BST *);
void getLeft(BST *, int , int );
void freeBST(BST *);
void getRight(BST *node, int , int);

void searchValue(Node *, int, int);


int main ()
{
    int option;
    Node * tree = NULL;

    do{
      printf("\n\n\n");
      printf("\t\t\t============MENU============\t\t\n");
      printf("\n\n");
      printf("\t\t\t[1] Pegar arvore de arquivo\n");
      printf("\t\t\t[2] Mostrar Arvore\n");
      printf("\t\t\t[3] Buscar Valor\n");
      printf("\t\t\t[4] Remover Valor\n");
      printf("\t\t\t[5] Arvore está cheia?\n");
      printf("\t\t\t[6] Altura da árvore\n");
      printf("\t\t\t[7] print in Order\n");
      printf("\t\t\t[8] print pre Order\n");
      printf("\t\t\t[9] print pos Order\n");
      printf("\t\t\t[0] SAIR\n\n");

      printf("\t\tDigite uma opção: ");
      scanf("%d", &option);
      printf("\n");

      switch (option) {
        case 1:
          tree = loadTreeFromFile();
          break;
        case 2:
          showTree(tree);
          break;
        case 3:
          {
            int value;
            printf("\t\tDigite o valor desejado: ");
            scanf("%d", &value);
            searchValue(tree, value, 1);
          }
          break;
        case 4:
          {
            int value;
            printf("\t\tDigite o valor desejado: ");
            scanf("%d", &value);
            removeValue(tree, value);
          }
          break;
        case 5:
          {
            bool isfull;
            isfull = isFull(tree);
            if (isfull)
              printf("Essa arvore é cheia!\n");
            else
              printf("Essa arvore não é cheia!\n");
          }
          break;
        case 6:
          {
            int height;
            height = getHeight(tree);
            printf("Altura: %d\n", height - 1 );
          }
          break;
        case 7:
          printInOrder(tree);
          break;
        case 8:
          printPreOrder(tree);
          break;
        case 9:
          printPosOrder(tree);
          break;
        case 0:
          return 0;
        default:
          printf ("\tOPÇÃO INVÁLIDA, DIGITE NOVAMENTE!\n\n");
      }
    }while(option);

  return 0;
}

Node *loadTreeFromFile(){
  char fileName[MAX];

  printf("Arquivo que deseja ler: ");
  scanf("%s", fileName);

  char right[20] = "./BSTs/";
  int arvore[MAX], i = 0, *p;
  Node *root = NULL;
  Node *new;
  Node *aux;

  p = arvore;

  strncat(right, fileName, 10);

  FILE *file = fopen (right, "r");

  if (file == NULL){
    printf("Falha de leitura\n");
    exit(1);
  } else {
      while( (fscanf(file,"%d \n", &arvore[i])) != EOF)
         i++;
  }

  fclose(file);

  for (i = 0; i < MAX; i++) {

    new =(Node *)malloc(sizeof(Node));

    //INSERÇÃO DA ARVORE
    new->value = arvore[i];
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

/////////FUNÇÕES DE TRAVESSIAS/////////
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

/////////FUNÇÃO DE ÁRVORE COMPLETA/////////

bool isFull (Node* pRoot)
{
    // If empty Node
    if (pRoot == NULL)
        return true;

    // If leaf node
    if (pRoot->left == NULL && pRoot->right == NULL)
        return true;

    // If both left and right are not NULL, and left & right subtrees
    // are full
    if ((pRoot->left) && (pRoot->right))
        return (isFull(pRoot->left) && isFull(pRoot->right));

    // We reach here when none of the above if conditions work
    return false;
}

/////////FUNÇÃO DE TALTURA DA ÁRVORE/////////

int getHeight(Node* node)
{
   if (node==NULL)
       return 0;
   else
   {
       /* compute the depth of each subtree */
       int lDepth = getHeight(node->left);
       int rDepth = getHeight(node->right);

       /* use the larger one */
       if (lDepth > rDepth)
           return(lDepth+1);
       else return(rDepth+1);
   }
}

/////////FUNÇÕES DE REMOÇÃO/////////

Node* minValueNode(Node* node)
{
    Node* current = node;

    /* loop down to find the leftmost leaf */
    while (current->left != NULL)
        current = current->left;

    return current;
}


Node* removeValue(Node* pRoot, int key)
{
    // base case
    if (pRoot == NULL) return pRoot;

    // If the key to be deleted is smaller than the pRoot's key,
    // then it lies in left subtree
    if (key < pRoot->value)
        pRoot->left = removeValue(pRoot->left, key);

    // If the key to be deleted is greater than the pRoot's key,
    // then it lies in right subtree
    else if (key > pRoot->value)
        pRoot->right = removeValue(pRoot->right, key);

    // if key is same as pRoot's key, then This is the node
    // to be deleted
    else
    {
        // node with only one child or no child
        if (pRoot->left == NULL)
        {
            Node *temp = pRoot->right;
            free(pRoot);
            return temp;
        }
        else if (pRoot->right == NULL)
        {
            Node *temp = pRoot->left;
            free(pRoot);
            return temp;
        }

        // node with two children: Get the inorder successor (smallest
        // in the right subtree)
        Node* temp = minValueNode(pRoot->right);

        // Copy the inorder successor's content to this node
        pRoot->value = temp->value;

        // Delete the inorder successor
        pRoot->right = removeValue(pRoot->right, temp->value);
    }
    return pRoot;
}


//////////FUNÇÕES DE PRINTAR/////////////////

void showTree(Node *t) {
    BST *pRoot;

    if (t == NULL)
        return;
    pRoot = createsBSTRecursive(t);
    fillBranch(pRoot);

    for (int i = 0; i < pRoot->height && i < MAX_HEIGHT; i++)
        left[i] = INFINITY;

    getLeft(pRoot, 0, 0);
    int xmin = 0;

    for (int i = 0; i < pRoot->height && i < MAX_HEIGHT; i++)
        xmin = MIN(xmin, left[i]);
    for (int i = 0; i < pRoot->height; i++) {
        printNext = 0;
        printLevel(pRoot, -xmin, i);
        printf("\n");
    }

    freeBST(pRoot);
}

BST *createsBSTRecursive(Node *t) {
    BST *node;
    if (t == NULL)
        return NULL;

    node = malloc(sizeof(BST));
    node->left = createsBSTRecursive(t->left);
    node->right = createsBSTRecursive(t->right);
    node->type = 0;

    if (node->left != NULL)
        node->left->type = -1;
    if (node->right != NULL)
        node->right->type = 1;

    sprintf(node->label, "%d", t->value);
    node->element = strlen(node->label);

    return node;
}

void fillBranch(BST *node) {
    int heightMin, delta;

    if (node == NULL)
        return;

    fillBranch(node->left);
    fillBranch(node->right);

    if (node->right == NULL && node->left == NULL)
        node->branch = 0;

    else {
        if (node->left != NULL) {
            for (int i = 0; i < node->left->height && i < MAX_HEIGHT; i++)
                right[i] = -INFINITY;

            getRight(node->left, 0, 0);
            heightMin = node->left->height;
        }
        else
            heightMin = 0;
        if (node->right != NULL) {
            for (int i = 0; i < node->right->height && i < MAX_HEIGHT; i++)
                left[i] = INFINITY;

            getLeft(node->right, 0, 0);
            heightMin = MIN(node->right->height, heightMin);
        }
        else
            heightMin = 0;

        delta = 4;
        for (int i = 0; i < heightMin; i++)
            delta = MAXBETWEEN(delta, space + 1 + right[i] - left[i]);

        if (((node->left != NULL && node->left->height == 1) ||
                    (node->right != NULL && node->right->height == 1)) && delta > 4) {
            delta--;
        }

        node->branch = ((delta + 1) / 2) - 1;
    }

    int height = 1;
    if (node->left != NULL) {
        height = MAXBETWEEN(node->left->height + node->branch + 1, height);
    }
    if (node->right != NULL) {
        height = MAXBETWEEN(node->right->height + node->branch + 1, height);
    }
    node->height = height;
}

void getLeft(BST *node, int x, int y) {
    if (node == NULL)
        return;

    int isLeft = (node->type == -1);
    left[y] = MIN(left[y], x - ((node->element - isLeft) / 2));

    if (node->left != NULL) {
        for(int i = 1; i <= node->branch  && y + i < MAX_HEIGHT; i++) {
            left[y + i] = MIN(left[y + i], x - i);
        }
    }

    getLeft(node->left, x - node->branch - 1, y + node->branch + 1);
    getLeft(node->right, x + node->branch + 1, y + node->branch + 1);
}

void getRight(BST *node, int x, int y) {
    if (node == NULL)
        return;
    int ttLeft = (node->type != -1);
    right[y] = MAXBETWEEN(right[y], x + ((node->element - ttLeft) / 2));

    if (node->right != NULL) {
        for(int i = 1; i <= node->branch && y + i < MAX_HEIGHT; i++) {
            right[y + i] = MAXBETWEEN(right[y + i], x + i);
        }
    }

    getRight(node->left, x - node->branch - 1, y + node->branch + 1);
    getRight(node->right, x + node->branch + 1, y + node->branch + 1);
}

void printLevel(BST *node, int x, int level) {
    if (node == NULL) return;
    int isleft = (node->type == -1);
    if (level == 0) {
        int i;
        for (i = 0; i < (x - printNext - ((node->element - isleft) / 2)); i++)
            printf(" ");

        printNext += i;
        printf("%s", node->label);
        printNext += node->element;
    }
    else if (node->branch >= level) {
        if (node->left != NULL) {
            int i;
            for (i = 0; i < (x - printNext - (level)); i++)
                printf(" ");

            printNext += i;
            printf("/");
            printNext++;
        }
        if (node->right != NULL) {
            int i;
            for (i = 0; i < (x - printNext + (level)); i++)
                printf(" ");

            printNext += i;
            printf("\\");
            printNext++;
        }
    }
    else {
        printLevel(node->left, x - node->branch - 1, level - node->branch - 1);
        printLevel(node->right, x + node->branch + 1, level - node->branch - 1);
    }
}

void freeBST(BST *node) {
    if (node == NULL) return;
    freeBST(node->left);
    freeBST(node->right);
    free(node);
}

////////////FUNCÃO DO searchValu/////////////

void searchValue(Node *root, int value, int level){
    if (root != NULL){
        if(value == root->value){
            printf("Nó Raiz\n");
            return;
        }
        else if((root->left != NULL) && value < root->value){
            if(root->left->value == value){
              printf("Nivel: %d\n", level );
                printf("Nó Pai: %d\n",root->value);
                if(root->right != NULL)
                    printf("Irmão: %d\n", root->right->value);
                else
                    printf("Esse valor NÃO TEM IRMÃO\n");
                return;
            }
            level++;
            searchValue(root->left, value, level);
        }
        else if((root->right != NULL)){
            if(root->right->value == value){
                printf("Nivel: %d\n", level );
                printf("Nó Pai: %d\n",root->value);
                if(root->left != NULL)
                    printf("Irmão: %d\n", root->left->value);
                else
                    printf("Esse valor NÃO TEM IRMÃO\n");
                return;
            }
            level++;
            searchValue(root->right, value, level);
        }
        else
            printf("Essa arvore não possui o valor %d!\n", value);
    }
}
