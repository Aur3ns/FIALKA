<div style="text-align: center;">
    <h1 style="font-size: 3em; font-weight: bold;">Fialka M-125 | ФИАЛКА M-125</h1>
</div>

![Machine Fialka](https://www.cryptomuseum.com/crypto/fialka/m125/img/302790/003/full.jpg)
*Machine Fialka (M-125) utilisée pour le chiffrement des communications pendant la guerre froide.*

## Table des matières

1. [Introduction](#introduction)
2. [Objectifs du Projet](#objectifs-du-projet)
3. [Fonctionnalités](#fonctionnalités)
4. [Histoire de la Machine Fialka](#histoire-de-la-machine-fialka)
    - [Origines et Contexte Historique](#origines-et-contexte-historique)
    - [Comparaison avec la Machine Enigma](#comparaison-avec-la-machine-enigma)
    - [Les Principes de Conception de la Fialka](#les-principes-de-conception-de-la-fialka)
5. [Théorie et Mathématiques Derrière le Chiffrement](#théorie-et-mathématiques-derrière-le-chiffrement)
    - [Les Rotors et leur Fonctionnement Mathématique](#les-rotors-et-leur-fonctionnement-mathématique)
    - [La Substitution et le Plugboard](#la-substitution-et-le-plugboard)
    - [Le Réflecteur et les Permutations](#le-réflecteur-et-les-permutations)
    - [La Sécurité et Réversibilité des Rotors](#la-sécurité-et-réversibilité-des-rotors)
    - [L'Algorithme de Chiffrement Global](#lalgorithme-de-chiffrement-global)
6. [Structure du Code](#structure-du-code)
7. [Installation et Configuration](#installation-et-configuration)
8. [Utilisation du Programme](#utilisation-du-programme)
9. [Fichiers créés par défaut en cas de non-détection](#fichiers-créés-par-défaut-en-cas-de-non-détection)

---

# Introduction

Le projet **Fialka** simule le fonctionnement d'une machine de chiffrement complexe utilisée par l'Union Soviétique pendant la guerre froide. Conçue pour sécuriser les communications sensibles, cette machine à rotors utilise des principes cryptographiques similaires à ceux de la machine **Enigma**, mais avec des caractéristiques plus avancées et complexes, telles que des configurations dynamiques des rotors et un système de **plugboard** pour les substitutions de caractères.

Le but du projet est de fournir une compréhension de cette machine en implémentant ses mécanismes de chiffrement et de déchiffrement dans un programme moderne. Ce simulateur permet à l'utilisateur de configurer la machine, de chiffrer et déchiffrer des messages, et de visualiser l'état de la machine. 

Je me suis inspiré de ces fichiers pour pouvoir tenter de créer un réplique de la vraie machine :

| https://www.cryptomuseum.com/pub/files/Fialka_200.pdf                                        |
|----------------------------------------------------------------------------------------------|
| FIALKA M-125, Detailed description of the Russian Fialka cipher machines, by Paul Reuvers & Marc Simons                                                                                                                            

| https://www.slate.fr/story/178587/fialka-machine-cryptage-services-secrets-sovietiques-secret|
|----------------------------------------------------------------------------------------------|
| L'histoire cachée de la Fialka, machine à chiffrer du KGB                                    |

| https://enigmamuseum.com/mfman.pdf                                                           |
|----------------------------------------------------------------------------------------------|
| RUSSIAN FIALKA M-125, Cipher Machine Manual, (German Language Version), 1978                 |

| https://mysterytwister.org/media/challenges/pdf/mtc3-madness-06-fialka-01-en.pdf             |                                      
|----------------------------------------------------------------------------------------------|
|                The Fialka Challenge by madness                                               |

## Objectifs du Projet

- **Simuler la Machine Fialka** : Reproduire le fonctionnement des rotors, du plugboard, du réflecteur et de la substitution des caractères pour chiffrer et déchiffrer des messages.
- **Offrir une Interface Utilisateur Intuitive** : Fournir un menu interactif permettant à l'utilisateur de configurer la machine, de chiffrer et de déchiffrer des messages, et de visualiser l'état de la machine.
- **Mettre en valeur les principes cryptographiques** : Démontrer comment les permutations et les substitutions peuvent être utilisées pour sécuriser un message.
- **Proposer un Outil d'Analyse** : Permettre aux utilisateurs d'observer l'état interne de la machine, de tester différentes configurations et de sauvegarder les résultats dans des fichiers.

## Fonctionnalités

- **Chiffrement et Déchiffrement** : La machine permet de chiffrer et de déchiffrer des messages en utilisant une clé de chiffrement spécifique.
- **Configuration Dynamique** : L'utilisateur peut ajuster les positions des rotors, la configuration du plugboard et choisir un réflecteur actif.
- **Visualisation de l'État de la Machine** : L'état actuel de la machine (positions des rotors, configuration du plugboard et réflecteur) peut être affiché à tout moment.
- **Sauvegarde des Résultats** : Le programme permet de sauvegarder les textes avant et après chiffrement et déchiffrement, ainsi que les configurations utilisées dans des fichiers.

# Histoire de la Machine Fialka

### Origines et Contexte Historique

- Le M-125, nom de code **Fialka (russe : ФИАЛКА)**, était une machine de chiffrement électromécanique à roue, **développée en URSS peu après la Seconde Guerre mondiale**.     Introduit pour la première fois en 1956, elle remplaça **l'Albatross**, une machine de chiffrement soviétique qui était elle-même plus complexe que l'Enigma. elle est 
   rapidement devenu une des machines préférées du Pacte de Varsovie et de certains pays alliés, comme Cuba. Pourtant, l'existence de la Fialka demeura des plus secrètes. **La Russie ne déclassifia pas l'information à propos de cette machine avant 2005**. Les méthodes de chiffrement de la Fialka étaient avancées, mais la technologie de base était ancienne. 
 - À l'instar de l'Enigma, c'était une machine de chiffrement électromécanique fonctionnant avec des rouleaux. Son clavier ressemblait à celui d'une machine à écrire, mais     son corps avait plus l'aspect d'une machine à calculer très perfectionnée, équipée d'une série de rotors qui remplaçaient toute lettre par une autre à mesure que le         message était tapé. Essentiellement utilisée par les militaires, la Fialka était un secret si jalousement gardé que la soldatesque formée pour l'utiliser aurait été         obligée de signer des contrats spéciaux précisant que ses membres ne devaient pas voyager à l'étranger pendant deux ans !

- La machine a **10 rotors de chiffrement**, chacun avec **30 contacts de chaque côté**. Les rotors adjacents se déplacent dans des directions opposées. De plus, la machine     dispose d’un lecteur de carte dans lequel une nouvelle carte-clé perforée était installée quotidiennement, ainsi qu'une imprimante, un lecteur de bande et un                perforateur de bande.

- Chaque pays du Pacte de Varsovie disposait de sa propre version personnalisée de la Fialka, adaptée à la langue locale, avec son propre clavier et sa tête d’impression         spécifique. Cependant, le câblage interne des roues était également distinct pour chaque pays.

### Comparaison avec la Machine Enigma

 - La machine **Enigma** a été utilisée par l'Allemagne nazie pendant la Seconde Guerre mondiale et repose sur un mécanisme de **rotors** pour permuter les lettres de           l'alphabet. Bien que l'Enigma ait été brisée par les Alliés pendant la guerre, elle était à l'époque considérée comme inviolable. La Fialka comblait les lacunes de          l'Enigma: comme elle fonctionnait avec dix rouleaux de lettres, contre trois ou quatre pour l'Enigma, le chiffrement sur la machine russe était mieux sécurisé. Chaque       rotation des rouleaux permettait à la Fialka de chiffrer chaque lettre individuellement. La machine pouvait produire plus de 500 billions de codes. Selon l'auteur de        Code Warriors, Stephen Budiansky, qui s'est intéressé aux travaux de **la National Security Agency (NSA**) pour déchiffrer les codes secrets soviétiques, le chiffrement     était si perfectionné qu'il n'aurait pu être découvert que par **erreur humaine, par vol ou par trahison.** Les avancées importantes que les États-Unis et l'URSS ont        faites, tout au long de la guerre froide, dans le déchiffrement des codes secrets de leur ennemi ont été obtenues soit par des moyens “directs” (comme le vol de listes      clés de combinaisons) ou à cause d'erreurs dans les procédures ayant éventé des détails essentiels sur les schémas de chiffrement internes du système de chiffrement.»

- La machine Fialka, tout comme l'Enigma, utilise des **rotors** pour permuter les lettres, mais elle offre plusieurs améliorations :
    - Un **plugboard** (ou tableau de connexions) qui permet d'effectuer des substitutions de lettres avant même qu'elles ne passent par les rotors.
    - Un **réflecteur** qui redirige les signaux à travers les rotors dans une direction différente, rendant le chiffrement plus complexe.
    - Des rotors à deux faces, qui peuvent être tournés indépendamment, augmentant ainsi le nombre de configurations possibles.

### Les Principes de Conception de la Fialka

La machine Fialka repose sur un système complexe de **rotors**, chacun capable de permuter les lettres en fonction de sa position et de sa face active. Chaque rotor contient une **permutation** unique de l'alphabet (30 caractères dans le cas de la Fialka), et la position du rotor détermine l'ordre des lettres qu'il va permuter. Les rotors sont tournés après chaque lettre chiffrée, et le plugboard permet d'introduire une couche supplémentaire de substitution.

# Théorie et Mathématiques Derrière le Chiffrement

### Les Rotors et leur Fonctionnement Mathématique

 - Les **rotors** de la Fialka effectuent des **permutations** sur les lettres du message. Chaque rotor possède une table de permutation qui spécifie comment chaque lettre     de l'alphabet doit être substituée. Lorsque le signal passe à travers un rotor, il est modifié en fonction de la permutation correspondante. Le rotor tourne après chaque     lettre traitée, ce qui modifie la permutation et rend le chiffrement plus difficile à déchiffrer sans connaître la configuration exacte de la machine.

### La Substitution et le Plugboard

 - Le **plugboard** fonctionne comme un mécanisme de substitution avant même que le signal n'entre dans les rotors. Il permet d'échanger certaines lettres de l'alphabet        selon une configuration spécifiée. Cela ajoute une couche de sécurité supplémentaire, car même si un rotor est connu, la substitution effectuée par le plugboard rend        l'attaque plus complexe.

### Le Réflecteur et les Permutations

 - Le **réflecteur** est une pièce clé de la machine Fialka. Après que le signal ait traversé les rotors, il est redirigé par le réflecteur vers les rotors dans l'autre         direction. Ce mécanisme crée une boucle et renforce la sécurité en augmentant le nombre de transformations qu'un caractère subit. Les réflecteurs sont configurables et     peuvent être choisis en fonction de la situation.

### La Sécurité et Réversibilité des Rotors

 - L'un des aspects les plus intéressants du fonctionnement des rotors dans la Fialka est la **réversibilité** du chiffrement. Les rotors sont conçus de manière à ce que         chaque permutation puisse être inversée, ce qui permet au même mécanisme de réaliser à la fois le chiffrement et le déchiffrement. Cela signifie qu'une fois le message     chiffré, on peut le déchiffrer en appliquant la même série de transformations mais dans l'ordre inverse.

### L'Algorithme de Chiffrement Global

 - Le processus global de chiffrement repose sur plusieurs étapes :
    1. Le signal passe d'abord par le **plugboard** pour une substitution initiale.
    2. Ensuite, il traverse les **rotors**, qui modifient sa valeur en fonction de leur position et de leur permutation.
    3. Le signal est ensuite renvoyé par le **réflecteur**, ce qui augmente la complexité de la transformation.
    4. Enfin, le signal traverse les rotors dans l'autre direction avant de sortir du système.

 - Le chiffrement est ainsi basé sur un enchaînement de transformations permutatives qui rendent le processus extrêmement difficile à casser sans la clé exacte.

## Structure du Code

 - Le code implémente la simulation de la machine Fialka en suivant les principes cryptographiques décrits ci-dessus. Il utilise des tables de permutations pour les rotors,     gère le mécanisme du plugboard et du réflecteur, et permet à l'utilisateur de configurer la machine selon ses besoins. Le programme simule également le pas des rotors         et permet de chiffrer et déchiffrer des messages en utilisant une clé spécifique.

# Installation et Configuration

### Prérequis

- Un compilateur C (par exemple, GCC).
- Les fichiers nécessaires à la configuration de la machine, tels que les clés et les configurations de plugboard.

### Installation

1. Clonez le projet :
   ```bash
   git clone https://github.com/Aur3ns/FIALKA.git 
   ```
2. Ouvrez le repo :
   ```bash
   cd FIALKA
   ```
3. Tapez ce que vous souhaitez chiffrer dans le fichier *input.tape*
   
4. Configurez les clés et le plugboard a votre guise

5. Compilez le programme :
   ```bash
   gcc -o fi fi.c
   ```

6. Exécutez le programme :
   ```bash
   ./fi
   ```
🚨ATTENTION : le fichier fialka.c est l'implémentation de l'institut d'informatique et de Mathématiques de l'Université de Technologie de Slovaquie. Cette implémentation ne fonctionne pas, mais l'article source est disponible dans les liens plus haut.
   
## Fichiers Créés Par Défaut en Cas de Non-Détection

Dans le cas où certains fichiers nécessaires ne sont pas trouvés dans le répertoire de travail, le programme **génère automatiquement** des fichiers par défaut pour garantir que les opérations puissent toujours être effectuées. Cela permet de s'assurer que l'utilisateur peut tester la machine même s'il manque des fichiers de configuration.

Voici les fichiers générés par défaut :

### 1. **`input.tape` (Bande perforée d'entrée par défaut)**

- Si le fichier **`input.tape`** n'est pas trouvé, le programme crée une bande perforée d'entrée avec un texte par défaut. Ce texte     est codé en **5 bits** pour être compatible avec le système de la machine Fialka. Le texte par défaut utilisé est :  
  **`HELLOFIALKA`**  
- Ce texte est ensuite converti en indices correspondant à chaque lettre (de A à Z) pour être traité par la machine. La bande 
  perforée est utilisée pour simuler le processus de chiffrement et de déchiffrement.

Exemple du fichier `input.tape` créé :
```yaml
METADATA: Bande par défaut générée
HELLOFIALKA
```

### 2. **`output.tape` (Résultats du chiffrement et déchiffrement)**

- Le fichier **`output.tape`** est automatiquement créé après que le programme ait effectué les opérations de chiffrement et de 
  déchiffrement. Il contient les résultats de ces opérations, ainsi que les textes avant et après chiffrement et déchiffrement.  
- Le fichier est sauvegardé au format texte, et chaque lettre est représentée par un caractère correspondant à son index dans 
  l'alphabet Fialka (de 0 à 29 pour l'alphabet étendu de la machine).

Exemple du fichier `output.tape` créé après un chiffrement et déchiffrement :

```yaml
Texte avant chiffrement : HELLOFIALKA

Texte après chiffrement : (Texte chiffré)

Texte après déchiffrement : HELLOFIALKA
```

### 3. **`logfile.log` (Fichier de log)**

Si aucun fichier de log n'est trouvé ou si le fichier de log spécifié ne peut pas être ouvert, le programme génère un fichier de log appelé **`logfile.log`** pour enregistrer toutes les actions effectuées par la machine. Ce fichier contient des informations détaillées sur le processus de chiffrement, les étapes de permutation des lettres, ainsi que les positions des rotors et des broches de blocage après chaque opération.

### 4. **`daily_key.txt` et `message_key.txt` (Clés de chiffrement par défaut)**

Si les fichiers de clé **`daily_key.txt`** ou **`message_key.txt`** sont absents, des fichiers de clé par défaut seront générés, permettant à l'utilisateur de configurer la machine. Ces clés contiennent des informations sur l'ordre et la position des rotors, ainsi que leur configuration pour le chiffrement. Ces fichiers sont créés pour permettre à la machine de fonctionner sans nécessiter une configuration de clé manuelle.

Exemple de contenu du fichier `daily_key.txt` généré par défaut :

```yaml
0 5 1 0 1 10 0 0 2 15 1 1 3 20 0 0 ...
```

Chaque ligne contient les informations suivantes pour chaque rotor :
- **Ordre du rotor**
- **Position du rotor**
- **Face du rotor (0 ou 1)**
- **Fixité du rotor (0 ou 1, où 0 signifie que le rotor peut tourner et 1 qu'il est fixé)**

Ces fichiers permettent de configurer la machine et de lancer des processus de chiffrement/déchiffrement sans dépendre de fichiers externes manquants.


## Pourquoi ces fichiers sont générés ?

 - Le but de cette fonctionnalité est de garantir une expérience utilisateur fluide, même si certains fichiers de configuration sont manquants ou corrompus. Les fichiers        par défaut permettent de s'assurer que les utilisateurs peuvent toujours tester et utiliser la machine Fialka sans avoir à configurer manuellement tous les paramètres.
 - Cela vous assure que le programme est **autonome** dans sa capacité à fonctionner, en générant les fichiers nécessaires à son bon fonctionnement et en permettant à          l'utilisateur de tester la machine sans être dépendant de la présence de fichiers de configuration externes.


## Explication des fichiers générés :

- **`input.tape`** : Fichier de bande perforée d'entrée utilisé pour tester le chiffrement et déchiffrement.
- **`output.tape`** : Résultats du chiffrement et déchiffrement.
- **`logfile.log`** : Contient les logs détaillés de toutes les étapes effectuées pendant le chiffrement.
- **Clés de chiffrement (`daily_key.txt`, `message_key.txt`)** : Fournissent des clés de chiffrement générées automatiquement si elles ne sont pas présentes.

Cela permet d'offrir une expérience utilisateur simplifiée et garantit que la machine Fialka peut être utilisée immédiatement, 
même sans configuration préalable.
