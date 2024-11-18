import os
import subprocess
import shutil

# Couleurs pour une touche esthétique soviétique
RED = '\033[91m'
CYAN = '\033[96m'
BOLD = '\033[1m'
RESET = '\033[0m'

# Récupérer la largeur du terminal pour centrer le contenu
def get_terminal_width():
    return shutil.get_terminal_size().columns

def center_text(text):
    term_width = get_terminal_width()
    text_width = len(text)
    if text_width >= term_width:
        return text  # Pas de centrage si le texte dépasse la largeur du terminal
    padding = (term_width - text_width) // 2
    return " " * padding + text

def display_centered(lines):
    for line in lines:
        print(center_text(line))

def clear_screen():
    os.system('cls' if os.name == 'nt' else 'clear')

def main_menu():
    clear_screen()
    lines = [
        f"{RED}{BOLD}===========================================================",
        "███████╗██╗ █████╗ ██╗      █████╗ ██╗  ██╗ █████╗ ",
        "██╔════╝██║██╔══██╗██║     ██╔══██╗██║ ██╔╝██╔══██╗",
        "███████╗██║███████║██║     ███████║█████╔╝ ███████║",
        "╚════██║██║██╔══██║██║     ██╔══██║██╔═██╗ ██╔══██║",
        "███████║██║██║  ██║███████╗██║  ██║██║  ██╗██║  ██║",
        "╚══════╝╚═╝╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝",
        "         ██████╗ ███╗   ███╗██╗   ██╗ █████╗ ",
        "        ██╔═══██╗████╗ ████║██║   ██║██╔══██╗",
        "        ██║   ██║██╔████╔██║██║   ██║███████║",
        "        ██║   ██║██║╚██╔╝██║██║   ██║██╔══██║",
        "        ╚██████╔╝██║ ╚═╝ ██║╚██████╔╝██║  ██║",
        "         ╚═════╝ ╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═╝",
        "-----------------------------------------------------------",
        "          Симулятор Шифровальной Машины ФИАЛКА",
        "             (Simulateur de Machine Fialka)               ",
        "===========================================================",
        "[1] ► CHIFFRER UN TEXTE",
        "[2] ► DÉCHIFFRER UN TEXTE",
        "[3] ► CONFIGURER LA MACHINE",
        "[4] ► ÉTAT ACTUEL DE LA MACHINE",
        "[5] ► QUITTER",
        "===========================================================",
        f"{CYAN}ENTREZ UNE COMMANDE : _{RESET}"
    ]
    display_centered(lines)

def execute_c_program(mode, input_file=None, output_file=None):
    command = ["./fi"]
    if mode == "encrypt":
        command.extend([input_file, output_file])
    elif mode == "decrypt":
        command.extend([input_file, output_file])
    
    try:
        result = subprocess.run(command, capture_output=True, text=True, check=True)
        print(result.stdout)
    except subprocess.CalledProcessError as e:
        print(f"{RED}Erreur lors de l'exécution : {e.stderr}{RESET}")

def encryption_menu():
    clear_screen()
    lines = [
        f"{CYAN}{BOLD}===========================================================",
        "   MODE : CHIFFREMENT DU TEXTE (ЗАШИФРОВКА)              ",
        "===========================================================",
        "[ИНСТРУКЦИЯ] Entrez le chemin du fichier contenant le texte clair : ",
    ]
    display_centered(lines)
    input_file = input("Fichier d'entrée : ")
    output_file = input("Fichier de sortie : ")
    execute_c_program("encrypt", input_file, output_file)

def decryption_menu():
    clear_screen()
    lines = [
        f"{CYAN}{BOLD}===========================================================",
        "   MODE : DÉCHIFFREMENT DU TEXTE (РАСШИФРОВКА)           ",
        "===========================================================",
        "[ИНСТРУКЦИЯ] Entrez le chemin du fichier contenant le texte chiffré : ",
    ]
    display_centered(lines)
    input_file = input("Fichier d'entrée : ")
    output_file = input("Fichier de sortie : ")
    execute_c_program("decrypt", input_file, output_file)

def config_menu():
    clear_screen()
    lines = [
        f"{CYAN}{BOLD}===========================================================",
        "        CONFIGURATION DE LA MACHINE (КОНФИГУРАЦИЯ)        ",
        "===========================================================",
        "[1] ► MODIFIER LA CLÉ QUOTIDIENNE",
        "[2] ► MODIFIER LA CLÉ DE MESSAGE",
        "[3] ► CONFIGURER LE PLUGBOARD",
        "[4] ► RETOUR",
        "===========================================================",
        f"{CYAN}ENTREZ UNE OPTION : _{RESET}"
    ]
    display_centered(lines)
    choice = input().strip()
    if choice == "1":
        print(f"{CYAN}Modification de la clé quotidienne...{RESET}")
    elif choice == "2":
        print(f"{CYAN}Modification de la clé de message...{RESET}")
    elif choice == "3":
        print(f"{CYAN}Configuration du plugboard...{RESET}")
    elif choice == "4":
        return
    else:
        print(f"{RED}Option invalide!{RESET}")

def status_menu():
    clear_screen()
    lines = [
        f"{CYAN}{BOLD}===========================================================",
        "          ÉTAT DE LA MACHINE (СТАТУС МАШИНЫ)              ",
        "===========================================================",
        "ROTORS :",
        " • ROTOR 0 : Position = 6, Ordre = 0, Fixe = Non",
        " • ROTOR 1 : Position = 11, Ordre = 1, Fixe = Non",
        " ...",
        "RÉFLECTEUR ACTIF : 2",
        "PLUGBOARD : ",
        "  A <-> Z  |  B <-> Y  |  C <-> X  |  ...",
        "===========================================================",
        "Appuyez sur Entrée pour revenir au menu principal.",
    ]
    display_centered(lines)
    input()

def main():
    while True:
        main_menu()
        choice = input().strip()
        if choice == "1":
            encryption_menu()
        elif choice == "2":
            decryption_menu()
        elif choice == "3":
            config_menu()
        elif choice == "4":
            status_menu()
        elif choice == "5":
            print(f"{RED}Merci d'avoir utilisé la Machine Fialka!{RESET}")
            break
        else:
            print(f"{RED}Option invalide, essayez à nouveau!{RESET}")

if __name__ == "__main__":
    main()
