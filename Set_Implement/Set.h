#ifndef SET_INCLUDED
#define SET_INCLUDED
#define Empty ""
#include<iostream>
#include <string>
// Later in the course, we'll see that templates provide a much nicer
// way of enabling us to have Sets of different types.  For now,
// we'll use a type alias.

using ItemType = std::string;

const int DEFAULT_MAX_ITEMS = 160;

class Set
{
    public:
        Set(int capacity = DEFAULT_MAX_ITEMS);
        // Create an empty set with the given capacity.

        bool empty() const;  // Return true if the set is empty, otherwise false.
        int size() const;    // Return the number of items in the set.

        bool insert(const ItemType& value);
        // Insert value into the set if it is not already present.  Return
        // true if the value is actually inserted.  Leave the set unchanged
        // and return false if value is not inserted (perhaps because it
        // was already in the set or because the set has a fixed capacity and
        // is full).

        bool erase(const ItemType& value);
        // Remove the value from the set if it is present.  Return true if the
        // value was removed; otherwise, leave the set unchanged and
        // return false.

        bool contains(const ItemType& value) const;
        // Return true if the value is in the set, otherwise false.

        bool get(int i, ItemType& value) const;
        // If 0 <= i < size(), copy into value the item in the set that is
        // strictly greater than exactly i items in the set and return true.
        // Otherwise, leave value unchanged and return false.

        void swap(Set& other);
        // Exchange the contents of this set with the other one.
        //void print() const;
        // Housekeeping functions
        ~Set();
        Set(const Set& other);
        Set& operator=(const Set& rhs);

    private:
        struct Node {
            ItemType value;
            Node* next;
            Node* prev;
        };
        Node* head;
        int m_size;        // the number of items in the set
        int m_capacity;    // the maximum number of items there could be

        // At any time, the elements of m_data indexed from 0 to m_size-1
        // are in use and are stored in increasing order.

        //find the not that contains the value we look at 
        Node* findNode(const ItemType& value) const
        {
            Node* p = head;
            while (p->next != head)
            {
                if (p->next->value == value)
                    break;
                p = p->next;
            }
            return p->next;
        }
       
        ItemType findpos(int pos) const;
};

// Inline implementations

inline
int Set::size() const
{
    return m_size;
}

inline
bool Set::empty() const
{
    return size() == 0;
}

void unite(const Set& s1, const Set& s2, Set& result);
void butNot(const Set& s1, const Set& s2, Set& result);
#endif 

