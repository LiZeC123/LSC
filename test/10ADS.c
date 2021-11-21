#include "00testhelper.h"

struct LinkedNode {
  int data;
  struct LinkedNode* next;
};

struct TreeNode {
  int value;
  struct TreeNode* left;
  struct TreeNode* right;
};

#define LNULL (struct LinkedNode*)0

void initList(struct LinkedNode* head) {
  struct LinkedNode* p = head;
  head->data = 9;

  for (int i = 9; i > 0; i--) {
    p->data = i;
    p->next = (struct LinkedNode*)lscmalloc(sizeof(struct LinkedNode));
    p = p->next;
  }

  p->data = 0;
  p->next = LNULL;
}

struct LinkedNode* reverseList(struct LinkedNode* head) {
  if (head->next == LNULL) {
    return head;
  }

  struct LinkedNode* newHead = reverseList(head->next);
  head->next->next = head;
  head->next = LNULL;
  return newHead;
}

void printList(struct LinkedNode* head) {
  for (; head != 0; head = head->next) {
    lscPrintInt(head->data);
    wrap();
  }
}

void ReverseTest() {
  struct LinkedNode* head =
      (struct LinkedNode*)lscmalloc(sizeof(struct LinkedNode));
  initList(head);
  head = reverseList(head);
  int count = 0;
  for (; head != LNULL; head = head->next) {
    checkEquals(head->data, count++, __LINE__);
  }
}

int main() {
  ReverseTest();

  // 实现链表数据结构
  // 实现二叉树数据结构
  // 翻转链表并比对输出结果是否一致
  // 翻转二叉树并比对结果是否一致
  // 层序输出二叉树
  return 0;
}
