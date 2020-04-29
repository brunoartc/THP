
#include <iostream>
#include <vector>
#include <string>

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

int eval_var(Node *node) { return node->value; }

int eval_num(Node *node) { return node->value; }

int eval_equal(Node *node) {
  // set token table = expression

  return node->value;
}

int eval_echo(Node *node) {
    std::cout << node->children[0].evaluate(&node->children[0]);

  return 0;
}


int eval_block(Node *node) {
  for(Node i : node->children){
    i.evaluate(&i);
  }
  
  return 0;
}



int eval_if(Node *node) {
  if (node->children[0].evaluate(&node->children[0])) {
    node->children[1].evaluate(&node->children[1]);
  } else {
    if (node->children.size()>1){
      node->children[2].evaluate(&node->children[2]);
    }
  }
  
  return 0;
}

int eval_while(Node *node) {
  while (node->children[0].evaluate(&node->children[0])) {
    node->children[1].evaluate(&node->children[1]);
  }
  
  return 0;
}



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

class Var : public Node {
 public:
  Var(std::string _value) {
    std::string full_value = _value;
    evaluate = &eval_var;
  }
};

class NoOp : public Node {
 public:
  NoOp() {}
};

class Equal : public Node {
 public:
  Equal(std::string _value, std::vector<Node> _children) {
    std::string full_value = _value;
    evaluate = &eval_equal;
    children = _children;
  }
};

class Echo : public Node {
 public:
  Echo(std::vector<Node> _children) {
    evaluate = &eval_echo;
    children = _children;
  }
};

class Block : public Node {
 public:
  Block(std::vector<Node> _children) {
    evaluate = &eval_block;
    children = _children;
  }
};

class IfNode : public Node {
 public:
  IfNode(std::vector<Node> _children) {
    evaluate = &eval_if;
    children = _children;
  }
};

class WhileNode : public Node {
 public:
  WhileNode(std::vector<Node> _children) {
    evaluate = &eval_while;
    children = _children;
  }
};