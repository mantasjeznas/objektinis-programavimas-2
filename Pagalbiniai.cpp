#include "Pagalbiniai.h"
#include <iostream>
#include <limits>
#include <cstdlib>
#include <cstdio>
using std::cin;
using std::cout;

int skaicius_input(){
    int sk;
    while (true) {
        if (cin >> sk) {
            int c = cin.peek();
            while (c == ' ' || c == '\t' || c == '\r') {
                cin.get();
                c = cin.peek();
            }
            if (c == '\n' || c == EOF) return sk;
        } else{
            cin.clear();
        }
        cout << "Netinkama ivestis. Iveskite skaiciu: ";
        ignoruoti_eilute();
    }
}

int pazymio_apribojimas(){
    while (true) {
        int x = skaicius_input();
        if (x >= 0 && x <= 10) return x;
        cout << "Pazymys turi buti nuo 0 iki 10: ";
    }
}

void ignoruoti_eilute(){
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int random_pazymys(){
    return rand() % 11;
}

int pasirinkimas_tarp(int nuo, int iki){
    while (true) {
        cout << "Pasirinkimas: ";
        int x = skaicius_input();
        if (x >= nuo && x <= iki) return x;
        cout << "Tokio pasirinkimo nera\n";
    }
}
