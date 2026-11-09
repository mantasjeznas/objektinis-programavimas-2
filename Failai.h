#ifndef FAILAI_H
#define FAILAI_H

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include "Studentas.h"
#include "Pagalbiniai.h"

const int failu_kiekiai[5] = {1000, 10000, 100000, 1000000, 10000000};
const char *const failu_vardai[5] = {
    "stud1000.txt",
    "stud10000.txt",
    "stud100000.txt",
    "stud1000000.txt",
    "stud10000000.txt"
};

void failo_generavimas(int stud_sk, const std::string &failo_pav);
template <class Pazymiai>
void rikiuoti_grupe(std::vector<studentas<Pazymiai>> &grupe, int balo_tipas, int rikiavimas){
    if (rikiavimas == 1) std::sort(grupe.begin(), grupe.end(), pagal_varda<Pazymiai>);
    else if (rikiavimas == 3 && balo_tipas == 2) std::sort(grupe.begin(), grupe.end(), pagal_mediana_mazejant<Pazymiai>);
    else if (rikiavimas == 3) std::sort(grupe.begin(), grupe.end(), pagal_vidurki_mazejant<Pazymiai>);
    else if (rikiavimas == 4 && balo_tipas == 2) std::sort(grupe.begin(), grupe.end(), pagal_mediana_didejant<Pazymiai>);
    else if (rikiavimas == 4) std::sort(grupe.begin(), grupe.end(), pagal_vidurki_didejant<Pazymiai>);
    else std::sort(grupe.begin(), grupe.end(), pagal_pavarde<Pazymiai>);
}

template <class Pazymiai>
void rikiuoti_grupe(std::list<studentas<Pazymiai>> &grupe, int balo_tipas, int rikiavimas){
    if (rikiavimas == 1) grupe.sort(pagal_varda<Pazymiai>);
    else if (rikiavimas == 3 && balo_tipas == 2) grupe.sort(pagal_mediana_mazejant<Pazymiai>);
    else if (rikiavimas == 3) grupe.sort(pagal_vidurki_mazejant<Pazymiai>);
    else if (rikiavimas == 4 && balo_tipas == 2) grupe.sort(pagal_mediana_didejant<Pazymiai>);
    else if (rikiavimas == 4) grupe.sort(pagal_vidurki_didejant<Pazymiai>);
    else grupe.sort(pagal_pavarde<Pazymiai>);
}

template <class Konteineris>
void rezultatu_isvedimas(std::ostream &out, Konteineris &grupe, int balo_tipas, int rikiavimas, bool ar_rikiuoti = true){
    if (ar_rikiuoti) rikiuoti_grupe(grupe, balo_tipas, rikiavimas);

    int pavardes_ilgis = 12;
    int vardo_ilgis = 12;
    for (const auto &S : grupe){
        int pav_ilgis = S.pavarde().size() + 2;
        int var_ilgis = S.vardas().size() + 2;
        if (pav_ilgis > pavardes_ilgis) pavardes_ilgis = pav_ilgis;
        if (var_ilgis > vardo_ilgis) vardo_ilgis = var_ilgis;
    }

    std::stringstream buf;
    buf << std::left << std::setw(pavardes_ilgis) << "Pavarde" << std::setw(vardo_ilgis) << "Vardas";
    if (balo_tipas == 1) buf << "Galutinis (Vid.)\n";
    else if (balo_tipas == 2) buf << "Galutinis (Med.)\n";
    else buf << std::setw(18) << "Galutinis (Vid.)" << "Galutinis (Med.)\n";

    int linijos_ilgis = pavardes_ilgis + vardo_ilgis + 16;
    if (balo_tipas == 3) linijos_ilgis = pavardes_ilgis + vardo_ilgis + 34;
    buf << std::string(linijos_ilgis, '-') << "\n";

    for (const auto &S : grupe) isvesti_studenta(buf, S, balo_tipas, pavardes_ilgis, vardo_ilgis);
    out << buf.str();
}

template <class Pazymiai, class Konteineris>
bool nuskaityti_faila(const std::string &failo_pav, Konteineris &grupe){
    std::ifstream fd(failo_pav);
    if (!fd) return false;

    grupe.clear();
    std::string eilute;
    std::stringstream buf;
    buf << fd.rdbuf();
    fd.close();
    getline(buf, eilute);

    studentas<Pazymiai> A;
    std::stringstream ss;
    while (getline(buf, eilute)){
        ss.clear();
        ss.str(eilute);
        ss >> A.pavarde_ >> A.vardas_;

        A.paz_.clear();
        int x;
        while (ss >> x) A.paz_.push_back(x);

        if (A.paz_.empty()) continue;
        A.egz_ = A.paz_.back();
        A.paz_.pop_back();
        grupe.push_back(A);
    }
    return true;
}

int rikiavimo_pasirinkimas();

template <class Konteineris>
void irasyti_kategorijas(Konteineris &vargsiukai, Konteineris &kietiakai, int balo_tipas, int rikiavimas){
    std::ofstream varg("vargsiukai.txt");
    std::ofstream kiet("kietiakai.txt");
    if (!varg || !kiet) {
        std::cout << "\nNepavyko sukurti/redaguoti failo.\n";
        return;
    }
    rezultatu_isvedimas(varg, vargsiukai, balo_tipas, rikiavimas);
    rezultatu_isvedimas(kiet, kietiakai, balo_tipas, rikiavimas);
    std::cout << "\nIrasyta i vargsiukai.txt (" << vargsiukai.size() << " studentu)\n";
    std::cout << "Irasyta i kietiakai.txt (" << kietiakai.size() << " studentu)\n";
}

void failu_generavimo_meniu();

template <class Konteineris, class Pazymiai>
double greicio_analize(const std::string &failo_pav, int strategija, double laikai[]){
    const int kartai = 3;
    Konteineris grupe;
    Konteineris kietiakai;
    Konteineris vargsiukai;

    std::ifstream patikra(failo_pav);
    if (!patikra){
        std::cout << "\nnerasta " << failo_pav << ". sugeneruokite meniu 5 punktu.\n";
        return 0;
    }
    patikra.close();

    double t_skaitymas = 0;
    double t_skirstymas = 0;
    double t_rasymas = 0;
    double t_rikiavimas = 0;

    int studentu_sk = 0;

    for (int i = 0; i < kartai; i++){
        auto start = std::chrono::steady_clock::now();
        if (!nuskaityti_faila<Pazymiai>(failo_pav, grupe)){
            std::cout << "\nNepavyko skaityti " << failo_pav << "\n";
            return 0;
        }
        auto end = std::chrono::steady_clock::now();
        t_skaitymas += std::chrono::duration<double>(end - start).count();
        studentu_sk = grupe.size();

        start = std::chrono::steady_clock::now();
        if (strategija == 1) strategija1(grupe, vargsiukai, kietiakai, false);
        else if (strategija == 2) strategija2(grupe, vargsiukai, false);
        else strategija3(grupe, vargsiukai, false);
        end = std::chrono::steady_clock::now();
        t_skirstymas += std::chrono::duration<double>(end - start).count();

        start = std::chrono::steady_clock::now();
        rikiuoti_grupe(vargsiukai, 1, 3);
        if (strategija == 1) rikiuoti_grupe(kietiakai, 1, 3);
        else rikiuoti_grupe(grupe, 1, 3);
        end = std::chrono::steady_clock::now();
        t_rikiavimas += std::chrono::duration<double>(end - start).count();

        start = std::chrono::steady_clock::now();
        std::ofstream varg("vargsiukai.txt");
        std::ofstream kiet("kietiakai.txt");
        if (!varg || !kiet){
            std::cout << "\nNepavyko irasyti rezultatu failu.\n";
            return 0;
        }
        rezultatu_isvedimas(varg, vargsiukai, 1, 3, false);
        if (strategija == 1) rezultatu_isvedimas(kiet, kietiakai, 1, 3, false);
        else rezultatu_isvedimas(kiet, grupe, 1, 3, false);
        varg.close();
        kiet.close();
        end = std::chrono::steady_clock::now();
        t_rasymas += std::chrono::duration<double>(end - start).count();
    }

    std::cout << "\n--- " << failo_pav << ", " << strategija << " strategija (" << kartai << " kartu vidurkiai) ---\n";
    std::cout << std::defaultfloat << std::setprecision(6);
    std::cout << std::left
     << std::setw(15) << "Studentu sk." << "| " << studentu_sk << "\n"
     << std::setw(15) << "Skaitymas" << "| " << t_skaitymas / kartai << " s\n"
     << std::setw(15) << "Skirstymas" << "| " << t_skirstymas / kartai << " s\n"
     << std::setw(15) << "Rasymas" << "| " << t_rasymas / kartai << " s\n"
     << std::setw(15) << "Rusiavimas" << "| " << t_rikiavimas / kartai << " s\n";

    double bendras = (t_skaitymas + t_skirstymas + t_rasymas + t_rikiavimas) / kartai;
    laikai[0] = t_skaitymas / kartai;
    laikai[1] = t_skirstymas / kartai;
    laikai[2] = t_rasymas / kartai;
    laikai[3] = t_rikiavimas / kartai;
    laikai[4] = bendras;
    std::cout << std::setw(15) << "Bendras laikas" << "| " << bendras << " s\n";
    return bendras;
}

template <class Konteineris, class Pazymiai>
void greicio_analize_meniu(){
    int esami[5];
    int n = 0;
    for (int i = 0; i < 5; i++) {
        std::ifstream fd(failu_vardai[i]);
        if (fd) esami[n++] = i;
    }
    if (n == 0) {
        std::cout << "\nNera sugeneruotu failu. Sugeneruokite meniu 5 punktu.\n";
        return;
    }

    std::cout << "\nKuri faila testuoti?\n";
    for (int i = 0; i < n; i++) {
        std::cout << i + 1 << " - " << failu_vardai[esami[i]] << "\n";
    }
    if (n > 1) std::cout << n + 1 << " - visus esamus\n";

    int max_pasirinkimas = (n > 1) ? n + 1 : n;
    int failo_nr = pasirinkimas_tarp(1, max_pasirinkimas);
    ignoruoti_eilute();

    std::cout << "\nKuri skirstymo strategija?\n"
    << "1 - kopijuoti i du naujus konteinerius\n"
    << "2 - perkelti vargsiukus ir istrinti is pradinio\n"
    << "3 - perkelti vargsiukus is karto (vektoriui - partition, o list - splice)\n"
    << "4 - visas tris palyginti\n";
    int strategija = pasirinkimas_tarp(1, 4);
    ignoruoti_eilute();

    int pirmas = failo_nr - 1;
    int paskutinis = failo_nr - 1;
    if (failo_nr == n + 1) {
        pirmas = 0;
        paskutinis = n - 1;
    }

    double laikai[3][5][5] = {};
    for (int s = 1; s <= 3; s++) {
        if (strategija != 4 && s != strategija) continue;
        double bendras = 0;
        for (int i = pirmas; i <= paskutinis; i++) {
            bendras += greicio_analize<Konteineris, Pazymiai>(failu_vardai[esami[i]], s, laikai[s - 1][i]);
        }
        if (pirmas != paskutinis) std::cout << "\nBendras visu failu laikas (" << s << " strategija): " << bendras << " s\n";
    }

    if (strategija == 4) {
        for (int s = 0; s < 3; s++) {
            std::cout << "\n=== STRATEGIJA-" << s + 1 << " ===\n";
            std::cout << std::left << std::setw(17) << "Failas" << std::right;
            std::cout << "|" << std::setw(12) << "Skaitymas" << " |" << std::setw(12) << "Skirstymas" << " |" << std::setw(12) << "Rasymas" << " |" << std::setw(12) << "Rusiavimas" << " |" << std::setw(15) << "Bendras laikas" << "\n";
            std::cout << "-----------------+-------------+-------------+-------------+-------------+---------------\n";
            for (int i = pirmas; i <= paskutinis; i++) {
                std::cout << std::left << std::setw(17) << failu_vardai[esami[i]] << std::right << std::fixed << std::setprecision(6);
                for (int k = 0; k < 4; k++) std::cout << "|" << std::setw(12) << laikai[s][i][k] << " ";
                std::cout << "|" << std::setw(15) << laikai[s][i][4] << "\n" << std::defaultfloat << std::left;
            }
        }
    }
}

#endif
