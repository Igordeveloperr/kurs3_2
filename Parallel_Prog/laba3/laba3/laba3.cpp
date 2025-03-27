#include <stdio.h>
#include <iostream>
#include <vector>
#include <complex>
#include <chrono>
#include <random>
#include <cmath>
#include <thread>
#include <iomanip>
#include <omp.h>
#include "mpi.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std;
// прямое преобразование Фурье
vector<complex<double>> FFT(const vector<complex<double>>& vect) {
    long n = vect.size();
    // Выход из рекурсии - преобразование Фурье для одного элемента просто равно этому элементу
    if (n == 1) {
        return vector<complex<double>>(1, vect[0]);
    }

    // вектор для комплексных экспонент
    vector<complex<double>> w(n);
    // вычисление комплексных экспонент
    for (long i = 0; i < n; i++) {
        // α=2π⋅x/n - вычисление полярного угла
        double alpha = 2 * M_PI * i / n;
        // ωi=cosα+i⋅sinα - корень из 1
        w[i] = complex<double>(cos(alpha), sin(alpha));
    }

    // Считаем коэффициенты полинома A и B
    vector<complex<double>> A(n / 2); // четные
    vector<complex<double>> B(n / 2); // нечетные
    // заполнение вектороф коэффицентами полинома

    for (long i = 0; i < n / 2; i++) {
        A[i] = vect[i * 2];
        B[i] = vect[i * 2 + 1];
    }

    // получение коэффицентов Фурье
    vector<complex<double>> Av = FFT(A);
    vector<complex<double>> Bv = FFT(B);

    vector<complex<double>> res(n);

    for (long i = 0; i < n; i++) {
        // P(ωi)=A(ω2i)+ωi⋅B(ω2i) - значение преобразования Фурье для частоты ωi
        res[i] = Av[i % (n / 2)] + w[i] * Bv[i % (n / 2)];
    }

    return res;
}

// обратное преобразование Фурье
void IFFT(vector<complex<double>>& x) {
    long N = x.size();
    // реализация комплексного сопряжения -> a+ib = a-ib
    for (long i = 0; i < N; i++) {
        x[i] = conj(x[i]);
    }
    // Применяем FFT к комплексно-сопряженным элементам
    x = FFT(x);
    // Обратное комплексное сопряжение и нормализация
    for (long i = 0; i < N; i++) {
        x[i] = conj(x[i]) / static_cast<double>(N);
    }
}

const int NUM_THREADS = 8;
const long LEN = pow(2, 20);
// умножение полиномов с применением быстрого преобразования mpi
vector<long> FFTMultMpi(const vector<double>& p1, const vector<double>& p2) {
    int rank, numtasks;
    // получение уникального id процесса
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // получение общего кол-ва процессов
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

    long n = p1.size() + p2.size() - 1;
    long size = 1;
    // корректировка длины к степени двойки
    while (size < n) size <<= 1; // Даем размеры 2, 4, 8, 16
    
    // распараллеливание циклов для перехода в комплексные числа
    vector<complex<double>> f1(size), f2(size);

    // размер блока данных для каждого процесса
    long local_size = p1.size() / numtasks;
    // остаток от деления общего размера на число процессов
    long remainder = p1.size() % numtasks;
    // Начальный индекс данных для текущего процесса
    long start = rank * local_size + ((rank < remainder) ? rank : remainder);
    // Конечный индекс данных для текущего процесса
    long end = start + local_size + (rank < remainder ? 1 : 0);
    // Фактический размер блока данных для текущего процесса
    local_size = end - start;
    // Локальный буфер для каждого процесса
    vector<complex<double>> local_f1(local_size);
    vector<complex<double>> local_f2(local_size);

    // каждый процесс переводит в комплексную плоскость свой набор данных
    for (long i = 0; i < local_size; i++) {
        local_f1[i] = complex<double>(p1[start + i], 0);
    }

    for (long i = 0; i < local_size; i++) {
        local_f2[i] = complex<double>(p2[start + i], 0);
    }

    // Вектор размеров принимаемых блоков от каждого процесса для массива f1
    vector<int> recv_countsF1(numtasks);
    // Вектор смещений для данных каждого процесса в итоговом массиве f1
    vector<int> displsF1(numtasks);
    // Вектор размеров принимаемых блоков от каждого процесса для массива f2
    vector<int> recv_countsF2(numtasks);
    // Вектор смещений для данных каждого процесса в итоговом массиве f2
    vector<int> displsF2(numtasks);

    //Собираем размеры локальных блоков со всех процессов в вектор recv_countsF1
    MPI_Gather(&local_size, 1, MPI_INT,
        recv_countsF1.data(), 1, MPI_INT,
        0, MPI_COMM_WORLD);

    //Собираем размеры локальных блоков со всех процессов в вектор recv_countsF2
    MPI_Gather(&local_size, 1, MPI_INT,
        recv_countsF2.data(), 1, MPI_INT,
        1, MPI_COMM_WORLD);

    // Вычисляем смещения
    if (rank == 0) {
        displsF1[0] = 0;
        for (int i = 1; i < numtasks; i++) {
            // Смещение для процесса i = смещение предыдущего процесса + его размер данных
            displsF1[i] = displsF1[i - 1] + recv_countsF1[i - 1];
        }
    }
    else if (rank == 1) {
        displsF2[0] = 0;
        for (int i = 1; i < numtasks; i++) {
            // Смещение для процесса i = смещение предыдущего процесса + его размер данных
            displsF2[i] = displsF2[i - 1] + recv_countsF2[i - 1];
        }
    }

    // Собираем распределенные данные из local_f1 всех процессов в единый массив f1 в процессе 0
    MPI_Gatherv(local_f1.data(), local_size, MPI_DOUBLE_COMPLEX,
        f1.data(), recv_countsF1.data(), displsF1.data(), MPI_DOUBLE_COMPLEX,
        0, MPI_COMM_WORLD);

    // Собираем распределенные данные из local_f2 всех процессов в единый массив f2 в процессе 1
    MPI_Gatherv(local_f2.data(), local_size, MPI_DOUBLE_COMPLEX,
        f2.data(), recv_countsF2.data(), displsF2.data(), MPI_DOUBLE_COMPLEX,
        1, MPI_COMM_WORLD);


    // Векторы для результатов FFT
    vector<complex<double>> resF1(size);
    vector<complex<double>> resF2(size);
    // Выполнение БПФ для полиномов
    if (rank == 0) {
        resF1 = FFT(f1); // Процесс 0 вычисляет resF1
    }
    else if (rank == 1) {
        resF2 = FFT(f2); // Процесс 1 вычисляет resF2
    }
    // Синхронизация процессов
    MPI_Barrier(MPI_COMM_WORLD);

    // Передача результатов между процессами
    if (rank == 0) {
        // получение сообщения
        MPI_Recv(resF2.data(), size, MPI_C_DOUBLE_COMPLEX, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    else if (rank == 1) {
        // передача сообщения
        MPI_Send(resF2.data(), size, MPI_C_DOUBLE_COMPLEX, 0, 0, MPI_COMM_WORLD);
    }

    // рассылка актуальных данных процессам с общим коммуникатором
    MPI_Bcast(resF1.data(), resF1.size(), MPI_C_DOUBLE_COMPLEX, 0, MPI_COMM_WORLD);
    MPI_Bcast(resF2.data(), resF2.size(), MPI_C_DOUBLE_COMPLEX, 0, MPI_COMM_WORLD);

    // размер блока данных для каждого процесса
    local_size = size / numtasks;
    // остаток от деления общего размера на число процессов
    remainder = size % numtasks;
    // Начальный индекс данных для текущего процесса
    start = rank * local_size + ((rank < remainder) ? rank : remainder);
    // Конечный индекс данных для текущего процесса
    end = start + local_size + (rank < remainder ? 1 : 0);
    // Фактический размер блока данных для текущего процесса
    local_size = end - start;
    

    // Локальный буфер для частичных результатов
    vector<complex<double>> local_resF(local_size);

    // Каждый процесс обрабатывает свою часть данных
    for (long i = 0; i < local_size; i++) {
        local_resF[i] = resF1[start + i] * resF2[start + i];
    }

    // Вектор размеров принимаемых блоков от каждого процесса
    vector<int> recv_counts(numtasks);
    // Вектор смещений для данных каждого процесса в итоговом массиве
    vector<int> displs(numtasks);

    // Собираем размеры локальных блоков со всех процессов в вектор recv_counts
    MPI_Gather(&local_size, 1, MPI_INT,
        recv_counts.data(), 1, MPI_INT,
        0, MPI_COMM_WORLD);

    // Вычисляем смещения на процессе 0
    if (rank == 0) {
        displs[0] = 0;
        for (int i = 1; i < numtasks; i++) {
            displs[i] = displs[i - 1] + recv_counts[i - 1];
        }
    }

    // Собираем распределенные данные из local_resF всех процессов в единый массив resF1 в процессе 0
    MPI_Gatherv(local_resF.data(), local_size, MPI_C_DOUBLE_COMPLEX,
        resF1.data(), recv_counts.data(), displs.data(),
        MPI_C_DOUBLE_COMPLEX,
        0, MPI_COMM_WORLD);

    // выдача результата
    if (rank == 0) {
        // Выполнение IFFT
        IFFT(resF1);

        //Получение результата
        vector<long> result(n);
        for (long i = 0; i < n; i++) {
            result[i] = round(resF1[i].real()); // Округляем до ближайшего целого
        }

        return result;
    }
    return vector<long>(); // Процесс 1 возвращает пустой вектор
}

// умножение полиномов с применением быстрого преобразования openMp
vector<long> FFTMultOpenMp(const vector<double>& p1, const vector<double>& p2) {
    long n = p1.size() + p2.size() - 1;
    long size = 1;
    // корректировка длины к степени двойки
    while (size < n) size <<= 1; // Даем размеры 2, 4, 8, 16

    vector<complex<double>> f1(size), f2(size);

#pragma omp parallel num_threads(NUM_THREADS)
    {
        // переход к комплексным числам
#pragma omp for
        for (long i = 0; i < p1.size(); ++i) {
            f1[i] = complex<double>(p1[i], 0);
        }

#pragma omp for
        for (long i = 0; i < p2.size(); ++i) {
            f2[i] = complex<double>(p2[i], 0);
        }
    }

    // Векторы для результатов FFT
    vector<complex<double>> resF1(size);
    vector<complex<double>> resF2(size);

#pragma omp parallel
    {
        // разделение работы между потоками
#pragma omp sections
        {
#pragma omp section
            {
                resF1 = FFT(f1);
            }
#pragma omp section
            {
                resF2 = FFT(f2);
            }
        }
    }

    // Перемножение (суть ускорения)
#pragma omp parallel num_threads(NUM_THREADS)
    {
#pragma omp for
        for (long i = 0; i < size; i++) {
            resF1[i] *= resF2[i];
        }
    }

    // Выполнение IFFT
    IFFT(resF1);

    //Получение результата
    vector<long> result(n);
#pragma omp parallel num_threads(NUM_THREADS)
    {
#pragma omp for
        for (long i = 0; i < n; i++) {
            result[i] = round(resF1[i].real()); // Округляем до ближайшего целого
        }
    }

    return result;
}

// перемножение полиномов без паралллельки
vector<long> MultPoly(vector<double>& p1, vector<double>& p2) {
    long n = p1.size() + p2.size() - 1;
    long size = 1;
    // корректировка длины к степени двойки
    while (size < n) size <<= 1;  // Даем размеры 2, 4, 8, 16, ...

    vector<complex<double>> f1(size), f2(size);

    // переход к комплексным числам
    for (long i = 0; i < p1.size(); ++i) {
        f1[i] = complex<double>(p1[i], 0);
    }
    for (long i = 0; i < p2.size(); ++i) {
        f2[i] = complex<double>(p2[i], 0);
    }
    // Выполнение FFT
    vector<complex<double>> resF1 = FFT(f1);
    vector<complex<double>> resF2 = FFT(f2);
    // Перемножение (суть ускорения)
    for (long i = 0; i < size; ++i) {
        resF1[i] *= resF2[i];
    }
    // Выполнение IFFT
    IFFT(resF1);
    //Получение результата
    vector<long> result(n);
    for (long i = 0; i < n; ++i) {
        result[i] = round(resF1[i].real()); // Округляем до ближайшего целого
    }
    return result;
}

// вывод итогового полинома
void PrintResult(vector<long>& data) {
    cout << "Mult result: ";
    for (long i = 0; i < data.size(); i++) {
        cout << data[i] << (i < data.size() - 1 ? " + " : " ");
    }
    cout << std::endl;
}

// вывод времени в секундах
chrono::duration<double> PrintTime(chrono::duration<double> time) {
    cout << "Mult time: " << time.count() << "s" << endl;
    return time;
}

// генерация случайных чисел типа double
double generateRandomDouble(double lower, double upper) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dis(lower, upper);
    return dis(gen);
}

// проверка результата
void checkAnswers(const vector<long> vect1, const vector<long> vect2) {
    bool allOk = true;
    long badNum1, badNum2;
    for (long i = 0; i < vect1.size(); i++) {
        if (vect1[i] != vect2[i]) {
            allOk = false;
            badNum1 = vect1[i];
            badNum2 = vect2[i];
            break;
        }
    }
    if (allOk) {
        cout << "Answer is Ok" << endl;
        return;
    }
    cout << "Bad answer!!!" << endl;
    cout << badNum1 << " != " << badNum2 << endl;
}

int main(int argc, char** argv)
{
    int numtasks, rank;
    // инициализация MPI
    MPI_Init(&argc, &argv);
    // номер потока который выполняет вычисления
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // получение кол-ва потоков, которые выполняют прогу
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

    // Входные данные
    vector<double> poly1;
    vector<double> poly2;
    poly1.resize(LEN);
    poly2.resize(LEN);

    if (rank == 0) {
        // заполнение полиномов
        #pragma omp parallel for schedule(static) num_threads(8)
        for (long i = 0; i < LEN; i++)
        {
            //poly1[i] = generateRandomDouble(1, 100);
            //poly2[i] = generateRandomDouble(1, 100);
            poly1[i] = 222;
            poly2[i] = 222;
        }
        std::cout << "gen end" << endl;
    }

    // Распространение данных на все процессы
    MPI_Bcast(poly1.data(), LEN, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(poly2.data(), LEN, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Умножение полиномов с применением MPI
    auto start = chrono::high_resolution_clock::now();
    vector<long> res1 = FFTMultMpi(poly1, poly2);
    auto end = chrono::high_resolution_clock::now();

    if (rank == 0) {
        std::cout << "FFT paral MPI -> ";
        chrono::duration<double> t1 = PrintTime(end - start);

        // Умножение полиномов с применением OpenMp
        start = chrono::high_resolution_clock::now();
        vector<long> res2 = FFTMultOpenMp(poly1, poly2);
        end = chrono::high_resolution_clock::now();
        std::cout << "FFT paral OpenMp -> ";
        chrono::duration<double> t2 = PrintTime(end - start);

        // Классическое умножение полиномов
        start = chrono::high_resolution_clock::now();
        vector<long> res3 = MultPoly(poly1, poly2);
        end = chrono::high_resolution_clock::now();
        std::cout << "FFT def -> ";
        chrono::duration<double> t3 = PrintTime(end - start);
        std::cout << "k_mpi = " << t3 / t1 << endl;
        std::cout << "k_openMp = " << t3 / t2 << endl;

        checkAnswers(res1, res3);
    }
    //printf("MPI thread = %d; total threads: %d\n", rank, numtasks);

    // завершение mpi
    MPI_Finalize();
}
