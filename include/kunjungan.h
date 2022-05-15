#pragma once
#include <stdio.h>
#include <fstream>
#include <iostream>

#include "./utility.h"
#include "./struct.h"

using namespace std;
namespace kunjungan {
  const string PATH = "../files/kunjungan.csv";

  void menu() {
    utility::header("VISITME - DATA KUNJUNGAN");
  }
  
  void list() {

  }

  void get() {
    
  }

  void create() {
    fstream fout;
    fout.open(PATH, ios::out | ios::app);
    fout<< "\n"
        << "KMR-3" << ", "
        << "Liliy" << ", "
        << "\n";
    fout.close();
  }

  void store() {

  }

  void edit() {

  }

  void update() {

  }

  void destroy() {

  }
}