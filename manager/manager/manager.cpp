// manager.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <windows.h>
#include <string>
#include <conio.h>
#include <fstream> 

using namespace std;

struct ProcessInfo {
    int id;
    DWORD pid;
    HANDLE hProcess;
    HANDLE hThread;
    DWORD priority;
    DWORD startTime;
    DWORD endTime;
    string outFile;
};

DWORD GetPriority(int pChoice) {
    switch (pChoice) {
    case 1: return IDLE_PRIORITY_CLASS;
    case 2: return BELOW_NORMAL_PRIORITY_CLASS;
    case 3: return NORMAL_PRIORITY_CLASS;
    case 4: return ABOVE_NORMAL_PRIORITY_CLASS;
    case 5: return HIGH_PRIORITY_CLASS;
    default: return NORMAL_PRIORITY_CLASS;
    }
}

string priorityName(DWORD p)
{
    if (p == IDLE_PRIORITY_CLASS) return "IDLE";
    if (p == BELOW_NORMAL_PRIORITY_CLASS) return "BELOW_NORMAL";
    if (p == NORMAL_PRIORITY_CLASS) return "NORMAL";
    if (p == ABOVE_NORMAL_PRIORITY_CLASS) return "ABOVE_NORMAL";
    if (p == HIGH_PRIORITY_CLASS) return "HIGH";
    return "UNKNOWN";
}

int main()
{
    setlocale(LC_ALL, "Russian");
    int racersCount;

    do {
        cout << "Введите количество процессов (2-5): ";
        cin >> racersCount;
    }
    while (racersCount < 2 || racersCount > 5);

    string startStr, endStr;
    cout << "Введите диапазон чисел: ";
    cin >> startStr >> endStr;

    ProcessInfo racers[5];

    cout << "\nСоздание процессов..." << endl;

    for (int i = 0; i < racersCount; i++)
    {
        racers[i].id = i + 1;
        racers[i].outFile = "res_" + to_string(i) + ".txt";

        string cmd = "pr13.exe " + startStr + " " + endStr + " " + racers[i].outFile;
        char* cmdChar = new char[cmd.length() + 1];
        strcpy_s(cmdChar, cmd.length() + 1, cmd.c_str());

        STARTUPINFOA si = { sizeof(si) };
        PROCESS_INFORMATION pi = {};

        if (CreateProcessA(NULL, cmdChar, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi))
        {
            racers[i].pid = pi.dwProcessId;
            racers[i].hProcess = pi.hProcess;
            racers[i].hThread = pi.hThread;

            racers[i].priority = NORMAL_PRIORITY_CLASS;
            SetPriorityClass(racers[i].hProcess, racers[i].priority);

            racers[i].endTime = 0;
            cout << "Процесс " << racers[i].id << " создан. PID: " << racers[i].pid << endl;
        }
        else
        {
            cout << "Ошибка создания процесса! Код: " << GetLastError() << endl;
        }
        delete[] cmdChar;
    }

    cout << "\nНажмите Enter для старта..." << endl;
    cin.get(); cin.get();

    DWORD globalStart = GetTickCount();
    for (int i = 0; i < racersCount; i++) {
        ResumeThread(racers[i].hThread);
        CloseHandle(racers[i].hThread);
        racers[i].startTime = globalStart;
    }

    cout << "\nГонка началась! (Нажмите p для смены приоритета)" << endl;

    int finishedCount = 0;
    while (finishedCount < racersCount)
    {
        finishedCount = 0;

        for (int i = 0; i < racersCount; i++)
        {
            if (racers[i].endTime == 0)
            {
                DWORD exitCode;
                if (GetExitCodeProcess(racers[i].hProcess, &exitCode) && exitCode != STILL_ACTIVE)
                {
                    racers[i].endTime = GetTickCount();
                }
            }
            else
            {
                finishedCount++;
            }
        }

        if (_kbhit())
        {
            char key = _getch();
            if (key == 'p' || key == 'P')
            {
                int id;
                cout << "\nВведите ID процесса для изменения приоритета: ";
                cin >> id;

                if (id >= 1 && id <= racersCount && racers[id - 1].endTime == 0)
                {
                    cout << "Выберите приоритет (1-IDLE, 2-BELOW_NORMAL, 3-NORMAL, 4-ABOVE_NORMAL, 5-HIGH): ";
                    int pChoice;
                    cin >> pChoice;

                    racers[id - 1].priority = GetPriority(pChoice);
                    SetPriorityClass(racers[id - 1].hProcess, racers[id - 1].priority);
                    cout << "Приоритет процесса " << id << " изменен на " << priorityName(racers[id - 1].priority) << endl;
                }
                cin.ignore(1000, '\n');
            }
        }
        Sleep(50);
    }

    cout << "\nРезультаты" << endl;
    for (int i = 0; i < racersCount; i++)
    {
        int res = 0;
        ifstream fin(racers[i].outFile);
        if (fin.is_open()) {
            fin >> res;
            fin.close();
        }

        DWORD time = racers[i].endTime - racers[i].startTime;

        cout << "Процесс " << racers[i].id << " (PID: " << racers[i].pid << ")"
            << " | Приоритет: " << priorityName(racers[i].priority)
            << " | Время: " << time << " мс"
            << " | Найдено: " << res << " чисел" << endl;
    }

    for (int i = 0; i < racersCount; i++)
    {
        CloseHandle(racers[i].hProcess);
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
