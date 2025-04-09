#include "baseFunc.h"
#include <pvm3.h>

// перемножение полиномов без паралллельки
vector<long> MultPoly(vector<double>& p1, vector<double>& p2, int master_id) {
    long n = p1.size() + p2.size() - 1;
    long size = 1;
    // корректировка длины к степени двойки
    while (size < n) size <<= 1;  // Даем размеры 2, 4, 8, 16, ...

    // вспомогательные массивы для спавна воркеров
    int message_tags[] = {FFT_WORKER1_MESSAGE_TAG, FFT_WORKER2_MESSAGE_TAG};
    int children[FFT_WORKERS_CNT] = {-1, -1};
    string paths[] = {
        BASE_PATH+"FFTworker1",
        BASE_PATH+"FFTworker2",
    };

    // подготовка данных к отправке в воркеры
    vector<vector<double>> toPackData = {p1, p2};
    
    // цикл генерации воркеров
    for(int i = 0; i < FFT_WORKERS_CNT; i++){
        int spw = pvm_spawn(const_cast<char*>(paths[i].c_str()), NULL, PvmTaskDefault, "", 1, &children[i]);
        if(spw <= 0){
            cout << "Error on spawn task!" << endl;
        }
        else{
            pvm_initsend(PvmDataDefault);
            // пакуем
            pvm_pkdouble(toPackData[i].data(), LEN, 1);
            // отправляем воркеру
            pvm_send(children[i], message_tags[i]);
        }
    }

    vector<vector<complex<double>>> resFArr(FFT_WORKERS_CNT);
    int total_received = 0;

    while (total_received < FFT_WORKERS_CNT) {
        // Проверяем наличие сообщений с тегом 1
        int bufid = pvm_recv(-1, -1);
        
        if (bufid > 0) {
            // Получаем информацию о сообщении
            int bytes, tag, tid;
            pvm_bufinfo(bufid, &bytes, &tag, &tid);
            
            // Принимаем сообщение
            // double data;
            for(int i = 0; i < FFT_WORKERS_CNT; i++){
                if(tag == message_tags[i]){
                    long responseCount;
                    pvm_upklong(&responseCount,1,1);
                    vector<double> responseData(responseCount);
                    pvm_upkdouble(responseData.data(), responseCount, 1);

                    // Преобразование обратно в vector<complex<double>>
                    vector<complex<double>> result(responseCount/2);
                    for (long i = 0; i < responseCount/2 ; i++) {
                        double real = responseData[2 * i];
                        double imag = responseData[2 * i + 1];
                        result[i] = complex<double>(real, imag);
                    }
                    resFArr[total_received] = result;
                    break;
                }
            }
            
            total_received++;
        }
    }
    
    // Выполнение FFT
    vector<complex<double>> resF1 = resFArr[0];
    vector<complex<double>> resF2 = resFArr[1];
    
    // Перемножение (суть ускорения)
    for (long i = 0; i < size; i++) {
        resF1[i] *= resF2[i];
    }
    // Выполнение IFFT
    IFFT(resF1);
    // //Получение результата
    vector<long> result(n);
    for (long i = 0; i < n; i++) {
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


    // Получаем TID мастера
    int master_tid = pvm_parent();

    // Получаем собственный TID
    int my_tid = pvm_mytid();

    auto start = chrono::high_resolution_clock::now();
    vector<long> res3 = MultPoly(poly1, poly2, my_tid);
    auto end = chrono::high_resolution_clock::now();
    long resLen = res3.size();

    // Получаем длительность в секундах как double
    chrono::duration<double> elapsed = end - start;
    double calcTime = elapsed.count();
    // double calcTime = poly1[23];

    // Открываем файл для записи (если файла нет, он создаётся)
    ofstream outfile(BASE_PATH+"pvmOut.txt");
    if (!outfile.is_open()) {
        return 1;
    }

    // Записываем данные из вектора в файл
    for (long i = 0; i < resLen; i++) {
        outfile << res3[i] << "\n";  // Каждое число на новой строке
    }
    outfile.flush();
    outfile.close();  // Закрываем файл

    // инитим передачу сообщения
    pvm_initsend(PvmDataDefault);
    // Упаковка данных
    pvm_pkdouble(&calcTime, 1, 1);

    // Отправка сообщения мастеру
    pvm_send(master_tid, PVM_MESSAGE_TAG); 

    pvm_exit();
}