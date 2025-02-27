#include <omp.h>
#include <iostream>
#include <chrono>
#include <vector>
#include <random>

using namespace std;
const double LEN = pow(2, 15);


// генерация случайных чисел типа double
double generateRandomDouble(double lower, double upper) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dis(lower, upper);
    return dis(gen);
}

void sosal() {
    vector<double> poly1;
    vector<double> poly2;
    poly1.resize(LEN);
    poly2.resize(LEN);
    cout<<omp_get_num_threads()<<endl;

#pragma omp for
    for (long i = 0; i < LEN; i++) {
        poly1[i] = 1111;
        poly2[i] = 1111;
    }
    
}

void lizal() {
    vector<double> poly1;
    vector<double> poly2;
    poly1.resize(LEN);
    poly2.resize(LEN);
    cout<< omp_get_num_threads()<<endl;
    //#pragma omp parallel num_threads(N)
    //{
    //    #pragma omp parallel for
        // заполнение полиномов
        for (long i = 0; i < LEN; i++)
        {
            poly1[i] = 1111;
            poly2[i] = 1111;
        }
    //}
}

// вывод времени в секундах
void PrintTime(chrono::duration<double> time) {
    cout << "Mult time: " << time.count() << "s" << endl;
}

int main()
{
//    omp_set_dynamic(1);     // Explicitly disable dynamic teams
    omp_set_num_threads(4); // Устанавливаем глобальное количество потоков
    //#pragma omp parallel for schedule(static, 1)
    //for (long long i = 0; i < 1000000; ++i) {
    //    std::cout << "Thread " << omp_get_thread_num() << " work with " << i << std::endl;
    //}
    auto start = chrono::high_resolution_clock::now();
    #pragma omp parallel
    {
        #pragma omp sections nowait
        {
            #pragma omp section
            {
                sosal();
            }
            //#pragma omp section
            //{
            //    lizal();
            //}
        }
    }
    auto end = chrono::high_resolution_clock::now();
    PrintTime(end - start);

    start = chrono::high_resolution_clock::now();
    lizal();
    end = chrono::high_resolution_clock::now();
    PrintTime(end - start);

    return 0;
}

