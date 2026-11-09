#ifndef STUDENTAS_H
#define STUDENTAS_H

#include <string>
#include <vector>
#include <list>
#include <iterator>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <stdexcept>
#include <utility>
#include "Pagalbiniai.h"

template <class Pazymiai>
class studentas {
public:
    std::string vardas_, pavarde_;
    Pazymiai paz_;
    int egz_;

    studentas() : egz_(0) {}

    studentas(const std::string &v, const std::string &p, const Pazymiai &pz, int e)
        : vardas_(v), pavarde_(p), paz_(pz), egz_(e) {}

    studentas(const studentas &kitas)
        : vardas_(kitas.vardas_), pavarde_(kitas.pavarde_), paz_(kitas.paz_), egz_(kitas.egz_) {}

    studentas(studentas &&kitas) noexcept
        : vardas_(std::move(kitas.vardas_)), pavarde_(std::move(kitas.pavarde_)), paz_(std::move(kitas.paz_)), egz_(kitas.egz_) {
        kitas.egz_ = 0;
    }

    studentas &operator=(const studentas &kitas){
        if (this != &kitas){
            vardas_ = kitas.vardas_;
            pavarde_ = kitas.pavarde_;
            paz_ = kitas.paz_;
            egz_ = kitas.egz_;
        }
        return *this;
    }

    studentas &operator=(studentas &&kitas) noexcept{
        if (this != &kitas){
            vardas_ = std::move(kitas.vardas_);
            pavarde_ = std::move(kitas.pavarde_);
            paz_ = std::move(kitas.paz_);
            egz_ = kitas.egz_;
            kitas.egz_ = 0;
        }
        return *this;
    }

    ~studentas(){
        vardas_.clear();
        pavarde_.clear();
        paz_.clear();
        egz_ = 0;
    }

    const std::string &vardas() const { return vardas_; }
    const std::string &pavarde() const { return pavarde_; }
    const Pazymiai &pazymiai() const { return paz_; }
    int egz() const { return egz_; }
};

template <class Pazymiai>
double vidurkis(const studentas<Pazymiai> &A){
    if (A.pazymiai().empty()) return 0;
    double suma = 0;
    for (int p : A.pazymiai()){
        suma += p;
    }
    return suma / A.pazymiai().size();
}

template <class Pazymiai>
double mediana(const studentas<Pazymiai> &A){
    if (A.pazymiai().empty()) return 0;
    std::vector<int> laikinas(A.pazymiai().begin(), A.pazymiai().end());
    std::sort(laikinas.begin(), laikinas.end());
    int n = laikinas.size();
    if (n % 2 == 1) return laikinas[n / 2];
    return (laikinas[n / 2 - 1] + laikinas[n / 2]) / 2.0;
}

template <class Pazymiai>
double galutinis(const studentas<Pazymiai> &A, bool ar_mediana){
    if (ar_mediana) return 0.4 * mediana(A) + 0.6 * A.egz();
    return 0.4 * vidurkis(A) + 0.6 * A.egz();
}

template <class Pazymiai>
void isvesti_studenta(std::ostream &out, const studentas<Pazymiai> &A, int balo_tipas, int pavardes_ilgis, int vardo_ilgis){
    out << std::left << std::setw(pavardes_ilgis) << A.pavarde()
        << std::setw(vardo_ilgis) << A.vardas()
        << std::fixed << std::setprecision(2);

    if (balo_tipas == 1) out << galutinis(A, false) << "\n";
    else if (balo_tipas == 2) out << galutinis(A, true) << "\n";
    else out << std::setw(18) << galutinis(A, false) << galutinis(A, true) << "\n";
}

template <class Pazymiai>
bool pagal_pavarde(const studentas<Pazymiai> &a, const studentas<Pazymiai> &b){
    if (a.pavarde() == b.pavarde()) return a.vardas() < b.vardas();
    return a.pavarde() < b.pavarde();
}

template <class Pazymiai>
bool pagal_varda(const studentas<Pazymiai> &a, const studentas<Pazymiai> &b){
    if (a.vardas() == b.vardas()) return a.pavarde() < b.pavarde();
    return a.vardas() < b.vardas();
}

template <class Pazymiai>
bool pagal_vidurki_mazejant(const studentas<Pazymiai> &a, const studentas<Pazymiai> &b){
    if (galutinis(a, false) == galutinis(b, false)) return pagal_pavarde(a, b);
    return galutinis(a, false) > galutinis(b, false);
}

template <class Pazymiai>
bool pagal_vidurki_didejant(const studentas<Pazymiai> &a, const studentas<Pazymiai> &b){
    if (galutinis(a, false) == galutinis(b, false)) return pagal_pavarde(a, b);
    return galutinis(a, false) < galutinis(b, false);
}

template <class Pazymiai>
bool pagal_mediana_mazejant(const studentas<Pazymiai> &a, const studentas<Pazymiai> &b){
    if (galutinis(a, true) == galutinis(b, true)) return pagal_pavarde(a, b);
    return galutinis(a, true) > galutinis(b, true);
}

template <class Pazymiai>
bool pagal_mediana_didejant(const studentas<Pazymiai> &a, const studentas<Pazymiai> &b){
    if (galutinis(a, true) == galutinis(b, true)) return pagal_pavarde(a, b);
    return galutinis(a, true) < galutinis(b, true);
}

template <class Pazymiai>
bool kietiakas_vid(const studentas<Pazymiai> &A){
    return galutinis(A, false) >= 5.0;
}

template <class Pazymiai>
bool kietiakas_med(const studentas<Pazymiai> &A){
    return galutinis(A, true) >= 5.0;
}

template <class Pazymiai>
studentas<Pazymiai> ivesti_studenta(){
    studentas<Pazymiai> A;
    std::cout<<"Iveskite per tarpa studento varda ir pavarde: ";
    std::cin>>A.vardas_>>A.pavarde_;
    ignoruoti_eilute();

    std::cout << "1 - pazymius vesti ranka, 2 - generuoti atsitiktinai: ";
    int rezimas = skaicius_input();
    while (rezimas != 1 && rezimas != 2) {
        std::cout << "Klaida. Iveskite 1 arba 2: ";
        rezimas = skaicius_input();
    }
    ignoruoti_eilute();

    std::cout<<"Iveskite semestro paz. kieki: ";

    if (rezimas == 2){
        int k = skaicius_input();
        while (k < 0){
            std::cout << "Pazymiu kiekis negali buti neigiamas!\n";
            std::cout<<"Iveskite semestro paz. kieki: ";
            k = skaicius_input();
        }
        for (int i = 0; i < k; i++){
            A.paz_.push_back(random_pazymys());
        }
        A.egz_ = random_pazymys();
        ignoruoti_eilute();
    } else{
        std::string ivedimas;
        getline(std::cin, ivedimas);

        if (ivedimas.empty()){
            int i = 0;
            while(true){
                std::cout << "Iveskite " << i + 1 << " paz: ";
                getline(std::cin, ivedimas);
                if (ivedimas.empty()) break;
                try{
                    int paz = std::stoi(ivedimas);
                    if (paz < 0 || paz > 10){
                        std::cout << "Pazymys turi buti nuo 0 iki 10\n";
                        continue;
                    }
                    A.paz_.push_back(paz);
                    i++;
                } catch(const std::exception&){
                    std::cout << "Klaida. Iveskite skaiciu\n";
                }
            }
        } else{
            int k;
            try{
                k = std::stoi(ivedimas);
            } catch (const std::exception&){
                k = -1;
            }
            while (k < 0){
                std::cout << "Bloga ivestis. Iveskite semestro paz. kieki: ";
                k = skaicius_input();
                ignoruoti_eilute();
            }
            for (int i = 0; i < k; i++){
                std::cout << "Iveskite " << i + 1 << " paz: ";
                A.paz_.push_back(pazymio_apribojimas());
            }
            if (k > 0) ignoruoti_eilute();
        }

        std::cout<<"Iveskite semestro Egzamino paz.: ";
        A.egz_ = pazymio_apribojimas();
        ignoruoti_eilute();
    }
    return A;
}

template <class Konteineris>
void strategija1(const Konteineris &grupe, Konteineris &vargsiukai, Konteineris &kietiakai, bool ar_mediana){
    vargsiukai.clear();
    kietiakai.clear();
    for (const auto &S : grupe){
        if (galutinis(S, ar_mediana) < 5.0) vargsiukai.push_back(S);
        else kietiakai.push_back(S);
    }
}

template <class Konteineris>
void strategija2(Konteineris &grupe, Konteineris &vargsiukai, bool ar_mediana){
    vargsiukai.clear();
    auto it = grupe.begin();
    while (it != grupe.end()){
        if (galutinis(*it, ar_mediana) < 5.0){
            vargsiukai.push_back(*it);
            it = grupe.erase(it);
        } else{
            ++it;
        }
    }
}

template <class Pazymiai>
void strategija2(std::vector<studentas<Pazymiai>> &grupe, std::vector<studentas<Pazymiai>> &vargsiukai, bool ar_mediana){
    vargsiukai.clear();
    std::size_t i = 0;
    while (i < grupe.size()){
        if (galutinis(grupe[i], ar_mediana) < 5.0){
            vargsiukai.push_back(grupe[i]);
            if (i != grupe.size() - 1){
                std::swap(grupe[i], grupe.back());
            }
            grupe.pop_back();
        } else{
            ++i;
        }
    }
}

template <class Pazymiai>
void strategija3(std::vector<studentas<Pazymiai>> &grupe, std::vector<studentas<Pazymiai>> &vargsiukai, bool ar_mediana){
    vargsiukai.clear();
    auto it = grupe.end();
    if (ar_mediana) it = std::partition(grupe.begin(), grupe.end(), kietiakas_med<Pazymiai>);
    else it = std::partition(grupe.begin(), grupe.end(), kietiakas_vid<Pazymiai>);
    std::copy(it, grupe.end(), std::back_inserter(vargsiukai));
    grupe.erase(it, grupe.end());
}

template <class Pazymiai>
void strategija3(std::list<studentas<Pazymiai>> &grupe, std::list<studentas<Pazymiai>> &vargsiukai, bool ar_mediana){
    vargsiukai.clear();
    auto it = grupe.begin();
    while (it != grupe.end()){
        auto kitas = it;
        ++kitas;
        if (galutinis(*it, ar_mediana) < 5.0) vargsiukai.splice(vargsiukai.end(), grupe, it);
        it = kitas;
    }
}

template <class Konteineris>
void grazinti_vargsiukus(Konteineris &grupe, Konteineris &vargsiukai){
    grupe.insert(grupe.end(), vargsiukai.begin(), vargsiukai.end());
    vargsiukai.clear();
}

#endif
