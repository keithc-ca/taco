#ifndef TAC_TREE_H
#define TAC_TREE_H

#include <memory>
#include <ostream>
#include <iostream>

namespace tac {

class TreeLevel;
class TreeVisitorStrict;
typedef std::shared_ptr<TreeLevel> TreeLevelPtr;

class TreeLevel {
public:
  static TreeLevelPtr make(std::string format);

  TreeLevel(const TreeLevelPtr& subLevel) : subLevel{subLevel} {}

  const TreeLevelPtr& getChildren() const;

  virtual void accept(TreeVisitorStrict* v) const = 0;

private:
  TreeLevelPtr subLevel;
};


/**
 * Tree level storing actual values. Value tree levels terminate a tree format.
 */
class Values : public TreeLevel {
public:
  Values() : TreeLevel(nullptr) {}
  void accept(TreeVisitorStrict *v) const;
};


class Dense : public TreeLevel {
public:
  Dense(const TreeLevelPtr& subLevel) : TreeLevel(subLevel) {}
  void accept(TreeVisitorStrict *v) const;
};


class Sparse : public TreeLevel {
public:
  Sparse(const TreeLevelPtr& subLevel) : TreeLevel(subLevel) {}
  void accept(TreeVisitorStrict *v) const;
};


class Fixed : public TreeLevel {
public:
  Fixed(const TreeLevelPtr& subLevel) : TreeLevel(subLevel) {}
  void accept(TreeVisitorStrict *v) const;
};


class Replicated : public TreeLevel {
public:
  Replicated(const TreeLevelPtr& subLevel) : TreeLevel(subLevel) {}
  void accept(TreeVisitorStrict *v) const;
};

// Factory functions
TreeLevelPtr values();
TreeLevelPtr dense(const TreeLevelPtr& subLevel);
TreeLevelPtr sparse(const TreeLevelPtr& subLevel);
TreeLevelPtr fixed(const TreeLevelPtr& subLevel);
TreeLevelPtr replicated(const TreeLevelPtr& subLevel);

// Visitors
class TreeVisitorStrict {
public:
  virtual void visit(const Values* tl)     = 0;
  virtual void visit(const Dense* tl)      = 0;
  virtual void visit(const Sparse* tl)     = 0;
  virtual void visit(const Fixed* tl)      = 0;
  virtual void visit(const Replicated* tl) = 0;
};

class TreeVisitor : public TreeVisitorStrict {
public:
  virtual void visit(const Values* tl);
  virtual void visit(const Dense* tl);
  virtual void visit(const Sparse* tl);
  virtual void visit(const Fixed* tl);
  virtual void visit(const Replicated* tl);
};

std::ostream &operator<<(std::ostream&, const std::shared_ptr<TreeLevel>&);

}
#endif