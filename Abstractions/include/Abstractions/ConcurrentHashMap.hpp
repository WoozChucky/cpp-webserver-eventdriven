//
// Created by Nuno on 3/26/2021.
//

#ifndef CONCURRENTHASHMAP_HPP
#define CONCURRENTHASHMAP_HPP

#include <cstdint>
#include <iostream>
#include <functional>
#include <mutex>
#include <thread>
#include <shared_mutex>

template <typename K, typename V>
class HashNode
{
public:
    HashNode() : next(nullptr)
    {}
    HashNode(K key_, V value_) : next(nullptr), _key(key_), _value(value_)
    {}
    ~HashNode()
    {
        next = nullptr;
    }

    const K& GetKey() const {return _key;}
    void SetValue(V value_) { _value = value_;}
    const V& GetValue() const {return _value;}

    HashNode *next; //Pointer to the next node in the same bucket
private:
    K _key;
    V _value;
};

template <typename K, typename V>
class HashBucket
{
public:
    explicit HashBucket() : _head(nullptr)
    {}

    ~HashBucket() //delete the bucket
    {
        Clear();
    }

    bool Find(const K &key, V &value) const
    {
        // A shared mutex is used to enable multiple concurrent reads
        std::shared_lock<std::shared_timed_mutex> lock(_mutex);
        HashNode<K, V> * node = _head;

        while (node != nullptr)
        {
            if (node->GetKey() == key)
            {
                value = node->GetValue();
                return true;
            }
            node = node->next;
        }
        return false;
    }

    void Insert(const K &key, const V &value)
    {
        //Exclusive lock to enable single write in the bucket
        std::unique_lock<std::shared_timed_mutex> lock(_mutex);
        HashNode<K, V> * prev = nullptr;
        HashNode<K, V> * node = _head;

        while (node != nullptr && node->GetKey() != key)
        {
            prev = node;
            node = node->next;
        }

        if (nullptr == node) //New entry, create a node and add to bucket
        {
            if(nullptr == _head)
            {
                _head = new HashNode<K, V>(key, value);
            }
            else
            {
                prev->next = new HashNode<K, V>(key, value);
            }
        }
        else
        {
            node->SetValue(value); //Key found in bucket, update the _value
        }

    }

    void Erase(const K &key)
    {
        //Exclusive lock to enable single write in the bucket
        std::unique_lock<std::shared_timed_mutex> lock(_mutex);
        HashNode<K, V> *prev  = nullptr;
        HashNode<K, V> * node = _head;

        while (node != nullptr && node->GetKey() != key)
        {
            prev = node;
            node = node->next;
        }

        if (nullptr == node) //Key not found, nothing to be done
        {
            return;
        }
        else  //Remove the node from the bucket
        {
            if(_head == node)
            {
                _head = node->next;
            }
            else
            {
                prev->next = node->next;
            }
            delete node; //Free up the memory
        }
    }

    void Clear()
    {
        //Exclusive lock to enable single write in the bucket
        std::unique_lock<std::shared_timed_mutex> lock(_mutex);
        HashNode<K, V> * prev = nullptr;
        HashNode<K, V> * node = _head;
        while(node != nullptr)
        {
            prev = node;
            node = node->next;
            delete prev;
        }
        _head = nullptr;
    }

private:
    HashNode<K, V> * _head; //The _head node of the bucket
    mutable std::shared_timed_mutex _mutex; //The mutex for this bucket
};

constexpr size_t HASH_SIZE_DEFAULT = 1031; // A prime number as hash size gives a better distribution of values in buckets

template <typename K, typename V, typename F = std::hash<K> >
class ConcurrentHashMap
{
public:
    explicit ConcurrentHashMap(size_t hashSize_ = HASH_SIZE_DEFAULT) : _hashSize(hashSize_)
    {
        _hashTable = new HashBucket<K, V>[_hashSize]; //create the hash table as an array of hash buckets
    }

    ~ConcurrentHashMap()
    {
        delete [] _hashTable;
    }
    ConcurrentHashMap(const ConcurrentHashMap&) = delete;
    ConcurrentHashMap(ConcurrentHashMap&&) = delete;
    ConcurrentHashMap& operator=(const ConcurrentHashMap&) = delete;
    ConcurrentHashMap& operator=(ConcurrentHashMap&&) = delete;

    bool Find(const K &key, V &value) const
    {
        size_t hashValue = _hashFn(key) % _hashSize ;
        return _hashTable[hashValue].Find(key, value);
    }

    void Insert(const K &key, const V &value)
    {
        size_t hashValue = _hashFn(key) % _hashSize ;
        _hashTable[hashValue].Insert(key, value);
    }

    void Erase(const K &key)
    {
        size_t hashValue = _hashFn(key) % _hashSize ;
        _hashTable[hashValue].Erase(key);
    }

    void Clear()
    {
        for(size_t i = 0; i < _hashSize; i++)
        {
            (_hashTable[i]).Clear();
        }
    }

private:
    HashBucket<K, V> * _hashTable;
    F _hashFn;
    const size_t _hashSize;
};

#endif //CONCURRENTHASHMAP_HPP
