// Copyright 2021 NNTU-CS
#include "train.h"
#include <cstdlib>

Train::Train() : countOp(0), current(nullptr), first(nullptr) {}

Train::~Train() {
    if (!first) return;

    Car* temp = first;
    do {
        Car* toDelete = temp;
        temp = temp->next;
        delete toDelete;
    } while (temp != first);
}

void Train::addCar(bool light) {
    Car* newCar = new Car{light, nullptr, nullptr};

    if (!first) {
        first = newCar;
        first->next = first;
        first->prev = first;
        current = first;  // инициализируем current
    } else {
        newCar->next = first;
        newCar->prev = first->prev;
        first->prev->next = newCar;
        first->prev = newCar;
    }
}

int Train::getLength() {
    countOp = 0;
    int counter = 0;

    if (current->light) {
        counter++;
    } else {
        current->light = true;
        counter++;
    }

    current = current->next;
    countOp++;

    while (true) {
        if (!current->light) {
            counter++;
            current = current->next;
            countOp++;
        } else {
            current->light = false;
            int stepsBack = counter;

            for (int i = 0; i < stepsBack; i++) {
                current = current->prev;
                countOp++;
            }

            if (!first->light) {
                return counter;
            } else {
                for (int i = 0; i < stepsBack; i++) {
                    current = current->next;
                    countOp++;
                }
            }
        }
    }
}

int Train::getOpCount() {
    return countOp;
}

void Train::resetOpCount() {
    countOp = 0;
}
