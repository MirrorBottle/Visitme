
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>

#include "./utility.h"
#include "./struct.h"
#include "./table.h"

using namespace std;
namespace wbp {
  
  const string PATH = "../files/wbp.csv";
  const int TABLE_COLUMNS_LENGTH = 3;
  string TABLE_COLUMNS[] = {"No.", "Kode WBP", "Nama WBP"};
  
  void list() {
    utility::header("VISITME - DAFTAR WBP");
    vector<vector<string>> content = utility::list(PATH);
    TextTable table = utility::table(TABLE_COLUMNS_LENGTH, content.size(), TABLE_COLUMNS, content);
    cout << table;
  }

  void store(structure::wbp wbp) {
    fstream fout;
    fout.open(PATH, ios::out | ios::app);
    fout<< wbp.kode << ","
        << wbp.nama << "\n";
    fout.close();
  }

  void create() {
    structure::wbp wbp;
    utility::header("VISITME - TAMBAH WBP");
    cout << "Kode WBP: "; cin >> wbp.kode;
    cout << "Nama WBP: "; cin >> wbp.nama;
    
    vector<vector<string>> list = utility::search(PATH, { 0 }, wbp.kode);

    if(list.size() < 1) {
      wbp::store(wbp);
      utility::notify("success", "WBP berhasil ditambahkan!");
    } else {
      utility::notify("error", "WBP dengan kode berikut sudah ada!");
    }
  }

  void update(string identifier, structure::wbp wbp) {
    string data[] = {wbp.kode, wbp.nama};
    utility::update(PATH, 0, 2, identifier, data);
  }

  void edit() {
    string code;
    utility::header("VISITME - UBAH WBP");
    cout << "Masukkan Kode WBP: "; cin >> code;
    
    vector<vector<string>> list = utility::search(PATH, { 0 }, code);

    if(list.size() > 0) {
      utility::notify("success", "WBP ditemukan");
      utility::header("VISITME - UBAH WBP");

      vector<string> old_wbp = list.front();
      structure::wbp new_wbp;

      utility::cout("yellow", "*Apabila tidak ada perubahan maka biarkan kosong!");

      cout << "Kode WBP [" + old_wbp[0] + "]: "; cin >> new_wbp.kode;
      cout << "Nama WBP [" + old_wbp[1] + "]: "; cin >> new_wbp.nama;
      new_wbp.kode = new_wbp.kode != "" ? new_wbp.kode : old_wbp[0];
      new_wbp.nama = new_wbp.nama != "" ? new_wbp.nama : old_wbp[1];

      wbp::update(code, new_wbp);
      utility::notify("success", "WBP berhasil diubah!");
    } else {
      utility::notify("error", "WBP dengan kode berikut sudah ada!");
    }
  }

  void destroy() {
    string code;
    bool is_confirmed;
    utility::header("VISITME - HAPUS WBP");
    cout << "Kode WBP: "; cin >> code;
    
    vector<vector<string>> list = utility::search(PATH, { 0 }, code);

    if(list.size() > 0) {
      is_confirmed = utility::confirm("wbp");
      if(is_confirmed) {
        utility::destroy(PATH, 0, 2, code);
        utility::notify("success", "WBP berhasil dihapus!");
      }
    } else {
      utility::notify("error", "WBP dengan kode berikut tidak ada!");
    }
  }

  void sort() {
    int column, type;
    utility::header("VISITME - SORTING WBP");
    utility::cout("yellow", "Daftar kolom: ");
    for(int i = 1; i < TABLE_COLUMNS_LENGTH; i++) { 
      utility::cout("yellow", to_string(i) + ". " + TABLE_COLUMNS[i]);
    }
    cout << "\nPilih kolom: "; cin >> column;
    cout << "Pilih tipe (1 = asc, 2 = desc): "; cin >> type;
    column = column - 1;
    if(column >= 0 && column < TABLE_COLUMNS_LENGTH) {
      vector<vector<string>> list = utility::sort(PATH, column, type);
      TextTable table = utility::table(TABLE_COLUMNS_LENGTH, list.size(), TABLE_COLUMNS, list);
      cout << table;
    } else {
      utility::notify("error", "Pilihan kolom tidak ada!");
    }
  }

  void search() {
    string keyword;
    utility::header("VISITME - CARI WBP");
    cout << "Kata kunci pencarian: "; fflush(stdin);
    getline(cin, keyword);
    vector<vector<string>> list = utility::search(PATH, { -1 }, keyword);
    if(list.size() > 0) {
      TextTable table = utility::table(TABLE_COLUMNS_LENGTH, list.size(), TABLE_COLUMNS, list);
      cout << table;
    } else {
      utility::notify("error", "Data tidak ditemukan!");
    }
  }

  int menu() {
    int choice;
    utility::header("VISITME - MANAJEMEN WBP");
    cout  << "1. Daftar WBP" << endl
          << "2. Tambah WBP" << endl
          << "3. Ubah WBP" << endl
          << "4. Hapus WBP" << endl
          << "5. Kembali" << endl
          << "Pilih : "; cin >> choice;
    return choice;
  }

  void index() {
    bool is_running = true;
    bool is_list_running = false;
    int choice, list_choice;
    while(is_running) {
      choice = wbp::menu();
      switch (choice) {
        case 1:
          is_list_running = true;
          wbp::list();
          
          while(is_list_running) {
            utility::cout("black", "\nApa yang ingin anda lakukan :");
            utility::cout("yellow", "1. Sorting     2. Searching    3. Kembali");
            cout << "Pilih : "; cin >> list_choice;
            switch (list_choice) {
              case 1:
                wbp::sort();
                break;
              case 2:
                wbp::search();
                break;
              case 3:
                is_list_running = false;
                break;
            }
          }
          break;
        case 2:
          wbp::create();
          break;
        case 3:
          wbp::edit();
          break;
        case 4:
          wbp::destroy();
          break;
        case 5:
          is_running = false;
          break;
        default:
          utility::notify("error", "Pilihan tidak ada!");
          break;
      }
    }
  }
}