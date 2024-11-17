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

template<typename T=double>
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

    ~MatrixDense() { delete[] data; }

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

        MatrixDense result(_m, other._n); // создаем новую матрицу результата

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


};

template<typename T = double>
class MatrixDiagonal {
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

    ~MatrixDense() { delete[] data; }

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
}

int main()
{
    std::cout << "Hello World!\n";
}