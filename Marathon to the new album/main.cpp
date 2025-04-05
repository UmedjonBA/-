#include <iostream>
#include <algorithm>
#include <vector>
#include <utility>
#include <map>
#include <iomanip>
#include <iterator>

template <typename K1, typename K2>
class AVLTree {
private:
  using K = std::pair<K1, K2>;
  struct Node {
    K key;
    int count;
    int height;
    Node* left;
    Node* right;

    Node(const K& k) : key(k), count(1), height(1), left(nullptr), right(nullptr) {}
  };

  Node* root = nullptr;

  int height(Node* node) const {
    return node ? node->height : 0;
  }

  int count(Node* node) const {
    return node ? node->count : 0;
  }

  void updateHeight(Node* node) {
    if (node) {
      node->height = 1 + std::max(height(node->left), height(node->right));
    }
  }

  void updateCount(Node* node) {
    if (node) {
      node->count = 1 + count(node->left) + count(node->right);
    }
  }

  int balanceFactor(Node* node) const {
    return node ? height(node->left) - height(node->right) : 0;
  }

  Node* rotateRight(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    updateHeight(y);
    updateHeight(x);
    updateCount(y);
    updateCount(x);

    return x;
  }

  Node* rotateLeft(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    updateHeight(x);
    updateHeight(y);
    updateCount(x);
    updateCount(y);

    return y;
  }

  Node* balance(Node* node) {
    updateHeight(node);
    updateCount(node);

    int bf = balanceFactor(node);
    if (bf > 1) {
      if (balanceFactor(node->left) < 0) {
        node->left = rotateLeft(node->left);
      }
      return rotateRight(node);
    }
    if (bf < -1) {
      if (balanceFactor(node->right) > 0) {
        node->right = rotateRight(node->right);
      }
      return rotateLeft(node);
    }
    return node;
  }

  Node* insert(Node* node, const K& key) {
    if (!node) {
      return new Node(key);
    }

    if (key.first < node->key.first) {
      node->left = insert(node->left, key);
    } else if (key.first > node->key.first) {
      node->right = insert(node->right, key);
    }
    else {
          if(key.second < node->key.second)
            node->left = insert(node->left, key);
          else if(key.second > node->key.second)
            node->right = insert(node->right, key);
         else
          return node;
    }
    return balance(node);
  }

  Node* findMin(Node* node) const {
    while (node->left) {
      node = node->left;
    }
    return node;
  }

  Node* remove(Node* node, const K& key) {
    if (!node) {
      return node;
    }

    if (key.first < node->key.first) {
      node->left = remove(node->left, key);
    } else if (key.first > node->key.first) {
      node->right = remove(node->right, key);
    }
    else {
      if(key.second < node->key.second)
        node->left = remove(node->left,key);
      else if (key.second > node->key.second)
        node->right = remove(node->right,key);
       else
        {
           if (!node->left || !node->right) {
            Node* temp = node->left ? node->left : node->right;
              delete node;
              return temp;
            } else {
              Node* temp = findMin(node->right);
              node->key = temp->key;
              node->right = remove(node->right, temp->key);
            }
       }
    }
    return balance(node);
  }

  int countSmaller(Node* node, const K& key) const {
    if (!node) return 0;
      if (key.first == node->key.first)
        {
           if(key.second == node->key.second)
             return count(node->left);
            else if(key.second < node->key.second)
               return countSmaller(node->left, key);
            else
                return count(node->left) + 1 + countSmaller(node->right, key);
         }


     if (key.first < node->key.first)
      return countSmaller(node->left, key);
    else
      return count(node->left) + 1 + countSmaller(node->right, key);
  }

  Node* find(Node* node, const K& key) const {
    if (!node) return nullptr;
      if (key.first == node->key.first && key.second == node->key.second) return node;
       if (key.first < node->key.first) return find(node->left, key);
       return find(node->right, key);
  }


  void inorderTraversal(Node* node, std::vector<K>& sortedKeys) const {
    if (node) {
      inorderTraversal(node->left, sortedKeys);
      sortedKeys.push_back(node->key);
      inorderTraversal(node->right, sortedKeys);
    }
  }

  Node* upperBound(Node* node, const K& key, Node* closestSoFar) const
   {
     if (!node)
        return closestSoFar;

     if (key.first <= node->key.first) {

         return upperBound(node->left, key, closestSoFar);

     } else {

         Node* upper = upperBound(node->right, key, node);
           if (upper != nullptr) {
            return upper;
           }
          else
             return node;
    }

 }

   Node* lowerBound(Node* node, const K& key) const{
         if (!node)
            return nullptr;
        if(key.first == node->key.first){
          if(key.second == node->key.second)
            return node;
          else if(key.second < node->key.second)
            {
                Node* lower_node = lowerBound(node->left, key);
                 if (lower_node == nullptr) {
                 return node;
                 } else{
                    return lower_node;
                 }
            }
          else{
             return lowerBound(node->right, key);
          }

       }
        if (key.first < node->key.first) {
            Node* lower_node = lowerBound(node->left, key);
            if (lower_node == nullptr) {
                return node;
            } else {
             return lower_node;
            }

        } else{
            return lowerBound(node->right, key);
        }

   }


    void destroyTree(Node* node) {
        if (node) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

 public:
  void insert(const K& key) {
    root = insert(root, key);
  }

  void remove(const K& key) {
    root = remove(root, key);
  }

  int countSmaller(const K& key) const {
    return countSmaller(root, key);
  }
  int getTreeSize() const {
    return count(root);
  }

  void printInorder(Node* node) const {
    if (node) {
      printInorder(node->left);
      std::cout << "Key: " << node->key.first << ", " << node->key.second
                << ", Count: " << node->count << std::endl;
      printInorder(node->right);
    }
  }

  void printInorder() const {
    printInorder(root);
  }

  int getIndexByValue(const K& key) const {
    if (!find(root, key)) {
      return -1;
    }
    return countSmaller(key);
  }

  K* upper_bound(const K& key) const{
      Node* upper_node = upperBound(root, key, nullptr);
      if (upper_node == nullptr)
        return nullptr;
      return &upper_node->key;
   }

   K* lower_bound(const K& key) const {
      Node* lower_node = lowerBound(root,key);
      if (lower_node == nullptr)
          return nullptr;

       return &lower_node->key;
    }


  ~AVLTree() {
    destroyTree(root);
  }
};

int main() {
  AVLTree<int, int> tree;
  int Q;
  std::cin >> Q;

  std::map<int, int> user_times;

  for (int i = 0; i < Q; ++i) {
    std::string type;
    std::cin >> type;

    if (type == "RUN") {
      int user, page;
      std::cin >> user >> page;

        if(user_times.count(user)) {
           int old_time = user_times[user];
           tree.remove({old_time, user});
        }
        user_times[user] = page;
        tree.insert({page, user});

    } else if (type == "CHEER") {
      int user;
      std::cin >> user;

      if (user_times.find(user) == user_times.end()) {
        std::cout << std::fixed << std::setprecision(6) << 0.0 << std::endl;
      } else {
         int user_time = user_times[user];
        if(tree.getTreeSize() <=1){
            std::cout << std::fixed << std::setprecision(6) << 1.0 << std::endl;
             continue;
        }
        auto* ptr = tree.upper_bound({user_time,user});
        int count = 0;
        if(ptr){
        count = tree.getIndexByValue(*ptr) + 1;
        }
        double ratio = static_cast<double>(count) / (tree.getTreeSize() - 1);
        std::cout << std::fixed << std::setprecision(6) << ratio << std::endl;
      }
    }
  }
  return 0;
}