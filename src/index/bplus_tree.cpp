#include "minisql/index/bplus_tree.hpp"

namespace minisql{

  void BPlusTree::InsertIntoLeaf(BPlusTreeNode* leaf, int64_t key, int64_t value){
    auto& keys = leaf->GetKeys();
    auto& values = leaf->GetValues();

    size_t pos = 0;
    while(pos < keys.size() && keys[pos] < key){
      pos++;
    }
    keys.insert(keys.begin() + pos, key);
    values.insert(values.begin() + pos, value);
  }


  void BPlusTree::SplitLeafAndInsert(BPlusTreeNode* leaf, int64_t key, int64_t value){
    std::vector<int64_t> temp_keys = leaf->GetKeys();
    std::vector<int64_t> temp_values = leaf->GetValues();

    size_t pos = 0;
    while(pos < temp_keys.size() && temp_keys[pos] < key){
      pos++;
    }

    temp_keys.insert(temp_keys.begin() + pos, key);
    temp_values.insert(temp_values.begin() + pos, value);

    size_t mid = temp_keys.size() / 2;

    BPlusTreeNode* new_leaf = new BPlusTreeNode(true);

    leaf->GetKeys().assign(temp_keys.begin(), temp_keys.begin() + mid);
    leaf->GetValues().assign(temp_values.begin(), temp_values.begin() + mid);

    new_leaf->GetKeys().assign(temp_keys.begin() + mid, temp_keys.end());
    new_leaf->GetValues().assign(temp_values.begin() + mid, temp_values.end());

    new_leaf->SetNext(leaf->GetNext());
    leaf->SetNext(new_leaf);

    int64_t push_key = new_leaf->GetKeys()[0];
    InsertIntoParent(leaf, push_key, new_leaf);

  }


  void BPlusTree::InsertIntoParent(BPlusTreeNode* left, int64_t key, BPlusTreeNode* right){
    BPlusTreeNode* parent = left->GetParent();

    if(parent == nullptr){
      BPlusTreeNode* new_root = new BPlusTreeNode(false);
      new_root->GetKeys().push_back(key);
      new_root->GetChildren().push_back(left);
      new_root->GetChildren().push_back(right);

      left->SetParent(new_root);
      right->SetParent(new_root);

      root_ = new_root;
      return;
    }

    auto& keys = parent->GetKeys();

    size_t pos = 0;
    while(pos < keys.size() && keys[pos] < key){
      pos++;
    }

    if(!parent->IsFull()){
      keys.insert(keys.begin() + pos, key);
      parent->GetChildren().insert(parent->GetChildren().begin() + pos + 1, right);
      right->SetParent(parent);
    } else {
      SplitInternalAndInsert(parent, pos, key, right);
    }
  }


  void BPlusTree::SplitInternalAndInsert(BPlusTreeNode* node, size_t insert_pos,
                                          int64_t key, BPlusTreeNode* right_child){
    std::vector<int64_t> temp_keys = node->GetKeys();
    std::vector<BPlusTreeNode*> temp_children = node->GetChildren();

    temp_keys.insert(temp_keys.begin() + insert_pos, key);
    temp_children.insert(temp_children.begin() + insert_pos + 1, right_child);

    size_t mid = temp_keys.size() / 2;
    int64_t push_key = temp_keys[mid];

    BPlusTreeNode* new_node = new BPlusTreeNode(false);

    node->GetKeys().assign(temp_keys.begin(), temp_keys.begin() + mid);
    node->GetChildren().assign(temp_children.begin(), temp_children.begin() + mid + 1);

    new_node->GetKeys().assign(temp_keys.begin() + mid + 1, temp_keys.end());
    new_node->GetChildren().assign(temp_children.begin() + mid + 1, temp_children.end());

    for(BPlusTreeNode* child : new_node->GetChildren()){
      child->SetParent(new_node);
    }

    InsertIntoParent(node, push_key, new_node);
  }


  void BPlusTree::Insert(int64_t key, int64_t value){
    if(root_ == nullptr){
      root_ = new BPlusTreeNode(true);
      root_->GetKeys().push_back(key);
      root_->GetValues().push_back(value);
      return;
    }
    BPlusTreeNode* leaf = FindLeaf(key);

    if(!leaf->IsFull()){
      InsertIntoLeaf(leaf, key, value);
    }else{
      SplitLeafAndInsert(leaf, key, value);
    }
  }



  BPlusTreeNode* BPlusTree::FindLeaf(int64_t key) const {
    if(root_ == nullptr){
      return nullptr;
    }
    BPlusTreeNode* current = root_;

    while(!current->IsLeaf()){
      const auto& keys = current->GetKeys();
      size_t child_index = 0;

      while(child_index < keys.size() && key >= keys[child_index]){
        child_index++;
      }
      current = current->GetChildren()[child_index];
    }
    return current;
  }

  bool BPlusTree::Search(int64_t key, int64_t* result_value) const {
    BPlusTreeNode* leaf = FindLeaf(key);
    if(leaf == nullptr){
      return false;
    }

    const auto& keys = leaf->GetKeys();
    for(size_t i = 0; i < keys.size(); i++){
      if(keys[i] == key){
        *result_value = leaf->GetValues()[i];
        return true;
      }
    }
    return false;

  }
}