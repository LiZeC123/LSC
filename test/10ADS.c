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

#define LNULL (struct LinkedNode *)0
#define TNULL (struct TreeNode *)0

void initList(struct LinkedNode *head) {
  struct LinkedNode *p = head;
  head->data = 9;

  for (int i = 9; i > 0; i--) {
    p->data = i;
    p->next = (struct LinkedNode *)lscmalloc(sizeof(struct LinkedNode));
    p = p->next;
  }

  p->data = 0;
  p->next = LNULL;
}

void initTree(struct TreeNode *root) {
  struct TreeNode *A;
  struct TreeNode *B;
  struct TreeNode *C;

  A = (struct TreeNode *)lscmalloc(sizeof(struct TreeNode));
  A->value = 1;
  B = (struct TreeNode *)lscmalloc(sizeof(struct TreeNode));
  B->value = 2;
  C = (struct TreeNode *)lscmalloc(sizeof(struct TreeNode));
  C->value = 3;

  root->left = A;
  root->right = B;
  A->left = TNULL;
  A->right = TNULL;
  B->left = C;
  B->right = TNULL;
  C->left = TNULL;
  C->right = TNULL;
}

struct LinkedNode *reverseList(struct LinkedNode *head) {
  if (head == LNULL || head->next == LNULL) {
    return head;
  }

  struct LinkedNode *newHead = reverseList(head->next);
  head->next->next = head;
  head->next = LNULL;
  return newHead;
}

void printList(struct LinkedNode *head) {
  for (; head != 0; head = head->next) {
    lscPrintInt(head->data);
    wrap();
  }
}

void ReverseTest() {
  struct LinkedNode *head =
      (struct LinkedNode *)lscmalloc(sizeof(struct LinkedNode));
  initList(head);
  head = reverseList(head);
  int count = 0;
  for (; head != LNULL; head = head->next) {
    checkEquals(head->data, count++, __LINE__);
  }
}

void ReverseTest2() {
  struct LinkedNode *head = LNULL;
  head = reverseList(head);
  checkEquals(head, LNULL, __LINE__);

  head = (struct LinkedNode *)lscmalloc(sizeof(struct LinkedNode));
  head->data = 42;
  head->next = LNULL;
  head = reverseList(head);
  checkEquals(head, head, __LINE__);
}

struct TreeNode *SwapTree(struct TreeNode *root) {
  if (root == TNULL) {
    return root;
  }

  struct TreeNode *left = SwapTree(root->left);
  struct TreeNode *right = SwapTree(root->right);
  root->left = right;
  root->right = left;

  return root;
}

int sameTree(struct TreeNode *A, struct TreeNode *B) {
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
  struct TreeNode *root;
  root = (struct TreeNode *)lscmalloc(sizeof(struct TreeNode));
  initTree(root);

  root = SwapTree(root);
  root = SwapTree(root);

  struct TreeNode *base;
  base = (struct TreeNode *)lscmalloc(sizeof(struct TreeNode));
  initTree(base);

  checkTrue(sameTree(base, root), __LINE__);
}

void SwapTest2() {
  struct TreeNode *root = TNULL;
  struct TreeNode *base = TNULL;
  root = SwapTree(root);
  root = SwapTree(root);
  checkTrue(sameTree(TNULL, root), __LINE__);

  root = (struct TreeNode *)lscmalloc(sizeof(struct TreeNode));
  base = (struct TreeNode *)lscmalloc(sizeof(struct TreeNode));
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

  return 0;
}
