// Copyright 2022 NNTU-CS
#include "../include/train.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

Train* createTrain(int length, int type) {
  Train* train = new Train();
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, 1);
  for (int i = 0; i < length; i++) {
    bool lightState;
    if (type == 0) {
      lightState = false;
    } else if (type == 1) {
      lightState = true;
    } else {
      lightState = dis(gen) == 1;
    }
    train->addCar(lightState);
  }
  return train;
}

int64_t measureOperations(int length, int type) {
  Train* train = createTrain(length, type);
  int len = train->getLength();
  int64_t operations = train->getOpCount();
  delete train;
  return operations;
}

int main() {
  std::cout << "Вычислительный эксперимент" << std::endl;
  std::vector<int> lengths;
  for (int i = 2; i <= 100; i += 2) {
    lengths.push_back(i);
  }
  std::vector<int64_t> resultsAllOff;
  std::vector<int64_t> resultsAllOn;
  std::vector<int64_t> resultsRandom;

  for (int len : lengths) {
    int64_t sumRandom = 0;
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

  std::ofstream script("plot.gp");
  script << "set terminal pngcairo size 1024,768\n";
  script << "set output 'result/plot.png'\n";
  script << "set title 'Dependence of operations on train length'\n";
  script << "set xlabel 'Train length (number of cars)'\n";
  script << "set ylabel 'Number of operations (transitions)'\n";
  script << "set grid\n";
  script << "set key left top\n";
  script << "plot 'result/data.txt' using 1:2 with linespoints "
         << "title 'All off', \\\n";
  script << "     'result/data.txt' using 1:3 with linespoints "
         << "title 'All on', \\\n";
  script << "     'result/data.txt' using 1:4 with linespoints "
         << "title 'Random'\n";
  script.close();

  int ret = system("gnuplot plot.gp");
  if (ret == 0) {
    std::cout << "Graph saved to result/plot.png\n";
  } else {
    std::cout << "Gnuplot not found, graph not created\n";
  }

  return 0;
}
