#include <iostream>
#include <random>

#include "btree.h"

int main(int argc, char *argv[]) {
  BTree<int> tree{3};

  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> dist(1, 300);

  tree.insert(25);
  tree.insert(12);
  tree.insert(120);

  for (int i = 0; i < 80; ++i) {
	tree.insert(dist(rng));
  }

  tree.print();

  tree.remove(25);

  tree.print();
}
