#include "Array.h"
#include <iostream>

using namespace std;

Array::Array() {
}

Array::~Array() {
    delete[] arr;
}

void Array::create(int newLen) {

    if (len < 0) {
        cout << "! | Размерность массива можно создать только из неотрицательных чисел!" << endl;
        this->len = 0;
    }
    if (arr != nullptr) {
        delete[] arr;
    }

    this->len = newLen;
    arr = new number[len];

}

void Array::printAll() {
    for (int i = 0; i < len; i++) {
        cout << *(arr + i) << " ";
    }

    cout << endl;
}

void Array::printAvgAndSKO() {
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

    cout << "# | Среднее значение массива: " << avg << " СКО равно: " << sko;
}

int Array::printLen() {
    return this->len;
}

void Array::fill() {
    for (int i = 0; i < len; i++) {
        cin >> *(arr + i);
    }
}

void Array::sort(bool ascending) {
    for (int i = 0; i < len - 1; ++i) {
        for (int j = 0; j < len - i - 1; ++j) {
            if ((ascending && arr[j] > arr[j + 1]) || (!ascending && arr[j] < arr[j + 1])) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}
void Array::changeElem(int index, number newValue) {
    if (index < 0 || index >= len) {
        cout << "! | Индекс выходит за границы массива! " << endl;
        return;
    }
    arr[index] = newValue;
}

void Array::resize(int newLen) {
    if (newLen < 0) {
        cout << "! | Размерность массива можно создать только из неотрицательных чисел!" << endl;
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
number Array::getElement(int index) const {
    if (index < 0 || index >= len) {
        cout << "! | Индекс выходит за границы массива!" << endl;
        return number(0);
    }
    return arr[index];
}
