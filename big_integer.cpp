#include <iostream>
#include <algorithm>
#include "big_integer.h"
#include "my_vector.cpp"
#define sz(a) int(a.number.size())
#define sz_(a) int(a->number.size())
#define this_sz int(number.size()) 
#define pb(a) push_back(a) 
//secondary function
void big_integer::clear_leader_zero() {
    int size = this_sz - 1;
    for (; size > 0 && number[size] == 0; size--) {
        number.pop_back();
    }

    if (number[size] == 0) {
        sign = false;
    }
}

// this   : [x, y, z]
// b      : [a, b, c]
// b'     : [0..0, a, b, c] count of 0 = shift
// result : this = this - b'
big_integer& big_integer::sub_long(big_integer b, bool f) {
    big_integer *ref_a = this;
    big_integer *ref_b = &b;
    big_integer *c;

    //for add
    b.sign ^= f;
    f = false;

    bool cur_sign = sign;

    if (ref_a->sign != ref_b->sign) { // if (-this - (+b)) or (this - (-b))
        ref_b->sign ^= true;
        return ref_a->add_with_shift(*ref_b, 0);
    } else  { //sign == b.sign == (false or true)
        ref_a->sign = ref_b->sign = false; // abs(a) < abs(b) -> 
        if (*ref_a < *ref_b) { // if abs(this) < abs(c) then swap(this, c);
            c = ref_a;
            ref_a = ref_b; ref_b = c;
            f = true; // at the end sign will be inverted
        }
    }

    // INV: abs(ref_a) > abs(ref_b)
    //      sign(this) == sign(b) == false
    vectorT carry = 0;
    vectorT res;
    for(int i = 0; i < sz_(ref_a); i++) {
        res = ref_a->number[i] - ref_b->number.get_safe_value(i) + carry;
        if (res < 0) {
            res += BASE;
            carry = -1;
        } else {
            carry = 0;
        }
        ref_a->number[i] = res;
    }
    assert(carry == 0);
    
    ref_a->sign = cur_sign ^ f; // if request from add and (-this + b) -> -(this - b) 
    ref_a->clear_leader_zero();

    if (this != ref_a) {
        *this = *ref_a;
    }
    return *this;
}

// this   : [x, y, z]
// b      : [a, b, c]
// b'     : [0..0, a, b, c] count of 0 = shift
// result : this = this + b'
big_integer& big_integer::add_with_shift(big_integer const& b, int shift) {
    if (sign != b.sign) { //sign != b.sign
        return sub_long(b, true);
    }// else sign == b.sign == (false or true)
           
    vectorT carry = 0;
    int max_size = std::max(this_sz, sz(b) + shift);

    for (int i = 0; i < max_size; i++) {
        number[i] += b.number.get_safe_value(i - shift) + carry;
        carry = number[i] >> POW_BASE; // / BASE
        number[i] &= (BASE - 1);  // %= BASE
    }

    if (carry) {
        number.pb(carry);
    }  

    clear_leader_zero();

    return *this;
}

// INV : scale < BASE
void big_integer::div_short(int scale, vectorT& r) {
    bool sign_scale = scale < 0;
    scale = abs(scale);
    vectorT temp_u = 0; 
    r = 0;
    for (int i = this_sz - 1; i >= 0; i--) {
        temp_u = r * BASE + number[i];
        number[i] = temp_u / scale;
        r = temp_u - number[i] * scale;
    }

    if (sign) {
        r *= -1;
    }

    sign ^= sign_scale;
    clear_leader_zero();
}

// INV : v < BASE
void big_integer::mul_short(int v) {
    bool temp_sign = v < 0;
    v = abs(v);
    vectorT carry = 0;
    vectorT temp;
    for (int i = 0; i < this_sz; i++) {
        temp = number[i] * v + carry;
        number[i] = temp & (BASE - 1); //%
        carry = temp >> POW_BASE; // / BASe
    }

    if (carry) {
        number.pb(carry);
    }

    
    sign = sign ^ temp_sign; // tmp.sign == false if a.sign == sign(v)
}

// A = B * Q + R
void big_integer::divide(big_integer a, big_integer b, big_integer& q, big_integer& r) {

   //if b is short
   if (sz(b) == 1) {
        vectorT temp_r;
        a.div_short(b.number[0] * (b.sign ? -1 : 1), temp_r);
        q = a;
        r = temp_r;
        return ; } bool a_sign = a.sign;
    bool sign = a.sign ^ b.sign;
    a.sign = b.sign = false;

    if (a < b) {
        q = big_integer();
        r = a;
        r.sign = r != 0 ? a_sign : false;
        return;
    }

    // Normalization
    bool wasNorm = false;
    int scale = BASE / (b.number[sz(b) - 1] + 1);
    if (scale > 1) {
        b.mul_short(scale);
        a.mul_short(scale);
        wasNorm = true;
    }

    // INV : a >= b
    //       a > 0 && b > 0
    int n = sz(b);     // b : [n - 1     .. 0]
    int m = sz(a) - n; // a : [n + m - 1 .. m .. 0]

    q.sign = r.sign = false;
    q.number.resize(m + 1);
    r.number.resize(n);

    //copy first elder digit from a to modulo
    for (int i = m; i < sz(a); i++) {
        r.number[i - m] = a.number[i];
    }

    if (r >= b) {
        q.number[m] = 1;
        r -= b;
    } else {
        q.number[m] = 0;
    }

    vectorT elderDigitOfb = b.number[n - 1]; 
    for (int j = m - 1; j >= 0; j--) {
        for (;j >= 0 && r < b; j--) {
            r.number.insert(r.number.begin(), a.number[j]);
        }
        j++;
        if (r < b) break;
        q.number[j] = std::min((vectorT) BASE - 1, ((r.number[sz(r) - 1] * BASE + r.number[sz(r) - 2]) / elderDigitOfb));
        r -= q.number[j] * b;
        while (r < 0) {
            q.number[j]--;
            r += b;
        }
    }

    q.sign = sign;
    r.sign = a_sign;
    q.clear_leader_zero(); // if q == 0 then clear and set sign = false
    r.clear_leader_zero();

    if (r != 0) {
        //if we Normalize b then
        //A = Q * B + R            ---> A * carry = B * carry * Q + R'
        //A = B * Q + R' / carry   ---> R = R' / carry
        if (wasNorm) {
            r.div_short(scale, elderDigitOfb); // elderDigitOfb - just temporary var
        }
    }
}

big_integer& big_integer::operator=(big_integer const& other) {
    number = other.number;
    sign = other.sign;
    return *this;
}

//destructor
big_integer::~big_integer() {
}

//constructors
big_integer::big_integer(big_integer const& other) {
    number = other.number;
    sign = other.sign;
}

big_integer::big_integer() {
    sign = false;
}

big_integer::big_integer(int value) {
    sign = value < 0;
    number.setSmallFalse();
    number.pb(abs(value));
    //do {
        //number.pb(value & (BASE - 1));
        //value >>= POW_BASE; 
    //} while (value != 0);
}

big_integer::big_integer(std::string const& s) {
    *this = big_integer();
    for (int i = 0 + (s[0] == '-'); i < (int) s.size(); i++) {   // if (sign == true) i = 1; else i = 0;
        mul_short(INPUT_BASE);
        *this += (s[i] - '0');  // if BASE < 10 then conver(s[i] - '0') to BASE          
    }                                                    
    sign = (s[0] == '-');
    clear_leader_zero();
}

//Operations
big_integer& big_integer::operator+=(big_integer const& b) {
    return add_with_shift(b, 0);
}

big_integer& big_integer::operator-=(big_integer const& b) {
    return sub_long(b);
}

big_integer& big_integer::operator*=(big_integer const& b) {
    if (sz(b) == 1) {
        mul_short(b.number[0] * (b.sign ? -1 : 1));
        return *this;
    }

    big_integer c;
    c.number.resize(this_sz + sz(b));
    int sz_b = sz(b);
    for (int i = 0; i < this_sz; i++) {
        for (int j = 0, carry = 0; j < sz_b || carry; j++) {
            vectorT cur = c.number[i + j] + number[i] * b.number.get_safe_value(j) + carry;
            c.number[i + j] = cur & (BASE - 1);
            carry = cur >> POW_BASE;
        }
    }
    c.clear_leader_zero();
    c.sign = sign ^ b.sign;

    return (*this = c);
}

big_integer& big_integer::operator/=(big_integer const& b) {
    big_integer temp;
    big_integer::divide(*this, b, *this, temp);
    return *this;
}

big_integer& big_integer::operator%=(big_integer const& b) {
    big_integer temp;
    big_integer::divide(*this, b, temp, *this);
    return *this;
}

//bitwise operation
big_integer& big_integer::operator<<=(int a) {
    int period = a / POW_BASE;
    int shift = a % POW_BASE;
    int shift_h_bits = POW_BASE - shift;
    int high_bits = ((1 << shift) - 1) << shift_h_bits;
    int old_size = this_sz;

    number.resize(this_sz + period + (shift != 0));
    
    for (int i = old_size - 1; i >= 0; i--) {
        vectorT lb = (number[i] & high_bits) >> shift_h_bits; 
        number[i] = (number[i] << shift) & (BASE - 1);
        if (period) {
            number[i + period] = number[i];
            number[i] = 0;
        }

        if (shift) {
            number[i + period + 1] |= lb;
        }
    }

    clear_leader_zero();

    return *this;
}

big_integer& big_integer::operator>>=(int a) {
    bool temp_sign = sign;
    if (temp_sign) {
        (*this)++; // this < 0 --->  this++ <--> abs(this)--
    }

    int period = a / POW_BASE;
    int shift = a % POW_BASE;
    int low_bits = (1 << shift) - 1;
    int shift_l_bit = POW_BASE - shift;

    if (this_sz <= period) {
        *this = big_integer();
    } else {
        for (int i = 0; i < this_sz; i++) {
            vectorT lb = (number[i] & low_bits) << shift_l_bit; 
            number[i] >>= shift;
            if (i - period >= 0) {
                number[i - period] = number[i];
                if (i - period - 1 >= 0) {
                    number[i - period - 1] |= lb;
                }
            }
        }

        number.resize(this_sz - period);
    }

    if (temp_sign) {
        (*this)--;
    }
    clear_leader_zero();

    return *this;
}

big_integer operator<<(big_integer a, int b) {
    return a <<= b; 
}

big_integer operator>>(big_integer a, int b) {
    return a >>= b;
}

void big_integer::dop_code(int cnt_bit) {
    operator++(); // a < 0 -> a++ -> abs(a)--
    number.resize(cnt_bit);
    for (int i = 0; i < this_sz; i++) {
        number[i] ^= BASE - 1;
    }
}

vectorT XOR(vectorT a, vectorT b) {
    return a ^ b;
}

vectorT OR(vectorT a, vectorT b) {
    return a | b;
}

vectorT AND(vectorT a, vectorT b) {
    return a & b;
}

void big_integer::bitwise_operations(big_integer& b, vectorT (*bit_op)(vectorT, vectorT)) {
    int max_size = std::max(this_sz, sz(b)); 
    b.number.resize(max_size);
    bool comm_sign = bit_op(sign, b.sign);

    if (b.sign) {
        b.dop_code(max_size);
    }

    if (sign) {
        dop_code(max_size);
    }

    for (int i = max_size - 1; i >= 0; i--) {
        number[i] = bit_op(number[i], b.number[i]);
        number[i] &= (BASE - 1);
    }

    if (comm_sign) {
        sign = true;
        dop_code(max_size);
    } else {
        sign = false;
    }

    clear_leader_zero();
}

big_integer& big_integer::operator&=(big_integer b) {
    bitwise_operations(b, AND);
    return *this;
}

big_integer& big_integer::operator|=(big_integer b) {
    bitwise_operations(b, OR);
    return *this;
}

big_integer& big_integer::operator^=(big_integer b) {
    bitwise_operations(b, XOR);
    return *this;
}

big_integer operator-(big_integer a, big_integer const& b) {
    return a -= b;
}

big_integer operator+(big_integer a, big_integer const& b) {
    big_integer temp = (a += b);
    return temp;
}

big_integer operator*(big_integer a, big_integer const& b) {
    return a *= b;
}

big_integer operator/(big_integer a, big_integer const& b) {
        return a /= b;
}

big_integer operator%(big_integer a, big_integer const& b) {
    return a %= b;
}

big_integer operator&(big_integer a, big_integer const& b) {
    return a &= b;
}

big_integer operator|(big_integer a, big_integer const& b) {
    return a |= b;
}

big_integer operator^(big_integer a, big_integer const& b) {
    return a ^= b;
}

big_integer big_integer::operator~() const {
    big_integer a = *this;
    a.sign ^= true;
    return --a;
}

big_integer& big_integer::operator++() {
    return (*this += 1);
}

big_integer big_integer::operator++(int) {
    big_integer temp = *this;
    *this += 1;
    return temp;
}

big_integer& big_integer::operator--() {
    return (*this -= 1);
}

big_integer big_integer::operator--(int) {
    big_integer temp = *this;
    *this -= 1;
    return temp;
}

bool operator==(big_integer const& a, big_integer const& b) {
    if (sz(a) == sz(b)) {
        for (int i = 0; i < sz(a); i++) {
            if (a.number[i] != b.number[i]) {
                return false;
            }
        }
        if (a.sign == b.sign || (sz(a) == 1 && a.number[0] == 0)) {
            return true;
        } else { 
            return false;
        }
    }
    return false;
}

bool operator!=(big_integer const& a, big_integer const& b) {
    return !(a == b);
}

bool operator<(big_integer const& a, big_integer const& b ) {
    if (a.sign ^ b.sign) {
        return (a.sign ? true : false); // if a < 0 && b > 0 then return true (a < b) else false (b < a)
    }
    // INV : sign(a) == sign(b)
    bool neg_both = a.sign && b.sign; // if a < 0 && b < 0 then abs(a) < abs(b) --> b < a
    bool res = (sz(a) < sz(b));
    if (sz(a) == sz(b)) {
        for (int i = sz(a) - 1; i >= 0; i--) {
            if (a.number[i] == b.number[i]) continue;
            if (a.number[i] < b.number[i]) {
                res = true;
            } else {
                res = false; 
            } 
            break;
        } 
    }

    return res ^ neg_both;
}

bool operator>(big_integer const& a, big_integer const& b ) {
    return !(a < b || a == b);
}

bool operator<=(big_integer const& a, big_integer const& b ) {
    return (a < b) || (a == b); 
}

bool operator>=(big_integer const& a, big_integer const& b ) {
    return (a > b) || (a == b);
}

big_integer big_integer::operator+() const {
    return *this;
}

big_integer big_integer::operator-() const {
    return (*this) * (-1);
}

std::string to_string(big_integer a) {
    if (a == 0) {
        return "0";
    }

    bool sign = a.sign;
    a.sign = false;
    std::string s;
    vectorT r;
    while (a != 0) {
        a.div_short(INPUT_BASE, r); 
        s.push_back('0' + r);
    }

    if (sign) {
        s.push_back('-');
    }

    reverse(s.begin(), s.end());

    return s;
}

std::ostream& operator<<(std::ostream& s, big_integer const& a) {
    return s << to_string(a);
}

int main() {
    std::cout << "hello" ;
    return 0;
}
