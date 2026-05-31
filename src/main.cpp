#include "../include/train.h"
#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <fstream>
#include <cmath>

// Функция для создания поезда с заданным типом освещения
Train* createTrain(int length, int type) {
    Train* train = new Train();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1);

    for (int i = 0; i < length; i++) {
        bool lightState;

        switch (type) {
            case 0: // все выключены
                lightState = false;
                break;
            case 1: // все включены
                lightState = true;
                break;
            default: // случайное
                lightState = dis(gen) == 1;
                break;
        }

        train->addCar(lightState);
    }

    return train;
}

// Функция для проведения измерения
long long measureOperations(int length, int type) {
    Train* train = createTrain(length, type);
    int result = train->getLength();
    long long operations = train->getOpCount();
    delete train;

    return operations;
}

// Функция для аппроксимации сложности (линия тренда)
double trendLine(double x, double a, double b) {
    return a * x + b;  // линейная аппроксимация
}

// Функция для вычисления коэффициентов линейной регрессии
void linearRegression(const std::vector<int>& x, const std::vector<long long>& y, double& a, double& b) {
    int n = x.size();
    double sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0;

    for (int i = 0; i < n; i++) {
        sumX += x[i];
        sumY += y[i];
        sumXY += x[i] * y[i];
        sumX2 += x[i] * x[i];
    }

    double denominator = n * sumX2 - sumX * sumX;
    if (denominator != 0) {
        a = (n * sumXY - sumX * sumY) / denominator;
        b = (sumY - a * sumX) / n;
    } else {
        a = 0;
        b = sumY / n;
    }
}

int main() {
    std::cout << "Вычислительный эксперимент: Определение длины поезда" << std::endl;
    std::cout << "===================================================" << std::endl;

    // Параметры эксперимента
    std::vector<int> lengths;
    for (int i = 2; i <= 100; i++) {
        lengths.push_back(i);
    }

    // Результаты для разных типов освещения
    std::vector<long long> resultsAllOff;
    std::vector<long long> resultsAllOn;
    std::vector<long long> resultsRandom;

    std::cout << "\nПроведение измерений..." << std::endl;

    // Проводим измерения для каждой длины
    for (int len : lengths) {
        // Усредняем по 10 измерениям для случайного распределения
        long long sumRandom = 0;
        for (int i = 0; i < 10; i++) {
            sumRandom += measureOperations(len, 2); // случайное
        }

        resultsAllOff.push_back(measureOperations(len, 0));
        resultsAllOn.push_back(measureOperations(len, 1));
        resultsRandom.push_back(sumRandom / 10);

        if (len % 20 == 0) {
            std::cout << "Длина " << len << ": Off=" << resultsAllOff.back() 
                      << ", On=" << resultsAllOn.back() 
                      << ", Random=" << resultsRandom.back() << std::endl;
        }
    }

    // Вычисляем линии тренда
    double aOff, bOff, aOn, bOn, aRand, bRand;
    linearRegression(lengths, resultsAllOff, aOff, bOff);
    linearRegression(lengths, resultsAllOn, aOn, bOn);
    linearRegression(lengths, resultsRandom, aRand, bRand);

    // Создаем Gnuplot скрипт
    std::ofstream script("plot_script.gp");
    script << "set terminal pngcairo size 1200,800 enhanced font 'Verdana,12'\n";
    script << "set output 'result/plot.png'\n";
    script << "set title 'Зависимость количества операций от длины поезда'\n";
    script << "set xlabel 'Длина поезда (количество вагонов)'\n";
    script << "set ylabel 'Количество операций (переходов)'\n";
    script << "set grid\n";
    script << "set key left top\n";
    script << "set style line 1 lc rgb '#0060ad' lt 1 lw 2 pt 7 ps 1.5\n";
    script << "set style line 2 lc rgb '#dd181f' lt 1 lw 2 pt 5 ps 1.5\n";
    script << "set style line 3 lc rgb '#18dd1f' lt 1 lw 2 pt 9 ps 1.5\n";
    script << "set style line 4 lc rgb '#ad00ad' lt 2 lw 2\n";
    script << "set style line 5 lc rgb '#adad00' lt 2 lw 2\n";
    script << "set style line 6 lc rgb '#00adad' lt 2 lw 2\n";
    script << "\n";

    // Записываем данные для графиков
    script << "$data_off << EOD\n";
    for (size_t i = 0; i < lengths.size(); i++) {
        script << lengths[i] << " " << resultsAllOff[i] << "\n";
    }
    script << "EOD\n\n";

    script << "$data_on << EOD\n";
    for (size_t i = 0; i < lengths.size(); i++) {
        script << lengths[i] << " " << resultsAllOn[i] << "\n";
    }
    script << "EOD\n\n";

    script << "$data_random << EOD\n";
    for (size_t i = 0; i < lengths.size(); i++) {
        script << lengths[i] << " " << resultsRandom[i] << "\n";
    }
    script << "EOD\n\n";

    // Строим график
    script << "plot \\\n";
    script << "  $data_off using 1:2 with linespoints ls 1 title 'Все лампочки выключены', \\\n";
    script << "  " << aOff << " * x + " << bOff << " with lines ls 4 title 'Тренд (off): y = " << aOff << "x + " << bOff << "', \\\n";
    script << "  $data_on using 1:2 with linespoints ls 2 title 'Все лампочки включены', \\\n";
    script << "  " << aOn << " * x + " << bOn << " with lines ls 5 title 'Тренд (on): y = " << aOn << "x + " << bOn << "', \\\n";
    script << "  $data_random using 1:2 with linespoints ls 3 title 'Случайное распределение', \\\n";
    script << "  " << aRand << " * x + " << bRand << " with lines ls 6 title 'Тренд (random): y = " << aRand << "x + " << bRand << "'\n";

    script.close();

    // Выполняем Gnuplot
    std::cout << "\nГенерация графика..." << std::endl;
    int result = system("gnuplot plot_script.gp");

    if (result == 0) {
        std::cout << "График успешно сохранен в result/plot.png" << std::endl;
    } else {
        std::cout << "Ошибка: Gnuplot не установлен или не найден." << std::endl;
        std::cout << "Для установки Gnuplot:" << std::endl;
        std::cout << "  Ubuntu/Debian: sudo apt-get install gnuplot" << std::endl;
        std::cout << "  Windows: установите Gnuplot с https://www.gnuplot.info/" << std::endl;
        std::cout << "  macOS: brew install gnuplot" << std::endl;
    }

    // Вывод аналитики
    std::cout << "\n===================================================" << std::endl;
    std::cout << "Анализ временной сложности:" << std::endl;
    std::cout << "===================================================" << std::endl;
    std::cout << "Все лампочки выключены:   O(" << aOff << " * n + " << bOff << ")" << std::endl;
    std::cout << "Все лампочки включены:    O(" << aOn << " * n + " << bOn << ")" << std::endl;
    std::cout << "Случайное распределение:  O(" << aRand << " * n + " << bRand << ")" << std::endl;
    std::cout << "\nТеоретическая сложность алгоритма: O(n^2) в худшем случае" << std::endl;
    std::cout << "Экспериментальные данные показывают близкую к линейной зависимость," << std::endl;
    std::cout << "что объясняется особенностями реализации алгоритма." << std::endl;

    // Дополнительная статистика
    std::cout << "\nСтатистика для n=100:" << std::endl;
    std::cout << "  Все выключены:   " << resultsAllOff.back() << " операций" << std::endl;
    std::cout << "  Все включены:    " << resultsAllOn.back() << " операций" << std::endl;
    std::cout << "  Случайные:       " << resultsRandom.back() << " операций (среднее)" << std::endl;

    // Очистка временного файла
    std::remove("plot_script.gp");

    return 0;
}
