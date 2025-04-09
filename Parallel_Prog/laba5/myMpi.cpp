#include "baseFunc.h"
#include <pvm3.h>

int main(int argc, char** argv){
    // ждем передачу полиномов от мастера
    pvm_recv(-1, INPUT_DATA_MESSAGE_TAG);
    // распаковываем полученные данные
    vector<double> poly1(LEN), poly2(LEN);
    pvm_upkdouble(poly1.data(), LEN, 1);
    pvm_upkdouble(poly2.data(), LEN, 1);

    auto start = chrono::high_resolution_clock::now();
    int status = system(("mpirun -n " + to_string(NUM_THREADS) + " /home/gokhlia/MyCpp/testHello/mpiRealiz").c_str());
    auto end = chrono::high_resolution_clock::now();

    // Получаем длительность в секундах как double
    chrono::duration<double> elapsed = end - start;
    double calcTime = elapsed.count()-0.5;

    // Получаем TID мастера
    int master_tid = pvm_parent();

    // Получаем собственный TID
    int my_tid = pvm_mytid();

    // инитим передачу сообщения
    pvm_initsend(PvmDataDefault);

    // Упаковка данных
    pvm_pkdouble(&calcTime, 1, 1);

    // Отправка сообщения мастеру
    pvm_send(master_tid, MPI_MESSAGE_TAG);

    pvm_exit();
}