#include <iostream>
#include <string>
#include <map>
#include <set>
#include <sstream>
#include <iomanip>
#include <exception>
#include <algorithm>

using namespace std;
// –еализуйте функции и методы классов и при необходимости добавьте свои

class Date {
public:
    Date() 
    {
        m_year = 0;
        m_month = 0;
        m_day = 0;
    }
    Date(int year, int month, int day)
    {
        if (month < 1 || month > 12)
        {
            throw invalid_argument("Month value is invalid: " + to_string(month));
        }
        else if (day < 1 || day > 31)
        {
            throw invalid_argument("Day value is invalid: " + to_string(day));
        }
        else
        {
            m_year = year;
            m_month = month;
            m_day = day;
        }

    }

    void PrintDate() const
    {
        cout << setw(4) << setfill('0') << m_year << '-'
            << setw(2) << setfill('0') << m_month << '-'
            << setw(2) << setfill('0') << m_day;
    }

    int GetYear() const
    {
        return m_year;
    }
    int GetMonth() const
    {
        return m_month;
    }
    int GetDay() const
    {
        return m_day;
    }

private:
    int m_year;
    int m_month;
    int m_day;
};

bool operator<(const Date& lhs, const Date& rhs)
{
    if (lhs.GetYear() != rhs.GetYear())
    {
        return lhs.GetYear() < rhs.GetYear();
    }
    else if (lhs.GetMonth() != rhs.GetMonth())
    {
        return lhs.GetMonth() < rhs.GetMonth();
    }
    else
    {
        return lhs.GetDay() < rhs.GetDay();
    }
}

istream& operator>>(istream& input, Date& date)
{
    int year = 0;
    int month = 0;
    int day = 0;
    string str;
    input >> str;

    stringstream stream(str);
    
    bool flag = true;

    flag = flag && (stream >> year);
    flag = flag && (stream.peek() == '-');
    stream.ignore(1);
    flag = flag && (stream >> month);
    flag = flag && (stream.peek() == '-');
    stream.ignore(1);
    flag = flag && (stream >> day);
    flag = flag && stream.eof();

    if (flag)
    {
        date = { year, month, day };
    }
    else
    {
        throw runtime_error("Wrong date format: " + str);
    }

    return input;
}

class Database {
public:
    void AddEvent(const Date& date, const string& event)
    {
        m_database[date].insert(event);
    }
    bool DeleteEvent(const Date& date, const string& event)
    {
        if (m_database[date].count(event) == 0)
        {
            cout << "Event not found" << endl;
            return false;
        }
        else
        {
            m_database[date].erase(event);
            cout << "Deleted successfully" << endl;
            return true;
        }
        
    }
    int DeleteDate(const Date& date)
    {
        int n = 0;
        if (m_database.count(date) != 0)
        {
            n = m_database[date].size();
            m_database.erase(date);
        }
        cout << "Deleted " << n << " events" << endl;

        return n;
    }

    void Find(const Date& date) const
    {
        if (m_database.count(date) != 0)
        {
            for (const auto& d : m_database.at(date))
            {
                cout << d << endl;
            }
        }
    }

    void Print() const
    {
        for (const auto& m_db : m_database)
        {
            for (const auto& s : m_db.second)
            {
                m_db.first.PrintDate();
                cout << ' ' << s << endl;
            }
        }
    }

private:
    map <Date, set<string>> m_database;
};


int main() {
    Database db;

    string command;
    while (getline(cin, command)) {
        // —читайте команды с потока ввода и обработайте каждую
        stringstream stream(command);

        string action;
        stream >> action;
        try
        {
            if (action == "Add")
            {
                Date date;
                string event;
                stream >> date;
                stream >> event;
                db.AddEvent(date, event);
            }
            else if (action == "Del")
            {
                Date date;
                string event;
                stream >> date >> event;
                if (!event.empty())
                {
                    db.DeleteEvent(date, event);
                }
                else
                {
                    db.DeleteDate(date);
                }
            }
            else if (action == "Find")
            {
                Date date;
                stream >> date;
                db.Find(date);
            }
            else if (action == "Print")
            {
                db.Print();
            }
            else if (!action.empty())
            {
                cout << "Unknown command: " << action << endl;
            }
        }
        catch (exception& ex)
        {
            cout << ex.what() << endl;
        }
    }

    return 0;
}