#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>

// Структура, представляющая персонажа игры
struct Character {
    std::string name; // Имя персонажа
    int health;       // Количество здоровья
    int armor;        // Уровень брони
    int damage;       // Урон, который может нанести персонаж
    int x, y;         // Координаты персонажа на карте
    bool isPlayer;    // Флаг, указывающий, является ли персонаж игроком
};

const int MAP_SIZE = 20;  // Размер карты (20x20)
const int NUM_ENEMIES = 5; // Количество противников
std::vector<Character> characters; // Вектор всех персонажей
char map[MAP_SIZE][MAP_SIZE]; // Карта игры

// Функция инициализации карты пустыми ячейками
void initializeMap() {
    for (int i = 0; i < MAP_SIZE; ++i)
        for (int j = 0; j < MAP_SIZE; ++j)
            map[i][j] = '.'; // Пустая ячейка обозначается точкой
}

// Функция удаления персонажей с нулевым здоровьем
void removeDeadCharacters() {
    characters.erase(
        std::remove_if(characters.begin(), characters.end(), [](const Character& character) {
            return character.health <= 0;
            }),
        characters.end()
    );
}

// Функция обновления карты, отображающая всех персонажей
void updateMap() {
    removeDeadCharacters(); // Удаление персонажей с нулевым здоровьем
    initializeMap();
    for (const auto& character : characters) {
        map[character.x][character.y] = character.isPlayer ? 'P' : 'E'; // Игрок - 'P', противники - 'E'
    }
}

// Функция отображения карты в консоли
void displayMap() {
    for (int i = 0; i < MAP_SIZE; ++i) {
        for (int j = 0; j < MAP_SIZE; ++j) {
            std::cout << map[i][j] << ' ';
        }
        std::cout << std::endl;
    }
}

// Функция расчёта урона с учётом брони
int calculateDamage(int baseDamage, int armor) {
    int effectiveDamage = baseDamage - armor / 2; // Урон уменьшается на половину брони
    return effectiveDamage > 0 ? effectiveDamage : 0; // Урон не может быть отрицательным
}

// Функция перемещения персонажа
void moveCharacter(Character& character, char direction) {
    int newX = character.x;
    int newY = character.y;

    // Определяем новое направление движения
    switch (direction) {
    case 'L': 
        newY--; 
        break;
    case 'R': 
        newY++; 
        break;
    case 'U': 
        newX--; 
        break;
    case 'D': 
        newX++; 
        break;
    default: 
        return;
    }

    // Проверяем, не выходит ли движение за границы карты
    if (newX < 0 || newX >= MAP_SIZE || newY < 0 || newY >= MAP_SIZE)
        return;

    // Проверяем столкновение с другими персонажами
    for (auto& target : characters) {
        if (target.x == newX && target.y == newY) {
            if (character.isPlayer && !target.isPlayer) {
                // Если игрок атакует противника
                int damage = calculateDamage(character.damage, target.armor);
                target.health -= damage;
                target.armor -= damage;
                if (target.armor < 0) {
                    target.armor = 0;
                }
                if (target.health <= 0) {
                    target.health = 0;
                }
            }
            return; // Ход завершён
        }
    }

    // Если клетка свободна, обновляем координаты персонажа
    character.x = newX;
    character.y = newY;
}

// Функция перемещения всех противников в случайном направлении
void moveEnemies() {
    for (auto& enemy : characters) {
        if (!enemy.isPlayer) {
            char directions[] = { 'L', 'R', 'U', 'D' }; // Возможные направления
            char direction = directions[std::rand() % 4]; // Случайное направление
            moveCharacter(enemy, direction);
        }
    }
}

// Функция сохранения игры в файл
void saveGame(const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    for (const auto& character : characters) {
        file.write((char*)&character, sizeof(Character));
    }
    file.close();
}

// Функция загрузки игры из файла
bool loadGame(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) return false; // Если файл не найден, вернуть false

    characters.clear(); // Очищаем текущий список персонажей
    Character temp;
    while (file.read((char*)&temp, sizeof(Character))) {
        characters.push_back(temp);
    }
    file.close();
    return true;
}

// Функция проверки окончания игры
bool isGameOver() {
    bool playerAlive = false; // Флаг, жив ли игрок
    bool enemiesAlive = false; // Флаг, живы ли противники
    for (const auto& character : characters) {
        if (character.isPlayer && character.health > 0) playerAlive = true;
        if (!character.isPlayer && character.health > 0) enemiesAlive = true;
    }
    if (!playerAlive) {
        std::cout << "You have been defeated. Game Over!" << std::endl;
    }
    else if (!enemiesAlive) {
        std::cout << "You have defeated all enemies. You win!" << std::endl;
    }
    return !playerAlive || !enemiesAlive;
}

int main() {
    std::srand(std::time(nullptr)); // Инициализация генератора случайных чисел

    // Создание игрока
    Character player;
    std::cout << "Enter your character's name: ";
    std::cin >> player.name;
    std::cout << "Enter health: ";
    std::cin >> player.health;
    std::cout << "Enter armor: ";
    std::cin >> player.armor;
    std::cout << "Enter damage: ";
    std::cin >> player.damage;
    player.x = std::rand() % MAP_SIZE;
    player.y = std::rand() % MAP_SIZE;
    player.isPlayer = true;
    characters.push_back(player);

    // Создание противников
    for (int i = 0; i < NUM_ENEMIES; ++i) {
        Character enemy;
        enemy.name = "Enemy #" + std::to_string(i + 1);
        enemy.health = 50 + std::rand() % 101;
        enemy.armor = std::rand() % 51;
        enemy.damage = 15 + std::rand() % 16;
        enemy.x = std::rand() % MAP_SIZE;
        enemy.y = std::rand() % MAP_SIZE;
        enemy.isPlayer = false;
        characters.push_back(enemy);
    }

    // Главный игровой цикл
    while (true) {
        updateMap(); // Обновление карты
        displayMap(); // Отображение карты

        if (isGameOver()) break; // Проверка на окончание игры

        //std::cout << "Player: " << player.name << " h=" << player.health << " a=" << player.armor << " d=" << player.damage << "\n";
        for (auto& target : characters) {
            std::cout << "Enemy: " << target.name << " h=" << target.health << " a=" << target.armor << " d=" << target.damage << "\n";
        }
        
        std::cout << "\nEnter move (L, R, U, D, save, load): ";
        std::string command;
        std::cin >> command;

        if (command == "save") {
            saveGame("save.bin"); // Сохранение игры
            std::cout << "Game saved!" << std::endl;
        }
        else if (command == "load") {
            if (loadGame("save.bin")) {
                std::cout << "Game loaded!" << std::endl;
            }
            else {
                std::cout << "Failed to load game." << std::endl;
            }
        }
        else if (command.size() == 1) {
            moveCharacter(characters[0], command[0]); // Ход игрока
            moveEnemies(); // Ходы противников
        }
    }

    return 0;
}
