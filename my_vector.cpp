#include "my_vector.h"
typedef long long vectorT; 
typedef int vectorSzT; 

// if arr == 0 unclear how to use push_back
// (arr == 0) don't matter, that we used smallObject
// it's useful just to recognize: smallObject was initialized or no

//constructors
my_vector::my_vector() {}

my_vector::my_vector(my_vector const& other) { 
    other.ptr->cnt++;
    ptr.ptr = other.ptr.ptr;
}

my_vector& my_vector::operator=(my_vector const& other) {
    ptr = other.ptr;
    return *this;
}

vectorT my_vector::operator[](vectorSzT id) const {
    return (ptr->usedSmall ? ptr->smallObject : (*ptr->arr)[id]);
}
vectorT& my_vector::operator[](vectorSzT id) { 
    copyData(); 
    return (ptr->usedSmall ? ptr->smallObject : (*ptr->arr)[id]);
}

// INV : ptr.ptr != 0
my_vector::~my_vector() {}

void my_vector::push_back(vectorT x) {
    copyData();
    if (!ptr->usedSmall && ptr->arr == 0) {
        ptr->smallObject = x;
        ptr->usedSmall = true;
    } else {
        copySmall();
        ptr->arr->push_back(x);
    } 
} 

void my_vector::pop_back() {
    copyData();
    if (!ptr->usedSmall && ptr->arr != 0) {
        ptr->arr->pop_back();
    } else {
        if (ptr->arr != 0) {
            delete ptr->arr;
            ptr->arr = 0;
        }
        ptr->usedSmall = false;
    }
}

void my_vector::resize(vectorSzT cnt) {
    copyData();
    if (cnt > 1) {
        copySmall();
        ptr->arr->resize(cnt, 0);
    } else {
        if (ptr->arr != 0) {
            delete ptr->arr;
            ptr->arr = 0;
        }
        // cnt == 0 then usedSmall = false
        // cnt == 1 then usedSmall = usedSmall
        ptr->usedSmall &= (cnt != 0);
    }
}

// INV : usedSmall == false && arr != 0 
void my_vector::insert(std::vector<vectorT>::iterator it, vectorT x) {
    assert(ptr->usedSmall == false && ptr->arr != 0);
    copyData();
    ptr->arr->insert(it, x);
}

// INV : usedSmall == false && arr != 0 
std::vector<vectorT>::iterator my_vector::begin() const {
    assert(ptr->usedSmall == false && ptr->arr != 0);
    return ptr->arr->begin();
}

vectorSzT my_vector::size() const {
    if (ptr->usedSmall) {
        return 1;
    }
    return ((ptr->arr == 0) ? 0 : ptr->arr->size());
}

vectorT my_vector::get_safe_value(vectorSzT id) const {
    if (id < 0 || id >= size()) {
        return 0;
    } else if (ptr->usedSmall) {
        return ptr->smallObject;
    } else {
        return (*ptr->arr)[id]; 
    }
}

void my_vector::setSmallFalse() {
    ptr->usedSmall = false;
}

bool my_vector::copySmall() {
    if (ptr->usedSmall) {
        ptr->arr = new std::vector<vectorT>();
        ptr->arr->push_back(ptr->smallObject);
        ptr->usedSmall = false;
        return true;
    } 
    return false;
}

void my_vector::copyData() {
    if (ptr->cnt > 1) {
        ptr.ptr = ptr->make_copy();
    }
}

data::data() {
    usedSmall = true;
    smallObject = 0;
    cnt = 1;
    arr = 0;
}

data::~data() {
    if (cnt == 0 && arr != 0) {
        delete arr;
        arr = 0;
    }
}

data* data::make_copy() {
    cnt--;
    data* temp = new data; // temp : arr = 0 && cnt = 1 , default constr 
    temp->usedSmall = usedSmall;
    temp->smallObject = smallObject;
    if (arr != 0) {
        temp->arr = new std::vector<vectorT>;
        (*temp->arr) = *arr;
    }
    return temp;
}

smart_ptr::smart_ptr() :
    ptr(new data)
{}

data* smart_ptr::operator->() {
    return ptr;
}

data* smart_ptr::operator->() const {
    return ptr;
}

smart_ptr::~smart_ptr() {
    ptr->cnt--;
    if (ptr->cnt == 0) {
        delete ptr;
    }
}

smart_ptr& smart_ptr::operator=(smart_ptr const& other) {
    ptr->cnt--;
    other.ptr->cnt++;
    if (ptr->cnt == 0) {
        delete ptr;
    }
    ptr = other.ptr;
    return *this;
}

