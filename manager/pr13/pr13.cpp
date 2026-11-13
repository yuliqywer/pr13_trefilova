// pr13.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

using namespace std;

bool checkSimple(int num)
{
    if (num <= 1) return false;

    int maxDivider = sqrt(num);
    for (int divider = 2; divider <= maxDivider; divider++) {
        if (num % divider == 0) {
            return false;
        }
    }
    return true;
}

int main(int argc, char* argv[])
{
    if (argc != 4) {
        return 1;
    }

    int firstNum = stoi(argv[1]);
    int lastNum = stoi(argv[2]);
    string fileName = argv[3];

    int totalSimple = 0;

    for (int current = firstNum; current <= lastNum; current++) {
        if (checkSimple(current)) {
            totalSimple++;
        }
    }

    ofstream resultFile(fileName);
    if (resultFile.is_open()) {
        resultFile << totalSimple;
        resultFile.close();
    }

    return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
