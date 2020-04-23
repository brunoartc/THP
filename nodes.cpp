
#include <iostream>
#include <vector>

class Node {
 public:
  int value;

  std::vector<Node> children;

  int (*evaluate)(Node *node);
  typedef void (Node::*test_Fun)(void);
};

int eval_biop(Node *node) {
  int resp;
  switch (node->value) {
    case '-':
      return node->children[0].evaluate(&node->children[0]) -
             node->children[1].evaluate(&node->children[1]);
      break;
    case '+':
      return node->children[0].evaluate(&node->children[0]) +
             node->children[1].evaluate(&node->children[1]);
      break;
    case '*':
      return node->children[0].evaluate(&node->children[0]) *
             node->children[1].evaluate(&node->children[1]);
      break;
    case '/':
      return node->children[0].evaluate(&node->children[0]) /
             node->children[1].evaluate(&node->children[1]);
      break;
    default:
      break;
  }
  return -1;
}

int eval_unop(Node *node) {
  switch (node->value) {
    case '-':
      return -node->children[0].evaluate(&node->children[0]);
      break;
    case '+':
      return +node->children[0].evaluate(&node->children[0]);
      break;
    default:
      break;
  }
  return -1;
}

int eval_num(Node *node) { return node->value; }

class BinOp : public Node {
 public:
  BinOp(int _value, std::vector<Node> _children) {
    value = _value;
    children = _children;
    evaluate = &eval_biop;
  }
};

class UnOp : public Node {
 public:
  UnOp(int _value, std::vector<Node> _children) {
    value = _value;
    children = _children;
    evaluate = &eval_unop;
  }
};

class Num : public Node {
 public:
  Num(int _value) {
    value = _value;
    evaluate = &eval_num;
  }
};
