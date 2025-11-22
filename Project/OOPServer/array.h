#pragma once
#include <string>
#include <iostream>

template <class number>
class Array
{
public:
    Array();
    ~Array();
    void create(int newLen);
    void printAll();
    void fill();
    void printAvgAndSKO();
    void sort(bool ascending);
    void changeElem(int index, number newValue);
    void resize(int newLen);
    int printLen();
    number getElement(int index) const;

    std::string getArrayAsString() const;
    std::string getStatsAsString() const;
    bool isValidIndex(int index) const;

private:
    int len = 0;
    number* arr = nullptr;
};


template <class number>
Array<number>::Array() {
}

template <class number>
Array<number>::~Array() {
    delete[] arr;
}

template <class number>
void Array<number>::create(int newLen) {
    if (newLen < 0) {
        std::cout << "! | Размерность массива можно создать только из неотрицательных чисел!" << std::endl;
        this->len = 0;
        return;
    }
    if (arr != nullptr) {
        delete[] arr;
    }

    this->len = newLen;
    arr = new number[len];
}

template <class number>
void Array<number>::printAll() {
    for (int i = 0; i < len; i++) {
        std::cout << *(arr + i) << " ";
    }
    std::cout << std::endl;
}

template <class number>
void Array<number>::printAvgAndSKO() {
    number sum = 0;
    number avg = 0;
    number sko = 0;

    for (int i = 0; i < len; i++) {
        sum += *(arr + i);
    }

    avg = sum / this->len;

    for (int i = 0; i < len; i++) {
        sko += pow(*(arr + i) - avg, 2);
    }

    sko /= len - 1;
    sko = pow(sko, 0.5);

    std::cout << "# | Среднее значение массива: " << avg << " СКО равно: " << sko;
}

template <class number>
int Array<number>::printLen() {
    return this->len;
}

template <class number>
void Array<number>::fill() {
    for (int i = 0; i < len; i++) {
        std::cin >> *(arr + i);
    }
}

template <class number>
void Array<number>::sort(bool ascending) {
    for (int i = 0; i < len - 1; ++i) {
        for (int j = 0; j < len - i - 1; ++j) {
            if ((ascending && arr[j] > arr[j + 1]) || (!ascending && arr[j] < arr[j + 1])) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}

template <class number>
void Array<number>::changeElem(int index, number newValue) {
    if (index < 0 || index >= len) {
        std::cout << "! | Индекс выходит за границы массива! " << std::endl;
        return;
    }
    arr[index] = newValue;
}

template <class number>
void Array<number>::resize(int newLen) {
    if (newLen < 0) {
        std::cout << "! | Размерность массива можно создать только из неотрицательных чисел!" << std::endl;
        return;
    }
    number* newArr = new number[newLen];
    int minLen = (newLen < len) ? newLen : len;

    for (int i = 0; i < minLen; ++i) {
        newArr[i] = arr[i];
    }
    for (int i = minLen; i < newLen; ++i) {
        newArr[i] = 0;
    }

    this->len = newLen;

    delete[] arr;
    arr = newArr;
}

template <class number>
number Array<number>::getElement(int index) const {
    if (index < 0 || index >= len) {
        std::cout << "! | Индекс выходит за границы массива!" << std::endl;
        return number(0);
    }
    return arr[index];
}
