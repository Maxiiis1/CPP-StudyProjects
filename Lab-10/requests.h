#include <iostream>
#include <cpr/cpr.h>
#include "json/single_include/nlohmann/json.hpp"
#include <string>
#include <vector>
#include <conio.h>
#include <cstdio>



using json = nlohmann::json;
const std::vector<std::string> cities{"Moscow", "Novosibirsk", "Chelyabinsk", "Penza", "Yekaterinburg",  "Kazan", "Omsk", "Kopeysk", "Ufa", "Krasnodar", "Samara"};

std::pair<std::string, std::string> GetCoordinates(int city_number);
void Request(int city_number, int days);