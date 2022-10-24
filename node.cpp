#include "node.h"

using namespace std;
using namespace myNode;
Node::Node()
{
    this->code = "";
    this->value = 0;
    this->isLeaf = false;
    this->leftChild = this->rightChild = this->parent = nullptr;
}

Node* Node::merge(Node* a, Node* b)
{
    if (a == nullptr || b == nullptr) {
        cout << "merge error, parameter is/are empty" << endl;
        return nullptr;
    }
    Node* c = new Node();
    if (!c) {
        cout << "merge error, allocate new memory failed." << endl;
        return nullptr;
    }
    c->leftChild = a->weight < b->weight ? a : b;
    c->rightChild = a->weight > b->weight ? a : b;
    c->parent = nullptr;
    c->weight = a->weight + b->weight;
    c->value = -1;
    c->isLeaf = false;
    return c;
}

void Node::inorderTraversal(Node* node, std::string code,QMap<int,string>* map)
{
    //qDebug()<<"inorder is called!";
    if (node == nullptr) return;
    string zero = "0";
    string one = "1";
    if(node->leftChild)
        inorderTraversal(node->leftChild, code + zero, map);
    if (node->isLeaf) {
        node->code = code;
        //qDebug()<<node->value;
        //qDebug() << node->value << "'s weight is" << node->weight << " and code is "<<QString::fromStdString(node->code);
        map->insert(node->value,node->code);
        return;
    }
    if(node->rightChild)
        inorderTraversal(node->rightChild, code + one, map);
}

Node* Node::buildHuffmanTree(vector<Node*> myList)
{
    while (myList.size() > 1) {
        std::sort(myList.begin(), myList.end(), asc);
        Node* a = myList[0];
        Node* b = myList[1];
        Node*c = Node::merge(a, b);
        myList.erase(myList.begin());
        myList.erase(myList.begin());
        myList.insert(myList.begin(), c);
    }
    return myList[0];
}

void Node::buildShannonFanoTree(Node set[], int begin, int end, string code)
{
    if (begin == end) {
        set[begin].code = code;
        //cout << set[begin].value << "'s weight is " << set[begin].weight << " and code is " << set[begin].code << endl;
        return;
    }
    int count = 0, sum = 0, i, j;
    for (i = begin;i <= end;i++) {
        sum += set[i].weight;
    }
    sum /= 2;
    for (j = begin;j <= end && count < sum;j++) {
        count += set[j].weight;
    }
    string one = "1";
    string zero = "0";
    buildShannonFanoTree(set, begin, j - 1, code + zero);
    buildShannonFanoTree(set, j, end, code + one);
}
