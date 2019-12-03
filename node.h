#ifndef NODE_H
#define NODE_H

#include <vector>

template <typename T>
class BTree;

template <typename T>
class Node {
  public: 
	Node(unsigned int size, bool isLeaf = true) : size(size), isLeaf(isLeaf) { 
	  keys.resize(20);
	  children.resize(20);
	}

	friend class BTree<T>;
  private:
	unsigned int size;
	std::vector<unsigned int> keys;
	std::vector<Node<T>*> children;
	bool isLeaf;

};

#endif
