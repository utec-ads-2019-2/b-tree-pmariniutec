#ifndef BTREE_H
#define BTREE_H

#include <iostream>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <utility>
#include <stdio.h>

#include "node.h"

#define NEW_ROOT 2
#define MODIFIED_NOT_ROOT 1
#define NOT_MODIFIED 0

using uint32 = uint32_t;

template <typename T>
class BTree {
  public:
	BTree(uint32 degree);
	~BTree();

	T search(T k);
	bool insert(T k);
	bool remove(T k);
	void print();

  private:
	void printNode(Node<T>* node, uint32 tabCount);
	uint32 insertNode(Node<T>* x, T k);
	char fixChildSize(Node<T>* parent, uint32 index);
	std::pair<Node<T>*, uint32> searchKey(T k);
	void deleteNode(Node<T>* x);
	uint32 findIndex(Node<T>* x, T k);
	T deleteNode(Node<T>* x, uint32 index);
	void splitChild(Node<T>* x, int i);
	char mergeChildren(Node<T>* parent, uint32 i);

	Node<T>* root;
	uint32 degree;
};

#endif



template <typename T>
BTree<T>::BTree(uint32 degree) : degree(degree) {
  root = new Node<T>{ 0, true };
}


template <typename T>
BTree<T>::~BTree<T>() {
  deleteNode(root);
}


template <typename T>
bool BTree<T>::insert(T k) {

  if (root->size == 2 * degree - 1) {
	auto newRoot = new Node<T>{ 0, false };
	newRoot->children[0] = root;
	root = newRoot;
	splitChild(newRoot, 0);
  }

  auto curr = root;
  while (!curr->isLeaf) {

	int index = curr->size - 1;
	while (index >= 0 && k < curr->keys[index]) {
	  index--;
	}
	index++;

	if (curr->children[index]->size == 2 * degree - 1) {
	  splitChild(curr, index);
	  if (curr->keys[index] < k) {
		index++;
	  }
	}
	curr = curr->children[index];
  }

  insertNode(curr, k);
  return true;
}


template <typename T>
bool BTree<T>::remove(T k) {
  auto curr = root;
  while (true) {
	uint32 i = findIndex(curr, k);

	if (i < curr->size && !(curr->keys[i] < k || k < curr->keys[i])) {
	  if (curr->isLeaf) {
		deleteNode(curr, i);
	  } else {
		auto leftChild = curr->children[i];
		auto rightChild = curr->children[i + 1];

		if (leftChild->size >= degree) {
		  while (!(leftChild->isLeaf)) {
			fixChildSize(leftChild, leftChild->size);
			leftChild = leftChild->children[leftChild->size];
		  }
		  curr->keys[i] = deleteNode(leftChild, leftChild->size - 1);
		} else if (rightChild->size >= degree) {
		  while (!(rightChild->isLeaf)) {
			fixChildSize(rightChild, 0);
			rightChild = rightChild->children[0];
		  }
		  curr->keys[i] = deleteNode(rightChild, 0);
		} else {
		  mergeChildren(curr, i);
		  curr = leftChild;
		  continue;
		}
	  }
	  return true;
	} else {

	  if (curr->isLeaf) {
		return false;
	  }

	  char result = fixChildSize(curr, i);
	  if (result == NEW_ROOT) {
		curr = root;
	  } else {
		curr = curr->children[findIndex(curr, k)];
	  }
	}
  }
}


// NOTE: first is the node the key is in.
//       second is the key's index 
template <typename T>
std::pair<Node<T>*, uint32> BTree<T>::searchKey(T k) {
  auto x = root;

  while (true) {
	uint32 i = findIndex(x, k);

	if (i < x->size && (!(k < x->keys[i]) || x->keys[i] < k)) {
	  return { x, i };
	} else if (x->isLeaf) {
	  return { nullptr, 0 };
	} else {
	  x = x->children[i];
	}
  }
}


template <typename T>
T BTree<T>::search(T k) {
  auto node = searchKey(k);

  if (node.first == nullptr) {
	std::cout << "Key " << k << " doesn't exists in the tree. ";
	return 0;
  } else {
	return node.first->keys[node.second];
  }
}


template <typename T>
void BTree<T>::print() {
  std::cout << "Printing B-Tree (degree: " << degree << ")\n";
  if (root != nullptr) {
	printNode(root, 0);
  }
}


template <typename T>
void BTree<T>::deleteNode(Node<T>* x) {
  if (!x->isLeaf) {
	for (uint32 i = 0; i <= x->size; ++i) {
	  deleteNode(x->children[i]);
	}
  }
  delete x;
}


template <typename T>
uint32 BTree<T>::findIndex(Node<T>* x, T k) {
  uint32 i = 0;
  while (i < x->size && x->keys[i] < k) {
	++i;
  }
  return i;
}


template <typename T>
uint32 BTree<T>::insertNode(Node<T>* x, T k) {
  int index;

  for (index = x->size; index > 0 && k < x->keys[index - 1]; index--) {
	x->keys[index] = x->keys[index - 1];
	x->children[index + 1] = x->children[index];
  }

  x->children[index + 1] = x->children[index];
  x->keys[index] = k;
  x->size++;

  return index;
}


template <typename T>
T BTree<T>::deleteNode(Node<T>* x, uint32 index) {

  T ret = x->keys[index];
  x->size--;

  while (index < x->size) {
	x->keys[index] = x->keys[index + 1];
	x->children[index + 1] = x->children[index + 2];
	index++;
  }
  return ret;
}


template <typename T>
void BTree<T>::splitChild(Node<T>* x, int i) {

  auto split = x->children[i];
  auto newNode = new Node<T>{ degree - 1, split->isLeaf };

  for (uint32 j = 0; j < degree - 1; ++j) {
	newNode->keys[j] = split->keys[j + degree];
  }
  if (!split->isLeaf) {
	for (uint32 j = 0; j < degree; ++j) {
	  newNode->children[j] = split->children[j + degree];
	}
  }
  split->size = degree - 1;

  insertNode(x, split->keys[degree - 1]);
  x->children[i + 1] = newNode;
}


template <typename T>
char BTree<T>::mergeChildren(Node<T>* parent, uint32 i) {

  auto leftChild = parent->children[i];
  auto rightChild = parent->children[i + 1];

  leftChild->keys[leftChild->size] = deleteNode(parent, i);
  uint32 j = ++(leftChild->size);

  for (uint32 k = 0; k < rightChild->size; ++k) {
	leftChild->keys[j + k] = rightChild->keys[k];
	leftChild->children[j + k] = rightChild->children[k];
  }
  leftChild->size += rightChild->size;
  leftChild->children[leftChild->size] = rightChild->children[rightChild->size];

  delete rightChild;

  if (parent->size == 0) {
	root = leftChild;
	delete parent;
	return NEW_ROOT;
  }

  return MODIFIED_NOT_ROOT;
}


template <typename T>
char BTree<T>::fixChildSize(Node<T>* parent, uint32 index) {
  auto child = parent->children[index];

  if (child->size < degree) {
	if (index != 0 && parent->children[index - 1]->size >= degree) {
	  auto leftChild = parent->children[index - 1];

	  for (uint32 i = insertNode(child, parent->keys[index - 1]); i != 0; i--) {
		child->children[i] = child->children[i - 1];
	  }

	  child->children[0] = leftChild->children[leftChild->size];
	  parent->keys[index - 1] = deleteNode(leftChild, leftChild->size - 1);
	} else if (index != parent->size && parent->children[index + 1]->size >= degree) {
	  auto rightChild = parent->children[index + 1];
	  insertNode(child, parent->keys[index]);

	  child->children[child->size] = rightChild->children[0];
	  rightChild->children[0] = rightChild->children[1];

	  parent->keys[index] = deleteNode(rightChild, 0);
	} else if (index != 0) {
	  return mergeChildren(parent, index - 1);
	} else {
	  return mergeChildren(parent, index);
	}

	return MODIFIED_NOT_ROOT;
  }

  return NOT_MODIFIED;
}


template <typename T>
void BTree<T>::printNode(Node<T>* node, uint32 tabCount) {

  for (uint32 i = 0; i < tabCount; ++i) {
	std::cout << '\t';
  }

  for (uint32 i = 0; i < node->size; ++i) {
	std::cout << node->keys[i] << ' ';
  }
  std::cout << '\n';

  if (!node->isLeaf) {
	tabCount++;
	for (uint32 i = 0; i <= node->size; ++i) {
	  printNode(node->children[i], tabCount);
	}
  }
}
