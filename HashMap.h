#ifndef AISDI_MAPS_HASHMAP_H
#define AISDI_MAPS_HASHMAP_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>

#include <vector>
#include <list>
#include <functional>


namespace aisdi
{

template <typename KeyType, typename ValueType>
class HashMap
{
public:
  using key_type = KeyType;
  using mapped_type = ValueType;
  using value_type = std::pair<const key_type, mapped_type>;
  using size_type = std::size_t;
  using reference = value_type&;
  using const_reference = const value_type&;

  using list_iterator = typename std::list<value_type*>::const_iterator;
  using VectorofLists = typename std::vector<std::list<value_type*>>;
  friend class ConstIterator;

  class ConstIterator;
  class Iterator;
  using iterator = Iterator;
  using const_iterator = ConstIterator;

private:

  size_type capacity = 1000;
  size_type current_size = 0;
  VectorofLists hashTable;


  void Check_Memory()
  {
    float part_of_Use =(current_size)/capacity;
    if (part_of_Use >= 0.75)
    {
        std::list<value_type*> List;
        for (size_type i = 0; i < capacity; i++)
        {
            hashTable.push_back(List);
        }
        capacity*=2;
        return;
    }
    else
        return ;
  }


public:
  HashMap()
  {
    hashTable.reserve(capacity);
    std::list<value_type*> List;
    for (size_type i = 0; i < capacity; i++)
    {
        hashTable.push_back(List);
    }
  }

  ~HashMap()
  {
    hashTable.clear();
  }

  HashMap(size_type capacity_) :capacity(capacity_)
  {}

  HashMap(std::initializer_list<value_type> list):HashMap()
  {
    for (auto it = list.begin(); it != list.end(); ++it) {
            //(*this)[it->first] = it->second;
            key_type index = it->first;
            mapped_type value = it->second;
            (*this)[index] = value;
    }
  }

  HashMap(const HashMap& other): HashMap()
  {
    for (auto it = other.begin(); it != other.end(); ++it)
            (*this)[it->first] = it->second;
  }

  HashMap(HashMap&& other): HashMap()
  {
    swap(*this, other);
    other.hashTable.clear();
    other.current_size = 0;
  }

  HashMap& operator=(const HashMap& other)
  {
    if (this == &other)
        return *this;
    for (size_type i = 0; i < capacity; i++)
    {
        hashTable[i].erase(hashTable[i].begin(),hashTable[i].end());
    }

    current_size = other.current_size;

    for (size_type i = 0; i < other.capacity; i++)
    {
        hashTable[i] = other.hashTable[i];
    }

    return *this;
  }


  void swap(HashMap& first, HashMap& second)
  {
    using std::swap;
    swap(first.hashTable, second.hashTable);
    swap(first.current_size, second.current_size);
  }

  HashMap& operator=(HashMap&& other)
  {
    if(&other == this)
      return *this;
    swap(*this, other);
    other.hashTable.clear();
    return *this;
  }

  bool isEmpty() const
  {
    if (current_size == 0)
        return true;
    return false;
  }

  size_type GetHash(const key_type &key) const
  {
    return std::hash<key_type>{}(key)%capacity;
  }

  mapped_type& operator[](const key_type& key)
  {
    auto to_find = find(key);
    if (to_find != end())
        return to_find->second;
    else
    {
        Check_Memory();
        current_size++;
        size_type Hash = GetHash(key);
        value_type* Value = new value_type(key, ValueType{});
        hashTable[Hash].push_back(Value);
        to_find = find(key);
        return to_find->second;
    }
  }

  const mapped_type& valueOf(const key_type& key) const
  {
    auto to_find = find(key);
    if (to_find != end())
    {
        return to_find->second;
    }
    else
        throw std::out_of_range("NOPE!");
  }

  mapped_type& valueOf(const key_type& key)
  {
    return const_cast<mapped_type & >((static_cast<const HashMap *>(this))->valueOf(key));
  }

  const_iterator find(const key_type& key) const
  {
    size_type Hash = GetHash(key);
    auto const &List_toFind = hashTable[Hash];
    auto iter = ConstIterator(this, Hash, List_toFind.begin());
    for (auto it = List_toFind.begin(); it != List_toFind.end(); it++)
    {
        if (iter->first == key)
        {
            return ConstIterator(this, Hash, it);
        }
        iter++;
    }
    return end();
  }

  iterator find(const key_type& key)
  {
    return (static_cast<const HashMap * >(this)->find(key));
  }

  void remove(const key_type& key)
  {
    auto to_find = find(key);
    if (to_find != end())
        return remove(to_find);
    else
        throw std::out_of_range("Cant remove nonexisting element");
  }

  void remove(const const_iterator& it)
  {
    if (it != end())
    {
        auto index_ = it.index;
        auto List_to_remove = hashTable[index_];
        List_to_remove.erase(it.actual_listNode);
        current_size--;
    }
    else
        throw std::out_of_range("nie wiem co napisac");
  }

  size_type getSize() const
  {
    return current_size;
  }

  bool operator==(const HashMap& other) const
  {
    if (current_size == other.current_size)
    {


        for (const auto &item : other) {
            const auto it = find(item.first);
            if (it == end())
               return false;
            if (it->second != item.second)
               return false;
         }

        return true;
    }
    else
        return false;
  }

  bool operator!=(const HashMap& other) const
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
    if (current_size == 0)
        return cend();
    for (size_type i = 0; i < capacity; i++)
    {
        if (!hashTable[i].empty())
            return ConstIterator(this, i, hashTable[i].begin());
    }
    throw std::out_of_range("O kurcze pusta lista!");
  }

  const_iterator cend() const
  {
    return ConstIterator(this, capacity-1, hashTable[capacity-1].end());
  }

  const_iterator begin() const
  {
    return cbegin();
  }

  const_iterator end() const
  {
    return cend();
  }
};

template <typename KeyType, typename ValueType>
class HashMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename HashMap::const_reference;
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename HashMap::value_type;
  using pointer = const typename HashMap::value_type*;

  using size_type = typename HashMap<KeyType, ValueType>::size_type;
  using list_iterator = typename HashMap<KeyType, ValueType>::list_iterator;

  friend class HashMap;

private:
  const HashMap *map;
  list_iterator actual_listNode;
  size_type index;

public:
  explicit ConstIterator(const HashMap *map_, size_type index_, list_iterator actual_listNode_)
  {
    map = map_;
    index = index_;
    actual_listNode = actual_listNode_;
  }

  ConstIterator(const ConstIterator& other)
  {
    map = other.map;
    index = other.index;
    actual_listNode = other.actual_listNode;
  }

  ConstIterator& operator++()
  {
    if (*this == map->end())
        throw std::out_of_range("Incrementing end");
    actual_listNode++;
    while (actual_listNode == map->hashTable[index].end())
    {
        index++;
        if (index == map->capacity)
        {
               *this = map->end();
               return *this;
        }

            actual_listNode = map->hashTable[index].begin();
    }
    return *this;
  }

  ConstIterator operator++(int)
  {
    auto result = *this;
    ++(*this);
    return result;
  }


  ConstIterator& operator--()
  {
    if (*this == map->begin())
    {
        throw std::out_of_range("Decrementing begin");
    }
    if (*this == map->end())
    {
        index = map->capacity - 1;
        actual_listNode = map->hashTable[index].end();
    }
    while (actual_listNode == map->hashTable[index].begin())
    {
        --index;
        actual_listNode = map->hashTable[index].end();
    }

    --actual_listNode;

    return *this;
  }

  ConstIterator operator--(int)
  {
    auto result = *this;
    --(*this);
    return result;
  }

  reference operator*() const
  {
    if (*this == map->end())
    {
        throw std::out_of_range("Dereferencing end");
    }
    return **actual_listNode;
  }

  pointer operator->() const
  {
    return &this->operator*();
  }

  bool operator==(const ConstIterator& other) const
  {
    return actual_listNode == other.actual_listNode;
  }

  bool operator!=(const ConstIterator& other) const
  {
    return !(*this == other);
  }
};

template <typename KeyType, typename ValueType>
class HashMap<KeyType, ValueType>::Iterator : public HashMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename HashMap::reference;
  using pointer = typename HashMap::value_type*;

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

#endif /* AISDI_MAPS_HASHMAP_H */
