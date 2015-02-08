#ifndef big_integerG_INTEGER_H
#define big_integerG_INTEGER_H

#include <string>
#include "my_vector.h"

// if BASE != 2 ^ k then substitute
// ">> POW_BASE" -> "/ BASE"
// "&="     -> "%=" && "(BASE - 1)"   ->  " BASE"
const int POW_BASE = 31; 
const unsigned int BASE = 1 << POW_BASE; 
const int INPUT_BASE = 10; 

class big_integer { 
public:
    big_integer();
    big_integer(big_integer const& other);
    big_integer(int);
    explicit big_integer(std::string const&);
    ~big_integer();

    big_integer& operator=(big_integer const& other);

    big_integer& operator+=(big_integer const&); 
    big_integer& operator-=(big_integer const&);
    big_integer& operator*=(big_integer const&);
    big_integer& operator/=(big_integer const&);
    big_integer& operator%=(big_integer const&);

    big_integer& operator&=(big_integer);
    big_integer& operator|=(big_integer);
    big_integer& operator^=(big_integer);

    big_integer& operator<<=(int);
    big_integer& operator>>=(int);

    big_integer operator+() const;
    big_integer operator-() const;
    big_integer operator~() const;

    big_integer& operator++();
    big_integer operator++(int);

    big_integer& operator--();
    big_integer operator--(int);

    friend bool operator==(big_integer const& a, big_integer const& b);
    friend bool operator!=(big_integer const& a, big_integer const& b);
    friend bool operator<(big_integer const& a, big_integer const& b);
    friend bool operator>(big_integer const& a, big_integer const& b);
    friend bool operator<=(big_integer const& a, big_integer const& b);
    friend bool operator>=(big_integer const& a, big_integer const& b);

    friend big_integer operator<<(big_integer a, int b);
    friend big_integer operator>>(big_integer a, int b);

    friend std::string to_string(big_integer a);

// sign == true  <-> a is negative
// sign == false <-> a is positive
private:
    my_vector number;
    bool sign;
    //secondary function
    big_integer& sub_long(big_integer, bool f = false);
    big_integer& add_with_shift(big_integer const&, int); 
    void clear_leader_zero();
    void bitwise_operations(big_integer& b, vectorT (*f)(vectorT, vectorT));
    void div_short(int, vectorT&);
    void mul_short(int); 
    void dop_code(int);
    static void divide(big_integer a, big_integer b, big_integer& q, big_integer& r);
};

big_integer operator+(big_integer a, big_integer const& b);
big_integer operator-(big_integer a, big_integer const& b);
big_integer operator*(big_integer a, big_integer const& b);
big_integer operator/(big_integer a, big_integer const& b);
big_integer operator%(big_integer a, big_integer const& b);

big_integer operator&(big_integer a, big_integer const& b);
big_integer operator|(big_integer a, big_integer const& b);
big_integer operator^(big_integer a, big_integer const& b);

big_integer operator<<(big_integer a, int b);
big_integer operator>>(big_integer a, int b);

bool operator==(big_integer const& a, big_integer const& b);
bool operator!=(big_integer const& a, big_integer const& b);
bool operator< (big_integer const& a, big_integer const& b);
bool operator> (big_integer const& a, big_integer const& b);
bool operator<=(big_integer const& a, big_integer const& b);
bool operator>=(big_integer const& a, big_integer const& b);

std::string to_string(big_integer a);
std::ostream& operator<<(std::ostream& s, big_integer const& a);
#endif

