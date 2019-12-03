#include <iostream>
#include <random>
#include <type_traits>

#include "btree.h"

constexpr int SAMPLE_SIZE = 300;

int main(int argc, char *argv[]) {
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<unsigned int> d6(2, 6);
  std::uniform_int_distribution<int> dist(1, 4000);

  BTree<int> tree{ d6(rng) };

  tree.insert(25);
  tree.insert(12);
  tree.insert(120);

  for (int i = 0; i < SAMPLE_SIZE; ++i) {
	tree.insert(dist(rng));
  }

  tree.print();

  tree.remove(25);
  tree.remove(120);
  tree.remove(12);

  tree.print();
}
