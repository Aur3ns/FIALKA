<div style="text-align: center;">
    <h1 style="font-size: 3em; font-weight: bold;">Fialka M-125 | ФИАЛКА M-125</h1>
</div>

![Machine Fialka](https://www.cryptomuseum.com/crypto/fialka/m125/img/302790/003/full.jpg)

## Table of Contents

1. [Introduction](#introduction)
2. [Project Objectives](#project-objectives)
3. [Features](#features)
4. [History of the Fialka Machine](#history-of-the-fialka-machine)
    - [Origins and Historical Context](#origins-and-historical-context)
    - [Comparison with the Enigma Machine](#comparison-with-the-enigma-machine)
    - [Design Principles of the Fialka](#design-principles-of-the-fialka)
5. [Theory and Mathematics Behind Encryption](#theory-and-mathematics-behind-encryption)
    - [Rotors and Their Mathematical Functioning](#rotors-and-their-mathematical-functioning)
    - [Substitution and the Plugboard](#substitution-and-the-plugboard)
    - [Reflector and Permutations](#reflector-and-permutations)
    - [Rotor Security and Reversibility](#rotor-security-and-reversibility)
    - [Overall Encryption Algorithm](#overall-encryption-algorithm)
6. [Code Structure](#code-structure)
7. [Installation and Configuration](#installation-and-configuration)
8. [Using the Program](#using-the-program)
9. [Default Files Created When Not Detected](#default-files-created-when-not-detected)

---

# Introduction

The Fialka project simulates the workings of a complex encryption machine used by the Soviet Union during the Cold War. 
Designed to secure sensitive communications, this rotor-based machine employs cryptographic principles similar to those 
of the Enigma Machine, but with more advanced and intricate features such as dynamic rotor configurations and a plugboard 
system for character substitutions.

The goal of the project is to provide an understanding of this machine by implementing its encryption and decryption mechanisms in a
modern program. This simulator allows users to configure the machine, encrypt and decrypt messages, and visualize the machine's state.

I was inspired by the following resources to attempt creating a replica of the real machine:

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



## Project Objectives
 - **Simulate the Fialka Machine**: Reproduce the workings of rotors, plugboard, reflector, and character substitution to encrypt and decrypt messages.
     Provide an Intuitive User Interface: Offer an interactive menu that allows users to configure the machine, encrypt and decrypt messages, and visualize       
     the machine's state.
 - **Highlight Cryptographic Principles**: Demonstrate how permutations and substitutions can secure a message.
 - **Offer an Analysis Tool**: Allow users to observe the machine's internal state, test different configurations, and save results to files.
     Features
 - **Encryption and Decryption**: The machine can encrypt and decrypt messages using a specific encryption key.
 - **Dynamic Configuration**: Users can adjust rotor positions, plugboard settings, and choose an active reflector.
 - **Machine State Visualization**: The current state of the machine (rotor positions, plugboard settings, and reflector) can be displayed at any time.
 - **Result Saving**: The program allows saving texts before and after encryption and decryption, as well as the configurations used, to files.

## History of the Fialka Machine

### Origins and Historical Context

 - The M-125, code-named Fialka (Russian: ФИАЛКА), was an electromechanical wheel cipher machine developed in the USSR shortly after World War II.      
   Introduced in 1956, it replaced the Albatross, a Soviet cipher machine that was itself more complex than the Enigma. It quickly became a favorite among   
   the Warsaw Pact and some allied countries, such as Cuba. However, the existence of the Fialka remained highly secretive. Russia did not declassify      
   information about this machine until 2005.

 - Like the Enigma, it was an electromechanical cipher machine working with wheels. Its keyboard resembled that of a typewriter, but its body looked more    
   like a highly sophisticated calculator, equipped with a series of rotors that replaced each letter with another as the message was typed. Primarily used 
   by the military, the Fialka was such a closely guarded secret that trained operators were reportedly required to sign special contracts barring them from 
   traveling abroad for two years!

 - The machine featured 10 encryption rotors, each with 30 contacts on each side. Adjacent rotors moved in opposite directions. Additionally, the machine     
   included a card reader, in which a new punched key card was installed daily, along with a printer, a tape reader, and a tape puncher.

 - Each Warsaw Pact country had its own customized version of the Fialka, tailored to the local language, with a unique keyboard and printing head. 
   Moreover, the internal wiring of the wheels was also different for each country.

### Comparison with the Enigma Machine

 - The **Enigma Machine** was used by Nazi Germany during World War II and relied on a rotor mechanism to permute letters of the alphabet. 
   Although the Enigma was broken by the Allies during the war, it was then considered unbreakable. The Fialka addressed the shortcomings of the Enigma: it      
   operated with ten-letter rotors, compared to three or four in the Enigma, making encryption on the Russian machine more secure. Each rotation of the     
   rotors enabled the Fialka to encrypt each letter individually, creating over 500 trillion codes. 
   According to Stephen Budiansky, author of Code Warriors, who studied the work of the National Security Agency (NSA) on breaking Soviet codes, the     
   encryption was so advanced it could only be cracked by human error, theft, or betrayal. Significant advances made by the United States and the USSR in    
   breaking their adversary’s secret codes during the Cold War were achieved through “direct” means (such as stealing key lists) or due to procedural errors   
   revealing critical details about the internal encryption patterns.

### Design Principles of the Fialka
 - The Fialka machine relies on a complex system of rotors, each capable of permuting letters based on its position and active side. Each rotor contains a      
   unique permutation of the alphabet (30 characters in the case of the Fialka), and the rotor's position determines the order of letters it permutes. The 
   rotors advance after each encrypted letter, while the plugboard adds an additional layer of substitution.

# Theory and Mathematics Behind Encryption

### Rotors and Their Mathematical Functioning

 - The rotors in the Fialka perform permutations on the message's letters. Each rotor has a permutation table specifying how each letter of the alphabet is 
   substituted. When a signal passes through a rotor, it is modified according to the corresponding permutation. The rotor advances after processing each   
   letter, which alters the permutation and makes decryption harder without knowing the machine's exact configuration.
   Substitution and the Plugboard

 - The plugboard acts as a substitution mechanism before the signal enters the rotors. It allows swapping specific letters of the alphabet according to a 
   defined configuration. This adds an extra layer of security since even if a rotor's wiring is known, the plugboard substitutions make attacks more    
   complex.

### Reflector and Permutations

 - The reflector is a critical component of the Fialka. After the signal passes through the rotors, it is redirected by the reflector back through the 
   rotors in the opposite direction. This mechanism creates a loop and enhances security by increasing the number of transformations a character undergoes.    
   Reflectors can be configured and chosen based on operational needs.

### Rotor Security and Reversibility

 - One of the most interesting aspects of the Fialka's rotors is the reversibility of encryption. The rotors are designed so that every permutation can be      
   reversed, enabling the same mechanism to perform both encryption and decryption. This means that once a message is encrypted, it can be decrypted by 
   applying the same transformations in reverse order.

### Overall Encryption Algorithm
The overall encryption process consists of several steps:

 - The signal first passes through the plugboard for an initial substitution.
    1. It then goes through the rotors, which modify its value based on their positions and permutations.
    2. The signal is redirected by the reflector, increasing the complexity of the transformation.
    3. Finally, the signal passes back through the rotors in the reverse direction before exiting the system.

 - This sequence of permutative transformations makes the encryption extremely difficult to break without the exact key.

### Code Structure
 - The code implements the Fialka machine simulation by following the cryptographic principles outlined above. It uses permutation tables for the rotors,     
   manages the plugboard and reflector mechanism, and allows users to configure the machine as needed. The program also simulates rotor stepping and enables 
   encryption and decryption using a specific key.

# Installation and Configuration

### Prerequisites
 - A C compiler (e.g., GCC).

 - Necessary configuration files for the machine, such as keys and plugboard settings.

### Installation

1. Clone the project :
   ```bash
   git clone https://github.com/BlackBird63030/FIALKA.git 
   ```
2. Open the repository :
   ```bash
   cd FIALKA
   ```
3. Compile the program :

   ```bash
   gcc -o fi fi.c
   ```
4. Type what you want to encrypt in the file input.tape.
   
5. Configure the keys and the plugboard as you wish.

6. Compile the program :
   ```bash
   gcc -o fi fi.c
   ```

7. Compile the program :
   ```bash
   ./fi
   ```

🚨NOTE: The fialka.c file is an implementation from the Slovak University of Technology's Institute of Informatics and Mathematics. This implementation is not functional, but the original article source is available in the links above.


## Default Files Created When Not Detected

 - If certain necessary files are missing from the working directory, the program automatically generates default files to ensure operations can still be performed. This feature ensures that users can test the machine even in the absence of configuration files.

**1.** *input.tape* (Default Input Tape)
 - If the input.tape file is not found, the program creates a default input tape with predefined text. This text is encoded in 5-bit format for 
   compatibility with the Fialka machine. The default text used is: HELLOFIALKA

 - The text is then converted into indices corresponding to each letter (from A to Z) for processing by the machine. The tape simulates the encryption and      
   decryption process.

Example of generated input.tape file:
```yaml
METADATA: Bande par défaut générée
HELLOFIALKA
```

**2.** *output.tape* (Encryption and Decryption Results)
 - The output.tape file is automatically generated after the program completes encryption and decryption. 
 - It contains the results of these operations, along with the text before and after encryption and decryption.

Example of generated output.tape file:
```yaml
Texte avant chiffrement : HELLOFIALKA

Texte après chiffrement : (Texte chiffré)

Texte après déchiffrement : HELLOFIALKA
```

**3.** *logfile.log* (Log File)
 - If no log file is found or the specified log file cannot be opened, the program generates a log file named logfile.log to record all actions performed by 
   the machine. 
 - This file includes detailed information about the encryption process, letter permutation steps, and rotor positions after each operation.

**4.** *daily_key.txt* and *message_key.txt* (Default Encryption Keys)
 - If the files daily_key.txt or message_key.txt are missing, default key files are generated to allow users to configure the machine. These keys contain        
   information about rotor order, positions, and configurations for encryption. The files ensure that the machine operates without requiring manual key 
   configuration.

Example content of the generated daily_key.txt file:

```yaml
0 5 1 0 1 10 0 0 2 15 1 1 3 20 0 0 ...
```
Each line contains the following information for each rotor :

 - **Rotor Order**
 - **Rotor Position**
 - **Rotor Face (0 or 1)**
 - **Rotor Fixity (0 or 1, where 0 means the rotor can turn, and 1 means it is fixed)**

These files allow users to configure the machine and perform encryption/decryption processes without external configuration files.

---

### Why are these files generated?

 - The purpose of this feature is to ensure a smooth user experience, even if some configuration files are missing or corrupted. The default files ensure that users can always test and use the Fialka machine without having to manually configure all the parameters.
 - This ensures that the program is **autonomous** in its ability to function, generating the files necessary for its proper operation and allowing the user to test the machine without being dependent on the presence of external configuration files.

---

### Explanation of generated files:

- **`input.tape`** : Input punch tape file used to test encryption and decryption.
- **`output.tape`** : Results of encryption and decryption.
- **`logfile.log`**: Contains detailed logs of all steps performed during encryption.
- Encryption keys (`daily_key.txt`, `message_key.txt`)**: Provide automatically generated encryption keys if they are not present.

This simplifies the user experience and ensures that the Fialka machine can be used immediately, 
even without prior configuration.
