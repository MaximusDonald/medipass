#include "ProfessionnelSante.h"


ProfessionnelSante::ProfessionnelSante() : Utilisateur(), specialite(""), service("") {}

ProfessionnelSante::ProfessionnelSante(const ProfessionnelSante& other)
    : Utilisateur(other), specialite(other.specialite), service(other.service) {}

ProfessionnelSante::ProfessionnelSante(std::string n, std::string p, std::string l, std::string mdp, std::string spec, std::string serv)
    : Utilisateur(n, p, l, mdp, PROFESSIONNEL), specialite(spec), service(serv) {}

ProfessionnelSante::~ProfessionnelSante() {}


std::string ProfessionnelSante::getTypeString() {
    return "Professionnel de Sante";
}

std::string ProfessionnelSante::get_specialite() const {
    return specialite;
}

std::string ProfessionnelSante::get_service() const {
    return service;
}


void ProfessionnelSante::set_specialite(std::string val) {
    specialite = val;
}

void ProfessionnelSante::set_service(std::string val) {
    service = val;
}
