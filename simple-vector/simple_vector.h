#pragma once
#include <cassert>
#include <initializer_list>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <utility>
#include <iterator>
class ReserveProxyObj {
public:
    size_t siz = 0;
    ReserveProxyObj(size_t capacity_to_reserve):siz(capacity_to_reserve) {
    }
};
ReserveProxyObj Reserve(size_t capacity_to_reserve) {
    return ReserveProxyObj(capacity_to_reserve);
}
template <typename Type>
class SimpleVector {
public:
    using Iterator = Type*;
    using ConstIterator = const Type*;

    SimpleVector() noexcept = default;

    //конструкторы и деструтор
    explicit SimpleVector(size_t size) : size_(size), capacity_(size) {
        arr_ = new Type[capacity_];
        std::fill(this->begin(), this->end(), Type());
    }

    SimpleVector(size_t size, const Type& value) : size_(size), capacity_(size) {
        arr_ = new Type[capacity_];
        for (int i = 0; i < static_cast<int>(size);i++) {
            arr_[i] = value;
        }
    }

    SimpleVector(std::initializer_list<Type> init) :size_(init.size()), capacity_(init.size()) {
        arr_ = new Type[init.size()];
        int i = 0;
        for (auto& element_init : init) {
            arr_[i] = std::move(element_init);
            i++;
        }
    }

    SimpleVector(const SimpleVector& other) {
            arr_ = new Type[other.capacity_];
            for (size_t i = 0; i < other.size_; i++) {
                arr_[i] = other.arr_[i];
            }
            size_ = other.size_;
            capacity_ = other.capacity_;
    }

    SimpleVector(SimpleVector&& other) {
            arr_ = other.arr_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            other.arr_ = nullptr;
            other.size_ = other.capacity_ = 0;
    }

    SimpleVector(ReserveProxyObj size_class) {
        size_t new_capacity = size_class.siz;
        if (new_capacity > capacity_) {
            arr_ = new Type[new_capacity];
            capacity_ = new_capacity;
        }
    }

    ~SimpleVector() {
        delete[] arr_;
    }

    //Операторы вектора
    SimpleVector& operator=(const SimpleVector& rhs) {
        if (this != &rhs) {
            delete[] arr_;
            size_ = rhs.size_;
            capacity_ = rhs.capacity_;
            arr_ = new Type[rhs.capacity_];
            for (int i = 0; i < static_cast<int>(rhs.size_); i++) {
                arr_[i] = rhs.arr_[i];
            }
        }
        return *this;
    }

    SimpleVector& operator=(SimpleVector&& other) {
        if (this != &other) {
            delete[] arr_;
            arr_ = other.arr_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            other.arr_ = nullptr;
            other.size_ = other.capacity_ = 0;
        }
        return *this;
    }

    Type& operator[](size_t index) noexcept {
        assert(index < size_);
        return arr_[index];
    }

    const Type& operator[](size_t index) const noexcept {
        assert(index < size_);
        return arr_[index];
    }

    //Итераторы для перемещения по вектору
    Iterator begin() noexcept {
        return &arr_[0];
    }

    Iterator end() noexcept {
        return &arr_[size_];
    }

    ConstIterator begin() const noexcept {
        return &arr_[0];
    }

    ConstIterator end() const noexcept {
        return &arr_[size_];
    }

    ConstIterator cbegin() const noexcept {
        return &arr_[0];
    }

    ConstIterator cend() const noexcept {
        return &arr_[size_];
    }

    //Методы над векторм
    size_t GetSize() const noexcept {
        return size_;
    }

    size_t GetCapacity() const noexcept {
        return capacity_;
    }

    bool IsEmpty() const noexcept {
        return size_ == 0;
    }

    Type& At(size_t index) {
        if (index >= size_) {
            throw std::out_of_range("Vector out of range!");
        }
        return arr_[index];
    }

    const Type& At(size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("Vector out of range!");
        }
        return arr_[index];
    }

    void Clear() noexcept {
        size_ = 0;
    }

    void Resize(size_t new_size) {
        if (new_size <= size_) {
            size_ = new_size;
        }
        if (new_size <= capacity_ && new_size > size_) {
            //запоминаем старый размер и добавляем типиризованние данные
            size_t old_size = size_;
            size_ = new_size;
            //заполняем данными
            for (size_t i = old_size;i < new_size; i++) {
                arr_[i] = Type();
            }
        }
        if (new_size > capacity_) {
            size_t old_size = size_;
            //заполняем данными
            this->Reserve(new_size);
            size_ = new_size;
            for (size_t i = old_size;i < new_size; i++) {
                arr_[i] = Type();
            }
        }
    }

    void PushBack(const Type& item) {
        if (capacity_ == 0 && size_ == 0) {
            capacity_ = 1;
            arr_ = new Type[capacity_];
            size_ = 1;
            arr_[0] = std::move(item);
        }
        else {
            if (size_ == capacity_) {
                this->Reserve(capacity_ * 2);
                arr_[size_++] = std::move(item);
            }
            else {
                if (size_ < capacity_) {
                    arr_[size_++] = std::move(item);
                }
            }  
        }
    }

    void PushBack(Type&& item) {
        if (capacity_ == 0 && size_ == 0) {
            capacity_ = 1;
            arr_ = new Type[capacity_];
            size_ = 1;
            arr_[0] = std::move(item);
        }
        else {
            if (size_ == capacity_) {
                this->Reserve(capacity_ * 2);
                arr_[size_++] = std::move(item);
            }
            else {
                if (size_ < capacity_) {
                    arr_[size_++] = std::move(item);
                }
            }
        }
    }

    Iterator Insert(ConstIterator pos, const Type& value) {
        assert(pos >= this->begin());
        assert(pos <= this->end());
        //пустой вектор
        if (capacity_ == 0) {
            //this->PushBack(value);
            delete[] arr_;
            arr_ = new Type[1];
            arr_[0] = std::move(value);
            size_ = 1;
            capacity_ = 1;
            return &arr_[0];
        }
        //находим номер куда надо вставить
        int shet = std::distance(this->begin(), const_cast<Iterator>(pos));
        //вектор полностью заполнен
        if (capacity_ == size_) {
            //увеличиваем вместимость в 2 раза
            this->Reserve(capacity_ * 2);
        }
        //копируем все элементы  после pos
        Type* back_pos = new Type[static_cast<int>(size_) - shet];
        int j = 0;
        for (int i = shet; i < static_cast<int>(size_);i++) {
            back_pos[j] = std::move(arr_[i]);
            j++;
        }
        //присваиваем
        size_++;
        arr_[shet] = value;
        int k = 0;
        for (int i = shet + 1; i < static_cast<int>(size_); i++) {
            arr_[i] = std::move(back_pos[k]);
            k++;
        }
        delete[] back_pos;
        return &arr_[shet];      
    }

    Iterator Insert(ConstIterator pos, Type&& value) {
        assert(pos >= this->begin());
        assert(pos <= this->end());
        //пустой вектор
        if (capacity_ == 0) {
            //this->PushBack(value);
            delete[] arr_;
            arr_ = new Type[1];
            arr_[0] = std::move(value);
            size_ = 1;
            capacity_ = 1;
            return &arr_[0];
        }
        //находим номер куда надо вставить
        int shet = std::distance(this->begin(), const_cast<Iterator>(pos));
        //вектор полностью заполнен
        if (capacity_ == size_) {
            //увеличиваем вместимость в 2 раза
            this->Reserve(capacity_ * 2);
        }
        //копируем все элементы  после pos
        Type* back_pos = new Type[static_cast<int>(size_) - shet];
        int j = 0;
        for (int i = shet; i < static_cast<int>(size_);i++) {
            back_pos[j] = std::move(arr_[i]);
            j++;
        }
        //присваиваем
        size_++;
        arr_[shet] = std::move(value);
        int k = 0;
        for (int i = shet + 1; i < static_cast<int>(size_); i++) {
            arr_[i] = std::move(back_pos[k]);
            k++;
        }
        delete[] back_pos;
        return &arr_[shet];
    }

    // "Удаляет" последний элемент вектора. Вектор не должен быть пустым
    void PopBack() noexcept {
        assert(!(this->IsEmpty()));
            size_--;
    }

    // Удаляет элемент вектора в указанной позиции
    Iterator Erase(ConstIterator pos) {
        assert(pos >= this->begin());
        assert(pos <= this->end());
        assert(!(this->IsEmpty()));
        size_t shet = std::distance(this->begin(), const_cast<Iterator>(pos));
        for (size_t i = shet + 1; i < size_; ++i) {
            arr_[i - 1] = std::move(arr_[i]);
        }
        --size_;
        return &arr_[shet];
    }

    // Обменивает значение с другим вектором
    void swap(SimpleVector& other) noexcept {
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
        std::swap(arr_, other.arr_);
    }

    void Reserve(size_t new_capacity) {
        if (new_capacity>capacity_) {
            Type* tmp = new Type[size_];
            for (size_t i = 0; i < size_;i++) {
                tmp[i] = std::move(arr_[i]);
            }
            delete[] arr_;
            arr_ = new Type[new_capacity];
            for (size_t i = 0; i < size_; i++) {
                arr_[i] = std::move(tmp[i]);
            }
            capacity_ = new_capacity;
            delete[] tmp;
        }
    }

private:
    Type* arr_ = nullptr;
    size_t size_ = 0;
    size_t capacity_ = 0;
};

template <typename Type>
inline bool operator==(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    if (lhs.GetSize() != rhs.GetSize()) {
        return false;
    }

    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename Type>
inline bool operator!=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
inline bool operator<(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
inline bool operator<=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(rhs < lhs);
}

template <typename Type>
inline bool operator>(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return (rhs < lhs);
}

template <typename Type>
inline bool operator>=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs < rhs);
}
