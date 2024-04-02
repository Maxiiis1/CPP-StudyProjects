#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <variant>
#include <algorithm>
#include <fstream>


class VARCHAR {
public:
    VARCHAR(size_t len)
        :len_(len)
    {}

    std::string& GetValue() {
        return value_;
    }

    void SetValue(const std::string& value) {
        if (value.length() > len_) {
            std::cout<<'\n'<<"VARCHAR Error"<<'\n';
        }
        value_ = value;
    }

private:
    size_t len_;
    std::string value_ = "";
};

bool StringToBool(const std::string& str);
std::vector<std::string> split(const std::string& str, char delimiter);

template<typename T>
class Column {
public:
    Column() = default;

    Column(const std::string& name)
            : name_(name)
    {}
    std::string GetName() {
        return name_;
    }

    void UpdateColumn(T& val){
        column_values_.push_back(val);
    }

    void SetColumn(T& val, int index){
        column_values_[index] = val;
    }

    std::vector<T>& GetColumnValues(){
        return column_values_;
    }

    void AddNULL(T& val){
        column_values_.push_back(val);
    }

private:
    std::string name_;
    std::vector<T> column_values_;
};

class Table {
public:
    Table() = default;


    Table(const std::string& name)
            : name_(name)
    {}

    ~Table() = default;

    void DeleteTable();

    void DeleteColumn(const std::string& column);

    std::string GetName();

    void ChangeName(const std::string& name);

    template<typename T>
    void AddColumn(Column<T>& column) {
        columns_[column.GetName()] = column;
    }

    int GetColumnCount();

    template<typename T>
    Column<T> GetColumn(std::string& name) {
        return columns_[name];
    }

    template<typename T>
    void AddValue(const std::string& name, T value){
        std::get<Column<T>>(columns_[name]).UpdateColumn(value);
    }

    template<typename T>
    void UpdateValue(const std::string& name, T value, int index){
        std::get<Column<T>>(columns_[name]).SetColumn(value, index);
    }

    void AddColumnNames(std::vector<std::string>& columns_names);

    void ShowTable(const std::vector<std::string>& columns_names);

    void ShowTable();

    void RowsInc();

    std::basic_string<char> GetValue(const std::string& column_name, int& index);

    std::vector<std::string>& GetColumnNames();

    std::map<std::string, std::variant<Column<int>, Column<std::string>, Column<float>, Column<bool>, Column<VARCHAR>>>& GetColumns();

private:
    size_t rows_count_ = 0;
    size_t column_count_;
    std::string name_;
    std::vector<std::string> columns_names_;
    std::map<std::string, std::variant<Column<int>, Column<std::string>, Column<float>, Column<bool>, Column<VARCHAR>>> columns_;
};


class Database {
public:

    Database() = default;

    Database(const std::string& name)
        :name_(name)
    {}

    ~Database(){
        tables_.clear();
    }

    void CreateTable(Table& table, std::vector<std::string>& columns_names);

    Table& GetTable(const std::string& name);

    void ShowTable(const std::vector<std::string>& columns_names, const std::string table_name);

    void ShowTable(const std::string table_name);

    void Query(const std::string& str);

    void DropTable(const std::string& name);

    void DeleteFrom(const std::string& table, const std::string& column);

    void Insert(std::vector<std::string>& values, std::vector<std::string>& columns, const std::string& table_name);

    void Insert(std::vector<std::string>& values, const std::string& table_name);

    void Join(const std::string& table1, const std::string& table2, std::vector<std::string>& columns, const std::string& command, const std::string& table_name);


private:
    std::string name_;
    std::map<std::string, Table> tables_;
};
