#pragma once
#include <cassert>
#include <initializer_list>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <utility>
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

    //конструкторы и операторы
    explicit SimpleVector(size_t size) : size_(size), capacity_(size) {
        arr_ = new Type[capacity_];
        for (int i = 0; i < static_cast<int>(size);i++) {
            arr_[i] = Type();
        }
    }

    SimpleVector(size_t size, const Type& value) : size_(size), capacity_(size) {
        arr_ = new Type[capacity_];
        for (int i = 0; i < static_cast<int>(size);i++) {
            arr_[i] = value;
        }
    }

    SimpleVector(size_t size, Type&& value) : size_(size), capacity_(size) {
        arr_ = new Type[capacity_];
        for (int i = 0; i < static_cast<int>(size);i++) {
            arr_[i] = std::move(value);
        }
    }

    SimpleVector(std::initializer_list<Type> init) :size_(init.size()), capacity_(init.size()) {
        arr_ = new Type[init.size()];
        int i = 0;
        for (const auto& znschenie : init) {
            arr_[i] = znschenie;
            i++;
        }
    }

    SimpleVector(const SimpleVector& other) {
        if (this != &other) {
            delete[] arr_;
            arr_ = new Type[other.capacity_];
            for (size_t i = 0; i < other.size_; i++) {
                arr_[i] = other.arr_[i];
            }
            size_ = other.size_;
            capacity_ = other.capacity_;
        }
    }

    SimpleVector(SimpleVector&& other) {
        if (this != &other) {
            delete[] arr_;
            arr_ = other.arr_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            other.arr_ = nullptr;
            other.size_ = other.capacity_ = 0;
        }
    }

    SimpleVector& operator=(const SimpleVector& rhs) {
        if (this != &rhs) {
            delete[] arr_;
            arr_ = new Type[rhs.capacity_];
            for (int i = 0; i < static_cast<int>(rhs.size_); i++) {
                arr_[i] = rhs.arr_[i];
            }
            size_ = rhs.size_;
            capacity_ = rhs.capacity_;
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

    size_t GetSize() const noexcept {
        return size_;
    }

    size_t GetCapacity() const noexcept {
        return capacity_;
    }

    bool IsEmpty() const noexcept {
        return size_ == 0;
    }

    Type& operator[](size_t index) noexcept {
        return arr_[index];
    }

    const Type& operator[](size_t index) const noexcept {
        return arr_[index];
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
        if (new_size > capacity_) {
            //копируем все элементы прошлого массива
            Type* mass;
            mass = new Type[size_];
            size_t old_size = size_;
            for (int i = 0; i < static_cast<int>(size_); i++) {
                mass[i] = std::move(arr_[i]);
            }
            arr_ = new Type[new_size];
            size_ = new_size;
            capacity_ = new_size;
            //заполняем данными
            int i = 0;
            for (;i < static_cast<int>(old_size); i++) {
                arr_[i] = std::move(mass[i]);
            }
            for (;i < static_cast<int>(new_size); i++) {
                arr_[i] = Type();
            }
        }
        if (new_size <= capacity_ and new_size > size_) {
            //копируем все элементы прошлого массива
            Type* mass;
            mass = new Type[size_];
            size_t old_size = size_;
            for (int i = 0; i < static_cast<int>(size_); i++) {
                mass[i] = std::move(arr_[i]);
            }
            arr_ = new Type[new_size];
            size_ = new_size;
            //заполняем данными
            int i = 0;
            for (;i < static_cast<int>(old_size); i++) {
                arr_[i] = std::move(mass[i]);
            }
            for (;i < static_cast<int>(new_size); i++) {
                arr_[i] = Type();
            }
        }
    }

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

    ~SimpleVector() {
        delete[] arr_;
    }

    void PushBack(const Type& item) {
        if (capacity_ == 0 and size_ == 0) {
            capacity_ = 1;
            arr_ = new Type[capacity_];
            size_ = 1;
            arr_[0] = std::move(item);
        }
        else {
            if (size_ == capacity_) {
                capacity_ *= 2;
                Type* copy_arr = arr_;
                arr_ = new Type[capacity_];
                for (int i = 0; i < static_cast<int>(size_); ++i) {
                    arr_[i] = std::move(copy_arr[i]);
                }
                delete[] copy_arr;
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
        if (capacity_ == 0 and size_ == 0) {
            capacity_ = 1;
            arr_ = new Type[capacity_];
            size_ = 1;
            arr_[0] = std::move(item);
        }
        else {
            if (size_ == capacity_) {
                capacity_ *= 2;
                Type* copy_arr = arr_;
                arr_ = new Type[capacity_];
                for (int i = 0; i < static_cast<int>(size_); ++i) {
                    arr_[i] = std::move(copy_arr[i]);
                }
                delete[] copy_arr;
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
        //вектор полностью заполнен
        if (capacity_ == size_) {
            //находим номер куда надо вставить
            int shet = 0;
            for (auto it = this->begin(); it != this->end(); it++) {
                if (pos == it) {
                    break;
                }
                shet++;
            }
            //увеличиваем вместимость в 2 раза
            capacity_ *= 2;
            Type* copy_arr = arr_;
            arr_ = new Type[capacity_];
            for (int i = 0; i < static_cast<int>(size_); ++i) {
                arr_[i] = std::move(copy_arr[i]);
            }
            delete[] copy_arr;
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
            return &arr_[shet];
        }
        //когда вместимость больше размера
        if (capacity_ > size_) {
            int shet = 0;
            for (auto it = this->begin(); it != this->end(); it++) {
                if (pos == it) {
                    break;
                }
                shet++;
            }
            Type* back_pos = new Type[static_cast<int>(size_) - shet];
            int j = 0;
            for (int i = shet; i < static_cast<int>(size_);i++) {
                back_pos[j] = std::move(arr_[i]);
                j++;
            }
            size_++;
            arr_[shet] = std::move(value);
            int k = 0;
            for (int i = shet + 1; i < static_cast<int>(size_); i++) {
                arr_[i] = std::move(back_pos[k]);
                k++;
            }
            return &arr_[shet];
        }
        return this->begin();
    }

    Iterator Insert(ConstIterator pos, Type&& value) {
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
        //вектор полностью заполнен
        if (capacity_ == size_) {
            //находим номер куда надо вставить
            int shet = 0;
            for (auto it = this->begin(); it != this->end(); it++) {
                if (pos == it) {
                    break;
                }
                shet++;
            }
            //увеличиваем вместимость в 2 раза
            capacity_ *= 2;
            Type* copy_arr = arr_;
            arr_ = new Type[capacity_];
            for (int i = 0; i < static_cast<int>(size_); ++i) {
                arr_[i] = std::move(copy_arr[i]);
            }
            delete[] copy_arr;
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
            return &arr_[shet];
        }
        //когда вместимость больше размера
        if (capacity_ > size_) {
            int shet = 0;
            for (auto it = this->begin(); it != this->end(); it++) {
                if (pos == it) {
                    break;
                }
                shet++;
            }
            Type* back_pos = new Type[static_cast<int>(size_) - shet];
            int j = 0;
            for (int i = shet; i < static_cast<int>(size_);i++) {
                back_pos[j] = std::move(arr_[i]);
                j++;
            }
            size_++;
            arr_[shet] = std::move(value);
            int k = 0;
            for (int i = shet + 1; i < static_cast<int>(size_); i++) {
                arr_[i] = std::move(back_pos[k]);
                k++;
            }
            return &arr_[shet];
        }
        return this->begin();
    }

    // "Удаляет" последний элемент вектора. Вектор не должен быть пустым
    void PopBack() noexcept {
        if (!(this->IsEmpty())) {
            size_--;
        }
    }

    // Удаляет элемент вектора в указанной позиции
    Iterator Erase(ConstIterator pos) {
        if (!this->IsEmpty()) {
            size_t shet = 0;
            for (auto it = this->begin(); it != this->end(); it++) {
                if (pos == it) {
                    break;
                }
                shet++;
            }
            for (size_t i = shet + 1; i < size_; ++i) {
                arr_[i - 1] = std::move(arr_[i]);
            }
            --size_;
            return &arr_[shet];
        }
        return this->begin();
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
                tmp[i] = arr_[i];
            }
            arr_ = new Type[new_capacity];
            for (size_t i = 0; i < size_; i++) {
                arr_[i] = tmp[i];
            }
            capacity_ = new_capacity;
            delete[] tmp;
        }
    }

    SimpleVector(ReserveProxyObj size_class) {
        size_t new_capacity = size_class.siz;
        if (new_capacity > capacity_) {
            Type* tmp = new Type[size_];
            for (size_t i = 0; i < size_;i++) {
                tmp[i] = arr_[i];
            }
            arr_ = new Type[new_capacity];
            for (size_t i = 0; i < size_; i++) {
                arr_[i] = tmp[i];
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
    else {
        auto it_lhs = lhs.begin(), it_rhs = rhs.begin();
        for (;it_lhs != lhs.end();) {
            if ((*it_lhs) != (*it_rhs)) {
                return false;
            }
            it_lhs++;
            it_rhs++;
        }
    }
    return true;
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
