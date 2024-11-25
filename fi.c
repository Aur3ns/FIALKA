#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

typedef unsigned char byte;
typedef uint64_t pint;

FILE *logFile = NULL;

#define ROT 10          // Nombre de rotors | Number of rotors
#define MOD 30          // Alphabet de 30 caractères (Fialka) | 30-character alphabet (Fialka)
#define MAX_TAPE 1024   // Longueur maximale de la bande perforée | Maximum length of the perforated strip
#define BITS_PER_CHAR 5 // Codage 5-bit pour les bandes perforées | 5-bit coding for perforated bands

// Prototypes de fonctions | Function prototype
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


// Tables globales | Global Tables 
byte REFLECTORS[3][MOD]; // Trois configurations possibles pour le réflecteur | Three possible reflector configurations
byte ENTRY_DISC[MOD];
byte ROTOR[ROT][2][MOD];  // Chaque rotor a deux faces (ajustables) | Each rotor has two (adjustable) faces
byte ROTOR_INV[ROT][2][MOD];
byte PLUGBOARD[MOD];      // Substitutions dynamiques via cartes perforées | Dynamic substitution via punched cards

// État de la machine | State of the machine
byte rotor_positions[ROT];
byte rotor_cores[ROT];    // Sélection du noyau de chaque rotor (0 ou 1) | Core selection for each rotor (0 or 1)
byte rotor_order[ROT];    // Ordre dynamique des rotors                  | Dynamic rotor order
pint blocking_pins[ROT];  // Broches de blocage pour chaque rotor        | Locking pins for each rotor
byte rotor_fixed[ROT];    // Rotors marqués comme fixes                  | Rotors marked as fixed
int active_reflector = 0; // Indice du réflecteur actif                  |  Active reflector index

void generateReflector(byte *reflector, int size) {
    if (!reflector || size <= 0) {
        fprintf(stderr, "Error: Invalid arguments for generateReflector.\n"); //Erreur : Arguments invalides pour generateReflector
        exit(EXIT_FAILURE);
    }

    // Tableau de suivi pour éviter les auto-mappages ou doublons | Tracking table to avoid auto-mapping or duplicates
    int assigned[size];
    for (int i = 0; i < size; i++) {
        assigned[i] = 0; // Marquer toutes les positions comme non assignées | Mark all positions as unassigned
        reflector[i] = 255; // Valeur par défaut invalide pour byte | Invalid default value for byte
    }

    for (int i = 0; i < size; i++) {
        if (reflector[i] == 255) { // Si cette position n'est pas encore assignée |  If this position has not yet been assigned
            int pair;
            do {
                pair = rand() % size; // Générer un index aléatoire | Generate a random index
            } while (pair == i || assigned[pair]); // Éviter auto-mappages et doublons | Avoid auto-mapping and duplication

            reflector[i] = pair;
            reflector[pair] = i;
            assigned[i] = assigned[pair] = 1; // Marquer ces positions comme assignées | Mark these positions as assigned
        }
    }

    // Validation finale pour garantir la symétrie | Final validation to guarantee symmetry
    for (int i = 0; i < size; i++) {
        if (reflector[i] < 0 || reflector[i] >= size || reflector[reflector[i]] != i) {
            fprintf(stderr, "Error: Invalid reflector after generation for %d -> %d.\n", i, reflector[i]); //Erreur : Réflecteur non valide après génération pour %d -> %d.\n
            exit(EXIT_FAILURE);
        }
    }
}


void generatePermutation(byte *perm, byte *invPerm, int size) {
    if (!perm || !invPerm || size <= 0) {
        fprintf(stderr, "Error: Invalid arguments for generatePermutation.\n"); //Erreur : Arguments invalides pour generatePermutation.\n
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < size; i++) {
        perm[i] = i; // Initialisation
    }

    // Algorithme de Fisher-Yates Shuffle pour mélanger les éléments | Fisher-Yates Shuffle algorithm for mixing elements
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        byte temp = perm[i];
        perm[i] = perm[j];
        perm[j] = temp;
    }

    // Générer la table d'inversion | Generate inversion table
    for (int i = 0; i < size; i++) {
        invPerm[perm[i]] = i;
    }
}


void initializeDefaults() {
    srand((unsigned int)time(NULL)); // Initialiser le générateur aléatoire | Initialize the random generator

    // Générer les réflecteurs | Generate reflectors
    for (int r = 0; r < 3; r++) {
        generateReflector(REFLECTORS[r], MOD);
    }

    // Initialiser le disque d'entrée et le plugboard | Initialize input disk and plugboard
    for (int i = 0; i < MOD; i++) {
        ENTRY_DISC[i] = i;  // Disque d'entrée par défaut | Default input disk
        PLUGBOARD[i] = i;   // Plugboard par défaut (identité) | Default plugboard (identity)
    }

    // Générer les permutations pour chaque rotor | Generate permutations for each rotor
    for (int r = 0; r < ROT; r++) {
        generatePermutation(ROTOR[r][0], ROTOR_INV[r][0], MOD); // Face 0
        generatePermutation(ROTOR[r][1], ROTOR_INV[r][1], MOD); // Face 1
        rotor_fixed[r] = 0; // Tous les rotors sont mobiles par défaut | All rotors are mobile by default
    }

    for (int i = 0; i < ROT; i++) {
        rotor_order[i] = i;
    }

    // Valider les réflecteurs, le plugboard et les rotors | Validate reflectors, plugboard and rotors
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
                    fprintf(stderr, "Error: Rotor %d core %d is not bijective.\n", r, core); //"Erreur : Rotor %d core %d non bijectif.\n"
                    exit(EXIT_FAILURE);
                }
                inverse[mapped] = i + 1;
                // Vérifiez l'inversion
                if (ROTOR_INV[r][core][mapped] != i) {
                    fprintf(stderr, "Error: Incorrect inversion for rotor %d core %d.\n", r, core); //"Erreur : Inversion incorrecte pour le rotor %d core %d.\n"
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
                    fprintf(stderr, "Error: Incorrect inversion in rotor %d core %d for %d -> %d -> %d.\n", r, core, i, forward, inverse); 
                    //fprintf(stderr, "Erreur : Inversion incorrecte dans le rotor %d core %d pour %d -> %d -> %d.\n",r, core, i, forward, inverse); 
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
            fprintf(stderr, "Error: Invalid plugboard configuration for %d -> %d.\n", i, PLUGBOARD[i]);
            //fprintf(stderr, "Erreur : Plugboard non valide pour %d -> %d.\n", i, PLUGBOARD[i]);
            exit(EXIT_FAILURE);
        }
        counts[PLUGBOARD[i]]++;
    }

    for (int i = 0; i < MOD; i++) {
        if (counts[i] != 1) {
            //fprintf(stderr, "Erreur : Plugboard n'est pas bijectif.\n");
            fprintf(stderr, "Error: Plugboard is not bijective.\n");
            exit(EXIT_FAILURE);
        }
    }
    if (logFile) fprintf(logFile, "Plugboard successfully validated\n"); //Plugboard validé avec succès.
}


void validateReflectors() {
    for (int r = 0; r < 3; r++) {
        for (int i = 0; i < MOD; i++) {
            int mapped = REFLECTORS[r][i];
            if (mapped < 0 || mapped >= MOD || REFLECTORS[r][mapped] != i) {
                //fprintf(stderr, "Erreur : Réflecteur %d non valide pour %d -> %d.\n", r, i, mapped);
                fprintf(stderr, "Error: Reflector %d is invalid for %d -> %d.\n", r, i, mapped);
                exit(EXIT_FAILURE);
            }
        }
    }
    if (logFile) fprintf(logFile, "Reflectors successfully validated.\n"); //Réflecteurs validés avec succès.\n
}


// Machine initialization with random positions and locking pins | Initialisation de la machine avec positions aléatoires et broches de blocage 
void initializeMachine() {
    srand((unsigned int)time(NULL));
    for (int i = 0; i < ROT; i++) {
        rotor_positions[i] = rand() % MOD;
        rotor_cores[i] = rand() % 2;  // Face aléatoire | Random Face
        blocking_pins[i] = rand() % 2;
    }
}

// Charger une clé quotidienne ou par message | Load a daily or message key
void loadKey(const char *keyFile, int isMessageKey) {
    FILE *file = fopen(keyFile, "r");
    if (!file) {
        perror("Error while opening the key file");
        //perror("Erreur lors de l'ouverture du fichier de clé");
        exit(EXIT_FAILURE);
    }

    printf("Loading the key file : %s\n", keyFile); //Chargement du fichier de clé

    char line[256]; // Buffer pour lire une ligne complète | Buffer to read a full line
    int lineCount = 0; // Compteur de lignes valides lues | Counter of valid lines read

    while (fgets(line, sizeof(line), file)) {
        // Ignore blank lines or comments | Ignorer les lignes vides ou les commentaires 
        if (line[0] == '#' || line[0] == '\n') continue;

        // Scan data for rotors | Scanner les données pour les rotors
        if (lineCount < ROT) {
            int res = sscanf(line, "%hhu %hhu %hhu %hhu", 
                             &rotor_order[lineCount], 
                             &rotor_positions[lineCount], 
                             &rotor_cores[lineCount], 
                             &rotor_fixed[lineCount]);
            if (res != 4) {
                fprintf(stderr, "Error reading the key at line %d. Line: %s\n", lineCount + 1, line);
                //fprintf(stderr, "Erreur dans la lecture de la clé à la ligne %d. Ligne : %s\n", lineCount + 1, line);
                exit(EXIT_FAILURE);
            }

            // Correct position if it exceeds MOD | Corriger la position si elle dépasse le MOD
            if (rotor_positions[lineCount] >= MOD) {
                printf("Correction: Rotor %d position=%hhu exceeded MOD=%d, corrected to %hhu\n", 
                //printf("Correction : Rotor %d position=%hhu dépassait MOD=%d, corrigé à %hhu\n", 
                       lineCount, rotor_positions[lineCount], MOD, rotor_positions[lineCount] % MOD);
                rotor_positions[lineCount] %= MOD;
            }

            printf("Rotor %d: order=%hhu, position=%hhu, face=%hhu, fixed=%hhu\n",
            //printf("Rotor %d : ordre=%hhu, position=%hhu, face=%hhu, fixe=%hhu\n",
                
                   lineCount, rotor_order[lineCount], rotor_positions[lineCount], 
                   rotor_cores[lineCount], rotor_fixed[lineCount]);
            lineCount++;
        } 
        // Scanner les données pour le réflecteur actif | Scan data for active reflector
        else if (isMessageKey && lineCount == ROT) {
            int res = sscanf(line, "%d", &active_reflector);
            if (res != 1) {
                fprintf(stderr, "Error reading the active reflector. Line: %s\n", line);
                //fprintf(stderr, "Erreur dans la lecture du réflecteur actif. Ligne : %s\n", line); 
                exit(EXIT_FAILURE);
            }
            printf("Réflecteur actif : %d\n", active_reflector); //Active reflector
            lineCount++;
        }
    }

    // Vérification de la complétude | Completeness check
    if (lineCount < ROT || (isMessageKey && lineCount != ROT + 1)) {
        fprintf(stderr, "The key file does not contain enough valid lines.\n");
        //fprintf(stderr, "Le fichier de clé ne contient pas assez de lignes valides.\n");
        exit(EXIT_FAILURE);
    }

    fclose(file);
}

// Load a dynamic plugboard configuration from a file | Charger une configuration dynamique du plugboard depuis un fichier
void loadPlugboardConfig(const char *plugboardFile) {
    FILE *file = fopen(plugboardFile, "r");
    if (!file) {
        perror("Error opening plugboard file"); // Erreur lors de l'ouverture du fichier plugboard
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < MOD; i++) {
        if (fscanf(file, "%hhu", &PLUGBOARD[i]) != 1) {
            fprintf(stderr, "Error in the plugboard configuration.\n");
            //fprintf(stderr, "Erreur dans la configuration du plugboard.\n");
            exit(EXIT_FAILURE);
        }
    }
    fclose(file);
}

// Lecture d'une bande perforée depuis un fichier | Read a punched tape from a file
size_t loadTape(const char *filename, byte *tape) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        //printf("Fichier '%s' non trouvé. Création d'une bande perforée par défaut.\n", filename);
        printf("File '%s' not found. Creating a default punched tape.\n", filename);
        const char *defaultText = "HELLOFIALKA";
        size_t length = strlen(defaultText);
        for (size_t i = 0; i < length; i++) {
            tape[i] = defaultText[i] - 'A';  // Convert to indexes for Fialka | Convertir en indices pour Fialka
        }
        saveTape(filename, tape, length, "Default tape generated"); //Bande par défaut générée
        return length;
    }

    char buffer[MAX_TAPE];
    size_t length = fread(buffer, sizeof(char), MAX_TAPE, file);
    fclose(file);

    // Convertir en indices | Convert to indices
    for (size_t i = 0; i < length; i++) {
        tape[i] = (buffer[i] >= 'A' && buffer[i] <= 'Z') ? buffer[i] - 'A' : 0;
    }
    return length;
}

// Codage en 5-bit pour bandes perforées | 5-bit coding for perforated tapes
void encodeTo5Bit(const byte *input, size_t length, byte *output, size_t *outputLength) {
    *outputLength = 0;
    for (size_t i = 0; i < length; i++) {
        output[*outputLength] = input[i] & 0x1F; // Conserver seulement les 5 bits | Keep only the 5 bits
        (*outputLength)++;
    }
}

// 5-bit decoding for perforated tapes | Décodage d'une bande perforée en 5-bit 
void decodeFrom5Bit(const byte *input, size_t length, byte *output, size_t *outputLength) {
    *outputLength = 0;
    for (size_t i = 0; i < length; i++) {
        output[*outputLength] = input[i] & 0x1F; // Décoder les 5 bits
        (*outputLength)++;
    }
}

// Save a punched tape with metadata | Sauvegarder une bande perforée avec métadonnées 
void saveTape(const char *filename, const byte *tape, size_t length, const char *metadata) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Error while writing the punched tape");
        //perror("Erreur lors de l'écriture de la bande perforée"); 
        return;
    }
    fprintf(file, "METADATA: %s\n", metadata);
    for (size_t i = 0; i < length; i++) {
        fprintf(file, "%c", tape[i] + 'A');
    }
    fclose(file);
}

// Rotor pitch simulation with locking pins | Simulation du pas des rotors avec broches de blocage
void stepRotors() {
    if (logFile) {
        fprintf(logFile, "Rotor steps forward: ");
        //fprintf(logFile, "Pas des rotors avant : ");
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
        fprintf(logFile, "Rotor steps after: ");
        //fprintf(logFile, "Pas des rotors après : ");
        for (int i = 0; i < ROT; i++) {
            fprintf(logFile, "%d ", rotor_positions[i]);
        }
        fprintf(logFile, "\n");
    }
}

// // Plugboard substitution | Substitution avec plugboard
byte plugboardSubstitute(byte input) {
    return PLUGBOARD[input];
}

// // Rotor substitution | Substitution via un rotor
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

// // Character processing | Traitement d'un caractère
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
        if (logFile) fprintf(logFile, "Error: ProcessByte is not reversible for %c -> %c -> %c\n", //"Erreur : ProcessByte n'est pas réversible pour %c -> %c -> %c\n"
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

// // Write results to a single file | Écriture des résultats dans un fichier unique
void saveResults(const char *filename, const byte *before, const byte *after, const byte *decrypted, size_t length) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Error while writing the results"); //"Erreur lors de l'écriture des résultats"
        return;
    }

    fprintf(file, "Text before encryption :\n"); //Texte avant chiffrement
    for (size_t i = 0; i < length; i++) {
        fprintf(file, "%c", before[i] + 'A');
    }
    fprintf(file, "\n\n");

    if (after) {
        fprintf(file, "Text after encryption:\n"); //Texte aprés chiffrement
        for (size_t i = 0; i < length; i++) {
            fprintf(file, "%c", after[i] + 'A');
        }
        fprintf(file, "\n\n");
    }

    if (decrypted) {
        fprintf(file, "Text after decryption :\n"); //Texte après déchiffrement
        for (size_t i = 0; i < length; i++) {
            fprintf(file, "%c", decrypted[i] + 'A');
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

int main() {

    // Open Logfile | Ouvrir le fichier de log
    logFile = fopen("machine_actions.log", "w");
    if (!logFile) {
        perror("Erreur lors de l'ouverture du fichier de log");
        exit(EXIT_FAILURE);
    }

    // Initialize default parameters | Initialiser les paramètres par défaut
    initializeDefaults();

    // Load a daily key | Charger une clé quotidienne
    loadKey("daily_key.txt", 0);

    // Load a message-specific key | Charger une clé par message
    loadKey("message_key.txt", 1);

    // Load a plugboard configuration | Charger une configuration de plugboard
    loadPlugboardConfig("plugboard_config.txt");

    // Load an input punched tape | Charger une bande perforée d'entrée
    byte inputTape[MAX_TAPE];
    size_t length = loadTape("input.tape", inputTape);

    // Buffers for output and decryption | Buffers pour sortie et déchiffrement
    byte outputTape[MAX_TAPE];
    byte decryptedTape[MAX_TAPE];

    // Save the initial states of the rotors | Sauvegarder les états initiaux des rotors
    byte initial_positions[ROT];
    byte initial_cores[ROT];
    memcpy(initial_positions, rotor_positions, ROT);
    memcpy(initial_cores, rotor_cores, ROT);

    // Encryption | Chiffrement
    encrypt(inputTape, outputTape, length);

    // Reset rotors before decryption | Réinitialiser les rotors avant le déchiffrement
    resetRotors(initial_positions, initial_cores);

    // Decryption | Déchiffrement
    decrypt(outputTape, decryptedTape, length);

    // Save the results in a file | Sauvegarder les résultats dans un fichier
    saveResults("output.tape", inputTape, outputTape, decryptedTape, length);

    // Print completion message | Afficher un message de fin
    printf("Encryption and decryption completed. Check 'output.tape' for results.\n");

    return 0;
