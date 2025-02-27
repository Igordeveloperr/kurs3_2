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

//vector<complex<double>> FFT(const vector<complex<double>>& as) {
//    long n = as.size();
//    long k = 0; // Длина n в битах
//    while ((1 << k) < n) k++;
//    vector<long> rev(n);
//    rev[0] = 0;
//    long high1 = -1;
//    for (long i = 1; i < n; i++) {
//        if ((i & (i - 1)) == 0) // Проверка на степень двойки. Если i ей является, то i-1 будет состоять из кучи единиц.
//            high1++;
//        rev[i] = rev[i ^ (1 << high1)]; // Переворачиваем остаток
//        rev[i] |= (1 << (k - high1 - 1)); // Добавляем старший бит
//    }
//
//    vector<complex<double>> roots(n);
//    for (long i = 0; i < n; i++) {
//        double alpha = 2 * M_PI * i / n;
//        roots[i] = complex<double>(cos(alpha), sin(alpha));
//    }
//
//    vector<complex<double>> cur(n);
//    for (long i = 0; i < n; i++)
//        cur[i] = as[rev[i]];
//
//    for (long len = 1; len < n; len <<= 1) {
//        vector<complex<double>> ncur(n);
//        long rstep = roots.size() / (len * 2);
//        for (long pdest = 0; pdest < n;) {
//            long p1 = pdest;
//            for (long i = 0; i < len; i++) {
//                complex<double> val = roots[i * rstep] * cur[p1 + len];
//                ncur[pdest] = cur[p1] + val;
//                ncur[pdest + len] = cur[p1] - val;
//                pdest++, p1++;
//            }
//            pdest += len;
//        }
//        cur.swap(ncur);
//    }
//    return cur;
//}

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
    for (auto& el : x) el = conj(el);
    // Применяем FFT к комплексно-сопряженным элементам
    x = FFT(x);
    // Обратное комплексное сопряжение и нормализация
    for (auto& el : x) el = conj(el) / static_cast<double>(N);
}

// умножение полиномов с применением быстрого преобразования 2 потока
vector<long> FFTMult(const vector<double>& p1, const vector<double>& p2) {
    long n = p1.size() + p2.size() - 1;
    long size = 1;
    // корректировка длины к степени двойки
    while (size < n) size <<= 1; // Даем размеры 2, 4, 8, 16

    vector<complex<double>> f1(size), f2(size);

    // переход к комплексным числам
    for (long i = 0; i < p1.size(); ++i) {
        f1[i] = complex<double>(p1[i], 0);
    }
    for (long i = 0; i < p2.size(); ++i) {
        f2[i] = complex<double>(p2[i], 0);
    }

    // Векторы для результатов FFT
    vector<complex<double>> resF1(size);
    vector<complex<double>> resF2(size);

    omp_set_num_threads(2);
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
    #pragma omp parallel for
    for (long i = 0; i < size; i++) {
        resF1[i] *= resF2[i];
    }

    // Выполнение IFFT
    IFFT(resF1);

    //Получение результата
    vector<long> result(n);
    #pragma omp parallel for
    for (long i = 0; i < n; i++) {
        result[i] = round(resF1[i].real()); // Округляем до ближайшего целого
    }

    return result;
}

// Умножение полиномов с применением быстрого преобразования (4 потока)
vector<long> FFTMult4(const vector<double>& p1, const vector<double>& p2, const vector<double>& p3, const vector<double>& p4) {
    long n = p1.size() + p2.size() + p3.size() + p4.size() - 1;
    long size = 1;
    // корректировка длины к степени двойки
    while (size < n) size <<= 1; // Даем размеры 2, 4, 8, 16

    vector<complex<double>> f1(size/2), f2(size/2), f3(size/2), f4(size/2);

    // переход к комплексным числам
    for (long i = 0; i < p1.size(); ++i) {
        f1[i] = complex<double>(p1[i], 0);
    }
    for (long i = 0; i < p2.size(); ++i) {
        f2[i] = complex<double>(p2[i], 0);
    }
    for (long i = 0; i < p3.size(); ++i) {
        f3[i] = complex<double>(p3[i], 0);
    }
    for (long i = 0; i < p4.size(); ++i) {
        f4[i] = complex<double>(p4[i], 0);
    }

    // Векторы для результатов FFT
    vector<complex<double>> resF1(size);
    vector<complex<double>> resF2(size);
    vector<complex<double>> resF3(size);
    vector<complex<double>> resF4(size);

    omp_set_num_threads(4);
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
            #pragma omp section
            {
                resF3 = FFT(f3);
            }
            #pragma omp section
            {
                resF4 = FFT(f4);
            }
        }
    }

    // Предположим, что resF1 и resF3 имеют размер size / 2
    vector<complex<double>> resF_cmb1(size);
    vector<complex<double>> resF_cmb2(size);

    // Объединение resF1 и resF3 в resF_combined
    //#pragma omp parallel for
    for (long i = 0; i < size / 2; i++) {
        resF_cmb1[i] = resF1[i];          // Первая половина (resF1)
        resF_cmb1[i + size / 2] = resF3[i]; // Вторая половина (resF3)
    }

    //#pragma omp parallel for
    for (long i = 0; i < size / 2; i++) {
        resF_cmb2[i] = resF2[i];          // Первая половина (resF1)
        resF_cmb2[i + size / 2] = resF4[i]; // Вторая половина (resF3)
    }

    // Перемножение (суть ускорения)
    //#pragma omp parallel for
    for (long i = 0; i < size; i++) {
        resF_cmb1[i] *= resF_cmb2[i];
    }

    // Выполнение IFFT
    IFFT(resF_cmb1);

    //Получение результата
    vector<long> result(n);
    //#pragma omp parallel for
    for (long i = 0; i < n; i++) {
        result[i] = round(resF_cmb1[i].real()); // Округляем до ближайшего целого
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
    const long LEN = pow(2, 16);
    //vector<double> poly1 = { 1, 2 };
    //vector<double> poly2 = { 5, 6 };
    //vector<double> poly3 = { 3, 4 };
    //vector<double> poly4 = { 7, 8 };
    vector<double> poly1;
    vector<double> poly2;
    poly1.resize(LEN);
    poly2.resize(LEN);

    // заполнение полиномов
    for (long i = 0; i < LEN; i++)
    {
        poly1[i] = generateRandomDouble(1, 100);
        poly2[i] = generateRandomDouble(1, 100);
    }

    cout << "gen end" << endl;

    // Умножение полиномов с применением быстрого преобразования 
    auto start = chrono::high_resolution_clock::now();
    vector<long> res1 = FFTMult(poly1, poly2);
    auto end = chrono::high_resolution_clock::now();
    cout << "FFT paral -> ";
    chrono::duration<double> t1 = PrintTime(end - start);


    // Классическое умножение полиномов
    start = chrono::high_resolution_clock::now();
    vector<long> res2 = MultPoly(poly1, poly2);
    end = chrono::high_resolution_clock::now();
    cout << "FFT def -> ";
    chrono::duration<double> t2 = PrintTime(end - start);
    cout << "k = " << t2 / t1 << endl;

    checkAnswers(res1, res2);
}
