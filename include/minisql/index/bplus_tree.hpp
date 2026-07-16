#pragma once

#include "minisql/index/bplus_tree_node.hpp"
#include <optional>

namespace minisql{
  class BPlusTree{
  public :
    BPlusTree() : root_(nullptr) {}
    BPlusTreeNode* parent_ = nullptr;

    bool Search(int64_t key, int64_t* result_value) const;
    void Insert(int64_t key, int64_t value);

  private :
    BPlusTreeNode* FindLeaf(int64_t key) const;
    void InsertIntoLeaf(BPlusTreeNode* leaf, int64_t key, int64_t value);

    BPlusTreeNode* GetParent() const { return parent_; }
    void SetParent(BPlusTreeNode* parent) { parent_ = parent ; }

    void SplitLeafAndInsert(BPlusTreeNode* leaf, int64_t key, int64_t value);

    void InsertIntoParent(BPlusTreeNode* left, int64_t key, BPlusTreeNode* right);

    BPlusTreeNode* root_;
  };

}