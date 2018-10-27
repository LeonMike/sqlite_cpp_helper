/*
  database.cpp is part of sqlite_cpp_helper.
  
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

#include <database.hpp>

namespace sqlite_helper {

  Database::Database(string filepath): db_filepath(filepath) {
    sqlite3_open(db_filepath.c_str(), &Db);
  }

  Database::~Database() {
    sqlite3_close(Db);
  }

  Table &Database::New_Table(string name) {
    tables[name] = new Table(Db);
    tables[name]->name = name;
    return *tables[name];
  }

  Table & Database::operator [](string name) {
    return *tables[name];
  }

}
