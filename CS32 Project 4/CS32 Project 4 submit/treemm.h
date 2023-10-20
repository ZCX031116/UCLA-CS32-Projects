#ifndef TREEMULTIMAP_INCLUDED
#define TREEMULTIMAP_INCLUDED

#include <iostream>
#include <list>
#include <vector>

template <typename KeyType, typename ValueType>
class TreeMultimap
{
private:
    struct Node
    {
        KeyType key;
        std::list<ValueType> values;
        Node* left;
        Node* right;
        Node(const KeyType& k, const ValueType& v) : key(k), left(nullptr), right(nullptr)
        {
            values.push_back(v);
        }
    };
    Node* root;

    void clear(Node* node)
    {
        if (node != nullptr)
        {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }
public:
    class Iterator
    {
    public:
        Iterator() : node(nullptr) {}
        Iterator(const Node* n, const typename std::list<ValueType>::const_iterator& i) : iter(i), node(n) {}
        const ValueType& get_value() const
        {
            return *iter;
        }
        bool is_valid() const
        {
            return node != nullptr && iter != node->values.end();
        }
        void advance()
        {
            if (node == nullptr)
                return;
            iter++;
            if (iter == node->values.end())
            {
                return;
            }
        }
    private:
        typename std::list<ValueType>::const_iterator iter;
        const Node* node;
    };

    TreeMultimap() : root(nullptr) {}

    ~TreeMultimap()
    {
        clear(root);
    }

    void insert(const KeyType& key, const ValueType& value)
    {
        Node** curr = &root;
        while (*curr != nullptr && (*curr)->key != key)
        {
            if (key < (*curr)->key) {
                curr = &((*curr)->left);
            }
            else {
                curr = &((*curr)->right);
            }
        }
        if (*curr == nullptr)
        {
            *curr = new Node(key, value);
        }
        else
        {
            (*curr)->values.push_back(value);
        }
    }

    Iterator find(const KeyType& key) const
    {
        Node* curr = root;
        while (curr != nullptr && curr->key != key)
        {
            if (key < curr->key)
            {
                curr = curr->left;
            }
            else
            {
                curr = curr->right;
            }
        }
        if (curr == nullptr)
        {
            return Iterator();
        }
        else
        {
            return Iterator(curr, curr->values.begin());
        }
    }
};
#endif // TREEMULTIMAP_INCLUDED