#include "Statistiques.h"
#include "Systeme.h"
#include <iostream>


using namespace std;
void Statistiques::afficherMenu() {
    while (true) {
        Outils::printHeader("STATISTIQUES", Outils::YELLOW);
        cout << "1. Nombre de patients\n";
        cout << "2. Professionnels par catégorie\n";
        cout << "3. Consultations par période\n";
        cout << "4. Rapport complet\n";
        cout << "0. Retour\n";

        int choix = 0;
        cout << "Choix : ";
        cin >> choix;
        cin.ignore();

        switch (choix) {
            case 1: nombrePatients(); break;
            case 2: professionnelsParCategorie(); break;
            case 3: consultationsParPeriode(); break;
            case 4: rapportComplet(); break;
            case 0: return;
        }
    }
}

void Statistiques::nombrePatients() {
    int total = Systeme::getInstance().getPatients().size();
    cout << "Nombre de patients : " << total << endl;
    Outils::pause();
}

void Statistiques::professionnelsParCategorie() {
    // Utiliser une map pour compter par spécialite
    cout << "Médecins généraux : XXX\n";
    cout << "Spécialistes : XXX\n";
    Outils::pause();
}

void Statistiques::consultationsParPeriode() {
    string debut = Outils::readNonEmpty("Début (AAAA-MM-JJ) : ");
    string fin = Outils::readNonEmpty("Fin (AAAA-MM-JJ) : ");
    // Filtrer les consultations
    cout << "Consultations dans cette période : XXX\n";
    Outils::pause();
}

void Statistiques::rapportComplet() {
    Outils::printHeader("RAPPORT COMPLET", Outils::GREEN);
    nombrePatients();
    professionnelsParCategorie();
    consultationsParPeriode();
    Outils::pause();
}
