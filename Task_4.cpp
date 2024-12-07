#include <iostream>
#include <vector>
#include <memory>
#include <string>

// Абстрактный класс Talent
class Talent {
public:
    virtual ~Talent() = default;
    virtual std::string getDescription() const = 0; // Чисто виртуальный метод
};

// Конкретный класс Swimming
class Swimming : public Talent {
public:
    std::string getDescription() const override {
        return "Swim";
    }
};

// Конкретный класс Dancing
class Dancing : public Talent {
public:
    std::string getDescription() const override {
        return "Dance";
    }
};

// Конкретный класс Counting
class Counting : public Talent {
public:
    std::string getDescription() const override {
        return "Count";
    }
};

// Класс Dog
class Dog {
private:
    std::string name;
    std::vector<std::unique_ptr<Talent>> talents; // Хранение уникальных указателей на таланты

public:
    explicit Dog(const std::string& dogName) : name(dogName) {}


    // Метод для добавления нового таланта
    void addTalent(std::unique_ptr<Talent> talent) {
        talents.push_back(std::move(talent));
    }

    // Метод для показа всех талантов
    void show_talents() const {
        std::cout << "This is " << name << " and it has some talents:" << std::endl;
        for (const auto& talent : talents) {
            std::cout << "\tIt can \"" << talent->getDescription() << "\"" << std::endl;
        }
    }
};

int main() {
    Dog dog1("Steve");

    // Добавляем таланты
    dog1.addTalent(std::make_unique<Dancing>());
    dog1.addTalent(std::make_unique<Counting>());

    // Выводим список талантов
    dog1.show_talents();

    return 0;
}
