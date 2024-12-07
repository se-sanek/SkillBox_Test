#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <map>

// Тип задачи
enum class TaskType { A, B, C };

// Конвертация задачи в строку
std::string taskToString(TaskType task) {
    switch (task) {
    case TaskType::A: return "A";
    case TaskType::B: return "B";
    case TaskType::C: return "C";
    }
    return "Unknown";
}

// Сотрудник
struct Employee {
    std::string name;
    bool isBusy = false;

    void assignTask(TaskType task) {
        isBusy = true;
        std::cout << "Employee " << name << " is assigned task " << taskToString(task) << std::endl;
    }
};

// Менеджер
struct Manager {
    std::string name;
    int id;
    std::vector<Employee> employees;

    void distributeTasks(int seed) {
        std::srand(seed + id);
        int taskCount = 1 + std::rand() % employees.size();

        std::cout << "Manager " << name << " is distributing " << taskCount << " tasks." << std::endl;
        for (int i = 0; i < taskCount; ++i) {
            TaskType task = static_cast<TaskType>(std::rand() % 3);
            for (auto& employee : employees) {
                if (!employee.isBusy) {
                    employee.assignTask(task);
                    break;
                }
            }
        }
    }
};

// Глава компании
struct CEO {
    std::string name;
    std::vector<Manager> managers;

    void giveOrder(int command) {
        std::cout << "CEO " << name << " gives order " << command << std::endl;
        for (auto& manager : managers) {
            manager.distributeTasks(command);
        }
    }
};

// Проверка занятости всех сотрудников
bool allEmployeesBusy(const CEO& ceo) {
    for (const auto& manager : ceo.managers) {
        for (const auto& employee : manager.employees) {
            if (!employee.isBusy) return false;
        }
    }
    return true;
}

int main() {
    // Ввод количества команд и сотрудников в каждой команде
    int teamCount, employeesPerTeam;
    std::cout << "Enter number of teams: ";
    std::cin >> teamCount;
    std::cout << "Enter number of employees per team: ";
    std::cin >> employeesPerTeam;

    // Создание структуры компании
    CEO ceo{ "Alice" };
    for (int i = 0; i < teamCount; ++i) {
        Manager manager{ "Manager" + std::to_string(i + 1), i + 1 };
        for (int j = 0; j < employeesPerTeam; ++j) {
            manager.employees.push_back({ "Employee" + std::to_string(i + 1) + "-" + std::to_string(j + 1) });
        }
        ceo.managers.push_back(manager);
    }

    // Приём указаний от пользователя
    std::cout << "Enter commands (negative number to stop):" << std::endl;
    int command;
    while (std::cin >> command && command >= 0) {
        ceo.giveOrder(command);
        if (allEmployeesBusy(ceo)) {
            std::cout << "All employees are busy. Simulation ends." << std::endl;
            break;
        }
    }

    return 0;
}
