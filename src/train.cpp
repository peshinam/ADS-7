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

  // Сохраняем начальное состояние
  bool initialState = first->light;

  // Выключаем свет в первом вагоне
  first->light = false;

  int length = 1;
  Car* current = first->next;
  countOp++;

  // Идем по кругу, пока не вернемся в начало
  while (current != first) {
    // Если нашли включенный свет - выключаем и начинаем с начала
    if (current->light) {
      current->light = false;
      length = 1;
      current = first->next;
      countOp++;
      continue;
    }

    length++;
    current = current->next;
    countOp++;
  }

  // Проверяем, все ли вагоны имеют выключенный свет
  bool allOff = true;
  Car* check = first->next;
  for (int i = 1; i < length; i++) {
    if (check->light) {
      allOff = false;
      break;
    }
    check = check->next;
  }

  // Восстанавливаем начальное состояние
  first->light = initialState;

  return allOff ? length : 0;
}

int Train::getOpCount() {
  return countOp;
}

void Train::resetOpCount() {
  countOp = 0;
}
