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

  // Классический алгоритм Дениса Шашина для задачи о поезде
  int n = 0;  // предполагаемая длина
  Car* current = first;

  // Выключаем свет в текущем вагоне
  first->light = false;
  
  while (true) {
    // Увеличиваем предполагаемую длину
    n++;

    // Делаем n шагов вперед
    for (int i = 0; i < n; i++) {
      current = current->next;
      countOp++;
    }

    // Проверяем свет в текущем вагоне
    if (!current->light) {
      // Свет выключен - возвращаемся на n шагов назад
      for (int i = 0; i < n; i++) {
        current = current->prev;
        countOp++;
      }
    } else {
      // Нашли вагон с включенным светом
      // Выключаем свет
      current->light = false;

      // Возвращаемся на n шагов назад
      for (int i = 0; i < n; i++) {
        current = current->prev;
        countOp++;
      }

      // Сбрасываем счетчик n
      n = 0;
    }

    // Проверяем, все ли вагоны в пределах n имеют выключенный свет
    bool all_off = true;
    Car* check = first;
    for (int i = 0; i < n; i++) {
      if (i > 0 && check->light) {
        all_off = false;
        break;
      }
      check = check->next;
    }

    if (all_off && n > 0) {
      // Нашли длину поезда
      break;
    }
  }

  // Восстанавливаем свет в первом вагоне
  first->light = true;

  return n;
}

int Train::getOpCount() {
  return countOp;
}

void Train::resetOpCount() {
  countOp = 0;
}
