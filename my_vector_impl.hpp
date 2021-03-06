#include "my_vector.hpp"

template <class T>
T* my_vector<T>::initialize(size_t num) {
    T* tmp = static_cast<T*>(::operator new(sizeof(T)*num));
    return tmp;
}

template <class T>
void my_vector<T>::destroy(T* ptr) {
    ptr->~T();
}

template <class T>
void my_vector<T>::destroy(T* beg, T* end) {
    T* cur = beg;
    while( cur != end ) {
        destroy(cur);
        ++cur;
    }
}
template<class T>
my_vector<T>::my_vector() {
    reserve(8);
}

template<class T>
my_vector<T>::my_vector(size_t sz) {
    resize(sz);
}

template<class T>
my_vector<T>::my_vector(size_t sz, T init) {
    resize(sz);
    for (size_t i = 0; i < sze; i++)
        elements[i] = init;

}

template<class T>
my_vector<T>::my_vector(std::initializer_list<T> lst) {
    resize(lst.size());
    int j = 0;
    for (auto i = lst.begin(); i != lst.end(); i++, j++) elements[j] = *i;
}

template<class T>
my_vector<T>::my_vector(const my_vector<T> &vect) {
    elements = initialize(vect.size());
    for (size_t i = 0; i < vect.size(); i++) {
        new(elements + i) T (vect.elements[i]);
    }
    sze = vect.size();
    capacty = vect.size();

}

template<class T>
my_vector<T>::my_vector(my_vector<T> &&vect) noexcept : sze(vect.size()), capacty(vect.capacity()),
                                                        elements(vect.elements) {
    vect.sze = 0;
    vect.capacty = 0;
    vect.elements = nullptr;
}

template<class T>
void my_vector<T>::assign(size_t count, const T &elem) {
    auto n = initialize(count);
    for (size_t i = 0; i < count; ++i)
        new(n+i) T(elem);
    destroy(elements, elements + sze);
    ::operator delete(elements);
    elements = n;
    sze = count;
    capacty = count;

}

template<class T>
my_vector<T>::~my_vector() {
    destroy(elements, elements + sze);
    ::operator delete(elements);
}

template<class T>
void my_vector<T>::reserve(size_t num) {
    if (num <= capacty) return;
    auto tmp = initialize(num);
    for (size_t i = 0; i < sze; i++) {
        new(tmp+i) T( elements[i] );
    }
    destroy(elements, elements + sze);
    ::operator delete(elements);
    elements = tmp;
    capacty = num;
}

template<class T>
void my_vector<T>::resize(size_t num) {
    reserve(num);
    if (num <= sze) {
        destroy(elements + num, elements + sze);
    } else {
        for (size_t i = sze; i < num; ++i)
            new(elements + i) T();
    }
    sze = num;
}

template<class T>
size_t my_vector<T>::capacity() const {
    return capacty;
}

template<class T>
size_t my_vector<T>::size() const {
    return sze;
}

template<class T>
bool my_vector<T>::empty() const {
    return sze == 0;
}

template<class T>
void my_vector<T>::shrink_to_fit() {
    if (sze == capacty) return;
    auto tmp = initialize(sze);
    for (size_t i = 0; i < sze; i++) {
        new(tmp+i) T( elements[i] );
    }
    destroy(elements, elements + sze);
    ::operator delete(elements);
    elements = tmp;
    capacty = sze;
}

template<class T>
void my_vector<T>::push_back(const T &num) {
    if (sze == capacty) {
        if (&num < elements || (&num > elements + sze)) {
            if (sze == capacty) reserve(capacty * 2);
            new(elements + sze++)T(num);
            return;
        }
    }
    new(elements + sze++) T(num);
}

template<class T>
void my_vector<T>::pop_back() {
    if (sze == 0) return;
    --sze;
    destroy(elements + sze);
}

template<class T>
T &my_vector<T>::at(size_t ind) {
    if (ind >= sze) throw wrong_index(ind);
    return elements[ind];
}

template<class T>
const T &my_vector<T>::at(size_t ind) const {
    if (ind >= sze) throw wrong_index(ind);
    return elements[ind];
}

template<class T>
T &my_vector<T>::operator[](size_t ind) {
    return elements[ind];
}

template<class T>
const T &my_vector<T>::operator[](size_t ind) const {
    return elements[ind];
}

template<class T>
T &my_vector<T>::front() {
    return elements[0];
}

template<class T>
const T &my_vector<T>::front() const {
    return elements[0];
}

template<class T>
T &my_vector<T>::back() {
    return elements[sze - 1];
}

template<class T>
const T &my_vector<T>::back() const {
    return elements[sze - 1];
}

template<class T>
T *my_vector<T>::data() {
    return elements;
}

template<class T>
const T *my_vector<T>::data() const {
    return elements;
};

template<class T>
my_vector<T> &my_vector<T>::operator=(const my_vector<T> &vect) {
    my_vector tmp(vect);
    this->swap(tmp);
    return *this;
}

template<class T>
my_vector<T> &my_vector<T>::operator=(my_vector<T> &&vect) noexcept {
    destroy(elements, elements+sze);
    ::operator delete(elements);
    sze = vect.sze;
    capacty = vect.capacty;
    elements = vect.elements;
    vect.sze = 0;
    vect.capacty = 0;
    vect.elements = nullptr;
    return *this;
}

template<class T>
void my_vector<T>::clear() {
    sze = 0;
    capacty = 0;
    destroy(elements, elements + sze);
    ::operator delete(elements);
    elements = nullptr;
}

template<class T>
void my_vector<T>::swap(my_vector &other) {
    T *tmp_el = other.elements;
    size_t tmp_sz = other.sze;
    size_t tmp_cap = other.capacty;
    other.elements = elements;
    elements = tmp_el;
    other.sze = sze;
    sze = tmp_sz;
    other.capacty = capacty;
    capacty = tmp_cap;
}

template<class T>
my_vector_iterator<T> my_vector<T>::begin() {
    return my_vector_iterator<T>(elements);
}

template<class T>
my_vector_iterator<T> my_vector<T>::end() {
    return my_vector_iterator<T>(elements + sze);
}
