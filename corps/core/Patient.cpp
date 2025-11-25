#include "Patient.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <regex>

using namespace std;

int Patient::compteurId = 0;

string Patient::genererId()
{
    ++compteurId;
    stringstream ss;
    ss << "PAT-" << setfill('0') << setw(5) << compteurId;
    return ss.str();
}

bool Patient::validerDateNaissance(const string& date)
{
    if (date.length() != 10) return false;
    if (date[4] != '-' || date[7] != '-') return false;

    regex pattern(R"(^\d{4}-\d{2}-\d{2}$)");
    if (!regex_match(date, pattern)) return false;

    if (date > Outils::getCurrentDate()) return false;

    int annee, mois, jour;
    if (sscanf(date.c_str(), "%d-%d-%d", &annee, &mois, &jour) != 3) return false;

    if (annee < 1900) return false;
    if (mois < 1 || mois > 12) return false;
    if (jour < 1 || jour > 31) return false;

    static const int joursParMois[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    if (mois == 2 && jour == 29) {
        if ((annee % 4 == 0 && annee % 100 != 0) || (annee % 400 == 0)) return true;
        else return false;
    }
    return jour <= joursParMois[mois];
}

Patient::Patient()
    : id(genererId()), nom(""), prenom(""), dateNaissance("1900-01-01"),
      sexe(""), contact(""), idMedecinTraitant(""), dossier()
{
}

Patient::Patient(const Patient& other)
    : id(other.id), nom(other.nom), prenom(other.prenom),
      dateNaissance(other.dateNaissance), sexe(other.sexe),
      contact(other.contact), idMedecinTraitant(other.idMedecinTraitant),
      dossier(other.dossier)
{
}

Patient::Patient(string nom, string prenom,
                 string dateNaiss, string sexe,
                 string contact, string idMedecinTraitant)
    : id(genererId()), nom(nom), prenom(prenom),
      sexe(sexe), contact(contact), idMedecinTraitant(idMedecinTraitant), dossier()
{
    if (!validerDateNaissance(dateNaiss)) dateNaiss = "1900-01-01";
    dateNaissance = dateNaiss;
}

Patient::Patient(string id, string nom, string prenom,
                 string dateNaiss, string sexe,
                 string contact, string idMedecinTraitant)
    : id(id), nom(nom), prenom(prenom),
      sexe(sexe), contact(contact), idMedecinTraitant(idMedecinTraitant), dossier()
{
    if (!validerDateNaissance(dateNaiss)) dateNaiss = "1900-01-01";
    dateNaissance = dateNaiss;
}

Patient::~Patient() { }

string Patient::get_id() const { return id; }
string Patient::get_nom() const { return nom; }
string Patient::get_prenom() const { return prenom; }
string Patient::get_nomComplet() const { return nom + " " + prenom; }
string Patient::get_dateNaissance() const { return dateNaissance; }
string Patient::get_sexe() const { return sexe; }
string Patient::get_contact() const { return contact; }
string Patient::get_idMedecinTraitant() const { return idMedecinTraitant; }

const DossierMedical& Patient::get_dossier() const { return dossier; }
DossierMedical& Patient::get_dossierModifiable() { return dossier; }

void Patient::set_id(const string& i) { id = i; }
void Patient::set_nom(const string& n) { nom = n; }
void Patient::set_prenom(const string& p) { prenom = p; }
bool Patient::set_dateNaissance(const string& date)
{
    if (validerDateNaissance(date)) {
        dateNaissance = date;
        return true;
    }
    return false;
}
void Patient::set_sexe(const string& s) { sexe = s; }
void Patient::set_contact(const string& c) { contact = c; }
void Patient::set_idMedecinTraitant(const string& idProf) { idMedecinTraitant = idProf; }

void Patient::afficherInfos() const
{
    cout << "Patient [" << id << "] " << nom << " " << prenom << "\n";
    cout << "Né(e) le : " << dateNaissance << " | Sexe : " << sexe << "\n";
    cout << "Contact  : " << contact << "\n";
    if (!idMedecinTraitant.empty())
        std::cout << "Médecin traitant : " << idMedecinTraitant << "\n";
}

void Patient::afficherComplet() const
{
    afficherInfos();
    cout << "\n";
    dossier.afficherHistoriqueComplet();
}
