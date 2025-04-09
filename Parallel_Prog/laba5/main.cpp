#include "baseFunc.h"
#include <pvm3.h>


int main(int argc, char** argv) {
    string res1 = BASE_PATH+"pvmOut.txt";
    string res2 = BASE_PATH+"lineOut.txt";
    // Входные данные
    vector<double> poly1(LEN);
    vector<double> poly2(LEN);
    // заполнение векторов
    for (long i = 0; i < LEN; i++)
    {
        // poly1[i] = generateRandomDouble(1, 100);
        // poly2[i] = generateRandomDouble(1, 100);
        poly1[i] = 111;
        poly2[i] = 222;
    }
    cout << "gen end" << endl;

    pvm_initsend(PvmDataDefault);
    pvm_pkdouble(poly1.data(), LEN, 1);
    pvm_pkdouble(poly2.data(), LEN, 1);

    // число воркеров
    int cnt = 5;
    // массив для tid воркеров
    int children[] = {-1, -1, -1, -1, -1};
    // массив тэгов
    int message_tags[] = {
        LINE_MESSAGE_TAG,
        PVM_MESSAGE_TAG,
        MPI_MESSAGE_TAG,
        POSIX_MESSAGE_TAG,
        OMP_MESSAGE_TAG
    };
    // заголовки для вывода
    string titles[] = {"line: ", "pvm: ", "mpi: ", "posix: ", "omp: "};
    // пути к исполнительным файлам воркеров
    string paths[] = {
        BASE_PATH+"line",
        BASE_PATH+"myPvm",
        BASE_PATH+"myMpi",
        BASE_PATH+"myPosix",
        BASE_PATH+"myOmp",
    };

    // получаем tid мастера
    int MId = pvm_mytid();
    cout << "Master TID: " << MId << endl;

    for(int i = 0; i < cnt; i++){
        int spw = pvm_spawn(const_cast<char*>(paths[i].c_str()), NULL, PvmTaskDefault, "", 1, &children[i]);
        if(spw <= 0){
            cout << "Error on spawn task!" << endl;
        }
        else{
            pvm_send(children[i], INPUT_DATA_MESSAGE_TAG);
        }
    }

    double times[cnt];
    int total_received = 0;

    while (total_received < cnt) {
        // Проверяем наличие сообщений с тегом 1
        int bufid = pvm_recv(-1, -1); // -1 означает "любой отправитель"
        
        if (bufid > 0) {
            // Получаем информацию о сообщении
            int bytes, tag, tid;
            pvm_bufinfo(bufid, &bytes, &tag, &tid);
            
            // Принимаем сообщение
            double data;
            pvm_upkdouble(&data, 1, 1);

            for(int i = 0; i < cnt; i++){
                if(message_tags[i] == tag){
                    times[i] = data;
                    titles[i] = titles[i] + " " + to_string(data);
                    break;
                }
            }
            
            total_received++;
        }
    }
    
    for(int i = 0; i < cnt; i++){
        cout << titles[i] << endl;
    }
    cout << "k_pvm = " << times[LINE_MESSAGE_TAG-1] / times[PVM_MESSAGE_TAG-1] << endl;
    cout << "k_mpi = " << times[LINE_MESSAGE_TAG-1] / times[MPI_MESSAGE_TAG-1] << endl;
    cout << "k_posix = " << times[LINE_MESSAGE_TAG-1] / times[POSIX_MESSAGE_TAG-1] << endl;
    cout << "k_omp = " << times[LINE_MESSAGE_TAG-1] / times[OMP_MESSAGE_TAG-1] << endl;

    if(checkResults(res1, res2)){
        cout << "Answer is Ok" << endl;
    }
    pvm_exit();
}