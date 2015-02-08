#ifndef MY_VECTOR_H
#define MY_VECTOR_H
#include <cassert>
#include<vector>

typedef long long vectorT; // my type
typedef int vectorSzT; 

class data {
public:
    data(); 
    ~data(); 

private:
    std::vector<vectorT>* arr; 
    vectorT smallObject;
    bool usedSmall;
    int cnt;
    data* make_copy(); 

    friend class my_vector;
    friend class smart_ptr;
};

class smart_ptr {
public:
    smart_ptr(); 
   ~smart_ptr(); 

    smart_ptr& operator=(smart_ptr const& a);
    data* operator->(); 
    data* operator->() const; 

private:
    data* ptr;
    friend class my_vector;
};

class my_vector {
public:
    my_vector();
    my_vector(my_vector const& other); 

    my_vector& operator=(my_vector const& other); 
    vectorT operator[](vectorSzT id) const;
    vectorT& operator[](vectorSzT id); 

    ~my_vector(); 

    void push_back(vectorT x) ; 
    void pop_back(); 
    void resize(vectorSzT cnt); 
    void insert(std::vector<vectorT>::iterator it, vectorT x); 
    std::vector<vectorT>::iterator begin() const; 
    vectorSzT size() const; 
    vectorT get_safe_value(vectorSzT id) const; 
    void setSmallFalse(); 

private:
    bool copySmall(); 
    void copyData(); 
    smart_ptr ptr;
};

#endif

