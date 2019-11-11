#ifndef BTREE_H
#define BTREE_H

#include "node.h"

template <typename T>
class BTree {
    private:
        Node<T>* root;
        unsigned int degree;

    public:
        BTree(unsigned int degree) : degree(degree), root(nullptr) {};

        T search(int k) { 
            // TODO
        } 

        bool insert(int k, T data) {
            // TODO
        }

        bool remove(int k) {
            // TODO
        }

        void print() {
            // TODO
        }

        ~BTree();
};

#endif