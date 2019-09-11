/*
  table.cpp is part of sqlite_cpp_helper.
  
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

#include <table.hpp>
#include <sstream>
#include <iostream>

using namespace std;

namespace sqlite_cpp_helper {

  string COLUMN_DESC::operator ()() {
    return value;
  }
  
  string COLUMN_DESC::Set(int newValue) {
    isNull = true;
    if (&newValue != NULL) {
      stringstream ss;
      ss << newValue;
      value = ss.str();
      isNull = false;
    }
    return value;
  }

  string COLUMN_DESC::Set(bool newValue) {
    isNull = true;
    if (&newValue != NULL) {
      stringstream ss;
      ss << newValue;
      value = ss.str();
      isNull = false;
    }
    return value;
  }
  
  string COLUMN_DESC::Set(float newValue) {
    isNull = true;
    if (&newValue != NULL) {
      stringstream ss;
      ss << newValue;
      value = ss.str();
      isNull = false;
    }
    return value;
  }

  string COLUMN_DESC::Set(string newValue) {
    isNull = false;
    value = newValue;
    return value;
  }

  string COLUMN_DESC::Set(const char *newValue) {
    isNull = true;
    if (newValue != NULL) {
      stringstream ss(newValue);
      value = ss.str();
      isNull = false;
    }
    return value;
  }
  
  Table::Table(sqlite3 *db): Db(db) {}
  Table::Table(Table &base): Db(base.Db) {}
  Table::~Table() { sqlite3_close(Db); columns.clear(); }

  void Table::create_column(string name, string type) {
    COLUMN_DESC desc;
    desc.type = type;
    desc.max_length = 0;
    desc.primary_key = false;
    desc.isNull = true;
    desc.not_null = false;
    desc.default_value = "NULL";
    desc.value = "";
    desc.foreign_key = false;
    desc.reference_table = "";
    desc.reference_column = "";
    columns.insert(COLUMN(name, desc));
    order.insert(COLUMNS_ORDER_ITEM(columns.size() - 1, name));
    last_column = name;
  }

  void Table::create_column(string name, string type, int max_length) {
    COLUMN_DESC desc;
    desc.type = type;
    desc.max_length = max_length;
    desc.primary_key = false;
    desc.isNull = true;
    desc.not_null = false;
    desc.default_value = "NULL";
    desc.value = "";
    desc.foreign_key = false;
    desc.reference_table = "";
    desc.reference_column = "";
    columns.insert(COLUMN(name, desc));
    order.insert(COLUMNS_ORDER_ITEM(columns.size() - 1, name));
    last_column = name;
  }
  
  void Table::Create() {
    char *zErrMsg = 0;
    cout << toSql() << endl;
    //int rc = sqlite3_exec(Db, toSql(), callback, 0, zErrMsg);
    int rc = sqlite3_exec(Db, toSql().c_str(), NULL, NULL, &zErrMsg);
    if (rc != SQLITE_OK) {
      cout << "SQL Error: " << zErrMsg << endl;
    }
  }
  
  Table &Table::integer(string name) { create_column(name, "INT"); return *this; }
  Table &Table::text(string name) { create_column(name, "TEXT"); return *this; }
  Table &Table::text(string name, int max_length) { create_column(name, "VARCHAR", max_length); return *this; }
  Table &Table::date(string name) { create_column(name, "DATE"); return *this; }
  Table &Table::real(string name) { create_column(name, "REAL"); return *this; }
  Table &Table::boolean(string name) { create_column(name, "BOOLEAN"); return *this; }
  
  Table &Table::primary() { columns[last_column].primary_key = true; return *this; }
  Table &Table::defaultValue(int value) { stringstream ss; ss << value; columns[last_column].default_value = ss.str(); return *this; }
  Table &Table::defaultValue(bool value) { stringstream ss; ss << (value ? 1 : 0); columns[last_column].default_value = ss.str(); return *this; }
  Table &Table::defaultValue(float value) { stringstream ss; ss << value; columns[last_column].default_value = ss.str(); return *this; }
  Table &Table::defaultValue(string value) { columns[last_column].default_value = "'" + value + "'"; return *this; }
  Table &Table::defaultValue(const char *value) { stringstream ss(value); columns[last_column].default_value = "'" + ss.str() + "'"; return *this; }
  Table &Table::foreign(string column) { columns[column].foreign_key = true; last_column = column; return *this; }
  Table &Table::references(string table, string column) { columns[last_column].reference_table = table; columns[last_column].reference_column = column; return *this; }

  Table &Table::NewRow() {
    for (COLUMN col : columns) {
      col.second.value = "";
      col.second.isNull = true;
    }
    return *this;
  }
  
  Table &Table::Set(string column, int value) {
    columns[column].isNull = true;
    if (&value != NULL) {
      stringstream ss("");
      ss << value;
      columns[column].value = ss.str();
      columns[column].isNull = false;
    }
    return *this;
  }

  Table &Table::Set(string column, bool value) {
    columns[column].isNull = true;
    if (&value != NULL) {
      stringstream ss("");
      ss << value;
      columns[column].value = ss.str();
      columns[column].isNull = false;
    }
    return *this;
  }
  
  Table &Table::Set(string column, float value) {
    columns[column].isNull = true;
    if (&value != NULL) {
      stringstream ss("");
      ss << value;
      columns[column].value = ss.str();
      columns[column].isNull = false;
    }
    return *this;
  }

  Table &Table::Set(string column, string value) {
    columns[column].value = value;
    columns[column].isNull = false;
    return *this;
  }

  Table &Table::Set(string column, const char *value) {
    columns[column].isNull = true;
    if (&value != NULL) {
      stringstream ss(value);
      columns[column].value = ss.str();
      columns[column].isNull = false;
    }
    return *this;
  }

  Table &Table::Save() {
    char *zErrMsg = 0;
    string command = sqlSave();
    cout << command << endl;
    int rc = sqlite3_exec(Db, command.c_str(), NULL, NULL, &zErrMsg);
    if (rc != SQLITE_OK) {
      cout << "SQL Error: " << zErrMsg << endl;
    }
    return *this;
  }

  string Table::sqlSave() {
    string result = "INSERT INTO " + name + " VALUES";
    if (columns.size() > 0) {
      stringstream columnsStr("");
      /*for (COLUMN col : columns) {
	cout << "Col (" << col.first << ")" << endl <<
	  "\ttype:\t" << col.second.type << endl <<
	  "\tmax_length:\t" << col.second.max_length << endl <<
	  "\tprimary_key:\t" << col.second.primary_key << endl <<
	  "\tnot_null:\t" << col.second.not_null << endl <<
	  "\tdefault_value:\t" << col.second.default_value << endl <<
	  "\tvalue:\t" << col.second.value << endl <<
	  "\tforeign_key:\t" << col.second.foreign_key << endl <<
	  "\treference_table:\t" << col.second.reference_table << endl <<
	  "\treference_colum:\t" << col.second.reference_column << endl;
      }*/
      for (COLUMNS_ORDER_ITEM item : order) {
	if (columnsStr.str() != "") {
	  columnsStr << ", ";
	}
	if (!columns[item.second].isNull) {
	  if (columns[item.second].type == "TEXT" ||
	      columns[item.second].type.substr(0, 7) == "VARCHAR" ||
	      columns[item.second].type == "DATE") {
	    columnsStr << "'" << columns[item.second].value << "'";
	  } else
	    columnsStr << columns[item.second].value;
	} else {
	  columnsStr << "NULL";
	}
      }
      result += " (" + columnsStr.str() + ")";
    }
    return result;
  }
  
  string Table::toSql() {
    string result = "CREATE TABLE " + name;
    if (columns.size() > 0) {
      stringstream columnsStr("");
      string foreignsStr = "";
      /*for (COLUMN col : columns) {
	cout << "Col (" << col.first << ")" << endl <<
	  "\ttype:\t" << col.second.type << endl <<
	  "\tmax_length:\t" << col.second.max_length << endl <<
	  "\tprimary_key:\t" << col.second.primary_key << endl <<
	  "\tnot_null:\t" << col.second.not_null << endl <<
	  "\tdefault_value:\t" << col.second.default_value << endl <<
	  "\tvalue:\t" << col.second.value << endl <<
	  "\tforeign_key:\t" << col.second.foreign_key << endl <<
	  "\treference_table:\t" << col.second.reference_table << endl <<
	  "\treference_colum:\t" << col.second.reference_column << endl;
      }*/
      for (COLUMNS_ORDER_ITEM item : order) {
	if (columnsStr.str() != "") {
	  columnsStr << ", ";
	}
	if (columns[item.second].foreign_key) {
	  foreignsStr += ", " \
	    "FOREIGN KEY(" + item.second + ") "				\
	    "REFERENCES " + columns[item.second].reference_table + "(" + columns[item.second].reference_column + ")";
	}
	columnsStr <<
	  item.second << " " <<
	  columns[item.second].type;
	if (columns[item.second].max_length != 0) {
	  columnsStr << "(" << columns[item.second].max_length << ")";
	}
	columnsStr <<
	  (columns[item.second].primary_key ? " PRIMARY KEY" : "") <<
	  (columns[item.second].default_value != "" ? " DEFAULT " + columns[item.second].default_value : "") <<
	  (columns[item.second].not_null ? " NOT NULL" : "");
      }
      result += " (" + columnsStr.str() + foreignsStr + ")";
    }
    return result + ";";
  }


  COLUMN_DESC &Table::operator [](string name) {
    return columns[name.c_str()];
  }

}
