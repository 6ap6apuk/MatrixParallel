#include <iostream>
#include <fstream>
#include <ctime>
#include <chrono>
#include <vector>
#include <limits>
#include <iomanip>
#include <thread>
#include <mutex>
#include <stdexcept>
#include <numeric>

using namespace std;

template<typename T = double>
class MatrixDense {
private:
    unsigned _m, _n;
    T* data;
    bool isInitialized = false;
public:
    MatrixDense(unsigned m, unsigned n) : _m(m), _n(n) {
        try {
            data = new T[m * n];
        }
        catch (const std::bad_alloc&) {
            std::cerr << "Недостаточно памяти для создания массива." << std::endl;
        }
    }

    ~MatrixDense() {
        if (data != nullptr) {
            delete[] data;
            data = nullptr;
        }
    }

    void initialize(double min, double max) {
        if (min >= max) {
            cout << "Ошибка: значение min должно быть меньше max." << std::endl;
            return;
        }

        for (size_t i = 0; i < _m; i++) {
            for (size_t j = 0; j < _n; j++)
            {
                data[i][j] = (T)(rand()) / RAND_MAX * (max - min) + min;
            }
        }
        isInitialized = true;
    }

    void sumMatrix(MatrixDense bMatrix) {
        if (_m == bMatrix._m && _n == bMatrix._n)
        {
            for (size_t i = 0; i < _m; i++) {
                for (size_t j = 0; j < _n; j++)
                {
                    data[i][j] += bMatrix[i][j];
                }
            }
        }
    }

    void subMatrix(MatrixDense bMatrix) {
        if (_m == bMatrix._m && _n == bMatrix._n)
        {
            for (size_t i = 0; i < _m; i++) {
                for (size_t j = 0; j < _n; j++)
                {
                    data[i][j] -= bMatrix[i][j];
                }
            }
        }
    }

    void multByAMatrix(double A) {
        for (size_t i = 0; i < _m; i++) {
            for (size_t j = 0; j < _n; j++)
            {
                data[i][j] *= A;
            }
        }
    }

    MatrixDense<T> multMatrix(const MatrixDense& other) const {
        if (_n != other._m) {
            throw std::invalid_argument("Матрицы несовместимы для умножения.");
        }

        MatrixDense result(_m, other._n);

        for (unsigned i = 0; i < _m; ++i) {
            for (unsigned j = 0; j < other._n; ++j) {
                T sum = 0;
                for (unsigned k = 0; k < _n; ++k) {
                    sum += (*this)(i, k) * other(k, j);
                }
                result(i, j) = sum;
            }
        }

        return result;
    }

    void transpondMatrix() {
        T element = 0;
        for (unsigned i = 0; i < _m; i++) {
            for (unsigned j = 0; j < _n; j++) {
                if (data[i][j] != data[j][i]) {
                    element = data[i][j];
                    data[i][j] = data[j][i];
                    data[j][i] = element;
                }
            }
        }
    }
};

template<typename T = double>
class MatrixDiagonal {
private:
    unsigned _m, _n;
    T* data;
    bool isInitialized = false;
public:
    MatrixDiagonal(unsigned m, unsigned n) : _m(m), _n(n) {
        try {
            data = new T[m * n];

        }
        catch (const std::bad_alloc&) {
            std::cerr << "Недостаточно памяти для создания массива." << std::endl;
        }
    }

    ~MatrixDiagonal() {
        if (data != nullptr) {
            delete[] data;
            data = nullptr;
        }
    }

    void initialize(T min, T max) {
        if (min >= max) {
            std::cout << "Ошибка: значение min должно быть меньше max." << std::endl;
            return;
        }

        srand(static_cast<unsigned int>(time(nullptr)));  // Инициализация генератора случайных чисел

        for (unsigned i = 0; i < _m; ++i) {
            for (unsigned j = 0; j < _n; ++j) {
                if (i == j) {
                    data[i * _n + j] = static_cast<T>(rand()) / RAND_MAX * (max - min) + min;
                }
                else {
                    data[i * _n + j] = static_cast<T>(0);  // Остальные элементы обнуляем
                }
            }
        }

        isInitialized = true;
    }


    bool isDiagonal() const {
        if (!isInitialized) return false;

        for (unsigned i = 0; i < _m; ++i) {
            for (unsigned j = 0; j < _n; ++j) {
                if (i != j && data[i * _n + j] != static_cast<T>(0)) {
                    return false;
                }
            }
        }
        return true;
    }

    void toOneDimensionalArray() {
        if (!isDiagonal()) {
            std::cerr << "Матрица не является диагональной!" << std::endl;
            return nullptr;
        }

        T* diagonalArray = new T[std::min(_m, _n)];
        for (unsigned i = 0; i < std::min(_m, _n); ++i) {
            diagonalArray[i] = data[i * (_n + 1)];  // Копирование элементов главной диагонали
        }
        delete[] data;
        data = diagonalArray;
    }

    void sumMatrix(MatrixDiagonal bMatrix) {
        if (_m == bMatrix._m)
        {
            for (size_t i = 0; i < _m; i++) {
                data[i] += bMatrix[i];
            }
        }
    }

    void subMatrix(MatrixDiagonal bMatrix) {
        if (_m == bMatrix._m)
        {
            for (size_t i = 0; i < _m; i++) {
                data[i] -= bMatrix[i];
            }
        }
    }

    void multByAMatrix(double A) {
        for (size_t i = 0; i < _m; i++) {
            data[i] *= A;
        }
    }

    MatrixDiagonal<T> multMatrix(const MatrixDiagonal& other) const {
        if (_n != other._m) {
            throw std::invalid_argument("Матрицы несовместимы для умножения.");
        }

        MatrixDense result(_m, other._n);

        for (unsigned i = 0; i < _m; ++i) {
            for (unsigned j = 0; j < other._n; ++j) {
                T sum = 0;
                for (unsigned k = 0; k < _n; ++k) {
                    sum += (*this)(i, k) * other(k, j);
                }
                result(i, j) = sum;
            }
        }

        return result;
    }

    //    T* transpondMatrix() {
    //        T* newArray = new T[_m];
    //        T element = 0;
    //        for(unsigned i = 0; i < _m; i++) {
    //                element = data[i][j];
    //                data[i][j] = data[j][i];
    //                data[j][i] = element;
    //        }
    //        return newArray;
    //    }
};

template<typename T = double>
class MatrixBlock {
private:
    unsigned _m, _n;
    T** data;
    bool isInitialized = false;
public:
    MatrixBlock(unsigned m, unsigned n) : _m(m), _n(n) {
        data = new T * [_m];
        for (unsigned i = 0; i < _m; ++i) {
            data[i] = new T[_n];
            for (unsigned j = 0; j < _n; ++j) {
                data[i][j] = nullptr;
            }
        }
    }

    ~MatrixBlock() {
        for (unsigned i = 0; i < _m; ++i) {
            delete[] data[i];
        }
        delete[] data;
        data = nullptr;
    }

    void initialize(T min, T max) {
        if (min >= max) {
            cout << "Ошибка: значение min должно быть меньше max." << std::endl;
            return;
        }

        for (size_t i = 0; i < _m; i++) {
            for (size_t j = 0; j < _n; j++)
            {
                data[i][j] = (T)(rand()) / RAND_MAX * (max - min) + min;
            }
        }
        isInitialized = true;
    }

    void sumMatrix(const MatrixBlock<T>& bMatrix) {
        if (_m == bMatrix._m && _n == bMatrix._n) {
            for (size_t i = 0; i < _m; i++) {
                for (size_t j = 0; j < _n; j++) {
                    if (data[i][j] != nullptr && bMatrix.data[i][j] != nullptr) {
                        *data[i][j] += *bMatrix.data[i][j]; // Сложить только если оба указателя не пустые
                    }
                    else if (data[i][j] == nullptr && bMatrix.data[i][j] != nullptr) {
                        data[i][j] = new T(*bMatrix.data[i][j]); // Копировать значение из второй матрицы
                    }
                }
            }
        }
    }

    void subMatrix(const MatrixBlock<T>& bMatrix) {
        if (_m == bMatrix._m && _n == bMatrix._n) {
            for (size_t i = 0; i < _m; i++) {
                for (size_t j = 0; j < _n; j++) {
                    if (data[i][j] != nullptr && bMatrix.data[i][j] != nullptr) {
                        *data[i][j] -= *bMatrix.data[i][j]; // Вычитаем значения
                    }
                    else if (data[i][j] == nullptr && bMatrix.data[i][j] != nullptr) {
                        data[i][j] = new T(-(*bMatrix.data[i][j])); // Учитываем отрицание
                    }
                }
            }
        }
    }

    void multByAMatrix(double A) {
        for (size_t i = 0; i < _m; i++) {
            for (size_t j = 0; j < _n; j++) {
                if (data[i][j] != nullptr) {
                    *data[i][j] *= A;
                }
            }
        }
    }

    MatrixBlock<T> multMatrix(const MatrixBlock<T>& other) const {
        if (_n != other._m) {
            throw std::invalid_argument("Матрицы несовместимы для умножения.");
        }

        MatrixBlock<T> result(_m, other._n);

        for (unsigned i = 0; i < _m; ++i) {
            for (unsigned j = 0; j < other._n; ++j) {
                T sum = 0;
                for (unsigned k = 0; k < _n; ++k) {
                    if (data[i][k] != nullptr && other.data[k][j] != nullptr) {
                        sum += (*data[i][k]) * (*other.data[k][j]);
                    }
                }
                if (sum != 0) {
                    result.data[i][j] = new T(sum);
                }
                else {
                    result.data[i][j] = nullptr;
                }
            }
        }

        return result;
    }

    void transpondMatrix() {
        MatrixBlock<T> temp(_n, _m);

        for (unsigned i = 0; i < _m; ++i) {
            for (unsigned j = 0; j < _n; ++j) {
                if (data[i][j] != nullptr) {
                    temp.data[j][i] = new T(*data[i][j]);
                }
                else {
                    temp.data[j][i] = nullptr;
                }
            }
        }

        swap(_m, temp._m);
        swap(_n, temp._n);

        for (unsigned i = 0; i < _m; ++i) {
            delete[] data[i];
        }
        delete[] data;

        data = temp.data;
    }

};

int main()
{
    std::cout << "Hello World!\n";
    return 0;
};