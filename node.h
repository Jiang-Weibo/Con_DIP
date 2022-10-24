#ifndef NODE_H
#define NODE_H
#pragma once
#include <QDebug>
#include <QMap>
#include<string>
#include<iostream>
#include<vector>
#include<algorithm>
#include<map>
using namespace std;
namespace myNode {
class Node {
public:
    Node();
    Node(int _value, int _weight) :value(_value), weight(_weight) { isLeaf = true; leftChild = rightChild = parent = nullptr;};
    Node* leftChild, * rightChild, * parent;
    int value;
    int weight;
    bool isLeaf;
    string code;
    static Node* merge(Node*, Node*);
    static void inorderTraversal(Node* node, string code,QMap<int,string>*);
    static Node* buildHuffmanTree(vector<Node*>);
    //static Node* buildShannonFanoTree(vector<Node*>);
    static void buildShannonFanoTree(Node arr[], int begin, int end, string code);
};
inline bool asc(Node* a, Node* b) {
    return a->weight < b->weight;
};
};

#endif // NODE_H
