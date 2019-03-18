#ifndef AISDI_MAPS_TREEMAP_H
#define AISDI_MAPS_TREEMAP_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>

namespace aisdi
{

template <typename KeyType, typename ValueType>
class TreeMap
{
public:
  using key_type = KeyType;
  using mapped_type = ValueType;
  using value_type = std::pair<const key_type, mapped_type>;
  using size_type = std::size_t;
  using reference = value_type&;
  using const_reference = const value_type&;

  class ConstIterator;
  class Iterator;
  using iterator = Iterator;
  using const_iterator = ConstIterator;
private:
  struct Node {

    Node *parent = nullptr;
    Node *left = nullptr;
    Node *right = nullptr;
    value_type data;
    int height = 0;

    Node (value_type data) : data(data) {}

    //Node (Node &other): parent(other.parent), left(other.left), right(other.right) {}

      };

  Node *Root = nullptr;
  size_type Size = 0;

  void erase_subTree (Node *node_)
  {
    if (node_ == nullptr)
        return;
    erase_subTree (node_->left);
    erase_subTree (node_->right);
    delete node_;
    return;
  }

  void set_all (Node *current)
  {
    if (current == nullptr)
        return;
    if (current->parent == nullptr)
        current->height = 0;
    else
        current->height = current->parent->height+1;
    set_all(current->left);
    set_all(current->right);
  }

  void swapNodes(Node *first, Node *second)
  {
    Node *temp;
    temp = first;
    if (second->parent && second->parent != first)
    {
        if (second->parent->left == second)
            second->parent->left = first;
        else
            second->parent->right = first;
    }
    first = second;
    if (second->parent && second->parent != first)
    {
        if (second->parent->left == second)
            second->parent->left = first;
        else
            second->parent->right = first;
    }
    second = temp;
    first->height = second->height;
    second->height = temp->height;
  }

  int get_max_height(Node *current, int MaxHeight)
  {
    return get_max_height_ref(current, MaxHeight);
  }

  int get_max_height_ref(Node *current, int &MaxHeight)
  {
    if (current == nullptr)
        return MaxHeight;
    if (current->left != nullptr)
        get_max_height_ref(current->left, MaxHeight);
    if (current->right != nullptr)
        get_max_height_ref(current->right, MaxHeight);
    if (MaxHeight <= (current->height))
        MaxHeight = (current->height);
    return MaxHeight;
  }


  Node *remove_node(key_type key, Node *current)
  {
    if (current == nullptr)
        return nullptr;
                                                                    //finding node to erase
    else if (key < current->data.first)
        current->left = remove_node(key, current->left);
    else if (key > current->data.first)
        current->right = remove_node(key, current->right);
                                                                    //found node to erase
    else if (current->right && current->left)                       //2 childs
    {
        delete_when_two(key, current);
    }
    else if (current->right && current->left==nullptr)              //only right child
    {
        Size--;
        delete_when_right_only(current);
    }
    else if (current->left && current->right==nullptr)              //only left child
    {
        Size--;
        delete_when_left_only(current);
    }
    else
    {
        Size--;
        delete current;
        return nullptr;
    }
    set_all(current);
    //check_left_rotate(current, key);
    //check_right_rotate(current, key);

    return current;
  }

  void delete_when_two(key_type key, Node *current)
  {
    Node *temp = current;
    temp = temp->right;
    while (temp->left != nullptr)
        temp = temp->left;
    swapNodes(temp, current);
    remove_node(key, current->right);
  }

  void delete_when_right_only(Node *current)
  {
    Node *temp = current;
    if (current->parent)
    {
        current->right->parent = current->parent;
        if (current->parent->right == current)
            current->parent->right = current->right;
        else
            current->parent->left = current->right;
    }
    else
    {
        current->right->parent = current->parent;
    }
    current = current->right;
    delete temp;
  }

  void delete_when_left_only(Node *current)
  {
    Node *temp = current;
    if (current->parent)
    {
        current->left->parent = current->parent;
        if (current->parent->right == current)
            current->parent->right = current->left;
        else
            current->parent->left = current->left;
    }
    else
    {
        current->left->parent = current->parent;
    }
    current = current->left;
    delete temp;
  }

  void check_left_rotate (Node *current, key_type key)
  {
    if (current == nullptr)
    {
        return;
    }
    int max_left;
    int max_right;

    if (current->left != nullptr) max_left = get_max_height(current->left, 0);
    else max_left = 0;

    if (current->right != nullptr) max_right = get_max_height(current->right, 0);
    else max_right = 0;

    if ((max_left - max_right) >= 2)
    {
        if (current->left->left)
            current = LeftLeft(current, Root);
        else
            current = LeftRight(current, Root);
    }
    if (key == 17809) throw std::out_of_range("DUpa");
  }

  void check_right_rotate (Node *current, key_type key)
  {
    if (current == nullptr)
    {
        return;
    }

        int max_left;
        int max_right;

        if (current->left !=nullptr) max_left = get_max_height(current->left, 0);
        else max_left = 0;

        if (current->right != nullptr) max_right = get_max_height(current->right, 0);
        else max_right = 0;

        if ((max_right - max_left) >= 2)
        {
            if (current->right->right)
                current = RightRight(current, Root);
            else
                current = RightLeft(current, Root);
        }
        if (key == 17689) throw std::out_of_range("DUpa");
  }

  Node *LeftLeft (Node *current, Node * &root)
  {
    Node *temp = current->left;
    Node *Par = current->parent;
    current->left = temp->right;
    if(current->left) current->left->parent = current;
    temp->right = current;
    temp->parent = Par;
    current->parent = temp;
    if (Par)
    {
        if(Par->left == current) Par->left = temp; else Par->right = temp;
    }
    else root = temp;
    set_all(root);
    return temp;
  }

  Node *RightLeft (Node *current, Node *&root)
  {
    Node *temp = current->right;
    Node *temp2 =temp->left;
    Node *Par = current->parent;

    temp->left = temp2->right;
    if (temp->left) temp->left->parent = temp;
    current->right = temp2->left;
    if (current->right) current->right->parent = current;
    temp2->left = current;
    temp2->right = temp;
    current->parent = temp->parent = temp2;
    temp2->parent = Par;
    if (Par)
    {
        if (Par->left == current) Par->left = temp2; else Par->right = temp2;
    }
    else root = temp2;
    set_all(root);
    return temp2;
  }

  Node *RightRight (Node *current, Node *&root)
  {
    Node *temp = current->right;
    Node *Par = current->parent;

    current->right = temp->left;
    if (current->right) current->right->parent = current;
    temp->left = current;
    temp->parent = Par;
    current->parent = temp;
    if (Par)
    {
        if (Par->left == current) Par->left = temp; else Par->right = temp;
    }
    else root = temp;
    set_all(root);
    return temp;
  }

  Node *LeftRight (Node *current, Node *&root)
  {
    Node *temp = current->left;
    Node *temp2 = temp->right;
    Node *Par = current->parent;

    temp->right = temp2->left;
    if (temp->right) temp->right->parent = temp;
    current->left = temp2->right;
    if (current->left) current->left->parent = current;
    temp2->right = current;
    temp2->left = temp;
    current->parent = temp->parent = temp2;
    temp2->parent = Par;
    if (Par)
    {
        if (Par->left == current) Par->left = temp2; else Par->right = temp2;
    }
    else root = temp2;
    set_all(root);
    return temp2;
  }

  Node *insert(value_type Value, Node *current, Node *currentParent)
  {
    key_type key = Value.first;
    if (current == nullptr)
    {
        current = new Node(Value);

        if (Root == nullptr)
            current->height = 1;
        else
        {
            current->height = currentParent->height + 1;
        }
        current->parent = currentParent;
        Size++;
    }
    else
    {
        if (current->data.first > key)
        {
            current->left = insert(Value, current->left, current);
            //check_left_rotate(current, key);
            /*if ((get_max_height(current->left, 0) - get_max_height(current->right, 0)) == 2)
            {
                if((get_max_height(current->left->left, 0) - get_max_height(current->left->right, 0)) > 0)
                    current = LeftLeft(current, Root);
                else
                    current = LeftRight(current, Root);
            }*/
        }
        else
        {
            current->right = insert(Value, current->right, current);

            //check_right_rotate(current, key);
            /*if ((get_max_height(current->right, 0) - get_max_height(current->left, 0)) == 2)
            {
                if((get_max_height(current->right->left, 0) - get_max_height(current->right->right, 0)) > 0)
                    current = RightRight(current, Root);
                else
                    current = RightLeft(current, Root);
            }*/
        }
    }
    return current;
  }

public:

  TreeMap()
  {}

  TreeMap(std::initializer_list<value_type> list)
  {
    for (auto it = list.begin(); it != list.end(); it++)
        (*this)[it->first] = it->second;
  }

  TreeMap(const TreeMap& other)
  {
    *this = other;
  }

  TreeMap(TreeMap&& other)
  {
    Root = other.Root;
    other.Root = nullptr;
    Size = other.Size;
    other.Size = 0;
  }

  ~TreeMap()
  {
    erase_subTree (Root);
  }

  TreeMap& operator=(const TreeMap& other)
  {
    if (*this != other){
        erase_subTree (Root);
        Root = nullptr;
        Size = 0;

        for (auto it = other.begin(); it != other.end(); it++)
            (*this)[it->first] = it->second;
    }
    return *this;
  }

  TreeMap& operator=(TreeMap&& other)
  {
    if (this != &other){
        erase_subTree (Root);
        Root = other.Root;
        Size = other.Size;
        other.Root = nullptr;
        other.Size = 0;
    }
    return *this;
  }

  bool isEmpty() const
  {
    if (Size == 0)
        return true;
    return false;
  }

  mapped_type& operator[](const key_type& key)
  {
    auto to_find = find(key);
    if (to_find != end()){
        return to_find->second;
    } else {
        //Root = insert ({key, ValueType{}}, Root, nullptr);

        Node* new_element = new Node({key, ValueType{}});
        if (isEmpty())
            Root = new_element;
        else
        {
            Node *temp = Root;
            Node *par = temp->parent;

            while (temp != nullptr)
            {
                par = temp;
                if (key < temp->data.first)
                {
                    temp = temp->left;

                }
                else if (key > temp->data.first)
                {
                    temp = temp->right;
                }
                else
                    break;
            }


            //if (key == 123) throw std::out_of_range("XD");
            temp = new_element;
            new_element->parent = par;

            if (new_element->parent)
            {
                if (key >par->data.first)
                {
                    new_element->parent->right = new_element;
                }
                else
                {
                    new_element->parent->left = new_element;
                }
            }
            set_all(Root);

            Node *tmp = new_element;

            while (tmp != nullptr)
            {
                int max_left = 0;
                int max_right = 0;
                int actual_height = 0;

                if (tmp->left != nullptr) max_left = get_max_height(tmp->left, 1);
                if (tmp->right != nullptr) max_right = get_max_height(tmp->right, 1);
                if (tmp != nullptr) actual_height = tmp->height;
                if (max_left != 0 && max_right != 0) actual_height = 0;
                if ((max_left - max_right - actual_height) == 2)
                {
                    if((get_max_height(tmp->left->left, 1) - get_max_height(tmp->left->right, 1)) > 0)
                        tmp = LeftLeft(tmp, Root);
                    else
                        tmp = LeftRight(tmp, Root);
                }
                else if ((max_right - max_left - actual_height) == 2)
                {
                    if((get_max_height(tmp->right->left, 1) - get_max_height(tmp->right->right, 1)) > 0)
                        tmp = RightLeft(tmp, Root);
                    else
                        tmp = RightRight(tmp, Root);
                }
                tmp = tmp->parent;
            }

        }

        Size++;
        to_find = find(key);
        return to_find->second;
    }
  }

  const mapped_type& valueOf(const key_type& key) const
  {
    auto to_find = find(key);
        if (to_find != end()){
            return to_find->second;
        }
        else
            throw std::out_of_range("Key not found");
  }

  mapped_type& valueOf(const key_type& key)
  {
    auto to_find = find(key);
        if (to_find != end()){
            return to_find->second;
        }
        else
            throw std::out_of_range("Key not found");
  }

  const_iterator find(const key_type& key) const
  {
    if (Root == nullptr)
        return cend();
    Node *temp = Root;
    while (temp != nullptr)
        {
            if(key > temp->data.first)
                temp = temp->right;
            else if (key < temp->data.first)
                temp = temp->left;
            else
            {
                return ConstIterator(temp, Root);
            }
        }
    return cend();
  }

  const_iterator get_root()
  {
    return ConstIterator(Root, Root);
  }

  iterator find(const key_type& key)
  {
    return (static_cast<const TreeMap * >(this)->find(key));
  }

  void remove(const key_type& key)
  {
    if (find(key) != end())
        Root = remove_node(key, Root);
    else
        throw std::out_of_range("Can't remove missing element");
    return;
  }

  void remove(const const_iterator& it)
  {
    remove(it->first);
  }

  size_type getSize() const
  {
    return Size;
  }

  bool operator==(const TreeMap& other) const
  {
    if (other.Size != Size)
        return 0;
    for (auto i = other.begin(); i != other.end(); i++)
    {
        if (find(i->first) == other.end() || find(i->first)->second != i->second)
            return 0;
    }
    return 1;
  }

  bool operator!=(const TreeMap& other) const
  {
    return !(*this == other);
  }

  iterator begin()
  {
    return Iterator(cbegin());
  }

  iterator end()
  {
    return Iterator(cend());
  }

  const_iterator cbegin() const
  {
    if (Size == 0)
        return cend();
    Node *temp = Root;
    while(temp->left != nullptr)
        temp = temp->left;
    return ConstIterator(temp, Root);
  }

  const_iterator cend() const
  {
    return ConstIterator(nullptr, Root);
  }

  const_iterator begin() const
  {
    return cbegin();
  }

  const_iterator end() const
  {
    return cend();
  }

  int getHeight (const key_type& key)
  {
    auto it = find(key);
    return it.actual->height;
  }

  int get_max()
  {
    return get_max_height(Root, 0);
  }

  void LL_rotate()
  {
    Root = LeftLeft(Root, Root);
  }

  void LR_rotate()
  {
    Root = LeftRight(Root, Root);
  }

  void RR_rotate()
  {
    Root = RightRight(Root, Root);
  }

  void RL_rotate()
  {
    Root = RightLeft(Root, Root);
  }

  /*void get_root_value()
  {
      if (Root != nullptr)
        std::cout << Root->data.second << std::endl;
  }*/

};

template <typename KeyType, typename ValueType>
class TreeMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename TreeMap::const_reference;
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename TreeMap::value_type;
  using pointer = const typename TreeMap::value_type*;

  friend class TreeMap;
private:
  Node *actual;
  Node *root;

public:

  explicit ConstIterator(Node *actual_, Node *root_)
  {
    actual = actual_;
    root = root_;
  }

  ConstIterator(const ConstIterator& other): actual(other.actual), root(other.root)
  {

  }

  ConstIterator& operator++()
  {

    if (root == nullptr)
        throw std::out_of_range("Incrementing empty root");
    if (actual == nullptr)
        throw std::out_of_range("Incrementing empty actual");

    if (actual->right != nullptr)
    {
        actual = actual->right;
        while (actual->left != nullptr)
            actual = actual->left;
    } else {
        while (actual->parent != nullptr && actual->parent->right == actual){
            actual = actual->parent;
        }
        actual = actual->parent;
    }
    return *this;
  }

  ConstIterator operator++(int)
  {
    auto ret = *this;
    ++*this;
    return ret;
  }

  ConstIterator& operator--()
  {
    if (root == nullptr)
        throw std::out_of_range("Decrementing empty root");
    if (actual == nullptr)
    {
        actual = root;
        while (actual->right != nullptr)
            actual = actual->right;
        return *this;
    }
    if (actual->left != nullptr)
    {
        actual = actual->left;

        while (actual->right != nullptr)
            actual = actual->right;
        return *this;
    }
    while (actual != nullptr)
    {
        if (actual->parent != nullptr && actual->parent->right == actual && actual->parent->left != nullptr)
        {
            actual = actual->parent->left;
            return *this;
        }
        actual = actual->parent;
    }
    throw std::out_of_range("Decrementing begin");

  }

  ConstIterator operator--(int)
  {
    auto ret = *this;
    --*this;
    return ret;
  }

  reference operator*() const
  {
    if (actual == nullptr)
        throw std::out_of_range("Dereferencing empty");
    return actual->data;
  }

  pointer operator->() const
  {
    return &this->operator*();
  }

  bool operator==(const ConstIterator& other) const
  {
    if (actual == other.actual)
        return true;
    else
        return false;
  }

  bool operator!=(const ConstIterator& other) const
  {
    return !(*this == other);
  }
};

template <typename KeyType, typename ValueType>
class TreeMap<KeyType, ValueType>::Iterator : public TreeMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename TreeMap::reference;
  using pointer = typename TreeMap::value_type*;

  explicit Iterator()
  {}

  Iterator(const ConstIterator& other)
    : ConstIterator(other)
  {}

  Iterator& operator++()
  {
    ConstIterator::operator++();
    return *this;
  }

  Iterator operator++(int)
  {
    auto result = *this;
    ConstIterator::operator++();
    return result;
  }

  Iterator& operator--()
  {
    ConstIterator::operator--();
    return *this;
  }

  Iterator operator--(int)
  {
    auto result = *this;
    ConstIterator::operator--();
    return result;
  }

  pointer operator->() const
  {
    return &this->operator*();
  }

  reference operator*() const
  {
    // ugly cast, yet reduces code duplication.
    return const_cast<reference>(ConstIterator::operator*());
  }
};

}
#endif /* AISDI_MAPS_MAP_H */
