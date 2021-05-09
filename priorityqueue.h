// priorityqueue.h
//
// Author: Mosrour Tafadar
// Project is written by Professor Shannon Rekinger
// Sping 2021, CS 251, UIC
#pragma once

#include <iostream>
#include <sstream>
#include <set>
#include <string>
using namespace std;
template<typename T>
class priorityqueue {
private:
    struct NODE {
        int priority;  // used to build BST
        T value;  // stored data for the p-queue
        bool dup;  // marked true when there are duplicate priorities
        NODE* parent;  // links back to parent
        NODE* link;  // links to linked list of NODES with duplicate priorities
        NODE* left;  // links to left child
        NODE* right;  // links to right child
    };
    NODE* root;  // pointer to root node of the BST
    int size;  // # of elements in the pqueue
    NODE* curr;  // pointer to next item in pqueue (see begin and next)
    void printInorder (NODE* r) {
        if ( r != nullptr) {
            printInorder(r->left);
            cout <<r-> priority<< " value: " << r->value << endl;
            printInorder(r->link);
            printInorder(r -> right);
        }
    }
    void _clear (NODE* r) {
        if (r == nullptr) {
            return;
        }
        else {
            _clear(r -> left);
            _clear(r->link);
            _clear(r->right);
            delete r;
        }
    }
    void resetNode (NODE* root) {
        if (root == nullptr) {
            return;
        } else {
            if (root ->link != nullptr) {  // there is duplicate
                  NODE* r = root;
                 while (r  != nullptr) {
                  enqueue(r ->value, r->priority);
                  r = r ->link;
                 }
            }
            else {
                enqueue(root ->value, root->priority);
            }
            resetNode(root -> left);
            resetNode(root -> right);
        }
    }
    
    bool checkList2 (NODE*& r, NODE*& other) const {
        if ( r == nullptr && other != nullptr) {
            return false;
        }
        if (r == nullptr && other == nullptr) {
            return true;
        }
        if (r != nullptr && other == nullptr) {
            return true;
        }
        return true;
    }
    
    bool checkList3(NODE* r, NODE* other){
        if (r->value != other->value || r->priority != other->priority) {
                return false;
        }
        if ( r->link == nullptr && other->link != nullptr) {
            return false;
        }
        if (r->link == nullptr && other->link == nullptr) {
            return true;
        }
        if (r->link != nullptr && other->link == nullptr) {
            return false;
        }
        if (r->link != nullptr && other->link != nullptr) {
            r = r->link;
            other = other->link;
        }
        return true;
    }
    bool checkList (NODE* r, NODE* other) const {
       if ( r == nullptr && other != nullptr) {
            return false;
        }
        if (r == nullptr && other == nullptr) {
            return true;
        }
        if (r != nullptr && other == nullptr) {
            return false;
        }
        while (true) {
            if (r->value != other->value || r->priority != other->priority) {
                return false;
            }
            if ( r->link == nullptr && other->link != nullptr) {
                return false;
            }
            if (r->link == nullptr && other->link == nullptr) {
                return true;
            }
            if (r->link != nullptr && other->link == nullptr) {
                return false;
            }
            if (r->link != nullptr && other->link != nullptr) {
                r = r->link;
                other = other->link;
            }
        }
        return true;
    }
    bool  checkEquality( NODE* r, NODE* other) const {
    if ( r == nullptr && other == nullptr) {
        return true;
        }
        if ( r !=  nullptr && other != nullptr ) {
            if (r -> value == other->value &&  r -> priority == other->priority && checkEquality(r -> left, other->left) && checkList(r->link, other->link) && checkEquality(r->right, other->right)
               ) {
               return true;
             } else {
                 return false;
             }
     } else {
         return false;
     }
       }
    void declaration(NODE*& n){
       n -> left = nullptr;
       n -> right = nullptr;
       n -> link = nullptr;
       n -> parent = nullptr;
       n-> dup = false;
        
    }
    
    T deque(){
        if (curr == root) {
          if (curr -> link == nullptr && root -> right != nullptr) {
            T tempV = curr -> value;
            root = curr -> right;
            root -> parent = nullptr;
            delete(curr);
            size--;
            return tempV;
          } else if (curr -> link != nullptr) {
            T tempV = curr -> value;
            root = curr -> link;
            root -> right = curr -> right;
            root -> parent = nullptr;
            delete(curr);
            size--;
            return tempV;
          } else {
            T tempV = curr -> value;
            root = nullptr;
            delete(curr);
            curr = nullptr;
            size--;
            return tempV;
          }
        }
    }
    public:
    //
    // default constructor:
    //
    // Creates an empty priority queue.
    // O(1)
    //
    priorityqueue() {
        root = nullptr; 
        size = 0; 
        curr = nullptr;
    }
    //
    // operator=
    //
    // Clears "this" tree and then makes a copy of the "other" tree.
    // Sets all member variables appropriately.
    // O(n), where n is total number of nodes in custom BST
    //
    priorityqueue& operator= (const priorityqueue& other) {
        if(this == &other) {
            return *this;
        }
        clear();
        resetNode(other.root);
        this -> size = other.size;
        return *this;
    }
    //
    // clear:
    //
    // Frees the memory associated with the priority queue but is public.
    // O(n), where n is total number of nodes in custom BST
    //
    void clear() {
        if( root == nullptr) {
            return;
        } else {
            _clear(root);
            root = nullptr;
            size = 0;    
            curr = nullptr;
        }
    }
    //
    // destructor:
    //
    // Frees the memory associated with the priority queue.
    // O(n), where n is total number of nodes in custom BST
    //
    ~priorityqueue() {
    if (root != nullptr) {
     _clear(root);
    }
    }
    //
    // enqueue:
    //
    // Inserts the value into the custom BST in the correct location based on
    // priority.
    // O(logn + m), where n is number of unique nodes in tree and m is number of
    // duplicate priorities
    //
    //
    void enqueue(T value, int priority) {
       NODE* prev = nullptr;
       NODE* current = root;
       while(current != nullptr) {
           if (priority == current -> priority) { 
           prev = current;
           current = current -> link;  
           }
           else if (priority < current -> priority) { 
               prev = current; 
               current = current -> left; 
           } else {
               prev = current;
               current = current -> right;
           }
       }
       NODE* n = new NODE(); 
       n -> value = value;
       n -> priority = priority;
       declaration(n);
       if (prev == nullptr) {
           root = n ;
       }
        else {
            if (priority < prev -> priority) {
              prev -> left = n;
            }
            else if (priority > prev -> priority) {
            prev -> right = n;
            }
            else {
            n->dup = true;
            prev -> link  = n;
       }
        n -> parent = prev;  
       }
        size++;
    }
    //
    // dequeue:
    //
    // returns the value of the next element in the priority queue and removes
    // the element from the priority queue.
    // O(logn + m), where n is number of unique nodes in tree and m is number of
    // duplicate priorities
    //
    T dequeue() {
      begin();
      if (curr == nullptr) {
        return T();
      }
      
      if (curr != nullptr) {
          
        if (curr == root) {
          if (curr -> link == nullptr && root -> right != nullptr) {
            T tempV = curr -> value;
            root = curr -> right;
            root -> parent = nullptr;
            delete(curr);
            size--;
            return tempV;
          } else if (curr -> link != nullptr) {
            T tempV = curr -> value;
            root = curr -> link;
            root -> right = curr -> right;
            root -> parent = nullptr;
            delete(curr);
            size--;
            return tempV;
          } else {
            T tempV = curr -> value;
            root = nullptr;
            delete(curr);
            curr = nullptr;
            size--;
            return tempV;
          }
        }
        if (curr != root) {
          if (curr -> right != nullptr && curr -> link == nullptr) {
            T tempV = curr -> value;
            NODE * temp = curr;
            NODE* temp2 = curr -> right;
            temp -> parent -> left = temp2;
            temp2 -> parent = temp -> parent;
            delete(curr);
            size--;
            return tempV;
          } else if (curr -> right == nullptr && curr -> link != nullptr) {
            T tempV = curr -> value;
            NODE* temp = curr;
            temp -> parent -> left = temp -> link;
            temp -> link -> parent = temp -> parent;
            delete(curr);
            size--;
            return tempV;
          } else if (curr -> link == nullptr && curr -> right == nullptr) {
            T tempV = curr -> value;
            curr -> parent -> left = nullptr;
            delete(curr);
            size--;
            return tempV;
          } else if (curr -> link != nullptr && curr -> right != nullptr) {
              T tempV = curr -> value;
              if(curr -> parent != nullptr) {
                  if (curr->priority < curr->parent->priority) {
                      curr->parent->left = curr ->link;
                      curr->link->parent = curr->parent;
                  }
                  if (curr->priority > curr->parent->priority) {
                      curr->parent->right = curr->link;
                      curr->link->parent = curr->parent;
                  }
              }
              curr->link ->right = curr->right;
              curr->right->parent = curr->parent;
            // T tempV = curr -> value;
            // NODE * temp = curr;
            // temp = curr -> link;
            // temp -> right = curr -> right;
            // temp -> parent -> left = temp;
            // temp -> parent = curr -> parent;
            // curr -> parent -> left = nullptr;
            delete(curr);
            size--;
            return tempV;
          }
        }
      }
      return T();
    }
    //
    // Size:
    //
    // Returns the # of elements in the priority queue, 0 if empty.
    // O(1)
    //
    int Size() {
        if (root == nullptr) {
            return 0;
        }
        return size;
    }
    //
    // begin
    //
    // Resets internal state for an inorder traversal.  After the
    // call to begin(), the internal state denotes the first inorder
    // node; this ensure that first call to next() function returns
    // the first inorder node value.
    //
    // O(logn), where n is number of unique nodes in tree
    //
    // Example usage:
    //    pq.begin();
    //    while (tree.next(value, priority)) {
    //      cout << priority << " value: " << value << endl;
    //    }
    //    cout << priority << " value: " << value << endl;
    void begin() {
         curr = root;
        if (curr == nullptr) {
            return;
        }
        while (curr ->left != nullptr) {
            curr = curr ->left;
        }
    }
    //
    // next
    //
    // Uses the internal state to return the next inorder priority, and
    // then advances the internal state in anticipation of future
    // calls.  If a value/priority are in fact returned (via the reference
    // parameter), true is also returned.
    //
    // False is returned when the internal state has reached null,
    // meaning no more values/priorities are available.  This is the end of the
    // inorder traversal.
    //
    // O(logn), where n is the number of unique nodes in tree
    //
    // Example usage:
    //    pq.begin();
    //    while (tree.next(value, priority)) {
    //      cout << priority << " value: " << value << endl;
    //    }
    //    cout << priority << " value: " << value << endl;
    //
    bool next(T & value, int & priority) {
     if (curr == nullptr) {
       return false;
     }
     value = curr -> value;
     priority = curr -> priority;
     
     if (curr -> link != nullptr) {
       curr = curr -> link;
       return true;
     }
     if (curr -> link == nullptr && curr -> dup == true) {
       NODE * temp = curr;
       int value = temp -> priority;
       while (temp -> priority == value) {
         curr = temp;
         temp = temp -> parent;
         if (temp == nullptr) {
           break;
         }
       }
     }
     if (curr -> right != nullptr) {
       curr = curr -> right;
       while (curr -> left != nullptr) {
         curr = curr -> left;
       }
       return true;
     } else {
       while (curr -> parent != nullptr) {
         if (curr -> parent -> priority > priority) {
           curr = curr -> parent;
           return true;
         }
         curr = curr -> parent;
       }
       return false;
     }
   }
    // toString:
    //
    // Returns a string of the entire priority queue, in order.  Format:
    // "1 value: Ben
    //  2 value: Jen
    //  2 value: Sven
    //  3 value: Gwen"
    //
    void  toString() {
        //char str = '';
        //stringstream buffer;
        NODE* temporary = root;
        printInorder(temporary);
       // str = buffer.str();
    }
    //
    // peek:
    //
    // returns the value of the next element in the priority queue but does not
    // remove the item from the priority queue.
    // O(logn + m), where n is number of unique nodes in tree and m is number of
    // duplicate priorities
    //
    T peek(){
        begin();
        T valueOut = curr -> value;
        return valueOut; 
    }
    //
    // ==operator
    //
    // Returns true if this priority queue as the priority queue passed in as
    // other.  Otherwise returns false.
    // O(n), where n is total number of nodes in custom BST
    //
    bool operator==(const priorityqueue& other) const {
        if (root  == nullptr && other.root == nullptr ) {
             return true;
         }
         if (size != other.size) {
             return false;
         }
        if ((checkEquality(root, other.root)) == true) {
            return true;
        }
         return false;
    }
    //
    // getRoot - Do not edit/change!
    //
    // Used for testing the BST.
    // return the root node for testing.
    //
    void* getRoot() {
        return root;
    }
};
