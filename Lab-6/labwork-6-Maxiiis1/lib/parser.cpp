#include <sstream>
#include "parser.h"


std::vector<std::string> CommonStrings(const std::vector<std::string>& buf1, const std::vector<std::string>& buf2) {
    std::vector<std::string> result;

    std::vector<std::string> sorted_buf1 = buf1;
    std::sort(sorted_buf1.begin(), sorted_buf1.end());

    std::vector<std::string> sorted_buf2 = buf2;
    std::sort(sorted_buf2.begin(), sorted_buf2.end());

    std::set_intersection(sorted_buf1.begin(), sorted_buf1.end(),
                          sorted_buf2.begin(), sorted_buf2.end(),
                          std::back_inserter(result));

    return result;
}

std::string GetPrime(const std::string& str){
    size_t prime_start = str.find("PRIMARY KEY(");

    std::string result = "";

    if (prime_start != std::string::npos) {
        size_t end = str.find(")", prime_start);
        if (end != std::string::npos) {
            result = str.substr(prime_start + 12, end - prime_start - 12);
        }
    }
    return result;
}

std::pair<std::string, std::string> GetKey(const std::string& str){
    size_t prime_start = str.find("PRIMARY KEY(");
    size_t foreign_start = str.find("FOREIGN KEY(");

    std::pair<std::string, std::string> result;

    if (prime_start != std::string::npos) {
        size_t end = str.find(")", prime_start);
        if (end != std::string::npos) {
            result.first = str.substr(prime_start + 12, end - prime_start - 12);
        }
    }

    if (foreign_start != std::string::npos) {
        size_t f_start = foreign_start + 12;
        size_t f_end = str.find(")", f_start);
        if (f_end != std::string::npos) {
            result.second = str.substr(f_start, f_end - f_start);
        }
    }

    return result;
}

std::string GetKeyWord(const std::string& str, const std::string& key_word) {
    std::string result = "";

    std::istringstream string(str);
    std::string word;
    while (string >> word) {
        if (word == key_word) {
            string >> result;
            break;
        }
    }
    return result;
}

bool StringToBool(const std::string& str) {
    if (str == "true" or str == "1") {
        return true;
    }
    else {
        return false;
    }
}

bool Integer(const std::string& str) {
    for (char c : str) {
        if (!std::isdigit(c)) {
            return false;
        }
    }
    return true;
}

std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> res;
    std::string substr = "";
    std::istringstream iss(str);


    while (std::getline(iss, substr, delimiter)) {
        substr.erase(remove_if(substr.begin(), substr.end(), isspace), substr.end());
        res.push_back(substr);
    }

    return res;
}



void Database::Join(const std::string& table_name1, const std::string& table_name2, std::vector<std::string>& columns, const std::string& command, const std::string& table_name){

    std::vector<std::string> column_names1 = tables_[table_name1].GetColumnNames();
    std::vector<std::string> column_names2 = tables_[table_name2].GetColumnNames();

    Table new_table(table_name);//создаем таблицу
    if (command == "LEFT"){
        new_table = tables_[table_name1];
        new_table.ChangeName(table_name);
        CreateTable(new_table, column_names1);
    }
    else if (command == "INNER"){
        std::vector<std::string> names = CommonStrings(column_names1, column_names2);
        CreateTable(new_table, names);
    }
    else if (command == "RIGHT"){
        new_table = tables_[table_name2];
        new_table.ChangeName(table_name);
        CreateTable(new_table, column_names2);
    }
}

void Database::ShowTable(const std::vector<std::string> &columns_names, const std::string table_name) {
    tables_[table_name].ShowTable(columns_names);
}

void Database::ShowTable(const std::string table_name) {
    tables_[table_name].ShowTable();
}

void Database::Query(const std::string &str) {
    std::string word = "";
    std::istringstream input(str);

    input >> word;

    if (word == "SELECT" and str.find("JOIN") == std::string::npos){
        input >> word;
        if (word == "*"){//SELECT * FROM <table_name>;
            std::string table_name;
            input >> table_name;//ignoring From
            input >> table_name;
            ShowTable(table_name);
        }
        else{//SELECT (<col_name1>, <col_name2>) FROM <table_name>;
            std::string colmns = str.substr(str.find('(') + 1, str.find(')') - str.find('(') - 1);

            std::vector<std::string> columns = split(colmns, ',');
            ShowTable(columns, GetKeyWord(str, "FROM"));
        }
    }
    else if (word == "DROP"){
        input >> word;
        std::string table_name;
        input >> table_name;
        DropTable(table_name);
    }
    else if (word == "CREATE"){
        input >> word;
        std::string table_name;
        input >> table_name;
        std::getline(input, word, ')');
        word = word.substr(2, word.length() - 2);
        std::stringstream st(word);

        Table new_table(table_name);//создаем таблицу
        std::vector<std::pair<std::string, std::string>> vals;//вектор названий колонок и типы(job STRING)

        std::vector<std::string> names;

        std::string sym = "";


        if (str.find("FOREIGN") != std::string::npos) {
            std::pair<std::string, std::string> keys = GetKey(str);
            Column<int> primary_column(keys.first);
            Column<int> foreign_column(keys.second);
            new_table.AddColumn(primary_column);
            new_table.AddColumn(foreign_column);
            names.push_back(keys.first);
            names.push_back(keys.second);
        }
        else if (str.find("PRIMARY") != std::string::npos){
            std::string prime = GetPrime(str);
            Column<int> primary_column(prime);
            new_table.AddColumn(primary_column);
            names.push_back(prime);
        }

        while (std::getline(st, sym, ',')) {
            std::istringstream s(sym);
            std::string name = "";
            std::string type = "";

            s >> name >> type;

            names.push_back(name);
            vals.push_back(std::make_pair(name, type));
        }

        for (auto colmn:vals){
            if (colmn.second == "INT"){
                Column<int> new_column(colmn.first);
                new_table.AddColumn<int>(new_column);
            }
            else if (colmn.second == "FLOAT"){
                Column<float> new_column(colmn.first);
                new_table.AddColumn<float>(new_column);
            }
            else if (colmn.second == "BOOL"){
                Column<bool> new_column(colmn.first);
                new_table.AddColumn<bool>(new_column);
            }
            else if (colmn.second == "STRING"){
                Column<std::string> new_column(colmn.first);
                new_table.AddColumn<std::string>(new_column);
            }
            else if (colmn.second.substr(0, 7) == "VARCHAR"){
                size_t var_size = std::stoi(colmn.second.substr(8, colmn.second.length()));

                VARCHAR varchar(var_size);
                varchar.SetValue(colmn.first);

                Column<std::string>new_column(colmn.first);
                new_table.AddColumn<std::string>(new_column);
            }
        }
        CreateTable(new_table, names);
    }
    else if (word == "DELETE"){
        input >> word;
        std::string table_name;
        std::string column_name;
        input >> table_name;
        input >> column_name;
        DeleteFrom(table_name,column_name);
    }
    else if (word == "INSERT"){
        if (std::count(str.begin(), str.end(), '(') == 1){
            input >> word;//INTO
            std::string table_name;
            input >> table_name;

            std::string vals = str.substr(str.find_last_of('(') + 1, str.find_last_of(')') - str.find_last_of('(') - 1);

            std::vector<std::string> values = split(vals, ',');

            Insert(values, table_name);
        }
        else {
            input >> word;//INTO
            std::string table_name;
            input >> table_name;

            std::string colmns = str.substr(str.find('(') + 1, str.find(')') - str.find('(') - 1);
            std::string vals = str.substr(str.find_last_of('(') + 1, str.find_last_of(')') - str.find_last_of('(') - 1);

            std::vector<std::string> columns = split(colmns, ',');
            std::vector<std::string> values = split(vals, ',');

            Insert(values, columns, table_name);
        }

    }
    else if (word == "SELECT" and str.find("JOIN") != std::string::npos){
        std::string colmns = str.substr(str.find('(') + 1, str.find(')') - str.find('(') - 1);
        std::vector<std::string> columns = split(colmns, ',');

        std::string table1 = GetKeyWord(str, "FROM");
        std::string join = "";
        if (str.find("INNER") != std::string::npos){
            join = "INNER";
        }
        else if (str.find("LEFT") != std::string::npos){
            join = "LEFT";
        }
        else if (str.find("RIGHT") != std::string::npos){
            join = "RIGHT";
        }
        std::string table2 = GetKeyWord(str, join);
        std::string table3 = GetKeyWord(str, "ON");

        Join(table1, table2, columns, join, table3);

    }
    else if (word == "UPDATE"){
        std::string table_name;
        input >> table_name;
        std::string check_column = "";

        std::string s = GetKeyWord(str, "WHERE");
        if (s.find('>') != std::string::npos) {
            check_column = s.substr(0, s.find('>'));//Age
        }

        std::size_t equalsPos = str.find('=');
        std::size_t wherePos = str.find("WHERE");

        std::string column = "";
        std::string val = "";

        if (equalsPos != std::string::npos and wherePos != std::string::npos and equalsPos < wherePos) {
            std::string words = str.substr(equalsPos + 1, wherePos - equalsPos - 1);

            std::size_t firstSpacePos = words.find_first_of(' ');
            std::size_t lastSpacePos = words.find_last_of(' ');
            if (firstSpacePos != std::string::npos && lastSpacePos != std::string::npos && firstSpacePos < lastSpacePos) {
                column = words.substr(0, firstSpacePos);
                val = words.substr(lastSpacePos + 1);
            }
        }

        GetTable(table_name).UpdateValue(column, val, 0);
    }
}


std::basic_string<char> Table::GetValue(const std::string& column_name, int& index){
    auto it = columns_.find(column_name);
    if (it != columns_.end()) {
        if (std::holds_alternative<Column<int>>(it->second)) {
            Column<int> &column = std::get<Column<int>>(it->second);
            if (column.GetColumnValues()[index]!=1000000){
                return std::to_string(column.GetColumnValues()[index]);
            }
            else{
                return "NULL";
            }
        }
        else if (std::holds_alternative<Column<std::string>>(it->second)) {
            Column<std::string> &column = std::get<Column<std::string>>(it->second);
            if (column.GetColumnValues()[index]!="1000000"){
                return column.GetColumnValues()[index];
            }
            else{
                return "NULL";
            }

        }
        else if (std::holds_alternative<Column<float>>(it->second)) {
            Column<float> &column = std::get<Column<float>>(it->second);
            if (column.GetColumnValues()[index]!=1000000){
                return std::to_string(column.GetColumnValues()[index]);
            }
            else{
                return "NULL";
            }

        }
        else if (std::holds_alternative<Column<bool>>(it->second)) {
            Column<bool> &column = std::get<Column<bool>>(it->second);
            if (column.GetColumnValues()[index]){
                return "true";
            }
            else{
                return "false";
            }
        }
    }
    return "NULL";
}


void Table::ShowTable(const std::vector<std::string> &columns_names) {
    std::ofstream file  ("C:\\Users\\maxfi\\Desktop\\C\\Lab6.C++.WTests\\labwork-6-Maxiiis1\\test.txt");
    if (file.is_open()) {
        // Запись названий
        for (auto& name : columns_names) {
            file << name << "\t";
        }
        file << "\n";

        // Запись значений
        for (size_t i = 0; i < rows_count_; ++i) {
            for (auto& name : columns_names) {
                auto it = columns_.find(name);
                if (it != columns_.end()) {
                    if (std::holds_alternative<Column<int>>(it->second)) {
                        Column<int> &column = std::get<Column<int>>(it->second);
                        if (column.GetColumnValues()[i]!=1000000){
                            file << column.GetColumnValues()[i];
                        }
                        else{
                            file << "NULL";
                        }
                    }
                    else if (std::holds_alternative<Column<std::string>>(it->second)) {
                        Column<std::string> &column = std::get<Column<std::string>>(it->second);
                        if (column.GetColumnValues()[i]!="1000000"){
                            file << column.GetColumnValues()[i];
                        }
                        else{
                            file << "NULL";
                        }

                    }
                    else if (std::holds_alternative<Column<float>>(it->second)) {
                        Column<float> &column = std::get<Column<float>>(it->second);
                        if (column.GetColumnValues()[i]!=1000000){
                            file << column.GetColumnValues()[i];
                        }
                        else{
                            file << "NULL";
                        }

                    }
                    else if (std::holds_alternative<Column<bool>>(it->second)) {
                        Column<bool> &column = std::get<Column<bool>>(it->second);
                        file << column.GetColumnValues()[i];
                    }
                }
                file << "\t";
            }
            file << "\n";
        }

        file.close();
        std::cout << "Data was upload"  << std::endl;
    }
    else {
        std::cout << "Data fail" << std::endl;
    }
}

void Table::ChangeName(const std::string& name){
    name_ = name;
}

void Table::ShowTable() {
    std::ofstream file  ("C:\\Users\\maxfi\\Desktop\\C\\Lab6.C++.WTests\\labwork-6-Maxiiis1\\test.txt");
    if (file.is_open()) {
        // Запись названий
        for (auto& name : columns_names_) {
            file << name << "\t";
        }

        file << "\n";

        // Запись значений
        for (size_t i = 0; i < rows_count_; ++i) {
            for (auto& name : columns_names_) {
                auto it = columns_.find(name);
                if (it != columns_.end()) {
                    if (std::holds_alternative<Column<int>>(it->second)) {
                        Column<int> &column = std::get<Column<int>>(it->second);
                        if (column.GetColumnValues()[i]!=1000000){
                            file << column.GetColumnValues()[i];
                        }
                        else{
                            file << "NULL";
                        }
                    }
                    else if (std::holds_alternative<Column<std::string>>(it->second)) {
                        Column<std::string> &column = std::get<Column<std::string>>(it->second);
                        if (column.GetColumnValues()[i]!="1000000"){
                            file << column.GetColumnValues()[i];
                        }
                        else{
                            file << "NULL";
                        }

                    }
                    else if (std::holds_alternative<Column<float>>(it->second)) {
                        Column<float> &column = std::get<Column<float>>(it->second);
                        if (column.GetColumnValues()[i]!=1000000){
                            file << column.GetColumnValues()[i];
                        }
                        else{
                            file << "NULL";
                        }

                    }
                    else if (std::holds_alternative<Column<bool>>(it->second)) {
                        Column<bool> &column = std::get<Column<bool>>(it->second);
                        file << column.GetColumnValues()[i];
                    }
                }
                file << "\t";
            }
            file << "\n";
        }

        file.close();
        std::cout << "Data was upload"  << std::endl;
    }
    else {
        std::cout << "Data fail" << std::endl;
    }
}

void Table::DeleteTable(){
    columns_.clear();
}

std::vector<std::string>& Table::GetColumnNames(){
    return columns_names_;
}

std::map<std::string, std::variant<Column<int>, Column<std::string>, Column<float>, Column<bool>, Column<VARCHAR>>>& Table::GetColumns(){
    return columns_;
}

void Table::RowsInc() {
    rows_count_++;
}

void Table::DeleteColumn(const std::string& column){
    columns_.erase(column);
}

std::string Table::GetName(){
    return name_;
}

int Table::GetColumnCount() {
    return columns_.size();
}

void Table::AddColumnNames(std::vector<std::string>& columns_names){
    columns_names_ = columns_names;
    column_count_ = columns_names.size();
}


void Database::CreateTable(Table& table, std::vector<std::string>& columns_names) {
    table.AddColumnNames(columns_names);
    tables_[table.GetName()] = table;

}

void Database::DropTable(const std::string &name) {
    tables_[name].DeleteTable();//удалили таблицу
    tables_.erase(name);//удалили из бд
}



Table& Database::GetTable(const std::string& name) {
    return tables_[name];
}

void Database::DeleteFrom(const std::string& table, const std::string& column){
    tables_[table].DeleteColumn(column);//удаляем столбец таблицы
}

void Database::Insert(std::vector<std::string>& values, std::vector<std::string>& columns, const std::string& table_name){
    tables_[table_name].RowsInc();
    if (columns.size() == values.size()) {//если заполняем все колонки
        for (int i = 0; i < values.size(); i++) {
            if (values[i].find('.') != std::string::npos) {//float
                tables_[table_name].AddValue<float>(columns[i], std::stof(values[i]));
            } else if (values[i].substr(0, 5) == "false" or values[i].substr(0, 4) == "true") {//bool
                tables_[table_name].AddValue<bool>(columns[i], StringToBool(values[0]));
            } else if (Integer(values[i])) {//int
                tables_[table_name].AddValue<int>(columns[i], std::stoi(values[i]));
            } else {//str
                tables_[table_name].AddValue<std::string>(columns[i], values[i]);
            }
        }
    }
    else{
        int values_count = values.size()-1;

        int i = 0;
        while(i<=values_count) {
            if (values[i].find('.') != std::string::npos) {//float
                tables_[table_name].AddValue<float>(columns[i], std::stof(values[i]));
            } else if (values[i].substr(0, 5) == "false" or values[i].substr(0, 4) == "true") {//bool
                tables_[table_name].AddValue<bool>(columns[i], StringToBool(values[0]));
            } else if (Integer(values[i])) {//int
                tables_[table_name].AddValue<int>(columns[i], std::stoi(values[i]));
            } else {//str
                tables_[table_name].AddValue<std::string>(columns[i], values[i]);
            }
            i++;
        }

        int rest_values = columns.size() - values.size();
        std::vector<std::string> cc;

        for (int k = values.size();k<columns.size();k++){
            cc.push_back(columns[k]);
        }

        for (auto& name : cc) {
            auto it = tables_[table_name].GetColumns().find(name);
            if (it != tables_[table_name].GetColumns().end()) {
                if (std::holds_alternative<Column<int>>(it->second)) {
                    Column<int> &column = std::get<Column<int>>(it->second);
                    int val = 1000000;
                    column.AddNULL(val);
                }
                else if (std::holds_alternative<Column<std::string>>(it->second)) {
                    Column<std::string> &column = std::get<Column<std::string>>(it->second);
                    std::string val = "1000000";
                    column.AddNULL(val);
                }
                else if (std::holds_alternative<Column<float>>(it->second)) {
                    Column<float> &column = std::get<Column<float>>(it->second);
                    float val = 1000000;
                    column.AddNULL(val);
                }
                else if (std::holds_alternative<Column<bool>>(it->second)) {
                    Column<bool> &column = std::get<Column<bool>>(it->second);
                    bool val = 0;
                    column.AddNULL(val);
                }
            }
        }

    }
}

void Database::Insert(std::vector<std::string>& values, const std::string& table_name){
    tables_[table_name].RowsInc();
    for (int i=0;i<values.size();i++) {
        if (values[i].find('.') != std::string::npos) {//float
            tables_[table_name].AddValue<float>(tables_[table_name].GetColumnNames()[i], std::stof(values[i]));
        }
        else if (values[i].substr(0, 5) == "false" or values[i].substr(0, 4) == "true") {//bool
            tables_[table_name].AddValue<bool>(tables_[table_name].GetColumnNames()[i], StringToBool(values[0]));
        }
        else if (Integer(values[i])) {//int
            tables_[table_name].AddValue<int>(tables_[table_name].GetColumnNames()[i], std::stoi(values[i]));
        }
        else {//str
            tables_[table_name].AddValue<std::string>(tables_[table_name].GetColumnNames()[i], values[i]);
        }
    }
}







