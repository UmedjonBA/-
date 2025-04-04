#include <iostream>
#include <limits>
#include <vector>
#include <memory>
#include <functional>
#include <utility>
#include <algorithm>
#include <string>

struct Node {
    std::pair<int64_t, size_t> value;
    std::weak_ptr<Node> parent;
    size_t heap_index;
    std::vector<std::shared_ptr<Node>> children;

    Node(std::pair<int64_t, size_t> val, std::shared_ptr<Node> p, size_t h_i, const std::vector<std::shared_ptr<Node>>& c)
        : value(val),
          parent(p),
          heap_index(h_i),
          children(c) {}

    size_t GetHeapIndex() const {
        if (auto p = parent.lock()) {
            return p->GetHeapIndex();
        }
        return heap_index;
    }
};

struct InputData {
    size_t num_heaps;
    size_t num_requests;
    struct Request {
        int64_t request_type;
        size_t heap_num_1 = 0;
        size_t heap_num_2 = 0;
        int64_t value = 0;
        size_t node_num = 0;
    };
    std::vector<Request> requests;
};

enum RequestType {
    Insert = 0,
    Move = 1,
    Delete = 2,
    Update = 3,
    GetMin = 4,
    ExtractMin = 5
};

class BinomialHeap {
public:
    BinomialHeap() : cmp_([](const std::pair<int64_t, size_t>& a,
                            const std::pair<int64_t, size_t>& b) {
                        return a < b;
                    }),
                    heap_index_(0),
                    history_(nullptr) {}

    BinomialHeap(const BinomialHeap&) = default;
    BinomialHeap& operator=(const BinomialHeap&) = default;
    ~BinomialHeap() = default;

    void SetHistoryIndex(std::shared_ptr<std::vector<std::shared_ptr<Node>>> history, size_t heap_index) {
        history_ = history;
        heap_index_ = heap_index;
    }

    bool IsEmpty() const { return forest_.empty(); }

    std::shared_ptr<Node> Insert(int64_t value, size_t index) {
        BinomialHeap single_heap;
        auto init_value = std::make_pair(value, index);
        auto new_node = std::make_shared<Node>(init_value, nullptr, heap_index_, std::vector<std::shared_ptr<Node>>());
        single_heap.forest_.push_back(new_node);
        Merge(single_heap);
        UpdateHeapIndices();
        return new_node;
    }

    std::vector<std::shared_ptr<Node>>::const_iterator GetMin() const {
        auto min_node = forest_.begin();
        for (auto iter = forest_.begin(); iter < forest_.end(); ++iter) {
            if (cmp_((*iter)->value, (*min_node)->value)) {
                min_node = iter;
            }
        }
        return min_node;
    }

    void MoveHeap(BinomialHeap& heap) {
        Merge(heap);
        heap.forest_.clear();
        UpdateHeapIndices();
    }

    void RemoveMin() {
        auto min_node = GetMin();
        BinomialHeap temp_heap((*min_node)->children);

        for (auto& child : (*min_node)->children) {
            child->parent.reset();
        }
        forest_.erase(min_node);

        Merge(temp_heap);
        UpdateHeapIndices();
    }

    void UpdateNode(std::shared_ptr<Node> node, int64_t new_value) {
        auto old_value = node->value;
        node->value.first = new_value;
        if (cmp_(old_value, node->value)) {
            ShiftDown(node);
        } else if (cmp_(node->value, old_value)) {
            ShiftUp(node);
        }
        UpdateHeapIndices();
    }

    void DeleteNode(std::shared_ptr<Node> target) {
        UpdateNode(target, std::numeric_limits<int64_t>::min());
        RemoveMin();
    }

    BinomialHeap(const std::vector<std::shared_ptr<Node>>& forest)
        : cmp_([](const std::pair<int64_t, size_t>& a,
                  const std::pair<int64_t, size_t>& b) {
                return a < b;
              }),
          heap_index_(0),
          history_(nullptr),
          forest_(forest) {}

    size_t GetHeapIndex() const { return heap_index_; }

    void Merge(BinomialHeap& heap) {
        if (IsEmpty()) {
            forest_ = heap.forest_;
            return;
        }
        if (heap.IsEmpty()) {
            return;
        }

        std::shared_ptr<Node> temp_tree = nullptr;
        auto iter_a = forest_.begin();
        auto iter_b = heap.forest_.begin();

        std::vector<std::shared_ptr<Node>> new_forest = MergeTwoHeaps(heap, iter_a, iter_b, temp_tree);
        new_forest = MergeRemainingTrees(new_forest, iter_a, iter_b, temp_tree, heap);

        forest_ = new_forest;
    }

private:
    std::function<bool(const std::pair<int64_t, size_t>&,
                       const std::pair<int64_t, size_t>&)> cmp_;
    size_t heap_index_;
    std::shared_ptr<std::vector<std::shared_ptr<Node>>> history_;
    std::vector<std::shared_ptr<Node>> forest_;

    void UpdateHeapIndices() {
        for (auto& root : forest_) {
            root->heap_index = heap_index_;
        }
    }

    std::shared_ptr<Node> MergeTrees(std::shared_ptr<Node> tree1, std::shared_ptr<Node> tree2) {
        if (!cmp_(tree2->value, tree1->value)) {
            tree1->children.push_back(tree2);
            tree2->parent = tree1;
            return tree1;
        }
        tree2->children.push_back(tree1);
        tree1->parent = tree2;
        return tree2;
    }

    void ShiftUp(std::shared_ptr<Node> node) {
        if (auto p = node->parent.lock()) {
            if (cmp_(node->value, p->value)) {
                std::swap(p->value, node->value);
                std::swap((*history_)[p->value.second - 1],
                          (*history_)[node->value.second - 1]);
                ShiftUp(p);
            }
        }
    }

    void ShiftDown(std::shared_ptr<Node> node) {
        if (node->children.empty()) {
            return;
        }

        std::shared_ptr<Node> min_child = node->children.front();
        for (const auto& child : node->children) {
            if (cmp_(child->value, min_child->value)) {
                min_child = child;
            }
        }

        if (cmp_(min_child->value, node->value)) {
            std::swap(min_child->value, node->value);
            std::swap((*history_)[min_child->value.second - 1],
                      (*history_)[node->value.second - 1]);
            ShiftDown(min_child);
        }
    }

    std::vector<std::shared_ptr<Node>> MergeTwoHeaps(BinomialHeap& heap,
                                                     std::vector<std::shared_ptr<Node>>::iterator& iter_a,
                                                     std::vector<std::shared_ptr<Node>>::iterator& iter_b,
                                                     std::shared_ptr<Node>& temp_tree) {
        std::vector<std::shared_ptr<Node>> new_forest;
        while (iter_a != forest_.end() && iter_b != heap.forest_.end()) {
            if (temp_tree == nullptr) {
                if ((*iter_a)->children.size() < (*iter_b)->children.size()) {
                    new_forest.push_back(*iter_a);
                    ++iter_a;
                } else if ((*iter_a)->children.size() > (*iter_b)->children.size()) {
                    new_forest.push_back(*iter_b);
                    ++iter_b;
                } else {
                    temp_tree = MergeTrees(*iter_a, *iter_b);
                    ++iter_a;
                    ++iter_b;
                }
            } else {
                new_forest = HandleTempTreeConflict(new_forest, iter_a, iter_b, temp_tree, heap);
            }
        }
        return new_forest;
    }

    std::vector<std::shared_ptr<Node>> HandleTempTreeConflict(std::vector<std::shared_ptr<Node>> new_forest,
                                                              std::vector<std::shared_ptr<Node>>::iterator& iter_a,
                                                              std::vector<std::shared_ptr<Node>>::iterator& iter_b,
                                                              std::shared_ptr<Node>& temp_tree,
                                                              BinomialHeap& heap) {
        if ((*iter_a)->children.size() < temp_tree->children.size() &&
            (*iter_a)->children.size() < (*iter_b)->children.size()) {
            new_forest.push_back(*iter_a);
            ++iter_a;
        } else if ((*iter_b)->children.size() < temp_tree->children.size() &&
                   (*iter_b)->children.size() < (*iter_a)->children.size()) {
            new_forest.push_back(*iter_b);
            ++iter_b;
        } else if (temp_tree->children.size() < (*iter_a)->children.size() &&
                   temp_tree->children.size() < (*iter_b)->children.size()) {
            new_forest.push_back(temp_tree);
            temp_tree = nullptr;
        } else if (temp_tree->children.size() == (*iter_a)->children.size()) {
            temp_tree = MergeTrees(temp_tree, *iter_a);
            ++iter_a;
        } else if (temp_tree->children.size() == (*iter_b)->children.size()) {
            temp_tree = MergeTrees(temp_tree, *iter_b);
            ++iter_b;
        }
        return new_forest;
    }

    std::vector<std::shared_ptr<Node>> MergeRemainingTrees(std::vector<std::shared_ptr<Node>> new_forest,
                                                           std::vector<std::shared_ptr<Node>>::iterator& iter_a,
                                                           std::vector<std::shared_ptr<Node>>::iterator& iter_b,
                                                           std::shared_ptr<Node>& temp_tree,
                                                           BinomialHeap& heap) {
        while (iter_a < forest_.end() && temp_tree != nullptr) {
            if ((*iter_a)->children.size() < temp_tree->children.size()) {
                new_forest.push_back(*iter_a);
                ++iter_a;
            } else if ((*iter_a)->children.size() > temp_tree->children.size()) {
                new_forest.push_back(temp_tree);
                temp_tree = nullptr;
            } else {
                temp_tree = MergeTrees(temp_tree, *iter_a);
                ++iter_a;
            }
        }

        while (iter_b < heap.forest_.end() && temp_tree != nullptr) {
            if ((*iter_b)->children.size() < temp_tree->children.size()) {
                new_forest.push_back(*iter_b);
                ++iter_b;
            } else if ((*iter_b)->children.size() > temp_tree->children.size()) {
                new_forest.push_back(temp_tree);
                temp_tree = nullptr;
            } else {
                temp_tree = MergeTrees(temp_tree, *iter_b);
                ++iter_b;
            }
        }

        if (temp_tree != nullptr) {
            new_forest.push_back(temp_tree);
            temp_tree = nullptr;
        }

        if (temp_tree == nullptr) {
            while (iter_a < forest_.end()) {
                new_forest.push_back(*iter_a);
                ++iter_a;
            }
            while (iter_b < heap.forest_.end()) {
                new_forest.push_back(*iter_b);
                ++iter_b;
            }
        }

        return new_forest;
    }
};

struct Solver {
    std::vector<BinomialHeap> heaps;
    std::shared_ptr<std::vector<std::shared_ptr<Node>>> history;
    size_t insert_index;

    Solver(size_t num_heaps)
        : heaps(num_heaps), history(std::make_shared<std::vector<std::shared_ptr<Node>>>()), insert_index(1) {
        for (size_t i = 0; i < num_heaps; ++i) {
            heaps[i].SetHistoryIndex(history, i);
        }
    }

    void ProcessRequest(const InputData::Request& req) {
        if (req.request_type == Insert) {
            history->push_back(heaps[req.heap_num_1 - 1].Insert(req.value, insert_index));
            ++insert_index;
        } else if (req.request_type == Move) {
            heaps[req.heap_num_2 - 1].MoveHeap(heaps[req.heap_num_1 - 1]);
        } else if (req.request_type == Delete) {
            size_t heap_num = (*history)[req.heap_num_1 - 1]->GetHeapIndex();
            heaps[heap_num].DeleteNode((*history)[req.heap_num_1 - 1]);
        } else if (req.request_type == Update) {
            size_t heap_num = (*history)[req.node_num - 1]->GetHeapIndex();
            heaps[heap_num].UpdateNode((*history)[req.node_num - 1], req.value);
        } else if (req.request_type == GetMin) {
            std::cout << (*heaps[req.heap_num_1 - 1].GetMin())->value.first << '\n';
        } else if (req.request_type == ExtractMin) {
            heaps[req.heap_num_1 - 1].RemoveMin();
        }
    }
};

InputData ReadInput() {
    InputData input;
    std::cin >> input.num_heaps;
    std::cin >> input.num_requests;
    input.requests.resize(input.num_requests);
    for (size_t i = 0; i < input.num_requests; ++i) {
        int64_t request_type;
        std::cin >> request_type;
        input.requests[i].request_type = request_type;

        if (request_type == Insert) {
            size_t heap_num;
            int64_t value;
            std::cin >> heap_num >> value;
            input.requests[i].heap_num_1 = heap_num;
            input.requests[i].value = value;
        } else if (request_type == Move) {
            size_t from, to;
            std::cin >> from >> to;
            input.requests[i].heap_num_1 = from;
            input.requests[i].heap_num_2 = to;
        } else if (request_type == Delete) {
            size_t request_num;
            std::cin >> request_num;
            input.requests[i].heap_num_1 = request_num;
        } else if (request_type == Update) {
            size_t node_num;
            int64_t new_value;
            std::cin >> node_num >> new_value;
            input.requests[i].node_num = node_num;
            input.requests[i].value = new_value;
        } else if (request_type == GetMin) {
            size_t heap_num;
            std::cin >> heap_num;
            input.requests[i].heap_num_1 = heap_num;
        } else if (request_type == ExtractMin) {
            size_t heap_num;
            std::cin >> heap_num;
            input.requests[i].heap_num_1 = heap_num;
        }
    }
    return input;
}

std::vector<int64_t> Solve(const InputData& input) {
    Solver solver(input.num_heaps);
    for (size_t i = 0; i < input.num_requests; ++i) {
        solver.ProcessRequest(input.requests[i]);
    }
    return {};
}

void PrintOutput(const std::vector<int64_t>&) {}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    InputData input = ReadInput();
    std::vector<int64_t> result = Solve(input);
    PrintOutput(result);
    return 0;
}
