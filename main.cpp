#include <iostream>
#include <vector>
#include <queue>
#include <chrono>
#include <iomanip>
#include <random>
#include <string>
#include <utility>

// ==========================================
// 1. Max Heap From Scratch
// ==========================================

class MaxHeap {
private:
    std::vector<int> tas;

public:

    // Réorganisation vers le haut
    void heapifyUp(int i) {
        while (i > 0) {
            int parent = (i - 1) / 2;

            if (tas[i] > tas[parent]) {
                std::swap(tas[i], tas[parent]);
                i = parent;
            } else {
                break;
            }
        }
    }

    // Réorganisation vers le bas
    void heapifyDown(int i) {
        int taille = static_cast<int>(tas.size());

        while (2 * i + 1 < taille) {
            int gauche = 2 * i + 1;
            int droite = 2 * i + 2;
            int plusGrand = i;

            if (gauche < taille && tas[gauche] > tas[plusGrand])
                plusGrand = gauche;

            if (droite < taille && tas[droite] > tas[plusGrand])
                plusGrand = droite;

            if (plusGrand != i) {
                std::swap(tas[i], tas[plusGrand]);
                i = plusGrand;
            } else {
                break;
            }
        }
    }

    // Insertion
    void inserer(int valeur) {
        tas.push_back(valeur);
        heapifyUp(static_cast<int>(tas.size()) - 1);
    }

    // Extraction seule :
    // Lire la racine, remplacer par le dernier élément
    // et supprimer le dernier élément.
    int extractionSeule() {
        if (tas.empty())
            return -1;

        int maximum = tas[0];

        tas[0] = tas.back();
        tas.pop_back();

        return maximum;
    }

    // Réorganisation seule
    void reorganisationSeule() {
        if (!tas.empty()) {
            heapifyDown(0);
        }
    }

    bool estVide() const {
        return tas.empty();
    }
};


// ==========================================
// 2. Programme principal : Benchmark
// ==========================================

int main() {

    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // Tailles des structures à tester
    std::vector<int> tailles = {
        1000,
        10000,
        100000,
        1000000
    };

    // Générateur de nombres aléatoires
    std::random_device rd;
    std::mt19937 generateur(rd());

    std::uniform_int_distribution<int> distribution(
        1, 10000000
    );

    // ==========================================
    // En-tête du tableau
    // ==========================================

    std::cout << "\n"
              << std::string(115, '=') << "\n";

    std::cout << std::left
              << std::setw(15) << "Taille N"
              << std::setw(20) << "FromScratchIns(ms)"
              << std::setw(20) << "PriorityQueueIns(ms)"
              << std::setw(20) << "Extraction(ms)"
              << std::setw(20) << "Reorganisation(ms)"
              << std::setw(20) << "PQ_Suppression(ms)"
              << "\n";

    std::cout << std::string(115, '=') << "\n";


    // ==========================================
    // 3. Tests pour chaque taille N
    // ==========================================

    for (int n : tailles) {

        // Génération des mêmes données pour les deux structures
        std::vector<int> donnees(n);

        for (int i = 0; i < n; i++) {
            donnees[i] = distribution(generateur);
        }


        // ==========================================
        // A. MAX HEAP FROM SCRATCH
        // ==========================================

        // Nouvelle instance vide pour chaque N
        MaxHeap tasPersonnalise;


        // ------------------------------------------
        // 1. Mesure du temps d'insertion
        // ------------------------------------------

        auto debut = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < n; i++) {
            tasPersonnalise.inserer(donnees[i]);
        }

        auto fin = std::chrono::high_resolution_clock::now();

        double tempsInsertion =
            std::chrono::duration<double, std::milli>(
                fin - debut
            ).count();


        // ------------------------------------------
        // 2. Mesure d'UNE SEULE extraction
        // ------------------------------------------

        volatile int valeurExtraite = 0;

        auto debutExtraction =
            std::chrono::high_resolution_clock::now();

        valeurExtraite = tasPersonnalise.extractionSeule();

        auto finExtraction =
            std::chrono::high_resolution_clock::now();

        double tempsExtraction =
            std::chrono::duration<double, std::milli>(
                finExtraction - debutExtraction
            ).count();


        // ------------------------------------------
        // 3. Mesure d'UNE SEULE réorganisation
        // ------------------------------------------

        auto debutReorganisation =
            std::chrono::high_resolution_clock::now();

        tasPersonnalise.reorganisationSeule();

        auto finReorganisation =
            std::chrono::high_resolution_clock::now();

        double tempsReorganisation =
            std::chrono::duration<double, std::milli>(
                finReorganisation - debutReorganisation
            ).count();


        // ==========================================
        // B. STL : PRIORITY QUEUE
        // ==========================================

        // Nouvelle instance vide pour chaque N
        std::priority_queue<int> priorityQueue;


        // ------------------------------------------
        // 1. Mesure du temps d'insertion
        // ------------------------------------------

        debut = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < n; i++) {
            priorityQueue.push(donnees[i]);
        }

        fin = std::chrono::high_resolution_clock::now();

        double tempsInsertionPQ =
            std::chrono::duration<double, std::milli>(
                fin - debut
            ).count();


        // ------------------------------------------
        // 2. Mesure d'UNE SEULE suppression
        // pop() réalise extraction + réorganisation
        // ------------------------------------------

        debut = std::chrono::high_resolution_clock::now();

        priorityQueue.pop();

        fin = std::chrono::high_resolution_clock::now();

        double tempsSuppressionPQ =
            std::chrono::duration<double, std::milli>(
                fin - debut
            ).count();


        // ==========================================
        // C. AFFICHAGE DES RESULTATS
        // ==========================================

        std::cout << std::left
                  << std::setw(15) << n
                  << std::setw(20) << std::fixed
                  << std::setprecision(6) << tempsInsertion
                  << std::setw(20) << tempsInsertionPQ
                  << std::setw(20) << tempsExtraction
                  << std::setw(20) << tempsReorganisation
                  << std::setw(20) << tempsSuppressionPQ
                  << "\n";
    }

    std::cout << std::string(115, '=') << "\n";

    return 0;
}
