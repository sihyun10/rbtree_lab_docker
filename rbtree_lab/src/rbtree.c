#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void)
{
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  if (p == NULL)
    return NULL;

  p->nil = (node_t *)calloc(1, sizeof(node_t));
  if (p->nil == NULL)
  {
    free(p);
    return NULL;
  }

  p->nil->color = RBTREE_BLACK;
  p->nil->left = p->nil->right = p->nil->parent = NULL;

  p->root = p->nil;

  return p;
}

// 좌회전, 우회전 구현
void left_rotate(rbtree *t, node_t *x)
{
  node_t *y = x->right;
  x->right = y->left;

  if (y->left != t->nil)
  {
    y->left->parent = x;
  }

  y->parent = x->parent;

  if (x->parent == t->nil)
  {
    t->root = y;
  }
  else if (x == x->parent->left)
  {
    x->parent->left = y;
  }
  else
  {
    x->parent->right = y;
  }

  y->left = x;
  x->parent = y;
}

void right_rotate(rbtree *t, node_t *y)
{
  node_t *x = y->left;
  y->left = x->right;

  if (x->right != t->nil)
  {
    x->right->parent = y;
  }

  x->parent = y->parent;

  if (y->parent == t->nil)
  {
    t->root = x;
  }
  else if (y == y->parent->right)
  {
    y->parent->right = x;
  }
  else
  {
    y->parent->left = x;
  }

  x->right = y;
  y->parent = x;
}

void delete_rbtree(rbtree *t)
{
  // TODO: reclaim the tree nodes's memory
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key)
{
  node_t *x = t->root;
  node_t *y = t->nil;

  node_t *z = (node_t *)calloc(1, sizeof(node_t));
  z->key = key;
  z->color = RBTREE_RED;
  z->left = t->nil;
  z->right = t->nil;
  z->parent = t->nil;

  while (x != t->nil)
  {
    y = x;
    if (key < x->key)
    {
      x = x->left;
    }
    else
    {
      x = x->right;
    }
  }

  z->parent = y;

  if (y == t->nil)
  {
    t->root = z;
  }
  else if (key < y->key)
  {
    y->left = z;
  }
  else
  {
    y->right = z;
  }

  insert_fixup(t, z);
  return z;
}

void insert_fixup(rbtree *t, node_t *z)
{
  while (z->parent->color == RBTREE_RED)
  {
    if (z->parent == z->parent->parent->left)
    {
      node_t *y = z->parent->parent->right;

      // Case 1
      if (y->color == RBTREE_RED)
      {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
        continue;
      }

      // Case 2
      if (z == z->parent->right)
      {
        z = z->parent;
        left_rotate(t, z);
      }

      // Case 3
      z->parent->color = RBTREE_BLACK;
      z->parent->parent->color = RBTREE_RED;
      right_rotate(t, z->parent->parent);
    }
    else
    {
      node_t *y = z->parent->parent->left;

      if (y->color == RBTREE_RED)
      {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
        continue;
      }

      if (z == z->parent->left)
      {
        z = z->parent;
        right_rotate(t, z);
      }

      z->parent->color = RBTREE_BLACK;
      z->parent->parent->color = RBTREE_RED;
      left_rotate(t, z->parent->parent);
    }
  }

  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_find(const rbtree *t, const key_t key)
{
  // TODO: implement find
  return t->root;
}

// 후계자 찾는 함수
node_t *rbtree_min(const rbtree *t, node_t *x)
{
  while (x->left != t->nil)
  {
    x = x->left;
  }
  return x;
}

// 전임자 찾는 함수
node_t *rbtree_max(const rbtree *t, node_t *x)
{
  while (x->right != t->nil)
  {
    x = x->right;
  }
  return x;
}

// 두 노드의 자리를 교체
void transplant(rbtree *t, node_t *u, node_t *v)
{
  if (u->parent == t->nil)
  {
    t->root = v;
  }
  else if (u == u->parent->left)
  {
    u->parent->left = v;
  }
  else
  {
    u->parent->right = v;
  }

  v->parent = u->parent;
}

int rbtree_erase(rbtree *t, node_t *z)
{
  node_t *y = z;
  node_t *x;
  color_t y_original_color = y->color;

  if (z->left == t->nil)
  {
    x = z->right;
    transplant(t, z, z->right);
  }
  else if (z->right == t->nil)
  {
    x = z->left;
    transplant(t, z, z->left);
  }
  else
  {
    y = rbtree_min(t, z->right);
    y_original_color = y->color;
    x = y->right;

    if (y->parent != z)
    {
      transplant(t, y, y->right);
      y->right = z->right;
      y->right->parent = y;
    }

    transplant(t, z, y);
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;
  }

  if (y_original_color == RBTREE_BLACK)
  {
    delete_fixup(t, x);
  }

  free(z);
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  // TODO: implement to_array
  return 0;
}
