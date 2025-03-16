#define HAVE_STRUCT_TIMESPEC
#include <iostream>
#include <vector>
#include <complex>
#include <chrono>
#include <random>
#include <cmath>
#include <iomanip>
#include <pthread.h>
#include <omp.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std;
const int NUM_THREAD = 2;


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

const int NUM_THREADS = 8;
const long LEN = pow(2, 25);
// умножение полиномов с применением быстрого преобразования
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

// умножение полиномов с применением быстрого преобразования
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
void PrintResult(vector<double>& data) {
    cout << "Mult result: ";
    for (size_t i = 0; i < data.size(); i++) {
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

int main()
{
    // Входные данные
    vector<double> poly1;
    vector<double> poly2;
    poly1.resize(LEN);
    poly2.resize(LEN);

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

    // Умножение полиномов с применением быстрого преобразования 
    auto start = chrono::high_resolution_clock::now();
    vector<long> res1 = FFTMultPOSIX(poly1, poly2);
    auto end = chrono::high_resolution_clock::now();
    std::cout << "FFT posix -> ";
    chrono::duration<double> t1 = PrintTime(end - start);


    start = chrono::high_resolution_clock::now();
    vector<long> res2 = FFTMultOpenMp(poly1, poly2);
    end = chrono::high_resolution_clock::now();
    std::cout << "FFT omp -> ";
    chrono::duration<double> t2 = PrintTime(end - start);


    // Классическое умножение полиномов
    start = chrono::high_resolution_clock::now();
    vector<long> res3 = MultPoly(poly1, poly2);
    end = chrono::high_resolution_clock::now();
    std::cout << "FFT line -> ";
    chrono::duration<double> t3 = PrintTime(end - start);


    std::cout << "k_posix = " << t3 / t1 << endl;
    std::cout << "k_omp = " << t3 / t2 << endl;

    checkAnswers(res1, res3);
}
