/*
table.hpp is part of sqlite_cpp_helper.

Foobar is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Foobar is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Foobar.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <sqlite3.h>
#include <map>
#include <string>
#include <sstream>

using namespace std;

namespace sqlite_helper {

  struct COLUMN_DESC {
    string type;
    int    max_length;
    bool   primary_key;
    bool   isNull;
    bool   not_null;
    string default_value;
    string value;
    bool   foreign_key;
    string reference_table;
    string reference_column;

    string operator()();
    
    string Set(int newValue);
    string Set(bool newValue);
    string Set(float newValue);
    string Set(string newValue);
    string Set(const char *newValue);
  };

  typedef pair<string, COLUMN_DESC> COLUMN;
  typedef pair<int, string> COLUMNS_ORDER_ITEM;
  typedef map<string, COLUMN_DESC> COLUMNS_MAP;
  typedef map<int, string> COLUMNS_ORDER;
  
  class Table {
  private:
    sqlite3 *Db;
    COLUMNS_MAP columns;
    COLUMNS_ORDER order;

    string last_column;

    void create_column(string name, string type);
    void create_column(string name, string type, int max_length);

    string sqlSave();
  public:
    Table(sqlite3 *db);
    ~Table();

    void   Create();

    string name;
    
    Table &integer(string name);
    Table &text(string name);
    Table &text(string name, int max);
    Table &date(string name);
    Table &real(string name);
    Table &boolean(string name);
    
    Table &primary();
    Table &defaultValue(int value);
    Table &defaultValue(bool value);
    Table &defaultValue(float value);
    Table &defaultValue(string value);
    Table &defaultValue(const char *value);
    Table &foreign(string column);
    Table &references(string table, string column);

    Table &NewRow();
    Table &Set(string column, int value);
    Table &Set(string column, bool value);
    Table &Set(string column, float value);
    Table &Set(string column, string value);
    Table &Set(string column, const char *value);
    Table &Save();
    
    string toSql();

    COLUMN_DESC &operator [](string name);
    string operator ()(string value);
  };
  
}
