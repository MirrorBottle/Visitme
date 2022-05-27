#pragma once
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <ctime>

#include "./utility.h"
#include "./struct.h"

using namespace std;
namespace kunjungan {
  const string PATH = "../files/kunjungan.csv";
  const string WBP_PATH = "../files/wbp.csv";

  void menu() {
    utility::header("VISITME - DATA KUNJUNGAN");
  }
  
  void list() {

  }

  void get() {
    
  }

  void store(structure::kunjungan kunjungan) {
    fstream fout;
    string today = utility::today();
    vector<string> latest = utility::latest(PATH);
    string number = latest.empty() ? "0" : latest[0].substr(3, latest[0].find("-"));

    kunjungan.kode = "KN-" + to_string(stoi(number) + 1);
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
      vector<vector<string>> wbps;
      while(!is_wbp_exist) {
        cout << "Masukkan Kode WBP: "; cin >> kunjungan.kode_wbp;
        wbps = utility::search(WBP_PATH, { 0 }, kunjungan.kode_wbp);
        if(wbps.size() > 0) {
          is_wbp_exist = true;
          utility::cout("green", "WBP Ditemukan: [" + wbps[0][1] + "]!");
        } else {
          utility::notify("error", "Kode WBP tidak terdaftar!");
        }
      }
      kunjungan.kode_wbp = wbps[0][0];
      cout << "Masukkan Nama Anda: "; fflush(stdin);
      getline(cin, kunjungan.nama_pengunjung);
      kunjungan::store(kunjungan);
    } else {
      utility::notify("error", "Anda sudah melakukan registrasi!");
    }
  }

  void edit() {

  }

  void update() {

  }

  void destroy() {

  }
}