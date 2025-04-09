#define NUM_THREAD 2
#include <stdio.h>
#include <iostream>
#include <vector>
#include <complex>
#include <string>
#include <chrono>
#include <random>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <cstdlib>

#define M_PI 3.14159265358979323846

using namespace std;

extern const int FFT_WORKERS_CNT = 2;

extern const int LINE_MESSAGE_TAG = 1;
extern const int PVM_MESSAGE_TAG = 2;
extern const int MPI_MESSAGE_TAG = 3;
extern const int POSIX_MESSAGE_TAG = 4;
extern const int OMP_MESSAGE_TAG = 5;
extern const int FFT_WORKER1_MESSAGE_TAG = 21;
extern const int FFT_WORKER2_MESSAGE_TAG = 22;
extern const int FFT_WORKER_RES_MESSAGE_TAG = 23;
extern const int INPUT_DATA_MESSAGE_TAG = 200;

extern const int NUM_THREADS = 4;
extern const long LEN = pow(2, 20);

extern const string BASE_PATH = "/home/gokhlia/MyCpp/testHello/";

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

// вывод итогового полинома
void PrintResult(vector<long>& data) {
    cout << "Mult result: ";
    for (long i = 0; i < data.size(); i++) {
        cout << data[i] << (i < data.size() - 1 ? " + " : " ");
    }
    cout << std::endl;
}

// Шаблонная функция для вывода вектора любого типа
template <typename T>
void PrintVector(const std::vector<T>& vec, const std::string& separator = " + ") {
    std::cout << "Vector elements: ";
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i];
        if (i < vec.size() - 1) {
            std::cout << separator;
        }
    }
    std::cout << std::endl;
}

bool checkResults(const string& file1, const string& file2) {
    ifstream f1(file1), f2(file2);
    
    if (!f1.is_open() || !f2.is_open()) {
        return false;
    }

    string line1, line2;
    bool result = true;
    while (getline(f1, line1) && getline(f2, line2)) {
        if (stol(line1) != stol(line2)) {
            result = false;
            break;  // Выходим раньше, если нашли несоответствие
        }
    }
    
    // Явное закрытие 
    f1.close();
    f2.close();
    
    return result;
}