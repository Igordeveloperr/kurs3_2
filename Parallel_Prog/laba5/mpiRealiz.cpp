#include "baseFunc.h"
#include <mpi/mpi.h>

// умножение полиномов с применением быстрого преобразования MPI
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

    // выдача результата
    if (rank == 0) {
        for (long i = 0; i < size; i++) {
            resF1[i] *= resF2[i];
        }

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

int main(int argc, char** argv)
{
    // const long LEN = pow(2, 20);
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
        for (long i = 0; i < LEN; i++)
        {
            // poly1[i] = generateRandomDouble(1, 100);
            // poly2[i] = generateRandomDouble(1, 100);
            poly1[i] = 111;
            poly2[i] = 222;
        }
        std::cout << "gen end" << endl;
    }

    // Распространение данных на все процессы
    MPI_Bcast(poly1.data(), LEN, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(poly2.data(), LEN, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    auto start = chrono::high_resolution_clock::now();
    FFTMultMpi(poly1, poly2);
    auto end = chrono::high_resolution_clock::now();

    // завершение mpi
    MPI_Finalize();
    return 0;
}