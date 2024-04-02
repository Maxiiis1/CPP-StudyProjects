#include "requests.h"
#include <sstream>
#include <iomanip>


std::string value_R (std::string str){

    if (str[0]!='0'){
        return "Rainy";
    }
    else if (str[0]=='0' and str[2]!='0'){
        return "Cloudy";
    }
    else{
        return "Sunny";
    }
}

std::string value_W (std::string str){
    std::string res = "wind: ";
    for (int i=0;str[i]!='.';i++){
        res+=str[i];
    }
    res+="m/s";
    return res;
}

std::string value_C (std::string str){
    std::string res = "temperature: ";
    if(str[0]=='-' and str[1]!=0){
        res += '-';
        for (int i=1;str[i]!='.';i++){
            res+= str[i];
        }

        return res;
    }
    else{
        res += '+';
        for (int i=0;str[i]!='.';i++){
            res+= str[i];
        }

        return res;
    }
}

std::pair<std::string, std::string> GetCoordinates(int city_number){
    std::string api_key = "Xa+r6+WpuL726wzMltvbrA==DfT4X5o2N8n52l5Q" ;
    std::string coordinates_url = "https://api.api-ninjas.com/v1/city?name=" + cities[city_number];
    auto get_coordinate = cpr::Get(cpr::Url{coordinates_url}, cpr::Authentication{"maxfink2003@mail.ru", "Maxutko007", cpr::AuthMode::BASIC},cpr::Header{ {"X-Api-Key", api_key} });//достаём координаты города
    json data_c = json::parse(get_coordinate.text);//результат координат
    std::string latitude = std::to_string(data_c[0]["latitude"].get<double>());
    std::string longitude = std::to_string(data_c[0]["longitude"].get<double>());
    return std::make_pair(latitude, longitude);
};


void Request(int city_number, int days){



    std::pair<std::string, std::string> coordinates = GetCoordinates(city_number);

    auto get_weather = cpr::Get(cpr::Url{"https://api.open-meteo.com/v1/forecast?latitude=" + coordinates.first + "&longitude=" + coordinates.second + "&hourly=temperature_2m,rain,showers,windspeed_10m&forecast_days=" + std::to_string(days)});
    json data_w = json::parse(get_weather.text);

    system("cls");

    std::string city_name = cities[city_number];
    std::string prev_city =  cities[city_number];
    std::string next_city =  cities[city_number+1];
    if (city_number > 0){
        prev_city = cities[city_number-1];
    }

    std::string p = "<--";
    std::string n = "-->";

    if (prev_city == city_name){
        std::cout<<'\t' << '\t'<< '\t'<< '\t'<< '\t'<<'\t'<< city_name<< '\t'<< '\t'<< '\t'<<'\t'<<n<<next_city<<'\n';
    }
    else{
        std::cout<< prev_city << p <<'\t' << '\t'<< '\t'<< '\t'<< '\t'<<'\t'<< city_name<< '\t'<< '\t'<< '\t'<<'\t'<<n<<next_city<<'\n';
    }


    for(int i=0; i<days-1;i++){//цикл для количества суток

        std::string date_str = data_w["hourly"]["time"][10+i*24];
        std::stringstream ss(date_str);

        std::tm date = {};
        ss >> std::get_time(&date, "%Y-%m-%dT%H:%M");

        // получаем год, месяц, день3040
        std::string months[] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
        std::string month = months[date.tm_mon];//название месяца
        int day = date.tm_mday;
        month = std::to_string(day) + ' ' + month;


        std::cout<< '\t' << '\t'<< '\t'<< '\t'<< '\t'<< '\t'<< '\t'<<month<<'\n';//вывод даты

        //Вывод времени суток
        std::cout << std::setw(25) << "Morning"
                  << std::setw(25) << "Day"
                  << std::setw(25) << "Evening"
                  << std::setw(25) << "Night" << std::endl;


// Вывод данных о погоде, 1-градусы, 2-дождь, 3-скорость ветер
        std::cout << std::setw(25) << value_C(std::to_string(data_w["hourly"]["temperature_2m"][15+i*24].get<double>()))
                  << std::setw(25) << value_C(std::to_string(data_w["hourly"]["temperature_2m"][10+i*24].get<double>()))
                  << std::setw(25) << value_C(std::to_string(data_w["hourly"]["temperature_2m"][19+i*24].get<double>()))
                  << std::setw(25) << value_C(std::to_string(data_w["hourly"]["temperature_2m"][24+i*24].get<double>()))<<'\n';



        std::cout << std::setw(25)  << value_R(std::to_string(data_w["hourly"]["rain"][10+i*24].get<double>()));
        std::cout << std::setw(25) << value_R(std::to_string(data_w["hourly"]["rain"][15+i*24].get<double>()));
        std::cout << std::setw(25)  << value_R(std::to_string(data_w["hourly"]["rain"][19+i*24].get<double>())) ;
        std::cout << std::setw(25) << value_R(std::to_string(data_w["hourly"]["rain"][24+i*24].get<double>()));
        std::cout << '\n';

        std::cout << std::setw(25)  << value_W(std::to_string(data_w["hourly"]["windspeed_10m"][10+i*24].get<double>()));
        std::cout << std::setw(25) <<  value_W(std::to_string(data_w["hourly"]["windspeed_10m"][15+i*24].get<double>())) ;
        std::cout << std::setw(25) << value_W(std::to_string(data_w["hourly"]["windspeed_10m"][19+i*24].get<double>())) ;
        std::cout << std::setw(25) << value_W(std::to_string(data_w["hourly"]["windspeed_10m"][24+i*24].get<double>())) ;
        std::cout <<'\n';

        std::cout <<'\n';
    }

}