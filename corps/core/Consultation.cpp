#include "Consultation.h"
#include <iostream>
#include <iomanip>
#include <sstream>

int Consultation::compteurParPatient = 0;
std::map<std::string, int> Consultation::compteursPatients;

std::string Consultation::genererId(const std::string& idPatient)
{
    if (compteursPatients.find(idPatient) == compteursPatients.end()) {
        compteursPatients[idPatient] = 0;
    }
    compteursPatients[idPatient]++;
    int numero = compteursPatients[idPatient];

    std::stringstream ss;
    ss << "CONS-" << idPatient << "-" << std::setfill('0') << std::setw(5) << numero;
    return ss.str();
}

Consultation::Consultation()
    : consultationId(""), date(Outils::getCurrentDate()), idPatient(""),
      idProfessionnel(""), nomProfessionnel(""), motif(""), observations("")
{
}

Consultation::Consultation(const Consultation& other)
    : consultationId(other.consultationId), date(other.date),
      idPatient(other.idPatient), idProfessionnel(other.idProfessionnel),
      nomProfessionnel(other.nomProfessionnel), motif(other.motif),
      observations(other.observations)
{
}

Consultation::Consultation(std::string pId, std::string profId, std::string profNom,
                           std::string m, std::string obs)
    : consultationId(genererId(pId)), date(Outils::getCurrentDate()),
      idPatient(pId), idProfessionnel(profId), nomProfessionnel(profNom),
      motif(m), observations(obs)
{
}

Consultation::~Consultation()
{
}

std::string Consultation::get_consultationId() const { return consultationId; }
std::string Consultation::get_date() const { return date; }
std::string Consultation::get_idPatient() const { return idPatient; }
std::string Consultation::get_idProfessionnel() const { return idProfessionnel; }
std::string Consultation::get_nomProfessionnel() const { return nomProfessionnel; }
std::string Consultation::get_motif() const { return motif; }
std::string Consultation::get_observations() const { return observations; }

void Consultation::set_consultationId(std::string val) { consultationId = val; }
void Consultation::set_date(std::string val) { date = val; }
void Consultation::set_idPatient(std::string val) { idPatient = val; }
void Consultation::set_idProfessionnel(std::string val) { idProfessionnel = val; }
void Consultation::set_nomProfessionnel(std::string val) { nomProfessionnel = val; }
void Consultation::set_motif(std::string val) { motif = val; }
void Consultation::set_observations(std::string val) { observations = val; }

void Consultation::afficher_details() const
{
    std::cout << "=== Consultation " << consultationId << " ===\n";
    std::cout << "Date           : " << date << "\n";
    std::cout << "Patient ID     : " << idPatient << "\n";
    std::cout << "Professionnel  : " << nomProfessionnel << " (ID: " << idProfessionnel << ")\n";
    std::cout << "Motif          : " << motif << "\n";
    std::cout << "Observations   : " << observations << "\n";
    std::cout << "============================\n";
}
