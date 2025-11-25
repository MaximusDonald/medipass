#ifndef UTILISATEUR_H
#define UTILISATEUR_H
#include <string>
#include <sstream>
#include <iomanip>

enum Role { PROFESSIONNEL, ADMINISTRATEUR };

class Utilisateur
{
    public:
        // Constructeurs
        Utilisateur();
        Utilisateur(const Utilisateur& other);
        Utilisateur(std::string n, std::string p, std::string l, std::string mdp, Role r); //Constructeur avec génération auto de l'ID
        // Destructeur
        virtual ~Utilisateur();
        // Accesseurs
        std::string get_id() const;
        std::string get_nom() const;
        std::string get_prenom() const;
        std::string get_nom_complet() const;
        std::string get_login() const;
        std::string get_motDePasse() const;
        Role get_role() const;
        // Mutateurs
        void set_id (std::string i);
        void set_nom (std::string n);
        void set_prenom (std::string p);
        void set_login (std::string l);
        void set_motDePasse (std::string mdp);
        // Autres méthodes
        bool verifierMotDePasse(std::string mdp);
        virtual std::string getTypeString() = 0;
        static std::string genererID(Role r);
        // Attributs
    protected:
        std::string id;
        std::string nom;
        std::string prenom;
        std::string login;
        std::string motDePasse;
        Role role;
    private:
        static int compteur;
        void set_role (Role r);
};

#endif // UTILISATEUR_H
