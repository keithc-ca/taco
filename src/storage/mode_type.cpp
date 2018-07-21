#include <string>
#include <memory>
#include <vector>

#include "taco/storage/mode_type.h"
#include "taco/util/collections.h"
#include "taco/util/strings.h"

using namespace std;
using namespace taco::ir;

namespace taco {

// class Mode
struct Mode::Content {
  Expr            tensor;          /// the tensor containing mode
  Dimension       size;            /// the size of the mode
  size_t          level;           /// the location of mode in a mode hierarchy
  ModeType        modeType;        /// the type of the mode

  const ModePack* pack;            /// the pack that contains the mode
  size_t          packLoc;         /// position within pack containing mode

  ModeType        parentModeType;  /// type of previous mode in the tensor

  std::map<std::string, Expr> vars;
};

Mode::Mode() : content(nullptr) {
}

Mode::Mode(ir::Expr tensor, Dimension size, size_t level, ModeType modeType,
     const ModePack* pack, size_t packLoc, ModeType parentModeType)
    : content(new Content) {
  content->tensor = tensor;
  content->size = size;
  content->level = level;
  content->modeType = modeType;
  content->pack = pack;
  content->packLoc = packLoc;
  content->parentModeType = parentModeType;
}

std::string Mode::getName() const {
  return util::toString(getTensorExpr()) + std::to_string(getLevel()+1);
}

Expr Mode::getTensorExpr() const {
  return content->tensor;
}

Dimension Mode::getSize() const {
  return content->size;
}

size_t Mode::getLevel() const {
  return content->level;
}

ModeType Mode::getModeType() const {
  return content->modeType;
}

const ModePack* Mode::getPack() const {
  return content->pack;
}

size_t Mode::getPackLocation() const {
  return content->packLoc;
}

ModeType Mode::getParentModeType() const {
  return content->parentModeType;
}

Expr Mode::getVar(std::string varName) const {
  taco_iassert(hasVar(varName));
  return content->vars.at(varName);
}

bool Mode::hasVar(std::string varName) const {
  return util::contains(content->vars, varName);
}

void Mode::addVar(std::string varName, Expr var) {
  taco_iassert(isa<Var>(var));
  content->vars[varName] = var;
}

bool Mode::defined() const {
  return content != nullptr;
}


// class ModePack
ModePack::ModePack(const vector<Mode>& modes) : modes(modes)  {
  for (auto& mode : this->modes) {
    mode.content->pack = this;
  }
}

size_t ModePack::getSize() const {
  return modes.size();
}

Expr ModePack::getArray(size_t i) const {
  for (size_t j = 0; j < getSize(); ++j) {
    const auto arr = modes[j].getModeType().impl->getArray(i, modes[j]);
    if (arr.defined()) {
      return arr;
    }
  }
  return Expr();
}

// class ModeTypeImpl
ModeTypeImpl::ModeTypeImpl(const std::string name,
                           bool isFull, bool isOrdered, bool isUnique,
                           bool isBranchless, bool isCompact,
                           bool hasCoordValIter, bool hasCoordPosIter,
                           bool hasLocate, bool hasInsert, bool hasAppend) :
    name(name), isFull(isFull), isOrdered(isOrdered),
    isUnique(isUnique), isBranchless(isBranchless), isCompact(isCompact), 
    hasCoordValIter(hasCoordValIter), hasCoordPosIter(hasCoordPosIter), 
    hasLocate(hasLocate), hasInsert(hasInsert), hasAppend(hasAppend) {
}

std::tuple<Stmt,Expr,Expr> ModeTypeImpl::getCoordIter(
    const std::vector<Expr>& i, Mode mode) const {
  return std::tuple<Stmt,Expr,Expr>(Stmt(), Expr(), Expr());
}

std::tuple<Stmt,Expr,Expr> ModeTypeImpl::getCoordAccess(Expr pPrev, 
    const std::vector<Expr>& i, Mode mode) const {
  return std::tuple<Stmt,Expr,Expr>(Stmt(), Expr(), Expr());
}

std::tuple<Stmt,Expr,Expr> ModeTypeImpl::getPosIter(Expr pPrev, 
    Mode mode) const {
  return std::tuple<Stmt,Expr,Expr>(Stmt(), Expr(), Expr());
}

std::tuple<Stmt,Expr,Expr> ModeTypeImpl::getPosAccess(Expr p, 
    const std::vector<Expr>& i, Mode mode) const {
  return std::tuple<Stmt,Expr,Expr>(Stmt(), Expr(), Expr());
}

std::tuple<Stmt,Expr,Expr> ModeTypeImpl::getLocate(Expr pPrev, 
    const std::vector<Expr>& i, Mode mode) const {
  return std::tuple<Stmt,Expr,Expr>(Stmt(), Expr(), Expr());
}
  
Stmt ModeTypeImpl::getInsertCoord(Expr p,
    const std::vector<Expr>& i, Mode mode) const {
  return Stmt();
}

Expr ModeTypeImpl::getSize(Mode mode) const {
  return Expr();
}

Stmt ModeTypeImpl::getInsertInitCoords(Expr pBegin,
    Expr pEnd, Mode mode) const {
  return Stmt();
}

Stmt ModeTypeImpl::getInsertInitLevel(Expr szPrev,
    Expr sz, Mode mode) const {
  return Stmt();
}

Stmt ModeTypeImpl::getInsertFinalizeLevel(Expr szPrev,
    Expr sz, Mode mode) const {
  return Stmt();
}

Stmt ModeTypeImpl::getAppendCoord(Expr p, Expr i,
    Mode mode) const {
  return Stmt();
}

Stmt ModeTypeImpl::getAppendEdges(Expr pPrev, Expr pBegin,
    Expr pEnd, Mode mode) const {
  return Stmt();
}

Stmt ModeTypeImpl::getAppendInitEdges(Expr pPrevBegin,
    Expr pPrevEnd, Mode mode) const {
  return Stmt();
}

Stmt ModeTypeImpl::getAppendInitLevel(Expr szPrev,
    Expr sz, Mode mode) const {
  return Stmt();
}

Stmt ModeTypeImpl::getAppendFinalizeLevel(Expr szPrev,
    Expr sz, Mode mode) const {
  return Stmt();
}

}

