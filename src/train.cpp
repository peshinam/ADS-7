// Copyright 2022 NNTU-CS
#include "../include/train.h"

Train::Train() : countOp(0), first(nullptr) {}

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
}

int Train::getLength() {
  if (!first) return 0;
  countOp = 0;
  // Классический алгоритм: идем вперед, выключаем свет,
  // возвращаемся и увеличиваем шаг
  int length = 0;
  Car* current = first;
  // Выключаем свет в текущем вагоне
  current->light = false;
  bool found = false;
  while (!found) {
    length++;
    // Идем вперед на length шагов
    for (int i = 0; i < length; i++) {
      current = current->next;
      countOp++;
    }
    // Проверяем состояние
    if (!current->light) {
      // Свет выключен - возвращаемся
      for (int i = 0; i < length; i++) {
        current = current->prev;
        countOp++;
      }
    } else {
      // Нашли включенный свет
      current->light = false;
      // Возвращаемся и ищем дальше
      for (int i = 0; i < length; i++) {
        current = current->prev;
        countOp++;
      }
      length = 0;
    }
    // Проверяем, все ли вагоны проверены
    bool allOff = true;
    Car* check = first;
    for (int i = 0; i <= length; i++) {
      if (check->light) {
        allOff = false;
        break;
      }
      check = check->next;
    }
    if (allOff && length > 0) {
      found = true;
    }
  }
  // Восстанавливаем состояние
  first->light = true;
  return length;
}

int Train::getOpCount() {
  return countOp;
}

void Train::resetOpCount() {
  countOp = 0;
}
