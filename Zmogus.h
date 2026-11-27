#ifndef ZMOGUS_H
#define ZMOGUS_H

#include <string>
#include <iostream>
#include <utility>

class Zmogus {
protected:
    std::string vardas_, pavarde_;

public:
    Zmogus() : vardas_(""), pavarde_("") {}

    Zmogus(const std::string &v, const std::string &p)
        : vardas_(v), pavarde_(p) {}

    Zmogus(const Zmogus &kitas)
        : vardas_(kitas.vardas_), pavarde_(kitas.pavarde_) {}

    Zmogus(Zmogus &&kitas) noexcept
        : vardas_(std::move(kitas.vardas_)), pavarde_(std::move(kitas.pavarde_)) {}

    Zmogus &operator=(const Zmogus &kitas){
        if (this != &kitas){
            vardas_ = kitas.vardas_;
            pavarde_ = kitas.pavarde_;
        }
        return *this;
    }

    Zmogus &operator=(Zmogus &&kitas) noexcept{
        if (this != &kitas){
            vardas_ = std::move(kitas.vardas_);
            pavarde_ = std::move(kitas.pavarde_);
        }
        return *this;
    }

    virtual ~Zmogus(){}

    const std::string &vardas() const { return vardas_; }
    const std::string &pavarde() const { return pavarde_; }

    virtual void spausdinti(std::ostream &out) const = 0;
};

#endif
