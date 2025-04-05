#include <algorithm>
#include <vector>
#include <iostream>
#include <stack>

template <typename T>
struct Node {
  int64_t prior;
  size_t tree_size;
  int64_t num;
  Node<T>* left = nullptr;
  Node<T>* right = nullptr;
  Node<T>* parent = nullptr;
  T key;

  Node(const T& value, int64_t prior, int64_t num)
      : prior(prior),
        tree_size(1),
        key(value),
        num(num) {
  }
  Node(T&& value, int64_t prior)
      : prior(prior),
        tree_size(1),
        key(std::move(value)) {
  }

  ~Node() {
    delete left;
    delete right;
  }
};

struct More {
  bool operator()(size_t num1, size_t num2) const {
    return num1 < num2;
  }
};

template <typename T>
class Treap {
 public:
  Treap() {
    root_ = nullptr;
  }

  Treap(std::vector<Node<T>*> arr){
    std::stack<Node<T>*> temp;
    temp.push(arr[0]);
    root_ = arr[0];
    for(int i=1; i<arr.size(); ++i) {
        while (!temp.empty() && temp.top()->prior > arr[i]->prior)
        {
            temp.pop();
        }
        if(temp.empty()) {
            arr[i]->left = root_;
            root_->parent = arr[i];
            root_ = arr[i];
            temp.push(arr[i]);
        }else {
            Merg(temp.top(), arr[i]);
            temp.push(arr[i]);
        }
    }
  }
   
  void Merg(Node<T>* a, Node<T>* b) {
    if(a->prior < b->prior) {
        Node<T>* temp = a->right;
        a->right = b;
        b->left = temp; 
        b->parent = a;
        if(temp != nullptr) temp->parent = b;
    } else {
        b->left = a;
        a->parent = b;
    }
  }
  ~Treap() {
    delete root_;
  }
 
 protected:
  Node<T>* root_;
};

int main() {
    int n, a, b;
    std::cin>> n;
    std::vector<Node<int>*> res(n);
    for(int i=0;i<n;++i) {
        std::cin>>a>>b;
        res[i] = new Node<int>(a,b,i+1);
    }
    Treap<int> solver(res);
    std::cout<<"YES\n";
    for(int i=0;i<n;++i) {
        if(res[i]->parent != nullptr) std::cout<< res[i]->parent->num <<' ';
        else std::cout<<0<< ' ';
        if(res[i]->left != nullptr) std::cout<< res[i]->left->num <<' ';
        else std::cout<<0<< ' ';
        if(res[i]->right != nullptr) std::cout<< res[i]->right->num <<'\n';
        else std::cout<<0<< '\n';
    }
    return 0;
}