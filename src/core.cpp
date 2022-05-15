#include <iostream>

#include "../include/utility.h"
#include "../include/kamar.h"
#include "../include/napi.h"
#include "../include/kunjungan.h"

using namespace std;


int menuMain() {
  int choice;
  utility::header("VISITME - KUNJUNGAN LAPAS");
  cout  << "1. Tambah Kunjungan" << endl
        << "2. Data Master" << endl
        << "3. Keluar" << endl
        << "Pilih : "; cin >> choice;
  return choice;
}

int menuMaster() {
  int choice;
  utility::header("VISITME - DATA MASTER");
  cout  << "1. Kunjungan" << endl
        << "2. WBP" << endl
        << "3. Kamar" << endl
        << "4. Kembali" << endl
        << "Pilih : "; cin >> choice;
  return choice;
}

int main() {
  bool is_running = true;
  bool is_master_running = false;
  int master_choice , main_choice;

  while(is_running) {
    main_choice = menuMain();
    switch (main_choice) {
      case 1:
        kunjungan::create();
        break;
      case 2:
        is_master_running = true;

        while(is_master_running) {
          master_choice = menuMaster();
          switch (master_choice) {
            case 1:
              kunjungan::menu();
              break;
            case 2:
              napi::index();
              break;
            case 3:
              kamar::index();
              break;
            case 4:
              is_master_running = false;
              break;
          }
        }
        break;
      case 3:
        is_running = false;
        utility::notify("info", "Terima kasih!");
        break;
      default:
        utility::notify("error", "Pilihan tidak ada!");
        break;
    }
  }
  return 0;
}

