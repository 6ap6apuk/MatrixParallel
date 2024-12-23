#include "MatrixDense.h"
#include "MatrixDiagonal.h"
#include "MatrixBlock.h"
#include <iostream>
#include <fstream>
#include <random>
#include <memory>

// Шаблонная функция для выполнения операций над матрицами
template <typename MatrixType>
void performMatrixOperations(const MatrixType& A, const MatrixType& B, const std::string& filename) {
    try {
        std::ofstream outfile(filename);
        if (!outfile) throw std::runtime_error("Не удалось открыть файл " + filename + " для записи.");

        // Операции
        auto C = A * B;
        auto D = A.elemMult(B);
        auto E = A.transpose();
        auto G = A;
        auto H = A;
        G += B;
        H -= B;

        // Запись в файл
        outfile << "Матрица A:\n";
        A.print(outfile);
        outfile << "\nМатрица B:\n";
        B.print(outfile);
        outfile << "\nМатрица C = A * B:\n";
        C->print(outfile);
        outfile << "\nМатрица D = A elemMult B:\n";
        D->print(outfile);
        outfile << "\nМатрица E = транспонированная(A):\n";
        E->print(outfile);
        outfile << "\nМатрица G = A + B:\n";
        G.print(outfile);
        outfile << "\nМатрица H = A - B:\n";
        H.print(outfile);

        // Произведение Кронекера только для диагональных матриц
        if constexpr (std::is_same<MatrixType, MatrixDiagonal<int>>::value) {
            std::cout << "Выполняется произведение Кронекера для диагональных матриц\n";
            auto kron = A.kroneckerProduct(B);
            outfile << "\nМатрица K = A ⊗ B (произведение Кронекера):\n";
            kron->print(outfile);
            delete kron; // Освобождение памяти
        }

        delete C;
        delete D;
        delete E;

    } catch (const std::exception& e) {
        std::cerr << "Ошибка при работе с файлом " << filename << ": " << e.what() << std::endl;
    }
}

int main() {
    try {
        // Генератор случайных чисел
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(-10, 10);

        std::cout << "=== MatrixDense ===\n";

        // Создаем и экспортируем случайные плотные матрицы A и B
        MatrixDense<int> A(10, 10);
        MatrixDense<int> B(10, 10);

        for (unsigned i = 0; i < A.rows(); ++i) {
            for (unsigned j = 0; j < A.cols(); ++j) {
                A(i, j) = dis(gen);
                B(i, j) = dis(gen);
            }
        }

        A.exportToFile("MatrixDense_1.txt");
        B.exportToFile("MatrixDense_2.txt");
        std::cout << "Матрицы 1 и 2 экспортированы в файлы MatrixA.txt и MatrixB.txt.\n";

        // Импортируем матрицы из файлов
        MatrixDense<int> A_imported(10, 10);
        MatrixDense<int> B_imported(10, 10);

        A_imported.importFromFile("MatrixDense_1.txt");
        B_imported.importFromFile("MatrixDense_2.txt");
        std::cout << "Матрицы 1 и 2 импортированы из файлов.\n";

        // Выполняем операции над импортированными матрицами
        performMatrixOperations(A_imported, B_imported, "MatrixDenseOperations.txt");

        std::cout << "Результаты операций для MatrixDense сохранены в файл MatrixDenseOperations.txt.\n";

        std::cout << "\n=== MatrixDiagonal ===\n";

        // Создаем случайные диагональные матрицы D1 и D2
        MatrixDiagonal<int> D1(10);
        MatrixDiagonal<int> D2(10);

        for (unsigned i = 0; i < D1.rows(); ++i) {
            D1(i, i) = dis(gen);
            D2(i, i) = dis(gen);
        }

        D1.exportToFile("MatrixDiagonal_1.txt");
        D2.exportToFile("MatrixDiagonal_2.txt");
        std::cout << "Диагональные матрицы экспортированы в файлы MatrixDiagonal1.txt и MatrixDiagonal2.txt.\n";

        // Импортируем диагональные матрицы
        MatrixDiagonal<int> D1_imported(10);
        MatrixDiagonal<int> D2_imported(10);
        for (unsigned i = 0; i < D1.rows(); ++i) {
                    D1(i, i) = dis(gen);
                    D2(i, i) = dis(gen);
                }

        D1_imported.importFromFile("MatrixDiagonal_1.txt");
        D2_imported.importFromFile("MatrixDiagonal_2.txt");
        std::cout << "Диагональные матрицы импортированы из файлов.\n";

        // Выполняем операции над диагональными матрицами
        performMatrixOperations(D1_imported, D2_imported, "MatrixDiagonalOperations.txt");

        std::cout << "Результаты операций для MatrixDiagonal сохранены в файл MatrixDiagonalOperations.txt.\n";

  std::cout << "\n=== MatrixBlock ===\n";

// Создаем первую блоковую матрицу размером 10x10 с блоками 2x2
        MatrixBlock<int> B1(5, 5, 2, 2); // 5x5 блоков, каждый размером 2x2
        for (unsigned i = 0; i < 5; ++i) {
            for (unsigned j = 0; j < 5; ++j) {
                auto block = std::make_shared<MatrixDense<int>>(2, 2);
                for (unsigned m = 0; m < 2; ++m) {
                    for (unsigned n = 0; n < 2; ++n) {
                        block->operator()(m, n) = dis(gen);
                    }
                }
                B1.setBlock(i, j, block);
            }
        }

        // Создаем вторую блоковую матрицу размером 10x10 с блоками 2x2
        MatrixBlock<int> B2(5, 5, 2, 2); // 5x5 блоков, каждый размером 2x2
        for (unsigned i = 0; i < 5; ++i) {
            for (unsigned j = 0; j < 5; ++j) {
                auto block = std::make_shared<MatrixDense<int>>(2, 2);
                for (unsigned m = 0; m < 2; ++m) {
                    for (unsigned n = 0; n < 2; ++n) {
                        block->operator()(m, n) = dis(gen);
                    }
                }
                B2.setBlock(i, j, block);
            }
        }

        // Экспортируем обе блоковые матрицы
        B1.exportToFile("MatrixBlock_1.txt");
        B2.exportToFile("MatrixBlock_2.txt");
        std::cout << "Блоковые матрицы экспортированы в файлы MatrixBlock1.txt и MatrixBlock2.txt.\n";

        // Импортируем блоковые матрицы
        MatrixBlock<int> B1_imported(5, 5, 2, 2);
        MatrixBlock<int> B2_imported(5, 5, 2, 2);

        B1_imported.importFromFile("MatrixBlock_1.txt");
        B2_imported.importFromFile("MatrixBlock_2.txt");
        std::cout << "Блоковые матрицы импортированы из файлов.\n";

        // Выполняем операции над импортированными блоковыми матрицами
        performMatrixOperations(B1_imported, B2_imported, "MatrixBlockOperations.txt");

        std::cout << "Результаты операций для MatrixBlock сохранены в файл MatrixBlockOperations.txt.\n";

    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }

    return 0;
}