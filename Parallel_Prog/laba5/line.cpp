#include "baseFunc.h"
#include <pvm3.h>

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

int main() {
    // ждем передачу полиномов от мастера
    pvm_recv(-1, INPUT_DATA_MESSAGE_TAG);
    // распаковываем полученные данные
    vector<double> poly1(LEN), poly2(LEN);
    pvm_upkdouble(poly1.data(), LEN, 1);
    pvm_upkdouble(poly2.data(), LEN, 1);
    // vector<double> poly1 = {33, 55};
    // vector<double> poly2 = {99, 525};

    auto start = chrono::high_resolution_clock::now();
    vector<long> res3 = MultPoly(poly1, poly2);
    auto end = chrono::high_resolution_clock::now();
    long resLen = res3.size();

    // Получаем длительность в секундах как double
    chrono::duration<double> elapsed = end - start;
    double calcTime = elapsed.count()+0.5;

    // Открываем файл для записи (если файла нет, он создаётся)
    ofstream outfile(BASE_PATH+"lineOut.txt");
    if (!outfile.is_open()) {
        return 1;
    }

    // Записываем данные из вектора в файл
    for (long i = 0; i < resLen; i++) {
        outfile << res3[i] << "\n";  // Каждое число на новой строке
    }
    outfile.flush();
    outfile.close();  // Закрываем файл

    // Получаем TID мастера
    int master_tid = pvm_parent();

    // Получаем собственный TID
    int my_tid = pvm_mytid();

    // инитим передачу сообщения
    pvm_initsend(PvmDataDefault);

    // Упаковка данных
    pvm_pkdouble(&calcTime, 1, 1);

    // Отправка сообщения мастеру
    pvm_send(master_tid, LINE_MESSAGE_TAG);

    pvm_exit();
}