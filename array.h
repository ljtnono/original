#ifndef ARRAY_H
#define ARRAY_H
#pragma once


#include <initializer_list>
#include "config.h"
#include <sstream>
#include "error.h"
#include "printable.h"
#include "serial.h"

namespace original{

    template<typename TYPE>
    class array : public printable, public serial<TYPE>{
        size_t size_;
        TYPE* body;

        public:
        explicit array(size_t size);
        array(std::initializer_list<TYPE> lst);
        array(const array& other);
        array& operator=(const array& other);
        array(array&& other) _GLIBCXX_NOEXCEPT;

        ~array();

        _GLIBCXX_NODISCARD size_t size() const override;
        TYPE get(int index) override;
        TYPE operator[](int index) override;
        void set(int index, TYPE e) override;
        void pushBegin(TYPE e) override;
        void push(int index, TYPE e) override;
        void pushEnd(TYPE e) override;
        TYPE popBegin() override;
        TYPE pop(int index) override;
        TYPE popEnd() override;
        std::string toString(bool enter) override;
    };

}

    template <typename TYPE>
    original::array<TYPE>::array(const size_t size)
        : size_(size), body(new TYPE[size_]) {}

    template <typename TYPE>
    original::array<TYPE>::array(std::initializer_list<TYPE> lst)
        : size_(lst.size()), body(new TYPE[size_]) {
        size_t i = 0;
        for (auto e : lst) {
            this->body[i] = e;
            i += 1;
        }
    }

    template <typename TYPE>
    original::array<TYPE>::array(const array& other)
        : size_(other.size_), body(new TYPE[size_]) {
        for (size_t i = 0; i < size_; i++) {
            body[i] = other.body[i];
        }
    }

    template <typename TYPE>
    auto original::array<TYPE>::operator=(const array& other) -> array&
    {
        if (this == &other) return *this;

        delete[] this->body;

        this->size_ = other.size_;
        this->body = new TYPE[this->size_];
        for (size_t i = 0; i < this->size_; i++) {
            this->body[i] = other.body[i];
        }
        return *this;
    }

    template <typename TYPE>
    original::array<TYPE>::array(array&& other) _GLIBCXX_NOEXCEPT
        : size_(other.size_), body(other.body) {
        other.size_ = 0;
        other.body = nullptr;
    }

    template <typename TYPE>
    original::array<TYPE>::~array() {
        delete[] this->body;
    }

    template <typename TYPE>
    auto original::array<TYPE>::size() const -> size_t
    {
        return this->size_;
    }

    template <typename TYPE>
    auto original::array<TYPE>::get(int index) -> TYPE
    {
        if (this->indexOutOfBound(index)){
            throw indexError();
        }
        return this->body[this->negIndex(index)];
    }

    template <typename TYPE>
    auto original::array<TYPE>::operator[](const int index) -> TYPE
    {
        return this->get(index);
    }

    template <typename TYPE>
    auto original::array<TYPE>::set(int index, TYPE e) -> void
    {
        if (this->indexOutOfBound(index)){
            throw indexError();
        }
        this->body[this->negIndex(index)] = e;
    }

    template <typename TYPE>
    void original::array<TYPE>::pushBegin(TYPE e){
        this->set(0, e);
    }

    template <typename TYPE>
    void original::array<TYPE>::push(int index, TYPE e){
        this->set(index, e);
    }

    template <typename TYPE>
    void original::array<TYPE>::pushEnd(TYPE e){
        this->set(this->size() - 1, e);
    }

    template <typename TYPE>
    TYPE original::array<TYPE>::popBegin(){
        return this->get(0);
    }

    template <typename TYPE>
    TYPE original::array<TYPE>::pop(int index){
        return this->get(index);
    }

    template <typename TYPE>
    TYPE original::array<TYPE>::popEnd(){
        return this->get(this->size() - 1);
    }

    template <typename TYPE>
    auto original::array<TYPE>::toString(const bool enter) -> std::string
    {
        std::stringstream ss;
        ss << "array" << "(";
        for (size_t i = 0; i < this->size_; ++i) {
            ss << this->body[i];
            if (i < this->size_ - 1) {
                ss << ", ";
            }
        }
        ss << ")";
        if (enter) {
            ss << "\n";
        }
        return ss.str();
    }

#endif //ARRAY_H
