#ifndef ANTECEDENT_H
#define ANTECEDENT_H

#include <string>
#include "Outils.h"


class Antecedent
{
    public:

        Antecedent();

        Antecedent(const Antecedent& other);

        Antecedent(std::string type, std::string description, int gravite = 1);


        ~Antecedent();


        std::string get_id() const;
        std::string get_type() const;
        std::string get_date() const;
        std::string get_description() const;
        int         get_gravite() const;
        std::string get_graviteLibelle() const;


        void set_type(const std::string& val);
        void set_description(const std::string& val);
        void set_gravite(int val);


        void afficher() const;

        static std::string genererId();

    private:
        static int compteurId;

        std::string id;           // ANT-00001, ANT-00002…
        std::string type;         // ex: "Allergie", "Chirurgie", "Pathologie chronique", "Traitement en cours"
        std::string date;         // format "AAAA-MM-JJ"
        std::string description;  // texte libre détaillé
        int         gravite;      // 1 = Légère, 2 = Modérée, 3 = Sévère, 4 = Critique
};

#endif // ANTECEDENT_H
