#include "baseFunc.h"
#include <pvm3.h>

int main() {
    // ждем передачу полиномов от мастера
    pvm_recv(-1, FFT_WORKER2_MESSAGE_TAG);
    // распаковываем полученные данные
    vector<double> poly2(LEN);
    pvm_upkdouble(poly2.data(), LEN, 1);

    vector<complex<double>> f2(poly2.size()+poly2.size());
    // переход к комплексным числам
    for (long i = 0; i < poly2.size(); i++) {
        f2[i] = complex<double>(poly2[i], 0);
    }
    vector<complex<double>> resF2 = FFT(f2);

    vector<double> responseData(resF2.size() * 2);
    long responseCount = responseData.size();
    
    for (long i = 0; i < resF2.size(); i++) {
        responseData[2 * i] = resF2[i].real();     // Чётные индексы: real()
        responseData[2 * i + 1] = resF2[i].imag(); // Нечётные индексы: imag()
    }

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
    pvm_send(master_tid, FFT_WORKER2_MESSAGE_TAG); // Тег сообщения = 2

    pvm_exit();
}