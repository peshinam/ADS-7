#include "../include/train.h"

Train::Train() : countOp(0), first(nullptr), trainLength(0) {}

Train::~Train() {
    if (!first) return;

    Car* current = first;
    Car* nextCar = nullptr;

    do {
        nextCar = current->next;
        delete current;
        current = nextCar;
    } while (current != first);
}

void Train::addCar(bool light) {
    Car* newCar = new Car(light);

    if (!first) {
        first = newCar;
        first->next = first;
        first->prev = first;
    } else {
        Car* last = first->prev;
        last->next = newCar;
        newCar->prev = last;
        newCar->next = first;
        first->prev = newCar;
    }

    trainLength++;
}

int Train::getLength() {
    if (!first) return 0;

    // Алгоритм: идем вперед, выключаем свет, ищем включенный вагон
    // Классическое решение задачи о поезде

    countOp = 0;  // обнуляем счетчик операций

    // Сначала выключаем свет в текущем вагоне (если он включен)
    first->light = false;

    int steps = 0;
    Car* current = first;
    
    while (true) {
        // Идем вперед на steps + 1 шагов
        for (int i = 0; i <= steps; i++) {
            current = current->next;
            countOp++;  // каждый переход из вагона в вагон
        }

        // Проверяем, включен ли свет
        if (current->light) {
            // Нашли включенный вагон, выключаем его
            current->light = false;

            // Возвращаемся назад на steps + 1 шагов
            for (int i = 0; i <= steps; i++) {
                current = current->prev;
                countOp++;
            }

            steps = 0;
        } else {
            // Свет выключен, продолжаем поиск
            // Возвращаемся назад
            for (int i = 0; i <= steps; i++) {
                current = current->prev;
                countOp++;
            }

            steps++;

            // Если steps достиг длины поезда, значит мы все проверили
            if (steps > trainLength) {
                break;
            }
        }
    }

    // Включим свет в первом вагоне обратно для корректности
    first->light = true;

    // Длина поезда = steps
    return steps;
}

int Train::getOpCount() {
    return countOp;
}

void Train::resetOpCount() {
    countOp = 0;
}

int Train::getRealLength() {
    return trainLength;
}
