#include "Set.h"
#include <iostream>
#include <cstdlib>

using namespace std;

Set::Set(int capacity): m_size(0), m_capacity(capacity)
{
    if (capacity < 0)
    {
        std::cout << "A Set capacity must not be negative." << std::endl;
        std::exit(1);
    }
    //initialize head node (dummy node)
    head = new Node;
    head->next = head;
    head->prev = head;
    head->value = Empty;
}

Set::~Set()
{
    //delete all nodes in the list 
    while (head->next!=head) {
        Node* p = head->next;
        head->next = p->next;
        delete p;
    }
    delete head;
}

Set::Set(const Set& other) : m_size(other.m_size), m_capacity(other.m_capacity)
{
    // Since the only elements that matter are those up to m_size, only
    // they have to be copied.
    head = new Node;
    head->next = head;
    head->prev = head;
    head->value = Empty;
    Node* s = other.head;
    while (s->next != other.head)
    {
        insert(s->next->value);
        s = s->next;
    }
    m_size = other.m_size;
}

Set& Set::operator=(const Set& rhs)
{
    //create a temporary node that copies rhs and swap it with this list
    if (this != &rhs) {
        Set temp(rhs);
        swap(temp);
    }
    return *this;
}
bool Set::insert(const ItemType& value)
{
    //check if it's legal to insert the value
    if (m_size == m_capacity)
        return false;
    if (contains(value))
        return false;
    //find the node whose next holds a value greater than the value we insert; otherwise we come to the tail
    Node* p = head;
    while (p->next != head)
    {
        if (p->next->value > value)
            break;
        p = p->next;
    }
    //insert the new node
    Node* next = p->next;
    Node* s = new Node;
    s->value = value;
    p->next = s;
    s->prev = p;
    next->prev = s;
    s->next = next;
    m_size++;
    return true;
}

bool Set::erase(const ItemType& value)
{
    //check if we can erase the value
    if (!contains(value))
        return false;
    //cut off the node holds the value
    Node* p = findNode(value);
    Node* prev = p->prev;
    Node* next = p->next;
    delete p;
    prev->next = next;
    next->prev = prev;
    m_size--;
    return true;
}

bool Set::contains(const ItemType& value) const
{
    if (findNode(value) != head)
        return true;
    return false;
}

bool Set::get(int i, ItemType& value) const
{
    if (i < 0 || i >= m_size)
        return false;
    value = findpos(i);
    return true;
}

void Set::swap(Set& other)
{
    // Swap pointers to the elements.
    Node* tempHead = this->head;
    this->head = other.head;
    other.head = tempHead;
    // Swap sizes
    int tempSize = this->m_size;
    this->m_size = other.m_size;
    other.m_size = tempSize;
    // Swap capacities
    int tempCapacity = this->m_capacity;
    this->m_capacity = other.m_capacity;
    other.m_capacity = tempCapacity;
}

ItemType Set::findpos(int pos) const
{
    //find value held by "pos"th node
    int cnt = 0;
    Node* p = head;
    while (p->next != head)
    {
        if (cnt == pos)
            return p->next->value;
        p = p->next;
        cnt++;
    }
    return Empty;
}

void unite(const Set& s1, const Set& s2, Set& result)
{
    //create a temp set that copies s1
    Set tmp(s1);
    ItemType value;
    int i = 0;
    //get every value s2 holds
    while (s2.get(i,value))
    {
        i++;
        //insert it to tmp if the value is not held by s1
        if (!tmp.contains(value))
            tmp.insert(value);
    }
    result = tmp;
}

void butNot(const Set& s1, const Set& s2, Set& result)
{
    //create an empty set
    Set tmp;
    int i = 0;
    ItemType value;
    //get every value s1 holds
    while (s1.get(i, value))
    {
        i++;
        //insert it to tmp if the value is not held by s2
        if (!s2.contains(value))
            tmp.insert(value);
    }
    result = tmp;
}
