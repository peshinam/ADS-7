#ifndef TRAIN_H
#define TRAIN_H

class Train {
private:
    struct Car {
        bool light;     // состояние лампочки
        Car* next;      // указатель на следующий вагон
        Car* prev;      // указатель на предыдущий вагон

        Car(bool l) : light(l), next(nullptr), prev(nullptr) {}
    };

    int countOp;        // счетчик шагов (число переходов)
    Car* first;         // точка входа в поезд (первый вагон)
    int trainLength;    // реальная длина поезда (для эксперимента)

public:
    Train();
    ~Train();

    void addCar(bool light);    // добавить вагон
    int getLength();             // вычислить длину поезда
    int getOpCount();            // вернуть число переходов
    void resetOpCount();         // сбросить счетчик операций
    int getRealLength();         // получить реальную длину (для эксперимента)
};

#endif
