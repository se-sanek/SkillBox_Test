#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <chrono>

using namespace std;

// Перечисление для флагов состояния переключателей
enum switches
{
    LIGHTS_INSIDE = 1,
    LIGHTS_OUTSIDE = 2,
    HEATERS = 4,
    WATER_PIPE_HEATING = 8,
    CONDITIONER = 16
};

class SmartHome {
private:
    // Переменная для состояния переключателей (битовая маска)
    unsigned int switches_state;

    // Параметры температуры и состояния движения
    float outsideTemp, insideTemp;
    /*bool motionOutside;
    bool lightInside;*/

    string motionOutside;
    string lightInside;

    bool motionOutsideB;
    bool lightInsideB;

    // Параметры освещения внутри
    float colorTemperature;

public:
    SmartHome() : switches_state(0), outsideTemp(0), insideTemp(0), motionOutside(""), lightInside(""), colorTemperature(5000) {}

    // Функция для включения устройства по флагу
    void turnOn(switches device) {
        if (!(switches_state & device)) {
            switches_state |= device;
            cout << getDeviceName(device) << " ON!" << endl;
        }
    }

    // Функция для выключения устройства по флагу
    void turnOff(switches device) {
        if (switches_state & device) {
            switches_state &= ~device;
            cout << getDeviceName(device) << " OFF!" << endl;
        }
    }

    // Функция для получения имени устройства по флагу
    string getDeviceName(switches device) {
        switch (device) {
        case LIGHTS_INSIDE: return "Lights Inside";
        case LIGHTS_OUTSIDE: return "Lights Outside";
        case HEATERS: return "Heaters";
        case WATER_PIPE_HEATING: return "Water Pipe Heating";
        case CONDITIONER: return "Conditioner";
        default: return "Unknown Device";
        }
    }

    // Симуляция работы устройства в зависимости от условий
    void update() {
        // Парсинг выражений yes/no в логическую переменную
        if (motionOutside == "yes") {
            motionOutsideB = true;
        }else if (motionOutside == "no"){
            motionOutsideB = false;
        }

        // Парсинг выражений on/off в логическую переменную
        if (lightInside == "on") {
            lightInsideB = true;
        }
        else if (lightInside == "off") {
            lightInsideB = false;
        }

        // Управление системой отопления водопровода
        if (outsideTemp < 0) {
            turnOn(WATER_PIPE_HEATING);
        }
        else if (outsideTemp > 5) {
            turnOff(WATER_PIPE_HEATING);
        }

        // Управление садовым освещением
        if (motionOutsideB && (timeOfDay() > 16 || timeOfDay() < 5)) {
            turnOn(LIGHTS_OUTSIDE);
        }
        else {
            turnOff(LIGHTS_OUTSIDE);
        }

        // Управление отоплением в доме
        if (insideTemp < 22) {
            turnOn(HEATERS);
        }
        else if (insideTemp >= 25) {
            turnOff(HEATERS);
        }

        // Управление кондиционером
        if (insideTemp >= 30) {
            turnOn(CONDITIONER);
        }
        else if (insideTemp <= 25) {
            turnOff(CONDITIONER);
        }

        // Управление цветом света
        if (lightInsideB) {
            turnOn(LIGHTS_INSIDE);
            updateLightColorTemperature();
        }
        else {
            turnOff(LIGHTS_INSIDE);
        }
    }

    // Функция для управления цветовой температурой
    void updateLightColorTemperature() {
        if (timeOfDay() >= 16 && timeOfDay() <= 20) {
            float targetColorTemp = 5000 - (timeOfDay() - 16) * 450; // плавное изменение с 5000K до 2700K
            colorTemperature = targetColorTemp;
        }
        else if (timeOfDay() >= 0 && timeOfDay() < 16) {
            colorTemperature = 5000;
        }
        cout << "Color temperature: " << colorTemperature << "K" << endl;
    }

    // Симуляция времени суток (часы)
    int timeOfDay() {
        return currentHour;
    }

    // Установка текущего времени
    void setCurrentHour(int hour) {
        currentHour = hour;
    }

    // Основная функция для обработки ввода
    void processInput() {
        // Ввод данных о состоянии датчиков
        string input;
        getline(cin, input);

        stringstream ss(input);
        ss >> insideTemp >> outsideTemp >> motionOutside >> lightInside;

        // Обновление состояния системы
        update();
    }

private:
    int currentHour; // текущий час
};

int main() {
    SmartHome home;

    // Симуляция работы два дня (48 часов)
    for (int day = 0; day < 2; ++day) {
        for (int hour = 0; hour < 24; ++hour) {
            home.setCurrentHour(hour);
            cout << "Day " << (day + 1) << ", Hour: " << hour << ":00" << endl;
            cout << "Temperature inside, temperature outside, movement, lights:" << endl;

            // Ввод данных о температуре, движении и свете
            home.processInput();

            // Пауза для симуляции времени (1 секунда — 1 час в симуляции)
            this_thread::sleep_for(chrono::seconds(1)); // Пауза в 1 секунду, чтобы симулировать каждый час

            cout << "--------------------------------------------------" << endl;
        }
    }

    return 0;
}
