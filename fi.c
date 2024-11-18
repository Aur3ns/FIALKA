#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

typedef unsigned char byte;
typedef uint64_t pint;

FILE *logFile = NULL;

#define ROT 10          // Nombre de rotors
#define MOD 30          // Alphabet de 30 caractères (Fialka)
#define MAX_TAPE 1024   // Longueur maximale de la bande perforée
#define BITS_PER_CHAR 5 // Codage 5-bit pour les bandes perforées

// Prototypes de fonctions
void saveResults(const char *filename, const byte *before, const byte *after, const byte *decrypted, size_t length);
void saveTape(const char *filename, const byte *tape, size_t length, const char *metadata);
size_t loadTape(const char *filename, byte *tape);
void loadKey(const char *keyFile, int isMessageKey);
void loadPlugboardConfig(const char *plugboardFile);
void encodeTo5Bit(const byte *input, size_t length, byte *output, size_t *outputLength);
void decodeFrom5Bit(const byte *input, size_t length, byte *output, size_t *outputLength);
void validateReflectors();
void validatePlugboard();
void validateRotors();
void validateRotorInversion();
void generatePermutation(byte *perm, byte *invPerm, int size);


// Tables globales
byte REFLECTORS[3][MOD]; // Trois configurations possibles pour le réflecteur
byte ENTRY_DISC[MOD];
byte ROTOR[ROT][2][MOD];  // Chaque rotor a deux faces (ajustables)
byte ROTOR_INV[ROT][2][MOD];
byte PLUGBOARD[MOD];      // Substitutions dynamiques via cartes perforées

// État de la machine
byte rotor_positions[ROT];
byte rotor_cores[ROT];    // Sélection du noyau de chaque rotor (0 ou 1)
byte rotor_order[ROT];    // Ordre dynamique des rotors
pint blocking_pins[ROT];  // Broches de blocage pour chaque rotor
byte rotor_fixed[ROT];    // Rotors marqués comme fixes
int active_reflector = 0; // Indice du réflecteur actif

void generateReflector(byte *reflector, int size) {
    if (!reflector || size <= 0) {
        fprintf(stderr, "Erreur : Arguments invalides pour generateReflector.\n");
        exit(EXIT_FAILURE);
    }

    // Tableau de suivi pour éviter les auto-mappages ou doublons
    int assigned[size];
    for (int i = 0; i < size; i++) {
        assigned[i] = 0; // Marquer toutes les positions comme non assignées
        reflector[i] = 255; // Valeur par défaut invalide pour byte
    }

    for (int i = 0; i < size; i++) {
        if (reflector[i] == 255) { // Si cette position n'est pas encore assignée
            int pair;
            do {
                pair = rand() % size; // Générer un index aléatoire
            } while (pair == i || assigned[pair]); // Éviter auto-mappages et doublons

            reflector[i] = pair;
            reflector[pair] = i;
            assigned[i] = assigned[pair] = 1; // Marquer ces positions comme assignées
        }
    }

    // Validation finale pour garantir la symétrie
    for (int i = 0; i < size; i++) {
        if (reflector[i] < 0 || reflector[i] >= size || reflector[reflector[i]] != i) {
            fprintf(stderr, "Erreur : Réflecteur non valide après génération pour %d -> %d.\n", i, reflector[i]);
            exit(EXIT_FAILURE);
        }
    }
}


void generatePermutation(byte *perm, byte *invPerm, int size) {
    if (!perm || !invPerm || size <= 0) {
        fprintf(stderr, "Erreur : Arguments invalides pour generatePermutation.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < size; i++) {
        perm[i] = i; // Initialisation
    }

    // Fisher-Yates Shuffle pour mélanger les éléments
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        byte temp = perm[i];
        perm[i] = perm[j];
        perm[j] = temp;
    }

    // Générer la table d'inversion
    for (int i = 0; i < size; i++) {
        invPerm[perm[i]] = i;
    }
}


void initializeDefaults() {
    srand((unsigned int)time(NULL)); // Initialiser le générateur aléatoire

    // Générer les réflecteurs
    for (int r = 0; r < 3; r++) {
        generateReflector(REFLECTORS[r], MOD);
    }

    // Initialiser le disque d'entrée et le plugboard
    for (int i = 0; i < MOD; i++) {
        ENTRY_DISC[i] = i;  // Disque d'entrée par défaut
        PLUGBOARD[i] = i;   // Plugboard par défaut (identité)
    }

    // Générer les permutations pour chaque rotor
    for (int r = 0; r < ROT; r++) {
        generatePermutation(ROTOR[r][0], ROTOR_INV[r][0], MOD); // Face 0
        generatePermutation(ROTOR[r][1], ROTOR_INV[r][1], MOD); // Face 1
        rotor_fixed[r] = 0; // Tous les rotors sont mobiles par défaut
    }

    for (int i = 0; i < ROT; i++) {
        rotor_order[i] = i;
    }

    // Valider les réflecteurs, le plugboard et les rotors
    validateReflectors();
    validatePlugboard();
    validateRotors();
    validateRotorInversion();
}

void validateRotors() {
    for (int r = 0; r < ROT; r++) {
        for (int core = 0; core < 2; core++) {
            int inverse[MOD] = {0};
            for (int i = 0; i < MOD; i++) {
                int mapped = ROTOR[r][core][i];
                if (inverse[mapped] != 0 && inverse[mapped] != i + 1) {
                    fprintf(stderr, "Erreur : Rotor %d core %d non bijectif.\n", r, core);
                    exit(EXIT_FAILURE);
                }
                inverse[mapped] = i + 1;
                // Vérifiez l'inversion
                if (ROTOR_INV[r][core][mapped] != i) {
                    fprintf(stderr, "Erreur : Inversion incorrecte pour le rotor %d core %d.\n", r, core);
                    exit(EXIT_FAILURE);
                }
            }
        }
    }
}

void validateRotorInversion() {
    for (int r = 0; r < ROT; r++) {
        for (int core = 0; core < 2; core++) {
            for (int i = 0; i < MOD; i++) {
                int forward = ROTOR[r][core][i];
                int inverse = ROTOR_INV[r][core][forward];
                if (inverse != i) {
                    fprintf(stderr, "Erreur : Inversion incorrecte dans le rotor %d core %d pour %d -> %d -> %d.\n",
                            r, core, i, forward, inverse);
                    exit(EXIT_FAILURE);
                }
            }
        }
    }
}


void validatePlugboard() {
    int counts[MOD] = {0};
    for (int i = 0; i < MOD; i++) {
        if (PLUGBOARD[i] < 0 || PLUGBOARD[i] >= MOD || PLUGBOARD[PLUGBOARD[i]] != i) {
            fprintf(stderr, "Erreur : Plugboard non valide pour %d -> %d.\n", i, PLUGBOARD[i]);
            exit(EXIT_FAILURE);
        }
        counts[PLUGBOARD[i]]++;
    }

    for (int i = 0; i < MOD; i++) {
        if (counts[i] != 1) {
            fprintf(stderr, "Erreur : Plugboard n'est pas bijectif.\n");
            exit(EXIT_FAILURE);
        }
    }
    if (logFile) fprintf(logFile, "Plugboard validé avec succès.\n");
}


void validateReflectors() {
    for (int r = 0; r < 3; r++) {
        for (int i = 0; i < MOD; i++) {
            int mapped = REFLECTORS[r][i];
            if (mapped < 0 || mapped >= MOD || REFLECTORS[r][mapped] != i) {
                fprintf(stderr, "Erreur : Réflecteur %d non valide pour %d -> %d.\n", r, i, mapped);
                exit(EXIT_FAILURE);
            }
        }
    }
    if (logFile) fprintf(logFile, "Réflecteurs validés avec succès.\n");
}


// Initialisation de la machine avec positions aléatoires et broches de blocage
void initializeMachine() {
    srand((unsigned int)time(NULL));
    for (int i = 0; i < ROT; i++) {
        rotor_positions[i] = rand() % MOD;
        rotor_cores[i] = rand() % 2;  // Face aléatoire
        blocking_pins[i] = rand() % 2;
    }
}

// Charger une clé quotidienne ou par message
void loadKey(const char *keyFile, int isMessageKey) {
    FILE *file = fopen(keyFile, "r");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier de clé");
        exit(EXIT_FAILURE);
    }

    printf("Chargement du fichier de clé : %s\n", keyFile);

    char line[256]; // Buffer pour lire une ligne complète
    int lineCount = 0; // Compteur de lignes valides lues

    while (fgets(line, sizeof(line), file)) {
        // Ignorer les lignes vides ou les commentaires
        if (line[0] == '#' || line[0] == '\n') continue;

        // Scanner les données pour les rotors
        if (lineCount < ROT) {
            int res = sscanf(line, "%hhu %hhu %hhu %hhu", 
                             &rotor_order[lineCount], 
                             &rotor_positions[lineCount], 
                             &rotor_cores[lineCount], 
                             &rotor_fixed[lineCount]);
            if (res != 4) {
                fprintf(stderr, "Erreur dans la lecture de la clé à la ligne %d. Ligne : %s\n", lineCount + 1, line);
                exit(EXIT_FAILURE);
            }

            // Corriger la position si elle dépasse le MOD
            if (rotor_positions[lineCount] >= MOD) {
                printf("Correction : Rotor %d position=%hhu dépassait MOD=%d, corrigé à %hhu\n", 
                       lineCount, rotor_positions[lineCount], MOD, rotor_positions[lineCount] % MOD);
                rotor_positions[lineCount] %= MOD;
            }

            printf("Rotor %d : ordre=%hhu, position=%hhu, face=%hhu, fixe=%hhu\n", 
                   lineCount, rotor_order[lineCount], rotor_positions[lineCount], 
                   rotor_cores[lineCount], rotor_fixed[lineCount]);
            lineCount++;
        } 
        // Scanner les données pour le réflecteur actif
        else if (isMessageKey && lineCount == ROT) {
            int res = sscanf(line, "%d", &active_reflector);
            if (res != 1) {
                fprintf(stderr, "Erreur dans la lecture du réflecteur actif. Ligne : %s\n", line);
                exit(EXIT_FAILURE);
            }
            printf("Réflecteur actif : %d\n", active_reflector);
            lineCount++;
        }
    }

    // Vérification de la complétude
    if (lineCount < ROT || (isMessageKey && lineCount != ROT + 1)) {
        fprintf(stderr, "Le fichier de clé ne contient pas assez de lignes valides.\n");
        exit(EXIT_FAILURE);
    }

    fclose(file);
}

// Charger une configuration dynamique du plugboard depuis un fichier
void loadPlugboardConfig(const char *plugboardFile) {
    FILE *file = fopen(plugboardFile, "r");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier plugboard");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < MOD; i++) {
        if (fscanf(file, "%hhu", &PLUGBOARD[i]) != 1) {
            fprintf(stderr, "Erreur dans la configuration du plugboard.\n");
            exit(EXIT_FAILURE);
        }
    }
    fclose(file);
}

// Lecture d'une bande perforée depuis un fichier
size_t loadTape(const char *filename, byte *tape) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Fichier '%s' non trouvé. Création d'une bande perforée par défaut.\n", filename);
        const char *defaultText = "HELLOFIALKA";
        size_t length = strlen(defaultText);
        for (size_t i = 0; i < length; i++) {
            tape[i] = defaultText[i] - 'A';  // Convertir en indices pour Fialka
        }
        saveTape(filename, tape, length, "Bande par défaut générée");
        return length;
    }

    char buffer[MAX_TAPE];
    size_t length = fread(buffer, sizeof(char), MAX_TAPE, file);
    fclose(file);

    // Convertir en indices
    for (size_t i = 0; i < length; i++) {
        tape[i] = (buffer[i] >= 'A' && buffer[i] <= 'Z') ? buffer[i] - 'A' : 0;
    }
    return length;
}

// Codage en 5-bit pour bandes perforées
void encodeTo5Bit(const byte *input, size_t length, byte *output, size_t *outputLength) {
    *outputLength = 0;
    for (size_t i = 0; i < length; i++) {
        output[*outputLength] = input[i] & 0x1F; // Conserver seulement les 5 bits
        (*outputLength)++;
    }
}

// Décodage d'une bande perforée en 5-bit
void decodeFrom5Bit(const byte *input, size_t length, byte *output, size_t *outputLength) {
    *outputLength = 0;
    for (size_t i = 0; i < length; i++) {
        output[*outputLength] = input[i] & 0x1F; // Décoder les 5 bits
        (*outputLength)++;
    }
}

// Sauvegarder une bande perforée avec métadonnées
void saveTape(const char *filename, const byte *tape, size_t length, const char *metadata) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Erreur lors de l'écriture de la bande perforée");
        return;
    }
    fprintf(file, "METADATA: %s\n", metadata);
    for (size_t i = 0; i < length; i++) {
        fprintf(file, "%c", tape[i] + 'A');
    }
    fclose(file);
}

// Simulation du pas des rotors avec broches de blocage
void stepRotors() {
    if (logFile) {
        fprintf(logFile, "Pas des rotors avant : ");
        for (int i = 0; i < ROT; i++) {
            fprintf(logFile, "%d ", rotor_positions[i]);
        }
        fprintf(logFile, "\n");
    }

    for (int i = 0; i < ROT; i++) {
        int rotorIndex = rotor_order[i];
        if (!rotor_fixed[rotorIndex]) {
            if (i == 0 || blocking_pins[rotor_order[i - 1]] == 0) {
                rotor_positions[rotorIndex] = (rotor_positions[rotorIndex] + 1) % MOD;
            }
        }
    }

    if (logFile) {
        fprintf(logFile, "Pas des rotors après : ");
        for (int i = 0; i < ROT; i++) {
            fprintf(logFile, "%d ", rotor_positions[i]);
        }
        fprintf(logFile, "\n");
    }
}

// Substitution avec plugboard
byte plugboardSubstitute(byte input) {
    return PLUGBOARD[input];
}

// Substitution via un rotor
byte rotorSubstitute(byte input, int rotor, int forward) {
    byte core = rotor_cores[rotor];
    byte pos = rotor_positions[rotor];

    if (forward) {
        byte mapped = (ROTOR[rotor][core][(input + pos) % MOD] - pos + MOD) % MOD;
        if (logFile) fprintf(logFile, "Rotor %d forward: %c -> %c\n", rotor, input + 'A', mapped + 'A');
        return mapped;
    } else {
        byte mapped = (ROTOR_INV[rotor][core][(input + pos) % MOD] - pos + MOD) % MOD;
        if (logFile) fprintf(logFile, "Rotor %d reverse: %c -> %c\n", rotor, input + 'A', mapped + 'A');
        return mapped;
    }
}

// Traitement d'un caractère
byte processByte(byte input, int mode) {
    byte original = input;
    if (logFile) fprintf(logFile, "Original: %c\n", original + 'A');

    byte output = plugboardSubstitute(input);
    if (logFile) fprintf(logFile, "After Plugboard: %c\n", output + 'A');

    output = ENTRY_DISC[output];
    if (logFile) fprintf(logFile, "After Entry Disc: %c\n", output + 'A');

    for (int i = 0; i < ROT; i++) {
        output = rotorSubstitute(output, rotor_order[i], 1);
        if (logFile) fprintf(logFile, "After Rotor %d (forward): %c\n", i, output + 'A');
    }

    output = REFLECTORS[active_reflector][output];
    if (logFile) fprintf(logFile, "After Reflector: %c\n", output + 'A');

    for (int i = ROT - 1; i >= 0; i--) {
        output = rotorSubstitute(output, rotor_order[i], 0);
        if (logFile) fprintf(logFile, "After Rotor %d (reverse): %c\n", i, output + 'A');
    }

    output = ENTRY_DISC[output];
    if (logFile) fprintf(logFile, "After Entry Disc: %c\n", output + 'A');

    output = plugboardSubstitute(output);
    if (logFile) fprintf(logFile, "Final Output: %c\n", output + 'A');

    if (mode == 0 && original != output) {
        if (logFile) fprintf(logFile, "Erreur : ProcessByte n'est pas réversible pour %c -> %c -> %c\n",
                             original + 'A', input + 'A', output + 'A');
    }

    return output;
}

void resetRotors(const byte *initial_positions, const byte *initial_cores) {
    memcpy(rotor_positions, initial_positions, ROT);
    memcpy(rotor_cores, initial_cores, ROT);
}

void encrypt(byte *plaintext, byte *ciphertext, size_t length) { //Chiffrement d'un texte
    if (logFile) fprintf(logFile, "Début du chiffrement :\n");
    for (size_t i = 0; i < length; i++) {
        ciphertext[i] = processByte(plaintext[i], 1);
        if (logFile) fprintf(logFile, "Plaintext[%zu]: %c -> Ciphertext[%zu]: %c\n",
                             i, plaintext[i] + 'A', i, ciphertext[i] + 'A');
    }
    if (logFile) fprintf(logFile, "Fin du chiffrement.\n");
}

void decrypt(byte *ciphertext, byte *plaintext, size_t length) { //Déchiffrement d'un texte chiffré
    if (logFile) fprintf(logFile, "Début du déchiffrement :\n");
    for (size_t i = 0; i < length; i++) {
        plaintext[i] = processByte(ciphertext[i], 0);
        if (logFile) fprintf(logFile, "Ciphertext[%zu]: %c -> Plaintext[%zu]: %c\n",
                             i, ciphertext[i] + 'A', i, plaintext[i] + 'A');
    }
    if (logFile) fprintf(logFile, "Fin du déchiffrement.\n");
}

// Écriture des résultats dans un fichier unique
void saveResults(const char *filename, const byte *before, const byte *after, const byte *decrypted, size_t length) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Erreur lors de l'écriture des résultats");
        return;
    }

    fprintf(file, "Texte avant chiffrement :\n");
    for (size_t i = 0; i < length; i++) {
        fprintf(file, "%c", before[i] + 'A');
    }
    fprintf(file, "\n\n");

    if (after) {
        fprintf(file, "Texte après chiffrement :\n");
        for (size_t i = 0; i < length; i++) {
            fprintf(file, "%c", after[i] + 'A');
        }
        fprintf(file, "\n\n");
    }

    if (decrypted) {
        fprintf(file, "Texte après déchiffrement :\n");
        for (size_t i = 0; i < length; i++) {
            fprintf(file, "%c", decrypted[i] + 'A');
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

int main() {

    // Ouvrir le fichier de log
    logFile = fopen("machine_actions.log", "w");
    if (!logFile) {
        perror("Erreur lors de l'ouverture du fichier de log");
        exit(EXIT_FAILURE);
    }

    // Initialiser les paramètres par défaut
    initializeDefaults();

    // Charger une clé quotidienne
    loadKey("daily_key.txt", 0);

    // Charger une clé par message
    loadKey("message_key.txt", 1);

    // Charger une configuration de plugboard
    loadPlugboardConfig("plugboard_config.txt");

    // Charger une bande perforée d'entrée
    byte inputTape[MAX_TAPE];
    size_t length = loadTape("input.tape", inputTape);

    // Buffers pour sortie et déchiffrement
    byte outputTape[MAX_TAPE];
    byte decryptedTape[MAX_TAPE];

    // Sauvegarder les états initiaux des rotors
    byte initial_positions[ROT];
    byte initial_cores[ROT];
    memcpy(initial_positions, rotor_positions, ROT);
    memcpy(initial_cores, rotor_cores, ROT);

    // Chiffrement
    encrypt(inputTape, outputTape, length);

    // Réinitialiser les rotors avant le déchiffrement
    resetRotors(initial_positions, initial_cores);

    // Déchiffrement
    decrypt(outputTape, decryptedTape, length);

    // Sauvegarder les résultats dans un fichier
    saveResults("output.tape", inputTape, outputTape, decryptedTape, length);

    printf("Chiffrement et déchiffrement terminés. Consultez 'output.tape' pour les résultats.\n");

    return 0;
}
