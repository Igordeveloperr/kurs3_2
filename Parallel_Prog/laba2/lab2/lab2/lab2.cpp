#include <iostream>
#include <vector>
#include <complex>
#include <chrono>
#include <random>
#include <cmath>
#include <thread>
#include <iomanip>
#include <omp.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std;

// прямое преобразование Фурье
vector<complex<double>> FFT(const vector<complex<double>>& vect) {
    int n = vect.size();
    // Выход из рекурсии - преобразование Фурье для одного элемента просто равно этому элементу
    if (n == 1) {
        return vector<complex<double>>(1, vect[0]);
    }

    // вектор для комплексных экспонент
    vector<complex<double>> w(n);
    // вычисление комплексных экспонент
    for (int i = 0; i < n; i++) {
        // α=2π⋅x/n - вычисление полярного угла
        double alpha = 2 * M_PI * i / n;
        // ωi=cosα+i⋅sinα - корень из 1
        w[i] = complex<double>(cos(alpha), sin(alpha));
    }

    // Считаем коэффициенты полинома A и B
    vector<complex<double>> A(n / 2); // четные
    vector<complex<double>> B(n / 2); // нечетные
    // заполнение вектороф коэффицентами полинома
    for (int i = 0; i < n / 2; i++) {
        A[i] = vect[i * 2];
        B[i] = vect[i * 2 + 1];
    }

    // получение коэффицентов Фурье
    vector<complex<double>> Av = FFT(A);
    vector<complex<double>> Bv = FFT(B);

    vector<complex<double>> res(n);
    for (int i = 0; i < n; i++) {
        // P(ωi)=A(ω2i)+ωi⋅B(ω2i) - значение преобразования Фурье для частоты ωi
        res[i] = Av[i % (n / 2)] + w[i] * Bv[i % (n / 2)];
    }

    return res;
}

// обратное преобразование Фурье
void IFFT(vector<complex<double>>& x) {
    int N = x.size();
    // реализация комплексного сопряжения -> a+ib = a-ib
    for (auto& el : x) el = conj(el);
    // Применяем FFT к комплексно-сопряженным элементам
    x = FFT(x);
    // Обратное комплексное сопряжение и нормализация
    for (auto& el : x) el = conj(el) / static_cast<double>(N);
}

// Вспомогательная функция для выполнения FFT в потоке
void ComputeFFT(const vector<complex<double>>& input, vector<complex<double>>& output) {
    output = FFT(input);
}

// умножение полиномов с применением быстрого преобразования
vector<double> FFTMult(const vector<double>& p1, const vector<double>& p2) {
    int n = p1.size() + p2.size() - 1;
    int size = 1;
    // корректировка длины к степени двойки
    while (size < n) size <<= 1;  // Даем размеры 2, 4, 8, 16, ...

    vector<complex<double>> f1(size), f2(size);

    // переход к комплексным числам
    for (size_t i = 0; i < p1.size(); ++i) {
        f1[i] = complex<double>(p1[i], 0);
    }
    for (size_t i = 0; i < p2.size(); ++i) {
        f2[i] = complex<double>(p2[i], 0);
    }

    // Выполнение FFT
    //vector<complex<double>> resF1 = FFT(f1);
    //vector<complex<double>> resF2 = FFT(f2);
    // Векторы для результатов FFT
    vector<complex<double>> resF1(size);
    vector<complex<double>> resF2(size);
    //resF1 = FFT(f1);
    //resF2 = FFT(f2);
        // Создаем два потока для параллельного выполнения FFT(f1) и FFT(f2)
    //std::thread thread1(ComputeFFT, cref(f1), ref(resF1));
    //std::thread thread2(ComputeFFT, cref(f2), ref(resF2));

    //// Ожидаем завершения потоков
    //thread1.join();
    //thread2.join();
    //#pragma omp parallel
    //{
    //    FFT(f1);
    //    resF1 = f1;
    //    #pragma omp critical (cout)
    //    {
    //        std::cout << "value 111111111" << std::endl;
    //    }
    //}
    //#pragma omp parallel
    //{
    //    FFT(f2);
    //    resF2 = f2;
    //    #pragma omp critical (cout)
    //    {
    //        std::cout << "value 2222222" << std::endl;
    //    }
    //}

    #pragma omp parallel
    {
        #pragma omp sections
        {
            #pragma omp section 
            {
                FFT(f1);
                resF1 = f1;
                #pragma omp critical (cout)
                {
                    std::cout << "value 111111111" << std::endl;
                }
            }
            #pragma omp section
            {
                FFT(f2);
                resF2 = f2;
                #pragma omp critical (cout)
                {
                    std::cout << "value 2222222" << std::endl;
                }
            }
        }
    }

    // Перемножение (суть ускорения
    #pragma omp parallel for
    for (int i = 0; i < size; ++i) {
        resF1[i] *= resF2[i];
    }

    // Выполнение IFFT
    IFFT(resF1);

    //Получение результата
    vector<double> result(n);
    for (int i = 0; i < n; ++i) {
        result[i] = round(resF1[i].real()); // Округляем до ближайшего целого
    }

    return result;
}

// перемножение полиномов без паралллельки
vector<double> MultPoly(vector<double>& p1, vector<double>& p2) {
    int n = p1.size() + p2.size() - 1;
    int size = 1;
    // корректировка длины к степени двойки
    while (size < n) size <<= 1;  // Даем размеры 2, 4, 8, 16, ...

    vector<complex<double>> f1(size), f2(size);

    // переход к комплексным числам
    for (size_t i = 0; i < p1.size(); ++i) {
        f1[i] = complex<double>(p1[i], 0);
    }
    for (size_t i = 0; i < p2.size(); ++i) {
        f2[i] = complex<double>(p2[i], 0);
    }

    // Выполнение FFT
    vector<complex<double>> resF1 = FFT(f1);
    vector<complex<double>> resF2 = FFT(f2);

    // Перемножение (суть ускорения)
    for (int i = 0; i < size; ++i) {
        resF1[i] *= resF2[i];
    }

    // Выполнение IFFT
    IFFT(resF1);

    //Получение результата
    vector<double> result(n);
    for (int i = 0; i < n; ++i) {
        result[i] = round(resF1[i].real()); // Округляем до ближайшего целого
    }

    return result;
}

// вывод итогового полинома
void PrintResult(vector<double>& data) {
    cout << "Mult result: ";
    for (size_t i = 0; i < data.size(); i++) {
        cout << data[i] << (i < data.size() - 1 ? " + " : " ");
        //if (data[i] >= 0)
        //{
        //    cout << data[i] << (i < data.size() - 1 ? " + " : " ");
        //}
        //else
        //{
        //    cout << data[i];
        //}

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


int main()
{
    // Входные данные
    const double LEN = pow(2, 22);
    vector<double> poly1;
    vector<double> poly2;
    poly1.resize(LEN);
    poly2.resize(LEN);

    // заполнение полиномов
    for (long i = 0; i < LEN; i++)
    {
        poly1[i] = generateRandomDouble(-10000000, 10000000);
        poly2[i] = generateRandomDouble(-10000000, 10000000);
    }

    cout << "gen end" << endl;

    // Умножение полиномов с применением быстрого преобразования 
    auto start = chrono::high_resolution_clock::now();
    vector<double> res = FFTMult(poly1, poly2);
    auto end = chrono::high_resolution_clock::now();
    cout << "FFT paral -> ";
    chrono::duration<double> t1 = PrintTime(end - start);
    //PrintResult(res);


    // Классическое умножение полиномов
    start = chrono::high_resolution_clock::now();
    res = MultPoly(poly1, poly2);
    end = chrono::high_resolution_clock::now();
    cout << "FFT def -> ";
    chrono::duration<double> t2 = PrintTime(end - start);
    //PrintResult(res);
    cout << "k = " << t2 / t1 << endl;
}
