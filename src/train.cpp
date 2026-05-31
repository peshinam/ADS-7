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

  // Классический алгоритм для кругового поезда
  // Выключаем свет в текущем вагоне
  first->light = false;

  int step = 1;
  int length = 0;
  Car* current = first;
  bool found = false;

  while (!found) {
    // Идем вперед на step вагонов
    for (int i = 0; i < step; i++) {
      current = current->next;
      countOp++;
    }

    if (current->light) {
      // Нашли вагон с включенным светом
      current->light = false;
      // Возвращаемся назад
      for (int i = 0; i < step; i++) {
        current = current->prev;
        countOp++;
      }
      step = 1;
      length = 0;
    } else {
      // Возвращаемся назад
      for (int i = 0; i < step; i++) {
        current = current->prev;
        countOp++;
      }
      length += step;
      step++;

      // Проверяем, все ли вагоны в пределах length имеют выключенный свет
      bool allOff = true;
      Car* check = first;
      for (int i = 0; i < length; i++) {
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
  }

  // Восстанавливаем свет в первом вагоне
  first->light = true;
  return length;
}

int Train::getOpCount() {
  return countOp;
}

void Train::resetOpCount() {
  countOp = 0;
}
