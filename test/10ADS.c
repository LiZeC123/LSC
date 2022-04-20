#include "00testhelper.h"

struct LinkedNode {
  int data;
  struct LinkedNode *next;
};

struct TreeNode {
  int value;
  struct TreeNode *left;
  struct TreeNode *right;
};

#define List struct LinkedNode
#define Tree struct TreeNode
#define LNULL (List *)0
#define TNULL (Tree *)0

void initList(List *head) {
  List *p = head;
  head->data = 9;

  for (int i = 9; i > 0; i--) {
    p->data = i;
    p->next = (List *)lscmalloc(sizeof(List));
    p = p->next;
  }

  p->data = 0;
  p->next = LNULL;
}

void initTree(Tree *root) {
  Tree *data[5];

  for (int i = 0; i < 5; i++) {
    data[i] = (Tree *)lscmalloc(sizeof(Tree));
    data[i]->value = 0x1234cacd + i;
    data[i]->left = TNULL;
    data[i]->right = TNULL;
  }

  root->left = data[0];
  root->right = data[1];
  data[0]->left = data[2];
  data[0]->right = data[3];
  data[1]->left = data[4];
}

List *reverseList(List *head) {
  if (head == LNULL || head->next == LNULL) {
    return head;
  }

  List *newHead = reverseList(head->next);
  head->next->next = head;
  head->next = LNULL;
  return newHead;
}

void printList(List *head) {
  for (; head != 0; head = head->next) {
    lscPrintInt(head->data);
    wrap();
  }
}

void ReverseTest() {
  List *head = (List *)lscmalloc(sizeof(List));
  initList(head);
  head = reverseList(head);
  int count = 0;
  for (; head != LNULL; head = head->next) {
    checkEquals(head->data, count++, __LINE__);
  }
}

void ReverseTest2() {
  List *head = LNULL;
  head = reverseList(head);
  checkEquals(head, LNULL, __LINE__);

  head = (List *)lscmalloc(sizeof(List));
  head->data = 42;
  head->next = LNULL;
  head = reverseList(head);
  checkEquals(head, head, __LINE__);
}

Tree *SwapTree(Tree *root) {
  if (root == TNULL) {
    return root;
  }

  Tree *left = SwapTree(root->left);
  Tree *right = SwapTree(root->right);
  root->left = right;
  root->right = left;

  return root;
}

int sameTree(Tree *A, Tree *B) {
  if (A == TNULL && B == TNULL) {
    return 1;
  }

  if (A == TNULL || B == TNULL) {
    return 0;
  }

  if (A->value != B->value) {
    return 0;
  }

  return sameTree(A->left, B->left) && sameTree(A->right, B->right);
}

void SwapTest() {
  Tree *root;
  root = (Tree *)lscmalloc(sizeof(Tree));
  initTree(root);

  root = SwapTree(root);
  root = SwapTree(root);

  Tree *base;
  base = (Tree *)lscmalloc(sizeof(Tree));
  initTree(base);

  checkTrue(sameTree(base, root), __LINE__);
}

void SwapTest2() {
  Tree *root = TNULL;
  Tree *base = TNULL;
  root = SwapTree(root);
  root = SwapTree(root);
  checkTrue(sameTree(TNULL, root), __LINE__);

  root = (Tree *)lscmalloc(sizeof(Tree));
  base = (Tree *)lscmalloc(sizeof(Tree));
  root->value = 42;
  base->value = 42;
  root->left = TNULL;
  base->left = TNULL;
  root->right = TNULL;
  base->right = TNULL;

  checkTrue(sameTree(base, root), __LINE__);
}

int main() {
  ReverseTest();

  ReverseTest2();

  SwapTest();

  SwapTest2();

  return 0;
}
