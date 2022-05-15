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

  void header(string title) {
    // system("cls");
    utility::cout("cyan", "\n\n=================================");
    utility::cout("cyan", title);
    utility::cout("cyan", "=================================");
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

    transform(keyword.begin(), keyword.end(), keyword.begin(), ::tolower);

    for(int index = 0; index < list.size(); index++) {
      // * IF THE FIELD IS -1, THEN IT MEANS AN UNIVERSAL SEARCH
      if(field == -1) {
        for(int second_index = 0; second_index < list[index].size(); second_index++) {
          compared = list[index][second_index];
          transform(compared.begin(), compared.end(), compared.begin(), ::tolower);
          if(compared == keyword) {
            filtered.push_back(list[index]);
          }
        }
      } else {
        compared = list[index][field];
        transform(compared.begin(), compared.end(), compared.begin(), ::tolower);
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
      for(int row = 0; row < content.size(); row++) {
        string val;
        if(content[row][field] != keyword) {
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
    } else {
      utility::notify("error", "File tidak ada!");
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