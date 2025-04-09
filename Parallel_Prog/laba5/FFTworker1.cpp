#include "baseFunc.h"
#include <pvm3.h>

int main() {
    // ждем передачу полиномов от мастера
    pvm_recv(-1, FFT_WORKER1_MESSAGE_TAG);
    // распаковываем полученные данные
    vector<double> poly1(LEN);
    pvm_upkdouble(poly1.data(), LEN, 1);

    vector<complex<double>> f1(poly1.size()+poly1.size());
    // переход к комплексным числам
    for (long i = 0; i < poly1.size(); ++i) {
        f1[i] = complex<double>(poly1[i], 0);
    }
    vector<complex<double>> resF1 = FFT(f1);

    vector<double> responseData(resF1.size() * 2);
    long responseCount = responseData.size();
    
    for (long i = 0; i < resF1.size(); i++) {
        responseData[2 * i] = resF1[i].real();     // Чётные индексы: real()
        responseData[2 * i + 1] = resF1[i].imag(); // Нечётные индексы: imag()
    }
    PrintVector(resF1);
    PrintVector(responseData);
    // Получаем TID мастера
    int master_tid = pvm_parent();

    // Получаем собственный TID
    int my_tid = pvm_mytid();

    // инитим передачу сообщения
    pvm_initsend(PvmDataDefault);

    // пакуем длину вектора
    pvm_pklong(&responseCount, 1, 1);

    // Упаковка данных (действительная и мнимая части)
    pvm_pkdouble(responseData.data(), responseData.size(), 1);

    // Отправка сообщения мастеру
    pvm_send(master_tid, FFT_WORKER1_MESSAGE_TAG); 

    pvm_exit();
}