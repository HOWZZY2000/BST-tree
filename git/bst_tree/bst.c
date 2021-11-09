#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct bst_node {
  int key;
  int value;
  struct bst_node *left;
  struct bst_node *right;
};

struct bst_tree {
  struct bst_node *root;
};

struct bst_tree *create_empty_bst(void) { // create an empty bst tree
  struct bst_tree *tree = malloc(sizeof(struct bst_tree));
  tree->root = NULL;
  return tree;
}

static struct bst_node *create_leaf(int k, int v) { // create a leaf node
  struct bst_node *leaf = malloc(sizeof(struct bst_node));
  leaf->key = k;
  leaf->value = v;
  leaf->left = NULL;
  leaf->right = NULL;
  return leaf;
}

static void free_node(struct bst_node *node) {
  if (node) {
    free_node(node->left);
    free_node(node->right);
    free(node);
  }
}

void destroy(struct bst_tree *tree) {
  assert(tree);
  free_node(tree->root);
  free(tree);
}

void bst_search(int k, const struct bst_tree *tree) { // search key 
  assert(tree);
  struct bst_node *node = tree->root;
  while (node) {
    if (node->key == k) {
      printf("Found key, value: %d", node->value);
      return;
    } if (k > node->key) {
      node = node->right;
    } else {
      node = node->left;
    }
  }
  printf("Error: key not found\n");
}

void bst_insert(int k, int v, struct bst_tree *tree) { // insert node
  assert(tree);
  struct bst_node *node = tree->root;
  struct bst_node *parent = NULL;
  while (node) {
    if (node->key == k) {
      printf("Warning: key already exists\n");
      return;
    }
    parent = node;
    if (k < node->key) {
      node = node->left;
    } else {
      node = node->right;
    }
  }
  if (parent == NULL) { // tree is empty
    tree->root = create_leaf(k, v);
  } else if (k < parent->key) {
    parent->left = create_leaf(k, v);
  } else {
    parent->right = create_leaf(k, v);
  }
}

void bst_delete(int k, struct bst_tree *tree) { // delete node
  assert(tree);
  struct bst_node *node = tree->root;
  struct bst_node *parent = NULL;
  // find the node and its parent
  while (node && node->key != k) {
    parent = node;
    if (k < node->key) {
      node = node->left;
    } else {
      node = node->right;
    }
  }
  if (node == NULL) {
    printf("ERROR: key not found\n");
    return; // key not found
  }
  // find the node that taking place of the target node
  struct bst_node *replace = NULL;
  if (node->left == NULL) {
    replace = node->right;
  } else if (node->right == NULL) {
    replace = node->left;
  } else { // neither child is NULL:
    // we choose the node that is the left most closest
    replace = node->left;
    struct bst_node *replace_par = node;
    while (replace->right) {
      replace_par = replace;
      replace = replace->right;
    }
    // update the child links for the replace and its parent
    replace->right = node->right;
    if (replace_par != node) {
      replace_par->right = replace->left;
      replace->left = node->left;
    }
  }
  // free the node, and update the node's parent
  free(node);
  if (parent == NULL) {
    tree->root = replace;
  } else if (k > parent->key) {
    parent->right = replace;
  } else {
    parent->left = replace;
  }
}

int main(void) {
  struct bst_tree *tree = create_empty_bst();
  bst_insert(7, 8, tree);
  bst_insert(8, 2, tree);
  bst_insert(3, 22, tree);
  bst_insert(2, 2, tree);
  bst_insert(4, 9, tree);
  bst_insert(6, 31, tree);
  bst_insert(5, 11, tree);
  bst_search(13, tree);
  bst_delete(7, tree);
  bst_search(5, tree);
  destroy(tree);
}