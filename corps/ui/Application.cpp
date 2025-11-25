#include "Application.h"
#include "Database.h"
#include "Outils.h"
#include <iostream>
#include <algorithm>

using namespace std;

void Application::run() {
    ecranConnexion();
}

void Application::creerPatient() {
    Outils::printHeader("Création d'un Nouveau Patient", Outils::GREEN);
    string n, p, dn, s, c;
    cout << "Nom: "; n = Outils::readLine();
    cout << "Prénom: "; p = Outils::readLine();
    cout << "Date Naissance (JJ/MM/AAAA): "; dn = Outils::readLine();
    cout << "Sexe (M/F): "; s = Outils::readLine();
    cout << "Contact: "; c = Outils::readLine();

    string newId = "PAT" + Outils::auto3(Database::patients.size() + 1);
    Patient newP(newId, n, p, dn, s, c);

    if(Database::utilisateurConnecte->get_role() == PROFESSIONNEL) {
        newP.set_MedecinTraitant(Database::utilisateurConnecte->get_id());
    }

    Database::patients.push_back(newP);
    Database::ajouterLog("Création patient " + newId);

    Outils::setColor(Outils::GREEN);
    cout << "\n>>> Patient" << newP.get_nom_complet() << "créé avec succès ! ID généré : " << newId << endl;
    Outils::setColor(Outils::DEFAULT);
    Outils::pause();
}


void Application::menuConsultations() {
    int choix;
    do {
        Outils::printHeader("Gestion des Consultations");
        cout << "1. Prendre en charge un patient (Nouvelle Consultation)" << endl;
        cout << "2. Mes consultations du jour" << endl;
        cout << "3. Historique de mes consultations" << endl;
        cout << "4. Modifier une consultation" << endl;
        cout << "5. Retour au menu principal" << endl;
        cout << "\nVotre choix: ";
        cin >> choix; cin.ignore();

        switch(choix) {
            case 1: {
                string rech;
                cout << "Rechercher le patients à pendre en charge (ID/Nom): "; getline(cin, rech);
                Patient* pat = Database::trouverPatient(rech);

                if(pat) {
                    Outils::printHeader("Nouvelle Consultation: " + pat->get_nom_complet(), Outils::GREEN);
                    string motif, obs;
                    cout << "Motif de consultation: "; getline(cin, motif);
                    cout << "Observations cliniques: "; getline(cin, obs);

                    string cid = "CS-" + pat->get_id() + "-" + Database::utilisateurConnecte->get_id() + Outils::auto3(Database::consultations.size());
                    Consultation cs(cid, pat->get_id(), Database::utilisateurConnecte->get_id(), Database::utilisateurConnecte->get_nom_complet(), motif, obs);
                    pat->get_dossier().ajouterConsultation(cs);

                    if(pat->get_MedecinTraitant() == "") pat->set_MedecinTraitant(Database::utilisateurConnecte->get_id());

                    cout << "\n>>> Consultation enregistrée et dossier mis à jour." << endl;
                    Database::ajouterLog("Consultation créée pour " + pat->get_id());

                    cout << "Voulez-vous prescrire une ordonnance ? (o/n): ";
                    char rep; cin >> rep;
                    if(rep == 'o' || rep == 'O') cout << ">>> Module Prescription (Simulé) ouvert..." << endl;

                } else {
                    cout << "❌ Patient introuvable." << endl;
                }
                Outils::pause();
                break;
            }
            case 2:
                for (const auto& x : Database::consultations)
                {
                    if (x.get_date() == Outils::getCurrentDate())
                    {
                        cout << "Consultation du patient" << x.get_idPatient() << endl;
                    }
                }
            case 3:
                cout << "Affichage de l'historique global du professionnel." << endl;
                for (const auto& x : Database::consultations)
                {
                    if (x.get_idProfessionnel() == Database::utilisateurConnecte->get_id())
                    {
                        cout << "Consultation du patient" << x.get_idPatient() << "le" << x.get_date() <<endl;
                    }
                }
                Outils::pause();
                break;
        }
    } while (choix != 5);
}

void Application::menuPatients() {
    int choix;
    do {
        Outils::printHeader("Gestion des Patients");
        cout << "1. Créer un nouveau patient" << endl;
        cout << "2. Rechercher un patient" << endl;
        cout << "3. Mes patients assignés" << endl;
        cout << "4. Retour au menu principal" << endl;
        cout << "\nVotre choix: ";
        cin >> choix; cin.ignore();

        switch(choix) {
            case 1:
                creerPatient();
                break;
            case 2: {
                string input;
                cout << "Entrez le Nom ou l'ID du patient: "; getline(cin, input);
                Patient* p = Database::trouverPatient(input);

                if(p) {
                    Outils::printHeader("Dossier: " + p->get_nom_complet(), Outils::YELLOW);
                    p->afficherInfosBase();

                    bool accesAutorise = false;
                    if(p->get_MedecinTraitant() == Database::utilisateurConnecte->get_id()) {
                        accesAutorise = true;
                    } else {
                        cout << "\n⚠️ Vous n'êtes pas le médecin traitant." << endl;
                        cout << "1. Prendre en charge (Créer consultation)" << endl;
                        cout << "2. Accès URGENCE (Justification requise)" << endl;
                        cout << "3. Annuler" << endl;
                        int subChoix; cin >> subChoix; cin.ignore();

                        if(subChoix == 1) {
                            menuConsultations();
                            break;
                        } else if (subChoix == 2) {
                            string justif;
                            cout << "Justification obligatoire: "; getline(cin, justif);
                            Database::ajouterLog("ACCES URGENCE Patient " + p->get_id() + ": " + justif);
                            accesAutorise = true;
                        }
                    }

                    if(accesAutorise) {
                        int dossierChoix;
                        do {
                            Outils::printHeader("Dossier Médical: " + p->get_nom_complet(), Outils::WHITE);
                            cout << "1. Voir historique complet" << endl;
                            cout << "2. Ajouter antécédent" << endl;
                            cout << "3. Retour recherche" << endl;
                            cout << "Choix: "; cin >> dossierChoix; cin.ignore();

                            if(dossierChoix == 1) {
                                p->get_dossier().afficherSynthese();
                                Database::ajouterLog("Lecture dossier " + p->get_id());
                                Outils::pause();
                            } else if (dossierChoix == 2) {
                                string ant;
                                cout << "Antécédent: "; getline(cin, ant);
                                p->get_dossier().ajouterAntecedent(ant);
                                cout << ">>> Enregistré." << endl;
                            }
                        } while (dossierChoix != 3);
                    }
                } else {
                    cout << "Aucun patient trouvé." << endl;
                    Outils::pause();
                }
                break;
            }
            case 3:
                Outils::printHeader("Mes Patients Assignés", Outils::BLUE);
                for(auto &p : Database::patients) {
                    if(p.get_MedecinTraitant() == Database::utilisateurConnecte->get_id()) {
                        cout << "- [" << p.get_id() << "] " << p.get_nom_complet() << endl;
                    }
                }
                Outils::pause();
                break;
        }
    } while(choix != 4);
}

void Application::menuProfessionnel() {
    do {
        Outils::printHeader("Espace Pro (" + Database::utilisateurConnecte->get_nom_complet() + ")", Outils::BLUE);
        cout << "1. Gestion des Patients" << endl;
        cout << "2. Gestion des Consultations" << endl;
        cout << "3. Gestion des Rendez-vous" << endl;
        cout << "4. Prescriptions et Examens (Bonus)" << endl;
        cout << "5. Mon Profil" << endl;
        cout << "6. Déconnexion" << endl;
        cout << "\nVotre choix: ";

        int choix; cin >> choix; cin.ignore();

        if (choix == 1) menuPatients();
        else if (choix == 2) menuConsultations();
        else if (choix == 3) break;
        else if (choix == 4) { cout << "Module Bonus..." << endl; Outils::pause(); }
        else if (choix == 5) {
            cout << "Nom: " << Database::utilisateurConnecte->get_nom_complet() << endl;
            cout << "Role: " << Database::utilisateurConnecte->getTypeString() << endl;
            Outils::pause();
        }
        else if (choix == 6) return;

    } while (true);
}

void Application::menuAdmin() {
    int choix;
    do {
        Outils::printHeader("Espace Admin (" + Database::utilisateurConnecte->get_nom_complet() + ")", Outils::RED);
        cout << "1. Gestion des Comptes Utilisateurs" << endl;
        cout << "2. Gestion des Droits" << endl;
        cout << "3. Statistiques Système" << endl;
        cout << "4. Consultation des Logs" << endl;
        cout << "5. Export CSV (Bonus)" << endl;
        cout << "6. Déconnexion" << endl;
        cout << "\nVotre choix: ";
        cin >> choix; cin.ignore();

        switch(choix) {
            case 1: {
                cout << "\n--- Création Utilisateur ---" << endl;
                string nom, prenom, login, mdp;
                cout << "Nom: "; cin >> nom;
                cout << "Prénom: "; cin >> prenom;
                cout << "Login: "; cin >> login;
                cout << "Mot de passe: "; cin >> mdp;
                cout << "Role (1=Pro, 2=Admin): "; int r; cin >> r;

                if(r == 1) {
                    string spec, serv;
                    cout << "Spécialité: "; cin >> spec;
                    cout << "Service: "; cin >> serv;
                    Database::utilisateurs.push_back(new ProfessionnelSante("USR"+to_string(rand()), nom, prenom, login, mdp, spec, serv));
                } else {
                    Database::utilisateurs.push_back(new Administrateur("ADM"+to_string(rand()), nom, prenom, login, mdp));
                }
                cout << ">>> Utilisateur créé." << endl;
                Database::ajouterLog("Admin a créé l'utilisateur " + login);
                Outils::pause();
                break;
            }
            case 3:
                Outils::printHeader("Statistiques Système", Outils::RED);
                cout << "Nombre de patients: " << Database::patients.size() << endl;
                cout << "Nombre d'utilisateurs: " << Database::utilisateurs.size() << endl;
                cout << "Consultations effectués: " << Database::consultations.size() << endl;
                Outils::pause();
                break;
            case 4:
                Outils::printHeader("Logs Système", Outils::RED);
                for(auto &l : Database::Logsysteme) l.afficher();
                Outils::pause();
                break;
            case 6: return;
        }
    } while (true);
}

void Application::ecranConnexion() {
    string login, mdp;
    while(true) {
        Outils::printHeader("Connexion", Outils::WHITE);
        cout << "1. Professionnel de Santé" << endl;
        cout << "2. Administrateur" << endl;
        cout << "3. Quitter" << endl;
        cout << "\nChoix: ";
        int type; cin >> type; cin.ignore();

        if(type == 3) exit(0);

        cout << "Login: "; getline(cin, login);
        cout << "Mot de passe: "; getline(cin, mdp);

        bool authentifie = false;

        for(auto* u : Database::utilisateurs) {
            if(u->get_login() == login && u->verifierMotDePasse(mdp)) {
                if(type == 1 && u->get_role() == PROFESSIONNEL) authentifie = true;
                if(type == 2 && u->get_role() == ADMINISTRATEUR) authentifie = true;

                if(authentifie) {
                    Database::utilisateurConnecte = u;
                    Database::ajouterLog("Connexion réussie");

                    if(u->get_role() == PROFESSIONNEL) menuProfessionnel();
                    else menuAdmin();

                    Database::utilisateurConnecte = nullptr;
                    break;
                }
            }
        }

        if(!authentifie) {
            Outils::setColor(Outils::RED);
            cout << "\n>>> Identifiants invalides ou rôle incorrect." << endl;
            Outils::setColor(Outils::DEFAULT);
            Outils::pause();
        }
    }
}
