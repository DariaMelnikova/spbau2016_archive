#pragma once

#include <string>
using namespace std;

struct code_tree
{
    code_tree(char character_=-1, int priority_ = 0, code_tree *left_ = NULL, code_tree *right_ = NULL)
            : character(character_), priority(priority_), code(""), left(left_), right(right_) {}
    ~code_tree()
    {
            delete left;
            delete right;
    }

    char character;
    int priority;
    std::string code;
    code_tree * left;
    code_tree * right;

};

struct compare
{
  bool operator() (code_tree const * lhs, code_tree const * rhs) const
  {
    return lhs->priority > rhs->priority;
  }
};



