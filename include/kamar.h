
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
namespace kamar {
  
  const string PATH = "../files/kamar.csv";
  const string NEW_FILE_PATH = "../files/newkamar.csv";
  const int TABLE_COLUMNS_LENGTH = 3;
  string TABLE_COLUMNS[] = {"No.", "Kode Kamar", "Nama Kamar"};
  
  void list() {
    utility::header("VISITME - DAFTAR KAMAR");
    vector<vector<string>> content = utility::list(PATH);
    TextTable table = utility::table(TABLE_COLUMNS_LENGTH, content.size(), TABLE_COLUMNS, content);
    cout << table;
  }

  void store(structure::kamar kamar) {
    fstream fout;
    fout.open(PATH, ios::out | ios::app);
    fout<< kamar.kode << ","
        << kamar.nama << "\n";
    fout.close();
  }

  void create() {
    structure::kamar kamar;
    utility::header("VISITME - TAMBAH KAMAR");
    cout << "Kode Kamar: "; cin >> kamar.kode;
    cout << "Nama Kamar: "; cin >> kamar.nama;
    
    vector<vector<string>> list = utility::search(PATH, 0, kamar.kode);

    if(list.size() < 1) {
      kamar::store(kamar);
      utility::notify("success", "Kamar berhasil ditambahkan!");
    } else {
      utility::notify("error", "Kamar dengan kode berikut sudah ada!");
    }
  }

  void update(string identifier, structure::kamar kamar) {
    string data[] = {kamar.kode, kamar.nama};
    utility::update(PATH, NEW_FILE_PATH, 0, 2, identifier, data);
  }

  void edit() {
    string code;
    utility::header("VISITME - UBAH KAMAR");
    cout << "Masukkan Kode Kamar: "; cin >> code;
    
    vector<vector<string>> list = utility::search(PATH, 0, code);

    if(list.size() > 0) {
      utility::notify("success", "Kamar ditemukan");
      utility::header("VISITME - UBAH KAMAR");

      vector<string> old_kamar = list.front();
      structure::kamar new_kamar;

      utility::cout("yellow", "*Apabila tidak ada perubahan maka biarkan kosong!");

      cout << "Kode Kamar [" + old_kamar[0] + "]: "; cin >> new_kamar.kode;
      cout << "Nama Kamar [" + old_kamar[1] + "]: "; cin >> new_kamar.nama;
      new_kamar.kode = new_kamar.kode != "" ? new_kamar.kode : old_kamar[0];
      new_kamar.nama = new_kamar.nama != "" ? new_kamar.nama : old_kamar[1];

      kamar::update(code, new_kamar);
      utility::notify("success", "Kamar berhasil diubah!");
    } else {
      utility::notify("error", "Kamar dengan kode berikut sudah ada!");
    }
  }

  void destroy() {
    string code;
    bool is_confirmed;
    utility::header("VISITME - HAPUS KAMAR");
    cout << "Kode Kamar: "; cin >> code;
    
    vector<vector<string>> list = utility::search(PATH, 0, code);

    if(list.size() > 0) {
      is_confirmed = utility::confirm("kamar");
      if(is_confirmed) {
        utility::destroy(PATH, NEW_FILE_PATH, 0, 2, code);
        utility::notify("success", "Kamar berhasil dihapus!");
      }
    } else {
      utility::notify("error", "Kamar dengan kode berikut tidak ada!");
    }
  }

  void sort() {
    int column, type;
    utility::header("VISITME - SORTING KAMAR");
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
    utility::header("VISITME - CARI KAMAR");
    cout << "Kata kunci pencarian: "; cin >> keyword;
    vector<vector<string>> list = utility::search(PATH, -1, keyword);
    if(list.size() > 0) {
      TextTable table = utility::table(TABLE_COLUMNS_LENGTH, list.size(), TABLE_COLUMNS, list);
      cout << table;
    } else {
      utility::notify("error", "Data tidak ditemukan!");
    }
  }

  int menu() {
    int choice;
    utility::header("VISITME - MANAJEMEN KAMAR");
    cout  << "1. Daftar Kamar" << endl
          << "2. Tambah Kamar" << endl
          << "3. Ubah Kamar" << endl
          << "4. Hapus Kamar" << endl
          << "5. Kembali" << endl
          << "Pilih : "; cin >> choice;
    return choice;
  }

  void index() {
    bool is_running = true;
    bool is_list_running = false;
    int choice, list_choice;
    while(is_running) {
      choice = kamar::menu();

      switch (choice) {
        case 1:

          list_choice;
          is_list_running = true;
          kamar::list();
          
          while(is_list_running) {
            utility::cout("black", "\nApa yang ingin anda lakukan :");
            utility::cout("yellow", "1. Sorting     2. Searching    3. Kembali");
            cout << "Pilih : "; cin >> list_choice;
            switch (list_choice) {
              case 1:
                kamar::sort();
                break;
              case 2:
                kamar::search();
                break;
              case 3:
                is_list_running = false;
                break;
            }
          }
          break;
        case 2:
          kamar::create();
          break;
        case 3:
          kamar::edit();
          break;
        case 4:
          kamar::destroy();
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