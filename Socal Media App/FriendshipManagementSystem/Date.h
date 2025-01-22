#ifndef DATE_H
#define DATE_H

#include <string>
#include <sstream>

class Date {
private:
    int year, month, day;

public:
    Date(); // Constructor declaration
    void setDate(int y, int m, int d);
    std::string getDate() const;
};

#endif // DATE_H