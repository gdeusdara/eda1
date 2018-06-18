#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define MAX(a,b) (((a)>(b))?(a):(b))

typedef struct Node{
    struct Node *left;
    struct Node *right;
    int data;
}Node;

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

//omit slahes if there is no data in that subtree
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

void printNode(int widthForEachElement, int data) {
    char buf[20];
    sprintf(buf, "%02d", data);
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
            printNode(widthForEachElement, nodes[i]->data);
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
        return MAX(maxDepth(node->left), maxDepth(node->right)) + 1;
    }
}


Node *NewNode(int number){
  Node *temp = (Node*)malloc(sizeof(Node));
  temp->data = number;
  temp->left = temp->right = NULL;
  return temp;
}

Node *insert(Node *node,int number){

  if (node == NULL) return NewNode(number);

   if(number < node->data)
  {
    node->left = insert(node->left,number);
  }
  else if(number > node->data){
    node->right = insert(node->right,number);
  }

  return node;
}



// iniciar lista -
// acho q nao vai precisar dessa pq tu já fez a de pegar do arquivo
Node * setupTree()
{
    Node * no = NewNode(37);
    insert(no,16);
    insert(no,26);
    insert(no,76);
    insert(no,58);
    insert(no,9);
    insert(no,74);
    insert(no,35);
    insert(no,81);
    insert(no,55);
    insert(no,38);
    insert(no, 1);

    return no;


}

void showTree(Node * tree){
  int depth = maxDepth(tree);
  for(int i = 0; i <= depth; i++) {
      printNodesForLevel(tree, i, depth);
  }
}


//Main
int main() {
    Node *tree = setupTree();
    showTree(tree);
}
