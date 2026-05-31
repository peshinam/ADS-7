// Copyright 2022 NNTU-CS
#include "../include/train.h"
#include <iostream>
#include <vector>
#include <random>
#include <fstream>
#include <cstdlib>

Train* createTrain(int length, int type) {
  Train* train = new Train();
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, 1);
  for (int i = 0; i < length; i++) {
    bool lightState;
    switch (type) {
      case 0:
        lightState = false;
        break;
      case 1:
        lightState = true;
        break;
      default:
        lightState = dis(gen) == 1;
        break;
    }
    train->addCar(lightState);
  }
  return train;
}

long long measureOperations(int length, int type) {
  Train* train = createTrain(length, type);
  train->getLength();
  long long operations = train->getOpCount();
  delete train;
  return operations;
}

int main() {
  std::cout << "Вычислительный эксперимент" << std::endl;
  std::vector<int> lengths;
  for (int i = 2; i <= 100; i += 2) {
    lengths.push_back(i);
  }
  std::vector<long long> resultsAllOff;
  std::vector<long long> resultsAllOn;
  std::vector<long long> resultsRandom;
  for (int len : lengths) {
    long long sumRandom = 0;
    for (int i = 0; i < 5; i++) {
      sumRandom += measureOperations(len, 2);
    }
    resultsAllOff.push_back(measureOperations(len, 0));
    resultsAllOn.push_back(measureOperations(len, 1));
    resultsRandom.push_back(sumRandom / 5);
  }
  std::ofstream dataFile("result/data.txt");
  if (dataFile.is_open()) {
    dataFile << "# Length AllOff AllOn Random\n";
    for (size_t i = 0; i < lengths.size(); i++) {
      dataFile << lengths[i] << " " << resultsAllOff[i] << " "
               << resultsAllOn[i] << " " << resultsRandom[i] << "\n";
    }
    dataFile.close();
  }
  // Создание Gnuplot скрипта
  std::ofstream script("plot.gp");
  script << "set terminal pngcairo size 1024,768\n";
  script << "set output 'result/plot.png'\n";
  script << "set title 'Зависимость операций от длины поезда'\n";
  script << "set xlabel 'Длина поезда'\n";
  script << "set ylabel 'Количество операций'\n";
  script << "set grid\n";
  script << "set key left top\n";
  script << "plot 'result/data.txt' using 1:2 with linespoints "
         << "title 'Все выключены', \\\n";
  script << "     'result/data.txt' using 1:3 with linespoints "
         << "title 'Все включены', \\\n";
  script << "     'result/data.txt' using 1:4 with linespoints "
         << "title 'Случайные'\n";
  script.close();
  system("gnuplot plot.gp");
  std::cout << "График сохранен в result/plot.png\n";
  return 0;
}
