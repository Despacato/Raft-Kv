//跳表实现基于leetcode 1206. 设计跳表实现
#ifndef SKIPLIST_H
#define SKIPLIST_H

#include <iostream>
#include <random>     
#include <vector> 
using namespace std;
constexpr int MAX_LEVEL = 32;
constexpr int P_FACTOR = 0.25;

template <typename K, typename V>
class SkiplistNode {
public:

    SkiplistNode(K k,V v,int level):key(k),value(v),forward(level,nullptr){};
    ~SkiplistNode();

    K get_key() const;
    V get_value() const;

    std::vector<SkiplistNode<K,V>*> forward;

private:

    K key;
    V value;
};

template <typename K, typename V>
class Skiplist {
public:

    Skiplist() : head(new SkiplistNode<K, V>(K(), V(), MAX_LEVEL)), level(0),gen(std::random_device{}()), dis(0.0, 1.0) {}
    ~Skiplist();

    bool insert(K key, V value);
    bool remove(K key);
    bool search(const K& key,const V& value) const ;
    int randomLevel();



private:
    SkiplistNode<K,V> * head;
    int level;
    mt19937 gen{random_device{}()};
    uniform_real_distribution<double> dis;
};

template <typename K, typename V>
bool Skiplist<K,V>::search(const K& key,const V& value) const {
    SkiplistNode<K,V> *curr = this->head;
    for(int i=level-1;i>=0;i--){
        while(curr->forward[i] && curr->forward[i]->get_key() < key){
            curr = curr->forward[i];
        }
    }
    curr = curr->forward[0];
    if(curr && curr->get_key() == key){
        if(curr->get_value() == value){
            return true;
        }
    }
    return false;
}

template <typename K, typename V>
bool Skiplist<K, V>::insert(K key, V value) {
    SkiplistNode<K,V> *curr = this->head;
    for(int i = level-1;i>=0;i--){
        while(curr->forward[i] && curr->forward[i]->get_key()<key){
            curr = curr->forward[i];
        }
    }
    int lv = randomLevel();
    level = max(level,lv);
    SkiplistNode<K,V>* newNode = new SkiplistNode(key,value,lv);

    for(int i=0;i<lv;i++){
        newNode->forward[i] = curr->forward[i];
        curr->forward[i] = newNode;
    }
    return true;  
}


template <typename K, typename V>
K SkiplistNode<K,V>::get_key() const {
    return key;
}

template <typename K, typename V>
V SkiplistNode<K,V>::get_value() const {
    return value;
}

template <typename K, typename V>
bool Skiplist<K, V>::remove(K key){
    vector<SkiplistNode<K,V>*> update(MAX_LEVEL, nullptr);
    SkiplistNode<K,V> *curr = this->head;
    for (int i = level - 1; i >= 0; i--) {
        /* 找到第 i 层小于且最接近 KEY 的元素*/
        while (curr->forward[i] && curr->forward[i]->get_key() < key) {
            curr = curr->forward[i];
        }
        update[i] = curr;
    }
    curr = curr->forward[0];
    /* 如果值不存在则返回 false */
    if (!curr || curr->get_key() != key) {
        std::cout << "don't have key" << std::endl;
        return false;
    }

    for (int i = 0; i < level; i++) {
        if (update[i]->forward[i] != curr) {
            break;
        }
        /* 对第 i 层的状态进行更新，将 forward 指向被删除节点的下一跳 */
        update[i]->forward[i] = curr->forward[i];
    }
    delete curr;
    /* 更新当前的 level */
    while (level > 1 && head->forward[level - 1] == nullptr) {
        level--;
    }
    return true;
}

// 实现 randomLevel
template <typename K, typename V>
int Skiplist<K, V>::randomLevel() {
    int lv = 1;
    while(dis(gen) < P_FACTOR && lv <MAX_LEVEL){
        lv++;
    }
    return lv;
}

template <typename K, typename V>
Skiplist<K, V>::~Skiplist() {
    SkiplistNode<K, V>* curr = head->forward[0];
    while (curr) {
        SkiplistNode<K, V>* next = curr->forward[0];
        delete curr;
        curr = next;
    }
    delete head;
}


template <typename K, typename V>
SkiplistNode<K, V>::~SkiplistNode() {
    forward.clear();
}














#endif


