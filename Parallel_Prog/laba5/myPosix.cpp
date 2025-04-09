#include "baseFunc.h"
#include <pvm3.h>
#include <pthread.h>

// Структура для передачи данных в поток
struct FFTData {
    vector<complex<double>>* input;
    vector<complex<double>>* output;
};
// функц для параллель FFT
void* FFTThread(void* arg) {
    FFTData* data = (FFTData*)arg;
    *(data->output) = FFT(*(data->input));
    return nullptr;
}

// Структура для передачи данных в поток
struct DoubleToComplexData {
    vector<double>* p;
    vector<complex<double>>* f;
    long start;
    long end;
};
// функнц для параллель перехода в комплексную плоскасть
void* DoubleToComplex(void* arg) {
    DoubleToComplexData* data = (DoubleToComplexData*)arg;
    for (long i = data->start; i < data->end; i++) {
        (*data->f)[i] = complex<double>((*data->p)[i], 0);
    }
    return nullptr;
}

// Структура для передачи данных в поток
struct MultData {
    vector<complex<double>>* resF1;
    vector<complex<double>>* resF2;
    long start;
    long end;
};
// функнц для параллель умножения
void* MultiplyVectors(void* arg) {
    MultData* data = (MultData*)arg;
    for (long i = data->start; i < data->end; ++i) {
        (*data->resF1)[i] *= (*data->resF2)[i];
    }
    return nullptr;
}

// Структура для передачи данных в поток
struct ResultData {
    vector<complex<double>>* resF1;
    vector<long>* result;
    long start;
    long end;
};
// Функция для параллель заполнения результата
void* FillResult(void* arg) {
    ResultData* data = (ResultData*)arg;
    for (long i = data->start; i < data->end; ++i) {
        (*data->result)[i] = std::round((*data->resF1)[i].real());
    }
    return nullptr;
}

// умножение полиномов с применением быстрого преобразования POSIX
vector<long> FFTMultPOSIX(vector<double>& p1, vector<double>& p2) {
    long n = p1.size() + p2.size() - 1;
    long size = 1;
    // корректировка длины к степени двойки
    while (size < n) size <<= 1;  // Даем размеры 2, 4, 8, 16, ...

    // массив потоков
    pthread_t threads[NUM_THREADS];

    DoubleToComplexData threadData[NUM_THREADS];
    vector<complex<double>> f1(size), f2(size);
    vector<complex<double>> resF1(size), resF2(size);
    vector<vector<complex<double>>*> complF = { &f1, &f2 };
    vector<vector<complex<double>>*> resF = { &resF1, &resF2 };

    // распараллеливание цикла перехода в комплексную плоскость
    for (int i = 0; i < NUM_THREADS; i++) {
        threadData[i].p = &p1;
        threadData[i].f = &f1;
        threadData[i].start = i * p1.size() / NUM_THREADS;
        threadData[i].end = (i == NUM_THREADS - 1) ? p1.size() : (i + 1) * p1.size() / NUM_THREADS;

        pthread_create(&threads[i], nullptr, DoubleToComplex, &threadData[i]);
    }
    // Ждем завершения всех потоков
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], nullptr);
    }

    // распараллеливание цикла перехода в комплексную плоскость
    for (int i = 0; i < NUM_THREADS; i++) {
        threadData[i].p = &p2;
        threadData[i].f = &f2;
        threadData[i].start = i * p2.size() / NUM_THREADS;
        threadData[i].end = (i == NUM_THREADS - 1) ? p2.size() : (i + 1) * p2.size() / NUM_THREADS;

        pthread_create(&threads[i], nullptr, DoubleToComplex, &threadData[i]);
    }
    // Ждем завершения всех потоков
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], nullptr);
    }

    // FFT
    FFTData fftData[NUM_THREAD];
    for (int i = 0; i < NUM_THREAD; i++)
    {
        fftData[i].input = complF[i];
        fftData[i].output = resF[i];
        pthread_create(&threads[i], nullptr, FFTThread, (void*)&fftData[i]);
    }
    for (int i = 0; i < NUM_THREAD; i++)
    {
        pthread_join(threads[i], nullptr);
    }

    // Перемножение (суть ускорения)
    MultData threadMultData[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) {
        threadMultData[i].resF1 = &resF1;
        threadMultData[i].resF2 = &resF2;
        threadMultData[i].start = i * size / NUM_THREADS;
        threadMultData[i].end = (i == NUM_THREADS - 1) ? size : (i + 1) * size / NUM_THREADS;

        pthread_create(&threads[i], nullptr, MultiplyVectors, &threadMultData[i]);
    }
    // Ждем завершения всех потоков
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], nullptr);
    }

    // Выполнение IFFT
    IFFT(resF1);

    //Получение результата
    vector<long> result(n);
    ResultData threadResData[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) {
        threadResData[i].resF1 = &resF1;
        threadResData[i].result = &result;
        threadResData[i].start = i * n / NUM_THREADS;
        threadResData[i].end = (i == NUM_THREADS - 1) ? n : (i + 1) * n / NUM_THREADS;

        pthread_create(&threads[i], nullptr, FillResult, &threadResData[i]);
    }
    // Ждем завершения всех потоков
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], nullptr);
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
    vector<long> res = FFTMultPOSIX(poly1, poly2);
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
    pvm_send(master_tid, POSIX_MESSAGE_TAG);

    pvm_exit();
}