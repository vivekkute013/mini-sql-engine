#pragma once

#include <vector>
#include <cstdint>

namespace minisql{
  // order determines how many keys/child a node can hold.
  // A small order (like 4) makes it easy to observe behaviour
  // while testing, real databases use much larger orders (100s), size to fit exactly one disk page
  constexpr int TREE_ORDER = 4;

  class BPlusTreeNode{
    public :
      explicit BPlusTreeNode(bool is_leaf) : is_leaf_(is_leaf) {}

      bool IsLeaf() const { return is_leaf_ ;}

      std::vector<int64_t>& GetKeys() { return keys_; }
      const std::vector<int64_t>& GetKeys() const { return keys_; }

      // for internal node : pointers to child node.
      std::vector<BPlusTreeNode*>& GetChildren() { return children_; }

      // for Leaf node - the actual values matching each keys, and a pointer to the next leaf (for the fast range scans.)
      std::vector<int64_t>& GetValues() { return values_; }
      BPlusTreeNode* GetNext() const { return next_; }
      void SetNext(BPlusTreeNode* next) { next_ = next; }

      BPlusTreeNode* GetParent() const { return parent_; }
      void SetParent(BPlusTreeNode* parent) { parent_ = parent; }

      bool IsFull() const { return keys_.size() >= static_cast<size_t>(TREE_ORDER - 1); }

    private : 
      bool is_leaf_;
      std::vector<int64_t> keys_;
      std::vector<BPlusTreeNode*> children_;  // only used if internal node
      std::vector<int64_t> values_;    // only used if Leaf node
      BPlusTreeNode* next_ = nullptr;    // only used if leaf node
      BPlusTreeNode* parent_ = nullptr;

  };

}