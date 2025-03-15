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

const int NUM_THREADS = 4;

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

// точное умножение полиномов
vector<double> MultPolyExactly(vector<double>& p1, vector<double>& p2) {
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
    vector<double> result(n);
    for (long i = 0; i < n; ++i) {
        result[i] = resF1[i].real(); // Округляем до ближайшего целого
    }
    return result;
}

// Функция для разбиения полинома на два меньших полинома
pair<vector<double>, vector<double>> splitPolynomial(const vector<double>& p, long k) {
    vector<double> low(p.begin(), p.begin() + k);
    vector<double> high(p.begin() + k, p.end());
    return { low, high };
}

// Структура для передачи данных в поток
struct FFTData4 {
    vector<double>* p1;
    vector<double>* p2;
    vector<double>* output;
    pthread_mutex_t* mutex; // Мьютекс для синхронизации
};

void* FFTThread4(void* arg) {
    FFTData4* data = (FFTData4*)arg;
    //pthread_mutex_lock(data->mutex);
    *(data->output) = MultPolyExactly(*(data->p1), *(data->p2));
    //pthread_mutex_unlock(data->mutex);
    return nullptr;
}

// Основная функция для умножения полиномов с разбиением
vector<long> FFTMultPOSIX4(const vector<double>& p1, const vector<double>& p2) {
    long n = p1.size();
    long k = n / 2;
    long size = 1;
    // корректировка длины к степени двойки
    while (size < n) size <<= 1;  // Даем размеры 2, 4, 8, 16, ...

    // Разбиваем полином p1 на две части: p1_low и p1_high
    pair<vector<double>, vector<double>> p1_split = splitPolynomial(p1, k);
    vector<double> p1_low = p1_split.first;   // Младшие коэффициенты
    vector<double> p1_high = p1_split.second; // Старшие коэффициенты

    // Разбиваем полином p2 на две части: p2_low и p2_high
    pair<vector<double>, vector<double>> p2_split = splitPolynomial(p2, k);
    vector<double> p2_low = p2_split.first;   // Младшие коэффициенты
    vector<double> p2_high = p2_split.second; // Старшие коэффициенты

    // инитим объекты синхронизации
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, nullptr);

    // Создаем структуры для передачи данных в потоки
    FFTData4 data1 = { &p1_low, &p2_low, new vector<double>(size), &mutex };
    FFTData4 data2 = { &p1_low, &p2_high, new vector<double>(size), &mutex };
    FFTData4 data3 = { &p1_high, &p2_low, new vector<double>(size), &mutex };
    FFTData4 data4 = { &p1_high, &p2_high, new vector<double>(size), &mutex };

    // Создаем потоки
    pthread_t thread1, thread2, thread3, thread4;
    pthread_create(&thread1, nullptr, FFTThread4, (void*)&data1);
    pthread_create(&thread2, nullptr, FFTThread4, (void*)&data2);
    pthread_create(&thread3, nullptr, FFTThread4, (void*)&data3);
    pthread_create(&thread4, nullptr, FFTThread4, (void*)&data4);

    // Ждем завершения потоков
    pthread_join(thread1, nullptr);
    pthread_join(thread2, nullptr);
    pthread_join(thread3, nullptr);
    pthread_join(thread4, nullptr);

    // Перемножаем меньшие полиномы
    vector<double> A = *(data1.output);
    vector<double> B = *(data2.output);;
    vector<double> C = *(data3.output);;
    vector<double> D = *(data4.output);;
    //vector<double> A = MultPolyExactly(p1_low, p2_low);
    //vector<double> B = MultPolyExactly(p1_low, p2_high);
    //vector<double> C = MultPolyExactly(p1_high, p2_low);
    //vector<double> D = MultPolyExactly(p1_high, p2_high);

    // Комбинируем результаты
    vector<double> fracResult(2 * n - 1, 0);
    for (long i = 0; i < A.size(); i++) fracResult[i] += A[i];
    for (long i = 0; i < B.size(); i++) fracResult[i + k] += B[i];
    for (long i = 0; i < C.size(); i++) fracResult[i + k] += C[i];
    for (long i = 0; i < D.size(); i++) fracResult[i + 2 * k] += D[i];

    vector<long> result(fracResult.size());
    for (long i = 0; i < fracResult.size(); ++i) {
        result[i] = round(fracResult[i]); // Округляем до ближайшего целого
    }
    return result;
}

// Структура для передачи данных в поток
struct FFTData {
    vector<complex<double>>* input;
    vector<complex<double>>* output;
    pthread_mutex_t* mutex; // Мьютекс для синхронизации
};

void* FFTThread(void* arg) {
    FFTData* data = (FFTData*)arg;
    //pthread_mutex_lock(data->mutex);
    *(data->output) = FFT(*(data->input));
    //pthread_mutex_unlock(data->mutex);
    return nullptr;
}

// умножение полиномов с применением быстрого преобразования
vector<long> FFTMultPOSIX(const vector<double>& p1, const vector<double>& p2) {
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

    // инитим объекты синхронизации
    pthread_mutex_t mutex1;
    pthread_mutex_t mutex2;
    pthread_mutex_init(&mutex1, nullptr);
    pthread_mutex_init(&mutex2, nullptr);

    // Создаем структуры для передачи данных в потоки
    FFTData data1 = { &f1, new vector<complex<double>>(size), &mutex1 };
    FFTData data2 = { &f2, new vector<complex<double>>(size), &mutex2 };

    // Создаем потоки
    pthread_t thread1, thread2;
    pthread_create(&thread1, nullptr, FFTThread, (void*)&data1);
    pthread_create(&thread2, nullptr, FFTThread, (void*)&data2);

    // Ждем завершения потоков
    pthread_join(thread1, nullptr);
    pthread_join(thread2, nullptr);

    // Получаем результаты БПФ
    vector<complex<double>> resF1 = *(data1.output);
    vector<complex<double>> resF2 = *(data2.output);

    // Освобождаем память
    pthread_mutex_destroy(&mutex1);
    pthread_mutex_destroy(&mutex2);
    delete data1.output;
    delete data2.output;

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
    const long LEN = pow(2, 17);
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
    vector<long> res1 = FFTMultPOSIX4(poly1, poly2);
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
