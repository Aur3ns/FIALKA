# Fialka: Simulateur de Machine de Chiffrement

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
9. [Détails des Fonctionnalités](#détails-des-fonctionnalités)
10. [Conclusions et Perspectives](#conclusions-et-perspectives)

---

## Introduction

Le projet **Fialka** simule le fonctionnement d'une machine de chiffrement complexe utilisée par l'Union Soviétique pendant la guerre froide. Conçue pour sécuriser les communications sensibles, cette machine à rotors utilise des principes cryptographiques similaires à ceux de la machine **Enigma**, mais avec des caractéristiques plus avancées et complexes, telles que des configurations dynamiques des rotors et un système de **plugboard** pour les substitutions de caractères.

Le but du projet est de fournir une compréhension de cette machine en implémentant ses mécanismes de chiffrement et de déchiffrement dans un programme moderne. Ce simulateur permet à l'utilisateur de configurer la machine, de chiffrer et déchiffrer des messages, et de visualiser l'état de la machine.

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

## Histoire de la Machine Fialka

### Origines et Contexte Historique

La machine **Fialka** (M-125) a été développée dans les années 1950 par l'Union Soviétique, pendant la guerre froide, pour sécuriser les communications militaires et diplomatiques. Elle était destinée à remplacer des systèmes de chiffrement plus anciens et moins sécurisés. La machine Fialka fait partie des **machines à rotors**, qui utilisent des cycles de permutations complexes pour chiffrer et déchiffrer des messages. Contrairement à des machines comme l'Enigma, qui étaient basées sur un nombre relativement restreint de configurations possibles, la Fialka intègre des composants supplémentaires permettant de sécuriser encore davantage les communications.

### Comparaison avec la Machine Enigma

La machine **Enigma** a été utilisée par l'Allemagne nazie pendant la Seconde Guerre mondiale et repose sur un mécanisme de **rotors** pour permuter les lettres de l'alphabet. Bien que l'Enigma ait été brisée par les Alliés pendant la guerre, elle était à l'époque considérée comme inviolable.

La machine Fialka, tout comme l'Enigma, utilise des **rotors** pour permuter les lettres, mais elle offre plusieurs améliorations :
- Un **plugboard** (ou tableau de connexions) qui permet d'effectuer des substitutions de lettres avant même qu'elles ne passent par les rotors.
- Un **réflecteur** qui redirige les signaux à travers les rotors dans une direction différente, rendant le chiffrement plus complexe.
- Des rotors à deux faces, qui peuvent être tournés indépendamment, augmentant ainsi le nombre de configurations possibles.

### Les Principes de Conception de la Fialka

La machine Fialka repose sur un système complexe de **rotors**, chacun capable de permuter les lettres en fonction de sa position et de sa face active. Chaque rotor contient une **permutation** unique de l'alphabet (30 caractères dans le cas de la Fialka), et la position du rotor détermine l'ordre des lettres qu'il va permuter. Les rotors sont tournés après chaque lettre chiffrée, et le plugboard permet d'introduire une couche supplémentaire de substitution.

## Théorie et Mathématiques Derrière le Chiffrement

### Les Rotors et leur Fonctionnement Mathématique

Les **rotors** de la Fialka effectuent des **permutations** sur les lettres du message. Chaque rotor possède une table de permutation qui spécifie comment chaque lettre de l'alphabet doit être substituée. Lorsque le signal passe à travers un rotor, il est modifié en fonction de la permutation correspondante. Le rotor tourne après chaque lettre traitée, ce qui modifie la permutation et rend le chiffrement plus difficile à déchiffrer sans connaître la configuration exacte de la machine.

### La Substitution et le Plugboard

Le **plugboard** fonctionne comme un mécanisme de substitution avant même que le signal n'entre dans les rotors. Il permet d'échanger certaines lettres de l'alphabet selon une configuration spécifiée. Cela ajoute une couche de sécurité supplémentaire, car même si un rotor est connu, la substitution effectuée par le plugboard rend l'attaque plus complexe.

### Le Réflecteur et les Permutations

Le **réflecteur** est une pièce clé de la machine Fialka. Après que le signal ait traversé les rotors, il est redirigé par le réflecteur vers les rotors dans l'autre direction. Ce mécanisme crée une boucle et renforce la sécurité en augmentant le nombre de transformations qu'un caractère subit. Les réflecteurs sont configurables et peuvent être choisis en fonction de la situation.

### La Sécurité et Réversibilité des Rotors

L'un des aspects les plus intéressants du fonctionnement des rotors dans la Fialka est la **réversibilité** du chiffrement. Les rotors sont conçus de manière à ce que chaque permutation puisse être inversée, ce qui permet au même mécanisme de réaliser à la fois le chiffrement et le déchiffrement. Cela signifie qu'une fois le message chiffré, on peut le déchiffrer en appliquant la même série de transformations mais dans l'ordre inverse.

### L'Algorithme de Chiffrement Global

Le processus global de chiffrement repose sur plusieurs étapes :
1. Le signal passe d'abord par le **plugboard** pour une substitution initiale.
2. Ensuite, il traverse les **rotors**, qui modifient sa valeur en fonction de leur position et de leur permutation.
3. Le signal est ensuite renvoyé par le **réflecteur**, ce qui augmente la complexité de la transformation.
4. Enfin, le signal traverse les rotors dans l'autre direction avant de sortir du système.

Le chiffrement est ainsi basé sur un enchaînement de transformations permutatives qui rendent le processus extrêmement difficile à casser sans la clé exacte.

## Structure du Code

Le code implémente la simulation de la machine Fialka en suivant les principes cryptographiques décrits ci-dessus. Il utilise des tables de permutations pour les rotors, gère le mécanisme du plugboard et du réflecteur, et permet à l'utilisateur de configurer la machine selon ses besoins. Le programme simule également le pas des rotors et permet de chiffrer et déchiffrer des messages en utilisant une clé spécifique.

## Installation et Configuration

### Prérequis

- Un compilateur C (par exemple, GCC).
- Les fichiers nécessaires à la configuration de la machine, tels que les clés et les configurations de plugboard.

### Installation

1. Clonez le projet :
   ```bash
   git clone https://github.com/username/fialka-simulator.git
   ```
2. Ouvrez le repo :
   ```bash
   cd FIALKA
   ```

3. Compilez le programme :
   ```bash
   gcc -o fialka fialka.c
   ```bash

4. Exécutez le programme :
   ```bash
   python int.py
   ```
   
## Utilisation du Programme
L'utilisateur peut naviguer à travers différents menus :

- **Chiffrer un texte** : Chiffre un message en utilisant la machine.
- **Déchiffrer un texte** : Déchiffre un message en utilisant la machine.
- **Configurer la machine** : Modifie les paramètres de la machine (rotors, plugboard, réflecteur).
- **État actuel de la machine** : Affiche l'état actuel de la machine, y compris les positions des rotors et les configurations.
- **Quitter** : Ferme le programme.

## Fichiers Créés Par Défaut en Cas de Non-Détection

Dans le cas où certains fichiers nécessaires ne sont pas trouvés dans le répertoire de travail, le programme **génère automatiquement** des fichiers par défaut pour garantir que les opérations puissent toujours être effectuées. Cela permet de s'assurer que l'utilisateur peut tester la machine même s'il manque des fichiers de configuration.

Voici les fichiers générés par défaut :

### 1. **`input.tape` (Bande perforée d'entrée par défaut)**

Si le fichier **`input.tape`** n'est pas trouvé, le programme crée une bande perforée d'entrée avec un texte par défaut. Ce texte est codé en **5 bits** pour être compatible avec le système de la machine Fialka. Le texte par défaut utilisé est :  
**`HELLOFIALKA`**  
Ce texte est ensuite converti en indices correspondant à chaque lettre (de A à Z) pour être traité par la machine. La bande perforée est utilisée pour simuler le processus de chiffrement et de déchiffrement.

Exemple du fichier `input.tape` créé :
```yaml
METADATA: Bande par défaut générée
HELLOFIALKA
```

### 2. **`output.tape` (Résultats du chiffrement et déchiffrement)**

Le fichier **`output.tape`** est automatiquement créé après que le programme ait effectué les opérations de chiffrement et de déchiffrement. Il contient les résultats de ces opérations, ainsi que les textes avant et après chiffrement et déchiffrement.  
Le fichier est sauvegardé au format texte, et chaque lettre est représentée par un caractère correspondant à son index dans l'alphabet Fialka (de 0 à 29 pour l'alphabet étendu de la machine).

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

---

### Pourquoi ces fichiers sont générés ?

Le but de cette fonctionnalité est de garantir une expérience utilisateur fluide, même si certains fichiers de configuration sont manquants ou corrompus. Les fichiers par défaut permettent de s'assurer que les utilisateurs peuvent toujours tester et utiliser la machine Fialka sans avoir à configurer manuellement tous les paramètres.

---

Cela vous assure que le programme est **autonome** dans sa capacité à fonctionner, en générant les fichiers nécessaires à son bon fonctionnement et en permettant à l'utilisateur de tester la machine sans être dépendant de la présence de fichiers de configuration externes.

---

### Explication des fichiers générés :

- **`input.tape`** : Fichier de bande perforée d'entrée utilisé pour tester le chiffrement et déchiffrement.
- **`output.tape`** : Résultats du chiffrement et déchiffrement.
- **`logfile.log`** : Contient les logs détaillés de toutes les étapes effectuées pendant le chiffrement.
- **Clés de chiffrement (`daily_key.txt`, `message_key.txt`)** : Fournissent des clés de chiffrement générées automatiquement si elles ne sont pas présentes.

Cela permet d'offrir une expérience utilisateur simplifiée et garantit que la machine Fialka peut être utilisée immédiatement, même sans configuration préalable.
