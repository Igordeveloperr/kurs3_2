#include "baseFunc.h"
#include <pvm3.h>
#include <omp.h>

// умножение полиномов с применением быстрого преобразования OpenMP
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

int main(int argc, char** argv){
    // ждем передачу полиномов от мастера
    pvm_recv(-1, INPUT_DATA_MESSAGE_TAG);
    // распаковываем полученные данные
    vector<double> poly1(LEN), poly2(LEN);
    pvm_upkdouble(poly1.data(), LEN, 1);
    pvm_upkdouble(poly2.data(), LEN, 1);

    auto start = chrono::high_resolution_clock::now();
    vector<long> res = FFTMultOpenMp(poly1, poly2);
    auto end = chrono::high_resolution_clock::now();

    // Получаем длительность в секундах как double
    chrono::duration<double> elapsed = end - start;
    double calcTime = elapsed.count();

    // Получаем TID мастера
    int master_tid = pvm_parent();

    // Получаем собственный TID
    int my_tid = pvm_mytid();

    // инитим передачу сообщения
    pvm_initsend(PvmDataDefault);

    // Упаковка данных
    pvm_pkdouble(&calcTime, 1, 1);

    // Отправка сообщения мастеру
    pvm_send(master_tid, OMP_MESSAGE_TAG);

    pvm_exit();
}