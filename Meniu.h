#ifndef MENIU_H
#define MENIU_H

#include <iostream>
#include <string>
#include <fstream>
#include "Studentas.h"
#include "Pagalbiniai.h"
#include "Failai.h"
using std::string;
using std::cout;
using std::cin;

template <class Konteineris, class Pazymiai>
void programa(){
    Konteineris grupe;
    Konteineris kietiakai;
    Konteineris vargsiukai;
    bool ar_suskirstyta = false;
    bool skirstymo_mediana = false;
    int skirstymo_strategija = 1;
    bool vargsiukai_atskirti = false;

    int meniu_veiksmas = -1;
    while (meniu_veiksmas != 0) {
        cout << "\n========= Studentu DB =========\n"
        << "1. Pazymiu ivedimas\n"
        << "2. Rodyti rezultatus\n"
        << "3. Skirstymas pagal bala\n"
        << "4. Nuskaityti duomenis is failo\n"
        << "5. Failu generavimas\n"
        << "6. Greicio analize\n"
        << "0. Iseiti\n"
        << "Pasirinkimas: ";
        meniu_veiksmas = skaicius_input();
        ignoruoti_eilute();

        if (meniu_veiksmas == 1){
            cout<<"Kiek studentu yra sarase: ";
            int n = skaicius_input();
            while (n < 0) {
                cout << "Studentu sk. negali buti neigiamas: ";
                n = skaicius_input();
            }

            ignoruoti_eilute();
            for (int j=0;j<n;j++){
                grupe.push_back(ivesti_studenta<Pazymiai>());
                cout << "objekto adresas: " << &grupe.back() << "\n";
                ar_suskirstyta = false;
            }
        }
        else if (meniu_veiksmas == 2) {
            if (vargsiukai_atskirti) {
                grazinti_vargsiukus(grupe, vargsiukai);
                vargsiukai_atskirti = false;
                ar_suskirstyta = false;
            }
            if (grupe.empty()) {
                cout << "\nNera studentu.\n";
                continue;
            }

            cout << "\nKur isvesti rezultatus?\n"
            << "1 - i ekrana\n"
            << "2 - i faila\n";
            int kur = pasirinkimas_tarp(1, 2);
            ignoruoti_eilute();

            string failo_pav;
            if (kur == 2) {
                cout << "Iveskite failo pav.: ";
                cin >> failo_pav;
                ignoruoti_eilute();
            }

            cout << "\nKaip skaiciuoti galutini bala?\n"
            <<"1 - pagal vidurki\n"
            <<"2 - pagal mediana\n"
            <<"3 - abu\n";
            int balo_tipas = pasirinkimas_tarp(1, 3);
            int rikiavimas = rikiavimo_pasirinkimas();

            std::ofstream fr;
            std::ostream *out = &cout;
            if (kur == 2) {
                fr.open(failo_pav);
                if (!fr) {
                    cout << "\nNepavyko sukurti failo.\n";
                    continue;
                }
                out = &fr;
            }

            if (kur == 1) cout << "\n";
            rezultatu_isvedimas(*out, grupe, balo_tipas, rikiavimas);

            if (kur == 2) {
                fr.close();
                cout << "\nRezultatai irasyti i " << failo_pav << "\n";
            }
        }
        else if (meniu_veiksmas == 3){
            cout << "\n1 - skirstyti i kategorijas\n"
            << "2 - irasyti i failus (tik jei jau suskirstyta)\n";
            int veiksmas = pasirinkimas_tarp(1, 2);

            if (veiksmas == 1){
                if (vargsiukai_atskirti) {
                    grazinti_vargsiukus(grupe, vargsiukai);
                    vargsiukai_atskirti = false;
                }
                if (grupe.empty()) {
                    cout << "\nNera studentu.\n";
                    continue;
                }

                cout << "\nPagal ka skaiciuoti galutini bala?\n"
                << "1 - vidurkis\n"
                << "2 - mediana\n";
                int pagal = pasirinkimas_tarp(1, 2);

                cout << "\nKuri skirstymo strategija?\n"
                << "1 - kopijuoti i du naujus konteinerius\n"
                << "2 - perkelti vargsiukus ir istrinti is pradinio\n"
                << "3 - perkelti vargsiukus is karto (vektoriui - partition, o list - splice)\n";
                skirstymo_strategija = pasirinkimas_tarp(1, 3);

                skirstymo_mediana = (pagal == 2);
                if (skirstymo_strategija == 1) {
                    strategija1(grupe, vargsiukai, kietiakai, skirstymo_mediana);
                } else {
                    kietiakai.clear();
                    if (skirstymo_strategija == 2) strategija2(grupe, vargsiukai, skirstymo_mediana);
                    else strategija3(grupe, vargsiukai, skirstymo_mediana);
                    vargsiukai_atskirti = true;
                }
                ar_suskirstyta = true;
                cout << "\nVargsiukai (galutinis < 5): " << vargsiukai.size() << "\n";
                if (skirstymo_strategija == 1) cout << "Kietiakai (galutinis >= 5): " << kietiakai.size() << "\n";
                else cout << "Kietiakai (galutinis >= 5): " << grupe.size() << "\n";
            } else {
                if (!ar_suskirstyta){
                    cout << "\nPirmiausia suskirstykite.\n";
                    continue;
                }

                int rikiavimas = rikiavimo_pasirinkimas();
                int balo_tipas = skirstymo_mediana ? 2 : 1;
                if (skirstymo_strategija == 1) irasyti_kategorijas(vargsiukai, kietiakai, balo_tipas, rikiavimas);
                else irasyti_kategorijas(vargsiukai, grupe, balo_tipas, rikiavimas);
            }
        }
        else if (meniu_veiksmas == 4){
            string failo_pav;
            cout << "Iveskite failo pav.: ";
            cin >> failo_pav;
            if (!nuskaityti_faila<Pazymiai>(failo_pav, grupe)){
                cout << "\nFailas nerastas.\n";
                continue;
            }
            ar_suskirstyta = false;
            vargsiukai_atskirti = false;
            vargsiukai.clear();
            cout << "\nDuomenys nuskaityti is " << failo_pav << ". Studentu sk.: " <<grupe.size() << "\n";
        }
        else if (meniu_veiksmas == 5){
            failu_generavimo_meniu();
        } else if (meniu_veiksmas == 6){
            greicio_analize_meniu<Konteineris, Pazymiai>();
        }
        else if (meniu_veiksmas != 0){
            cout << "\nTokio pasirinkimo nera!\n";
        }
    }
}

#endif
