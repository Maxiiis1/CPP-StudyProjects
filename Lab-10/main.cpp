#include "requests.h"
#include <windows.h>

BOOL WINAPI ConsoleHandler(DWORD signal) {
    if (signal == CTRL_C_EVENT) {
        return TRUE;
    }
    return FALSE;
}


int main(int argc, char* argv[]) {


    SetConsoleCtrlHandler(&ConsoleHandler, TRUE);
    HANDLE consoleHandle = GetStdHandle(STD_INPUT_HANDLE);
    DWORD consoleMode;
    GetConsoleMode(consoleHandle, &consoleMode);
    SetConsoleMode(consoleHandle, consoleMode & ~ENABLE_PROCESSED_INPUT);


    int cities = 11;
    int days = 4;
    int city_number = 0;



    try {
        auto r = cpr::Get(cpr::Url{"https://api.open-meteo.com/v1/forecast?latitude=59.87&longitude=30.31&hourly=temperature_2m,rain,showers&forecast_days=16"});
        Request(city_number, days);
    }
    catch (const std::exception& e) {
        std::cout << "Error: connect the Internet" << std::endl;
    }


    while(true) {

        if (_kbhit()) {
            char button;
            button = _getch();


            if (button == 27) {
                exit(0);
            } else if (button == '=') {
                try {
                    auto r = cpr::Get(cpr::Url{
                            "https://api.open-meteo.com/v1/forecast?latitude=59.87&longitude=30.31&hourly=temperature_2m,rain,showers&forecast_days=16"});
                    system("cls");
                    std::cout << "Searching..." << '\n';
                    days++;

                    Request(city_number, days);
                }
                catch (const std::exception &e) {
                    std::cout << "Error: connect the Internet" << std::endl;
                }

            } else if (button == '-') {
                try {
                    auto r = cpr::Get(cpr::Url{
                            "https://api.open-meteo.com/v1/forecast?latitude=59.87&longitude=30.31&hourly=temperature_2m,rain,showers&forecast_days=16"});
                    system("cls");
                    std::cout << "Searching..." << '\n';
                    days--;

                    Request(city_number, days);
                }
                catch (const std::exception &e) {
                    std::cout << "Error: connect the Internet" << std::endl;
                }

            } else if (button == 'n' and city_number == cities - 1) {
                try {
                    auto r = cpr::Get(cpr::Url{
                            "https://api.open-meteo.com/v1/forecast?latitude=59.87&longitude=30.31&hourly=temperature_2m,rain,showers&forecast_days=16"});
                    system("cls");
                    std::cout << "Searching..." << '\n';
                    Request(city_number, days);
                }
                catch (const std::exception &e) {
                    std::cout << "Error: connect the Internet" << std::endl;
                }

            } else if (button == 'n') {
                try {
                    auto r = cpr::Get(cpr::Url{
                            "https://api.open-meteo.com/v1/forecast?latitude=59.87&longitude=30.31&hourly=temperature_2m,rain,showers&forecast_days=16"});
                    city_number++;
                    system("cls");
                    std::cout << "Searching..." << '\n';
                    Request(city_number, days);
                }
                catch (const std::exception &e) {
                    std::cout << "Error: connect the Internet" << std::endl;
                }

            } else if (button == 'p' and city_number == 0) {
                try {
                    auto r = cpr::Get(cpr::Url{
                            "https://api.open-meteo.com/v1/forecast?latitude=59.87&longitude=30.31&hourly=temperature_2m,rain,showers&forecast_days=16"});
                    system("cls");
                    std::cout << "Searching..." << '\n';
                    Request(city_number, days);
                }
                catch (const std::exception &e) {
                    std::cout << "Error: connect the Internet" << std::endl;
                }

            } else if (button == 'p') {
                try {
                    auto r = cpr::Get(cpr::Url{
                            "https://api.open-meteo.com/v1/forecast?latitude=59.87&longitude=30.31&hourly=temperature_2m,rain,showers&forecast_days=16"});
                    city_number--;
                    system("cls");
                    std::cout << "Searching..." << '\n';
                    Request(city_number, days);
                }
                catch (const std::exception &e) {
                    std::cout << "Error: connect the Internet" << std::endl;
                }
            }

        }


    }

    return 0;
}
