#pragma once
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <ctime>

#include "./utility.h"
#include "./struct.h"
#include "./wbp.h"
#include "./kamar.h"

using namespace std;
namespace kunjungan {
  const string PATH = "../files/kunjungan.csv";
  const int TABLE_COLUMNS_LENGTH = 11;
  string TABLE_COLUMNS[] = {"No.", "Kode", "WBP", "Tanggal", "Nama", "NIK", "Status", "Jam Mulai", "Jam Selesai", "Kode Kamar", "Catatan"};



  vector<vector<string>> formatter(vector<vector<string>> list) {

    vector<vector<string>> formatted;
    for(int rowIdx = 0; rowIdx < list.size(); rowIdx++) {
      vector<string> row = list[rowIdx];

      string col_data;
      for(int colIdx = 0; colIdx < TABLE_COLUMNS_LENGTH - 1; colIdx++) {
        col_data = row[colIdx];
        if(colIdx == 1) {
          vector<string> wbp = utility::find(wbp::path(), { 0 }, col_data);
          row[colIdx] = col_data + " [" + wbp[1] + "]";
        }

        if(colIdx == 5) {
          int status = stoi(col_data);
          row[colIdx] = data::label_status_kunjungan[status];
        }

        if(colIdx == 8) {
          if(col_data != "-") {
            vector<string> kamar = utility::find(kamar::path(), { 0 }, col_data);
            row[colIdx] = col_data + " [" + kamar[1] + "]";
          }
        }
      }

      formatted.push_back(row);
    }
    return formatted;
  }
  void list() {
    utility::header("VISITME - DAFTAR KUNJUNGAN");
    vector<vector<string>> content = utility::list(PATH);
    vector<vector<string>> list = kunjungan::formatter(content);
    TextTable table = utility::table(TABLE_COLUMNS_LENGTH, list.size(), TABLE_COLUMNS, list);
    cout << table;
  }

  structure::kunjungan get(vector<string> data) {
    string path_wbp = wbp::path();
    string path_kamar = kamar::path();
    
    structure::kunjungan kunjungan;

    kunjungan.kode = data[0];
    kunjungan.kode_wbp = data[1];
    kunjungan.tanggal = data[2];
    kunjungan.nama_pengunjung = data[3];
    kunjungan.nik_pengunjung = data[4];
    kunjungan.status = stoi(data[5]);
    kunjungan.jam_mulai = data[6];
    kunjungan.jam_selesai = data[7];
    kunjungan.kode_kamar = data[8];
    kunjungan.catatan = data[9];

    vector<string> wbp = utility::find(path_wbp , { 0 }, kunjungan.kode_wbp);
    kunjungan.wbp.kode = wbp[0];
    kunjungan.wbp.nama = wbp[1];

    if(kunjungan.kode_kamar != "-") {
      vector<string> kamar = utility::find(path_kamar , { 0 }, kunjungan.kode_kamar);
      kunjungan.kamar.kode = kamar[0];
      kunjungan.kamar.nama = kamar[1];
    }

    return kunjungan;
  }

  void store(structure::kunjungan kunjungan) {
    fstream fout;
    string today = utility::today();
    vector<string> latest = utility::latest(PATH);
    int number = latest.empty() ? 0 : stoi(latest[0].substr(3, latest[0].find("-")));
    string next_number = number > 9 ? to_string(number + 1) : "0" + to_string(number + 1);
    kunjungan.kode = "KN-" + next_number;
    kunjungan.tanggal = today;
    kunjungan.status = data::status_kunjungan["menunggu"];

    kunjungan.jam_mulai = "-";
    kunjungan.jam_selesai = "-";
    kunjungan.kode_kamar = "-";
    kunjungan.catatan = "-";

    fout.open(PATH, ios::out | ios::app);
    fout<< kunjungan.kode << ","
        << kunjungan.kode_wbp << ","
        << kunjungan.tanggal << ","
        << kunjungan.nama_pengunjung << ","
        << kunjungan.nik_pengunjung << ","
        << kunjungan.status << ","
        << kunjungan.jam_mulai << ","
        << kunjungan.jam_selesai << ","
        << kunjungan.kode_kamar << ","
        << kunjungan.catatan << "\n";
    fout.close();

    utility::notify("success", "Berhasil Melakukan Registrasi, Kode Kunjungan: [" + kunjungan.kode + "]");
  }

  void create() {
    bool is_not_exist = true;
    bool is_wbp_exist = false;
    string today = utility::today();
    string wbp_path = wbp::path();
    structure::kunjungan kunjungan;
    utility::header("VISITME - REGISTRASI KUNJUNGAN");
    utility::cout("yellow", "*Kunjungan hanya bisa dilakukan sekali sehari!");
    utility::cout("yellow", "*Cek status kunjungan anda secara berkala!\n");


    cout << "Masukkan NIK Anda: "; cin >> kunjungan.nik_pengunjung;

    // ** CHECK IF USER ALREADY REGIST TODAY
    vector<vector<string>> list = utility::list(PATH);

    for(int index = 0; index < list.size(); index++) {
      vector<string> row = list[index];
      if(row[2] == today && row[4] == kunjungan.nik_pengunjung) {
        is_not_exist = false;
      }
    }

    if (is_not_exist) {
      vector<string> wbp;
      while(!is_wbp_exist) {
        cout << "Masukkan Kode WBP: "; cin >> kunjungan.kode_wbp;
        wbp = utility::find(wbp_path, { 0 }, kunjungan.kode_wbp);
        if(!wbp.empty()) {
          is_wbp_exist = true;
          utility::cout("green", "WBP Ditemukan: [" + wbp[1] + "]!");
        } else {
          utility::notify("error", "Kode WBP tidak terdaftar!");
        }
      }
      kunjungan.kode_wbp = wbp[0];
      cout << "Masukkan Nama Anda: "; fflush(stdin);
      getline(cin, kunjungan.nama_pengunjung);
      kunjungan::store(kunjungan);
    } else {
      utility::notify("error", "Anda sudah melakukan registrasi!");
    }
  }

  void check() {
    string keyword;
    utility::header("VISITME - CEK STATUS KUNJUNGAN");
    cout << "Masukkan Kode Kunjungan / NIK Anda: "; cin >> keyword;
    vector<vector<string>> list = utility::search(PATH, { 0, 4 }, keyword);

    if(list.size() > 0) {
      for(int index = 0; index < list.size(); index++) {
        string description;
        vector<string> row = list[index];

        structure::kunjungan kunjungan = kunjungan::get(row);
        if(index == 0) {
          cout << "Nama Pengunjung: " << kunjungan.nama_pengunjung << endl;
          cout << "NIK Pengunjung: " << kunjungan.nik_pengunjung << endl << endl;
        }

        string label_status = data::label_status_kunjungan[kunjungan.status];
        string label_color = data::color_status_kunjungan[kunjungan.status];

        switch (kunjungan.status) {
          case 1:
            description = "Kunjungan masih dalam tahap validasi. Mohon cek kembali nanti.";
            break;
          case 2:
            description = "Kamar: " + kunjungan.kamar.nama +  ", Jam: " + kunjungan.jam_mulai + " - " + kunjungan.jam_selesai;
            break;
          case 3:
            description = kunjungan.catatan;
            break;
        }
        utility::cout("cyan", kunjungan.kode, false);
        cout << " " << kunjungan.tanggal << " ";
        utility::cout(label_color, label_status);
        cout << "Keterangan: " << description << endl << endl;
      }
    } else {
      utility::notify("error", "Tidak ada kunjungan!");
    }
    getch();
  }

  void validate() {

  }

  void edit() {

  }

  void update() {

  }

  void destroy() {

  }

  void sort() {
    int column, type;
    utility::header("VISITME - SORTING KUNJUNGAN");
    tie(column, type) = menu::sorting(TABLE_COLUMNS, TABLE_COLUMNS_LENGTH);
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
    utility::header("VISITME - CARI KUNJUNGAN");
    cout << "Kata kunci pencarian: "; cin >> keyword;
    vector<vector<string>> list = utility::search(PATH, { -1 }, keyword);
    if(list.size() > 0) {
      TextTable table = utility::table(TABLE_COLUMNS_LENGTH, list.size(), TABLE_COLUMNS, list);
      cout << table;
    } else {
      utility::notify("error", "Data tidak ditemukan!");
    }
  }

  void index() {
    bool is_running = true;
    bool is_list_running = false;
    int choice, list_choice;
    while(is_running) {
      choice = menu::kunjungan();

      switch (choice) {
        case 1:

          list_choice;
          is_list_running = true;
          kunjungan::list();
          
          while(is_list_running) {
            list_choice = menu::searching();
            switch (list_choice) {
              case 1:
                kunjungan::sort();
                break;
              case 2:
                kunjungan::search();
                break;
              case 3:
                is_list_running = false;
                break;
            }
          }
          break;
        case 2:
          kunjungan::create();
          break;
        case 3:
          kunjungan::edit();
          break;
        case 4:
          kunjungan::destroy();
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