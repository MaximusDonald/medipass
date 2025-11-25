#include "Utilisateur.h"


int Utilisateur::compteur = 0;


std::string Utilisateur::genererID(Role r) {
    compteur++;
    std::stringstream ss;
    ss << (r == PROFESSIONNEL ? "PRO-" : "ADM-") << std::setfill('0') << std::setw(3) << compteur;
    return ss.str();
}


Utilisateur::Utilisateur() : id(""), nom(""), prenom(""), login(""), motDePasse(""), role(PROFESSIONNEL) {}

Utilisateur::Utilisateur(const Utilisateur& other)
    : id(other.id), nom(other.nom), prenom(other.prenom), login(other.login), motDePasse(other.motDePasse), role(other.role) {}


Utilisateur::Utilisateur(std::string n, std::string p, std::string l, std::string mdp, Role r)
    : id(genererID(r)), nom(n), prenom(p), login(l), motDePasse(mdp), role(r) {}

Utilisateur::~Utilisateur() {}


std::string Utilisateur::get_id() const {
    return id;
}

std::string Utilisateur::get_nom() const {
    return nom;
}

std::string Utilisateur::get_prenom() const {
    return prenom;
}

std::string Utilisateur::get_nom_complet() const {
    return nom + " " + prenom;
}

std::string Utilisateur::get_login() const {
    return login;
}

std::string Utilisateur::get_motDePasse() const {
    return motDePasse;
}

Role Utilisateur::get_role() const {
    return role;
}


void Utilisateur::set_id(std::string i) {
    id = i;
}

void Utilisateur::set_nom(std::string n) {
    nom = n;
}

void Utilisateur::set_prenom(std::string p) {
    prenom = p;
}

void Utilisateur::set_role(Role r) {
    role = r;
}

void Utilisateur::set_login(std::string l) {
    login = l;
}

void Utilisateur::set_motDePasse(std::string mdp) {
    motDePasse = mdp;
}


bool Utilisateur::verifierMotDePasse(std::string mdp) {
    return motDePasse == mdp;
}
