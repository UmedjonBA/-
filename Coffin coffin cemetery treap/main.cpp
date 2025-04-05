#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include <tuple>
#include <functional>

typedef long long LongLong;

class ImplicitTreap {
private:
    struct Node {
        LongLong pr;
        LongLong val;
        LongLong left;
        LongLong right;
        LongLong add;
        LongLong sum_tree;
        bool is_direct_tree;
        bool is_reversed_tree;
        bool rev;
        bool assign_to_zero;
        LongLong size;
        Node *le_ft;
        Node *rai_t;

        static std::mt19937 gen;
        static std::uniform_int_distribution<LongLong> uniform_distribution;

        static LongLong RandInt() {
            return uniform_distribution(gen);
        }

        Node()
            : pr(RandInt()), val(0), left(0), right(0),
              add(0), sum_tree(0),
              is_direct_tree(true), is_reversed_tree(true),
              rev(false), assign_to_zero(false),
              size(1), le_ft(nullptr), rai_t(nullptr) {
        }

        explicit Node(LongLong val)
            : pr(RandInt()), val(val), left(val), right(val),
              add(0), sum_tree(val),
              is_direct_tree(true), is_reversed_tree(true),
              rev(false), assign_to_zero(false),
              size(1), le_ft(nullptr), rai_t(nullptr) {
        }

        ~Node() {
            delete le_ft;
            delete rai_t;
        }

        LongLong Size() const {
            return size;
        }

        LongLong SumTree() const {
            return sum_tree;
        }

        void AddRecalc(LongLong delta) {
            val += delta;
            sum_tree += Size() * delta;
            left += delta;
            right += delta;
        }

        void UpdateAssignToZero() {
            is_direct_tree = true;
            is_reversed_tree = true;
            val = 0;
            left = 0;
            right = 0;
            sum_tree = 0;
            add = 0;
        }

        void PushReverse() {
            if (rev) {
                rev = false;
                std::swap(left, right);
                std::swap(le_ft, rai_t);
                std::swap(is_direct_tree, is_reversed_tree);
                if (le_ft != nullptr) {
                    le_ft->rev ^= 1;
                }
                if (rai_t != nullptr) {
                    rai_t->rev ^= 1;
                }
            }
        }

        void PushZero() {
            if (assign_to_zero) {
                if (le_ft != nullptr) {
                    le_ft->UpdateAssignToZero();
                    le_ft->assign_to_zero = true;
                }
                if (rai_t != nullptr) {
                    rai_t->UpdateAssignToZero();
                    rai_t->assign_to_zero = true;
                }
                assign_to_zero = false;
            }
        }

        void PushAdd() {
            if (add != 0) {
                if (le_ft != nullptr) {
                    le_ft->AddRecalc(add);
                    le_ft->add += add;
                }
                if (rai_t != nullptr) {
                    rai_t->AddRecalc(add);
                    rai_t->add += add;
                }
                add = 0;
            }
        }

        void Push() {
            PushReverse();
            PushZero();
            PushAdd();
        }

        template<bool is_direct = false>
        void UpdateReverse() {
            bool is_ordered_tree = (is_direct ? is_direct_tree : is_reversed_tree);
            if (le_ft == nullptr && rai_t == nullptr) {
                is_ordered_tree = true;
            } else if (le_ft != nullptr && rai_t == nullptr) {
                if (is_direct) {
                    is_ordered_tree = (le_ft->is_direct_tree && le_ft->right <= val);
                } else {
                    is_ordered_tree = (le_ft->is_reversed_tree && le_ft->right >= val);
                }
            } else if (le_ft == nullptr && rai_t != nullptr) {
                if (is_direct) {
                    is_ordered_tree = (rai_t->is_direct_tree && rai_t->left >= val);
                } else {
                    is_ordered_tree = (rai_t->is_reversed_tree && rai_t->left <= val);
                }
            } else if (le_ft != nullptr && rai_t != nullptr) {
                if (is_direct) {
                    is_ordered_tree = (le_ft->is_direct_tree && rai_t->is_direct_tree
                                     && le_ft->right <= val && rai_t->left >= val);
                } else {
                    is_ordered_tree = (le_ft->is_reversed_tree && rai_t->is_reversed_tree
                                     && le_ft->right >= val && rai_t->left <= val);
                }
            }

            (is_direct ? is_direct_tree : is_reversed_tree) = is_ordered_tree;
        }

        void Update() {
            if (le_ft != nullptr) {
                le_ft->Push();
            }
            if (rai_t != nullptr) {
                rai_t->Push();
            }

            LongLong ls = (le_ft == nullptr ? 0 : le_ft->Size());
            LongLong rs = (rai_t == nullptr ? 0 : rai_t->Size());
            LongLong lsum = (le_ft == nullptr ? 0 : le_ft->SumTree());
            LongLong rsum = (rai_t == nullptr ? 0 : rai_t->SumTree());

            size = ls + rs + 1;
            sum_tree = lsum + rsum + val;

            UpdateReverse<true>();
            UpdateReverse<false>();

            left = (le_ft == nullptr) ? val : le_ft->left;
            right = (rai_t == nullptr) ? val : rai_t->right;
        }

        void Assign(LongLong assignment) {
            assign_to_zero = true;
            UpdateAssignToZero();
            add = assignment;
            AddRecalc(assignment);
        }
    };

    explicit ImplicitTreap(Node *teet) : root_(teet) {}

    typedef std::pair<Node *, Node *> Nodes;

    Node *root_;

    template<bool is_direct = false>
static Nodes Split(Node *current_node, LongLong split_key, LongLong accumulated_size = 0) {
    if (current_node == nullptr) {
        return Nodes(nullptr, nullptr);
    }
    current_node->Push();
    LongLong left_subtree_size = (current_node->le_ft == nullptr) ? 0 : current_node->le_ft->Size();
    LongLong compare_value = is_direct ? current_node->val - 1 : accumulated_size + left_subtree_size;

    Nodes result;
    if (split_key > compare_value) {
        Nodes tmp = Split<is_direct>(current_node->rai_t, split_key, accumulated_size + 1 + left_subtree_size);
        current_node->rai_t = tmp.first;
        current_node->Update();
        result.first = current_node;
        result.second = tmp.second;
    } else {
        Nodes tmp = Split<is_direct>(current_node->le_ft, split_key, accumulated_size);
        current_node->le_ft = tmp.second;
        current_node->Update();
        result.first = tmp.first;
        result.second = current_node;
    }
    return result;
}

static Node *Merge(Node *left_subtree, Node *right_subtree) {
    if (left_subtree == nullptr) {
        return right_subtree;
    }
    if (right_subtree == nullptr) {
        return left_subtree;
    }
    left_subtree->Push();
    right_subtree->Push();
    if (left_subtree->pr <= right_subtree->pr) {
        right_subtree->le_ft = Merge(left_subtree, right_subtree->le_ft);
        right_subtree->Update();
        return right_subtree;
    }
        left_subtree->rai_t = Merge(left_subtree->rai_t, right_subtree);
        left_subtree->Update();
        return left_subtree;
    
}

std::tuple<Node*, Node*, Node*> Prepare(LongLong left_index, LongLong right_index) {
    // Разделяем дерево по правой границе (right_index + 1)
    Nodes first_split = Split(root_, right_index + 1);
    Node *middle_part = first_split.first;
    Node *right_part = first_split.second;

    // Разделяем полученную левую часть по левой границе (left_index)
    Nodes second_split = Split(middle_part, left_index);
    Node *left_part = second_split.first;
    middle_part = second_split.second;

    return std::make_tuple(left_part, middle_part, right_part);
}

template<typename U>
LongLong Operate(U lambda, LongLong left_index, LongLong right_index) {
    Node *left_part;
    Node *middle_part;
    Node *right_part;
    std::tie(left_part, middle_part, right_part) = Prepare(left_index, right_index);
    LongLong return_value = 0;
    if (middle_part != nullptr) {
        return_value = lambda(middle_part);
    }
    root_ = Merge(Merge(left_part, middle_part), right_part);
    return return_value;
}

template<bool is_direct>
static LongLong LenOfSuffix(Node *current_node) {
    if (current_node == nullptr) {
        return 0;
    }
    current_node->Push();
    if (current_node->le_ft != nullptr) {
        current_node->le_ft->Push();
    }
    if (current_node->rai_t != nullptr) {
        current_node->rai_t->Push();
    }

    if (current_node->le_ft && current_node->le_ft->le_ft != nullptr) {
        current_node->le_ft->le_ft->Push();
    }
    if (current_node->le_ft && current_node->le_ft->rai_t != nullptr) {
        current_node->le_ft->rai_t->Push();
    }
    if (current_node->rai_t && current_node->rai_t->le_ft != nullptr) {
        current_node->rai_t->le_ft->Push();
    }
    if (current_node->rai_t && current_node->rai_t->rai_t != nullptr) {
        current_node->rai_t->rai_t->Push();
    }

    LongLong left_subtree_size = (current_node->le_ft == nullptr) ? 0 : current_node->le_ft->Size();
    LongLong tleft = (current_node->rai_t == nullptr) ? current_node->val : current_node->rai_t->left;
    LongLong tright = (current_node->le_ft == nullptr) ? current_node->val : current_node->le_ft->right;

    if constexpr (is_direct) {
    if (current_node->rai_t && !current_node->rai_t->is_direct_tree) {
        return 1 + left_subtree_size + LenOfSuffix<is_direct>(current_node->rai_t);
    }
    if (current_node->rai_t && current_node->val > tleft) {
        return 1 + left_subtree_size;
    }
    if (current_node->le_ft && current_node->val < tright) {
        return left_subtree_size;
    }
    return LenOfSuffix<is_direct>(current_node->le_ft);
}

// Для случая, когда is_direct == false:
if (current_node->rai_t && !current_node->rai_t->is_reversed_tree) {
    return 1 + left_subtree_size + LenOfSuffix<is_direct>(current_node->rai_t);
}
if (current_node->rai_t && current_node->val < tleft) {
    return 1 + left_subtree_size;
}
if (current_node->le_ft && current_node->val > tright) {
    return left_subtree_size;
}
return LenOfSuffix<is_direct>(current_node->le_ft);

    
}

template<bool is_next>
LongLong ApplyPermutation(Node *&root_node) {
    bool is_ordered_tree = is_next ? root_node->is_reversed_tree : root_node->is_direct_tree;

    if (is_ordered_tree) {
        root_node->rev = true;
        return -1;
    }

    LongLong length = is_next ? LenOfSuffix<false>(root_node) : LenOfSuffix<true>(root_node);
    Node *left_part;
    Node *broken_node;
    Node *right_part;

    Nodes first_split = Split(root_node, length - 1);
    left_part = first_split.first;
    right_part = first_split.second;

    Nodes second_split = Split(right_part, 1);
    broken_node = second_split.first;
    right_part = second_split.second;

    if (right_part != nullptr) {
        right_part->rev |= is_next;
    }

    Node *suffix_left;
    Node *suffix_right;
    Nodes third_split = Split<true>(right_part, broken_node->val - (1 - int(is_next)));
    suffix_left = third_split.first;
    suffix_right = third_split.second;

    LongLong suffix_left_size = (suffix_left == nullptr ? 0 : suffix_left->Size());
    Nodes final_split;
    if (is_next) {
        final_split = Split(suffix_right, 1);
    } else {
        final_split = Split(suffix_left, suffix_left_size - 1);
    }
    Node *near_node = is_next ? final_split.first : final_split.second;

    if (is_next) {
        suffix_right = final_split.second;
        root_node = Merge(Merge(Merge(Merge(left_part, near_node), suffix_left), broken_node), suffix_right);
    } else {
        suffix_left = final_split.first;
        if (suffix_right != nullptr) {
            suffix_right->rev = true;
        }
        if (suffix_left != nullptr) {
            suffix_left->rev = true;
        }

        root_node = Merge(Merge(Merge(Merge(left_part, near_node), suffix_right), broken_node), suffix_left);
    }
    return -1;
}

void InOrder(Node *current_node, std::vector<LongLong> &array) {
    if (current_node == nullptr) {
        return;
    }
    current_node->Push();
    InOrder(current_node->le_ft, array);
    array.push_back(current_node->val);
    InOrder(current_node->rai_t, array);
}

public:
    ImplicitTreap() : root_(nullptr) {}

    ~ImplicitTreap() {
        delete root_;
    }

LongLong SumOn(LongLong left_index, LongLong right_index) {
    return Operate([](Node *&current_node) {
        if (current_node == nullptr) {
            return 0LL;
        } else {
            return current_node->SumTree();
        }
    }, left_index, right_index);
}

void AssignOn(LongLong assignment, LongLong left_index, LongLong right_index) {
    Operate([assignment](Node *&current_node) {
        if (current_node != nullptr) {
            current_node->Assign(assignment);
        }
        return -1LL;
    }, left_index, right_index);
}

void IncreaseOn(LongLong delta, LongLong left_index, LongLong right_index) {
    Operate([delta](Node *&current_node) {
        if (current_node != nullptr) {
            current_node->add += delta;
            current_node->AddRecalc(delta);
        }
        return -1LL;
    }, left_index, right_index);
}


 void RemoveElement(LongLong position) {
    Operate([](Node *&current_node) {
        delete current_node;
        current_node = nullptr;
        return -1LL;
    }, position, position);
}

void InsertElement(LongLong position, LongLong new_value) {
    Node *leftPart;
    Node *rightPart;
    Nodes splitted = Split(root_, position);
    leftPart = splitted.first;
    rightPart = splitted.second;
    root_ = Merge(Merge(leftPart, new Node(new_value)), rightPart);
}

void NextPermutation(LongLong left_index, LongLong right_index) {
    Operate([this](Node *&current_node) {
        if (current_node != nullptr) {
            ImplicitTreap::ApplyPermutation<true>(current_node);
        }
        return -1LL;
    }, left_index, right_index);
}

void PrevPermutation(LongLong left_index, LongLong right_index) {
    Operate([this](Node *&current_node) {
        if (current_node != nullptr) {
            ImplicitTreap::ApplyPermutation<false>(current_node);
        }
        return -1LL;
    }, left_index, right_index);
}


    std::vector<LongLong> Extract() {
        std::vector<LongLong> array;
        InOrder(root_, array);
        return array;
    }
};

std::mt19937 ImplicitTreap::Node::gen = std::mt19937(unsigned(time(nullptr)));
std::uniform_int_distribution<LongLong> ImplicitTreap::Node::uniform_distribution = 
    std::uniform_int_distribution<LongLong>(-UINT32_MAX, UINT32_MAX);

enum query_types { sum = 1, ins = 2, rmv = 3, assign = 4, increase = 5, next_perm = 6, prevperm = 7 };

class IQuery {
public:
    IQuery() = default;
    virtual ~IQuery() = default;
    virtual void Handle(ImplicitTreap *tree) = 0;
    query_types GetType() {
        return type_;
    }
protected:
    query_types type_;
};

class SumQuery : public IQuery {
private:
    LongLong le_ft;
    LongLong rai_t;
public:
    static std::vector<LongLong> sums;
    SumQuery() : le_ft(0), rai_t(0) {
        type_ = sum;
    }
    explicit SumQuery(LongLong l, LongLong r) : le_ft(l), rai_t(r) {
        type_ = sum;
    }
    void Handle(ImplicitTreap *t) override {
        sums.push_back(t->SumOn(le_ft, rai_t));
    }
};
std::vector<LongLong> SumQuery::sums;

void PrintSums(std::ostream &out_stream = std::cout) {
    for (auto &sum_val : SumQuery::sums) {
        out_stream << sum_val << "\n";
    }
}

class InsertQuery : public IQuery {
private:
    LongLong pos_;
    LongLong value_;
public:
    InsertQuery() : pos_(0), value_(0) {
        type_ = ins;
    }
    explicit InsertQuery(LongLong pos, LongLong value) : pos_(pos), value_(value) {
        type_ = ins;
    }
    void Handle(ImplicitTreap *t) override {
        t->InsertElement(pos_, value_);
    }
};

class RemoveQuery : public IQuery {
private:
    LongLong pos_;
public:
    RemoveQuery() : pos_(0) {
        type_ = rmv;
    }
    explicit RemoveQuery(LongLong pos) : pos_(pos) {
        type_ = rmv;
    }
    void Handle(ImplicitTreap *t) override {
        t->RemoveElement(pos_);
    }
};

class AssignmentQuery : public IQuery {
private:
    LongLong le_ft;
    LongLong rai_t;
    LongLong value_;
public:
    AssignmentQuery() : le_ft(0), rai_t(0), value_(0) {
        type_ = assign;
    }
    explicit AssignmentQuery(LongLong value, LongLong l, LongLong r) : value_(value), le_ft(l), rai_t(r) {
        type_ = assign;
    }
    void Handle(ImplicitTreap *t) override {
        t->AssignOn(value_, le_ft, rai_t);
    }
};

class IncreaseQuery : public IQuery {
private:
    LongLong le_ft;
    LongLong rai_t;
    LongLong delta_;
public:
    IncreaseQuery() : le_ft(0), rai_t(0), delta_(0) {
        type_ = increase;
    }
    explicit IncreaseQuery(LongLong value, LongLong l, LongLong r) : delta_(value), le_ft(l), rai_t(r) {
        type_ = increase;
    }
    void Handle(ImplicitTreap *t) override {
        t->IncreaseOn(delta_, le_ft, rai_t);
    }
};

class PermutationQuery : public IQuery {
private:
    LongLong le_ft;
    LongLong rai_t;
    bool is_next_;
public:
    PermutationQuery() : le_ft(0), rai_t(0), is_next_(true) {
        type_ = next_perm;
    }
    explicit PermutationQuery(LongLong l, LongLong r, bool is_next) : le_ft(l), rai_t(r), is_next_(is_next) {
        type_ = (is_next ? next_perm : prevperm);
    }
    void Handle(ImplicitTreap *t) override {
        if (is_next_) {
            t->NextPermutation(le_ft, rai_t);
        } else {
            t->PrevPermutation(le_ft, rai_t);
        }
    }
};

std::vector<LongLong> ReadData(std::istream &in_stream = std::cin) {
    std::vector<LongLong> array;
    LongLong n;
    in_stream >> n;
    for (int i = 0; i < n; i++) {
        LongLong value;
        in_stream >> value;
        array.push_back(value);
    }
    return array;
}

ImplicitTreap* ConstructTree(std::vector<LongLong> &array) {
    auto tree = new ImplicitTreap();
    for (int i = 0; i < (int)array.size(); i++) {
        tree->InsertElement(i, array[i]);
    }
    array.clear();
    return tree;
}

void PrintTreap(std::vector<LongLong> extracted_tree, std::ostream &out_stream = std::cout) {
    for (auto &i : extracted_tree) {
        out_stream << i << " ";
    }
    out_stream << std::endl;
}

std::vector<IQuery*> ReadQueries(std::istream &in_stream = std::cin) {
    auto *query_manager = new std::vector<IQuery*>();
    LongLong q, l, r, pos, x;
    short mode;
    in_stream >> q;
    query_manager->reserve(q);
    for (LongLong i = 0; i < q; i++) {
        in_stream >> mode;
        if (mode == sum) {
            in_stream >> l >> r;
            {
                query_manager->push_back(new SumQuery(l, r));
            }
        } else if (mode == ins) {
            in_stream >> x >> pos;
            {
                query_manager->push_back(new InsertQuery(pos, x));
            }
        } else if (mode == rmv) {
            in_stream >> pos;
            {
                query_manager->push_back(new RemoveQuery(pos));
            }
        } else if (mode == assign) {
            in_stream >> x >> l >> r;
            {
                query_manager->push_back(new AssignmentQuery(x, l, r));
            }
        } else if (mode == increase) {
            in_stream >> x >> l >> r;
            {
                query_manager->push_back(new IncreaseQuery(x, l, r));
            }
        } else if (mode == next_perm) {
            in_stream >> l >> r;
            {
                query_manager->push_back(new PermutationQuery(l, r, true));
            }
        } else if (mode == prevperm) {
            in_stream >> l >> r;
            {
                query_manager->push_back(new PermutationQuery(l, r, false));
            }
        }
    }
    return *query_manager;
}

void OperateQueries(std::vector<IQuery *> query_manager, ImplicitTreap *tree) {
    for (auto &query : query_manager) {
        query->Handle(tree);
    }
}

std::pair<std::vector<LongLong>, std::vector<IQuery*>> Input(std::istream &in_stream = std::cin) {
    auto data = ReadData(in_stream);
    auto queries = ReadQueries(in_stream);
    return {data, queries};
}

class InputData {
private:
    std::vector<LongLong> elems_;
    std::vector<IQuery*> query_;
public:
    std::vector<LongLong> GetElems() {
        return elems_;
    }
    std::vector<IQuery*> GetQuery() {
        return query_;
    }
    explicit InputData(std::vector<LongLong> a, std::vector<IQuery*> b) : elems_(std::move(a)), query_(std::move(b)) {}
    explicit InputData(std::pair<std::vector<LongLong>, std::vector<IQuery*>> p) :
            elems_(p.first), query_(p.second) {}

    InputData(const InputData &other) : elems_(other.elems_) {
        query_.clear();
        for (int i = 0; i < (int)other.query_.size(); i++) {
            switch (other.query_[i]->GetType()) {
                case sum:{
                    {
                        SumQuery *new_node = dynamic_cast<SumQuery*>(other.query_[i]);
                        query_.push_back(new_node);
                    }
                    break;
                }
                case rmv: {
                    {
                        RemoveQuery *new_node = dynamic_cast<RemoveQuery*>(other.query_[i]);
                        query_.push_back(new_node);
                    }
                    break;
                }
                case ins: {
                    {
                        InsertQuery *new_node = dynamic_cast<InsertQuery*>(other.query_[i]);
                        query_.push_back(new_node);
                    }
                    break;
                }
                case assign: {
                    {
                        AssignmentQuery *new_node = dynamic_cast<AssignmentQuery*>(other.query_[i]);
                        query_.push_back(new_node);
                    }
                    break;
                }
                case increase: {
                    {
                        IncreaseQuery *new_node = dynamic_cast<IncreaseQuery*>(other.query_[i]);
                        query_.push_back(new_node);
                    }
                    break;
                }
                default: {
                    {
                        PermutationQuery *new_node = dynamic_cast<PermutationQuery*>(other.query_[i]);
                        query_.push_back(new_node);
                    }
                    break;
                }
            }
        }
    }
    InputData& operator=(const InputData &other) {
        *this = InputData(other);
        return *this;
    }
};

class OutputData {
private:
    std::vector<LongLong> sums_;
    std::vector<LongLong> extracted_tree_;
public:
    std::vector<LongLong> GetSums() {
        return sums_;
    }
    std::vector<LongLong> GetExtractedTree() {
        return extracted_tree_;
    }

    explicit OutputData(std::vector<LongLong> a, std::vector<LongLong> b) : sums_(std::move(a)), extracted_tree_(std::move(b)) {}
    explicit OutputData(std::pair<std::vector<LongLong>, std::vector<LongLong>> p) :
            sums_(p.first), extracted_tree_(p.second) {}

    OutputData(const OutputData &other) : sums_(other.sums_), extracted_tree_(other.extracted_tree_) {}
    OutputData& operator=(const OutputData &other) {
        *this = OutputData(other);
        return *this;
    }
};

OutputData ProcessTree(InputData inp_data) {
    auto data_forai_ttree = inp_data.GetElems();
    auto tree = ConstructTree(data_forai_ttree);
    auto query_manager = inp_data.GetQuery();
    OperateQueries(query_manager, tree);
    auto extracted_tree = tree->Extract();

    return OutputData(SumQuery::sums, extracted_tree);
}

void Output(OutputData out, std::ostream &out_stream = std::cout) {
    PrintSums(out_stream);
    PrintTreap(std::move(out.GetExtractedTree()), out_stream);
}

void Solve(std::istream &in_stream = std::cin, std::ostream &out_stream = std::cout) {
    InputData input_data_obj(Input(in_stream));
    OutputData output_data_obj = ProcessTree(input_data_obj);
    Output(output_data_obj, out_stream);
}

int main() {
    Solve();
    return 0;
}
