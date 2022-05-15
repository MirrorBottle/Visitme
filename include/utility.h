#pragma once

#include <iostream>
#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <map>
#include <functional>
#include <algorithm>

#include "./struct.h"
#include "./data.h"
#include "./table.h"


namespace utility {
  void cout(string color, string message, bool with_endl = true) {
    string hex = data::colors[color];
    string new_msg = hex + message + "\033[0m";
    with_endl ? std::cout << new_msg << endl : std::cout << new_msg;
  }

  bool confirm(string message, bool is_formatted = true) {
    string is_confirmed = "t";
    string new_message = is_formatted ? "Data " + message + " tidak akan bisa dikembalikan lagi! (y/t): " : message;
    std::cout << endl;
    utility::cout("red", "Apa anda yakin?");
    utility::cout("red", new_message, false);
    cin >> is_confirmed;

    return is_confirmed == "y" || is_confirmed == "Y";
  }

  void header(string title) {
    // system("cls");
    utility::cout("cyan", "\n\n=================================");
    utility::cout("cyan", title);
    utility::cout("cyan", "=================================");
  }

  string toLower(string word) {
    string transformed = word;
    transform(transformed.begin(), transformed.end(), transformed.begin(), ::tolower);
    return transformed;
  }

  void notify(string type, string message) {
    map<string, string> types = {
      { "success", "green" },
      { "error", "red" },
      { "warning", "yellow" },
      { "info", "cyan" },
    };
    utility::cout(types[type], message);
    getch();
  }

  vector<vector<string>> list(string path) {
    fstream file;
    file.open(path, ios::in);
    vector<vector<string>> content;
    vector<string> row;
    string line, word;

    if(file.is_open()) {
      while(getline(file, line)) {
        row.clear();
        stringstream str(line);
        while(getline(str, word, ','))
          row.push_back(word);
        content.push_back(row);
      }
    } else {
      utility::notify("error", "File tidak ada!");
    }
    file.close();
    return content;
  }

  vector<string> latest(string path) {
    vector<vector<string>> content = utility::list(path);
    return content.back();
  }
  vector<vector<string>> search(string path, int field, string keyword) {
    vector<vector<string>> filtered;
    vector<vector<string>> list = utility::list(path);
    string compared;

    keyword = utility::toLower(keyword);

    for(int index = 0; index < list.size(); index++) {
      // * IF THE FIELD IS -1, THEN IT MEANS AN UNIVERSAL SEARCH
      if(field == -1) {
        for(int second_index = 0; second_index < list[index].size(); second_index++) {
          compared = utility::toLower(list[index][second_index]);
          if(compared == keyword) {
            filtered.push_back(list[index]);
          }
        }
      } else {
        compared = utility::toLower(list[index][field]);
        if(compared == keyword) {
          filtered.push_back(list[index]);
        }
      }
    }
    return filtered;
  }

  vector<vector<string>> sort(string path, int field, int type) {
    vector<vector<string>> list = utility::list(path);
    for(int i=0; i < list.size() - 1; i++) {           
      for(int j=i+1; j < list.size(); j++) { 
        // * 1 = ASC, 2 = DESC
        bool is_pass = type == 1 ? list[i][field] > list[j][field] : list[i][field] < list[j][field];
        if(is_pass) {      
          vector<string> temp = list[j];
          list[j] = list[i];
          list[i] = temp;
        }
      }
    }

    
    return list;
  }

  void update(std::string path, std::string newpath, int field, int field_length, string keyword, string new_data[]) {
    fstream file, newfile;
    file.open(path, ios::in);
    newfile.open(newpath, ios::out);

    keyword = utility::toLower(keyword);

    vector<vector<string>> content = utility::list(path);
    for(int row = 0; row < content.size(); row++) {
      string val;
      string compared = utility::toLower(content[row][field]);
      if(compared != keyword) {
        for(int col = 0; col < content[row].size(); col++) {
          val += content[row][col] + ",";
        }
      } else {
        for(int new_data_col = 0; new_data_col < field_length; new_data_col++) {
          val += new_data[new_data_col] + ",";
        }
      }
      val += "\n";
      newfile << val;
    }
    file.close();
    newfile.close();
    remove(path.c_str());
    rename(newpath.c_str(), path.c_str());
  }

  void destroy(std::string path, std::string newpath, int field, int field_length, string keyword) {
    fstream file, newfile;
    file.open(path, ios::in);
    newfile.open(newpath, ios::out);

    keyword = utility::toLower(keyword);

    vector<vector<string>> content = utility::list(path);
    for(int row = 0; row < content.size(); row++) {
      string val;
      string compared = utility::toLower(content[row][field]);
      if(compared != keyword) {
        for(int col = 0; col < content[row].size(); col++) {
          val += content[row][col] + ",";
        }
        val += "\n";
      }
      newfile << val;
    }
    file.close();
    newfile.close();
    remove(path.c_str());
    rename(newpath.c_str(), path.c_str());
  }



  TextTable table(int cols, int rows, string headers[], vector<vector<string>> data) {
    TextTable table( '-', '|', '+' );

    // SET HEADERS
    for(int col = 0; col < cols; col++) {
      table.add(headers[col]);
    }
    table.endOfRow();

    // SET ROWS
    for(int row = 0; row < rows; row++) {
      for(int col = 0; col < cols; col++) {
        string val = col != 0 ? data[row][col - 1] : to_string(row + 1);
        table.add(val);
      }
      table.endOfRow();
    }
    return table;
  }
};