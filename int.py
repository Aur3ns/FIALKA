import os
import subprocess
import curses

def execute_c_program(mode, input_file=None, output_file=None):
    """
    Exécute le programme C avec des arguments spécifiques.
    """
    command = ["./fi"]
    if mode == "encrypt":
        command.extend(["-e", input_file, output_file])
    elif mode == "decrypt":
        command.extend(["-d", input_file, output_file])
    elif mode == "status":
        command.append("-s")
    elif mode == "configure":
        command.append("-c")

    try:
        result = subprocess.run(command, capture_output=True, text=True, check=True)
        return result.stdout.strip()
    except subprocess.CalledProcessError as e:
        return f"Erreur lors de l'exécution : {e.stderr.strip()}"

def draw_boxed_window(stdscr, content, title=""):
    """
    Affiche le contenu dans une fenêtre avec un encadré.
    La taille de la fenêtre et la pagination sont dynamiques en fonction de la taille du terminal.
    """
    stdscr.clear()
    height, width = stdscr.getmaxyx()

    # Définir la largeur de la fenêtre et le nombre de lignes par page en fonction de la taille du terminal
    box_width = min(width - 4, 80)  # Largeur maximale de la boîte
    box_height = min(height - 6, len(content) + 6)

    start_y = max((height - box_height) // 2, 0)
    start_x = max((width - box_width) // 2, 0)

    # Dessiner la bordure de la boîte
    for i in range(box_height):
        stdscr.addstr(start_y + i, start_x, "│")
        stdscr.addstr(start_y + i, start_x + box_width - 1, "│")
    stdscr.addstr(start_y, start_x, "┌" + "─" * (box_width - 2) + "┐")
    stdscr.addstr(start_y + box_height - 1, start_x, "└" + "─" * (box_width - 2) + "┘")

    if title:
        title_str = f" {title} "
        title_x = start_x + (box_width - len(title_str)) // 2
        stdscr.addstr(start_y, title_x, title_str, curses.A_BOLD)

    # Affichage des lignes pour la page actuelle
    for idx, line in enumerate(content):
        # Si l'index dépasse la taille de l'écran, on ne l'affiche pas
        if start_y + 2 + idx < start_y + box_height - 1:
            truncated_line = line[:box_width - 4]
            # Prévenir le débordement de texte
            if len(truncated_line) > width - start_x - 4:
                truncated_line = truncated_line[:width - start_x - 4]
            stdscr.addstr(start_y + 2 + idx, start_x + 2, truncated_line)

    # Affichage de la pagination si nécessaire
    total_pages = (len(content) // box_height) + (1 if len(content) % box_height != 0 else 0)
    pagination_info = f"Page {1}/{total_pages}"
    stdscr.addstr(height - 3, (width - len(pagination_info)) // 2, pagination_info, curses.A_BOLD)

    stdscr.refresh()
    stdscr.getch()

def encryption_menu(stdscr):
    curses.echo()
    stdscr.clear()
    height, width = stdscr.getmaxyx()

    input_file_msg = "Entrez le chemin du fichier clair :"
    output_file_msg = "Entrez le chemin pour sauvegarder le texte chiffré :"

    stdscr.addstr(height // 2 - 2, (width - len(input_file_msg)) // 2, input_file_msg)
    input_file = stdscr.getstr(height // 2 - 1, (width - len(input_file_msg)) // 2, 50).decode("utf-8")

    stdscr.addstr(height // 2 + 1, (width - len(output_file_msg)) // 2, output_file_msg)
    output_file = stdscr.getstr(height // 2 + 2, (width - len(output_file_msg)) // 2, 50).decode("utf-8")
    curses.noecho()

    stdscr.clear()
    result = execute_c_program("encrypt", input_file, output_file)
    draw_boxed_window(stdscr, [result], title="CHIFFREMENT TERMINÉ")

def decryption_menu(stdscr):
    curses.echo()
    stdscr.clear()
    height, width = stdscr.getmaxyx()

    input_file_msg = "Entrez le chemin du fichier chiffré :"
    output_file_msg = "Entrez le chemin pour sauvegarder le texte déchiffré :"

    stdscr.addstr(height // 2 - 2, (width - len(input_file_msg)) // 2, input_file_msg)
    input_file = stdscr.getstr(height // 2 - 1, (width - len(input_file_msg)) // 2, 50).decode("utf-8")

    stdscr.addstr(height // 2 + 1, (width - len(output_file_msg)) // 2, output_file_msg)
    output_file = stdscr.getstr(height // 2 + 2, (width - len(output_file_msg)) // 2, 50).decode("utf-8")
    curses.noecho()

    stdscr.clear()
    result = execute_c_program("decrypt", input_file, output_file)
    draw_boxed_window(stdscr, [result], title="DÉCHIFFREMENT TERMINÉ")

def config_menu(stdscr):
    """
    Sous-menu pour configurer la machine avec plusieurs options.
    """
    options = [
        "MODIFIER LA CLÉ QUOTIDIENNE",
        "MODIFIER LA CLÉ DE MESSAGE",
        "CONFIGURER LE PLUGBOARD",
        "RETOUR"
    ]
    current_row = 0

    while True:
        stdscr.clear()
        height, width = stdscr.getmaxyx()

        title = "CONFIGURATION DE LA MACHINE"
        stdscr.addstr(2, (width - len(title)) // 2, title, curses.A_BOLD)

        # Afficher les options
        for idx, option in enumerate(options):
            x = (width - len(option) - 4) // 2
            y = idx + 6
            if idx == current_row:
                stdscr.addstr(y, x, f"► {option}", curses.color_pair(1) | curses.A_BOLD)
            else:
                stdscr.addstr(y, x, f"  {option}", curses.color_pair(2))

        stdscr.refresh()
        key = stdscr.getch()

        if key == curses.KEY_UP and current_row > 0:
            current_row -= 1
        elif key == curses.KEY_DOWN and current_row < len(options) - 1:
            current_row += 1
        elif key == curses.KEY_ENTER or key in [10, 13]:
            if current_row == 0:
                result = execute_c_program("configure")  # Simulation pour clé quotidienne
                draw_boxed_window(stdscr, result.splitlines(), title="CLÉ QUOTIDIENNE MODIFIÉE")
            elif current_row == 1:
                result = execute_c_program("configure")  # Simulation pour clé de message
                draw_boxed_window(stdscr, result.splitlines(), title="CLÉ DE MESSAGE MODIFIÉE")
            elif current_row == 2:
                result = execute_c_program("configure")  # Simulation pour plugboard
                draw_boxed_window(stdscr, result.splitlines(), title="PLUGBOARD CONFIGURÉ")
            elif current_row == 3:
                break  # Retourner au menu principal

def status_menu(stdscr):
    """
    Sous-menu pour afficher l'état actuel de la machine.
    """
    stdscr.clear()
    height, width = stdscr.getmaxyx()

    result = execute_c_program("status")
    draw_boxed_window(stdscr, result.splitlines(), title="ÉTAT ACTUEL DE LA MACHINE")

def draw_menu(stdscr):
    curses.curs_set(0)
    curses.start_color()
    curses.init_pair(1, curses.COLOR_GREEN, curses.COLOR_BLACK)
    curses.init_pair(2, curses.COLOR_WHITE, curses.COLOR_BLACK)

    menu = [
        "CHIFFRER UN TEXTE",
        "DÉCHIFFRER UN TEXTE",
        "CONFIGURER LA MACHINE",
        "ÉTAT ACTUEL DE LA MACHINE",
        "QUITTER"
    ]
    current_row = 0

    while True:
        stdscr.clear()
        height, width = stdscr.getmaxyx()

        header = [
            "############################################################",
            "##                                                        ##",
            "##        Фиалка: СИМУЛЯТОР КРИПТОГРАФИЧЕСКОЙ МАШИНЫ      ##",
            "##                                                        ##"
        ]
        for idx, line in enumerate(header):
            stdscr.addstr(idx, (width - len(line)) // 2, line, curses.A_BOLD)

        for idx, row in enumerate(menu):
            x = (width - len(row) - 4) // 2
            y = idx + 6
            if idx == current_row:
                stdscr.addstr(y, x, f"► {row}", curses.color_pair(1) | curses.A_BOLD)
            else:
                stdscr.addstr(y, x, f"  {row}", curses.color_pair(2))

        footer = [
            "############################################################",
            ">> UTILISEZ LES FLÈCHES POUR NAVIGUER ET APPUYEZ SUR ENTRÉE"
        ]
        for idx, line in enumerate(footer):
            stdscr.addstr(height - len(footer) + idx - 1, (width - len(line)) // 2, line, curses.A_BOLD)

        stdscr.refresh()
        key = stdscr.getch()

        if key == curses.KEY_UP and current_row > 0:
            current_row -= 1
        elif key == curses.KEY_DOWN and current_row < len(menu) - 1:
            current_row += 1
        elif key == curses.KEY_ENTER or key in [10, 13]:
            if current_row == 0:
                encryption_menu(stdscr)
            elif current_row == 1:
                decryption_menu(stdscr)
            elif current_row == 2:
                config_menu(stdscr)
            elif current_row == 3:
                status_menu(stdscr)
            elif current_row == 4:
                break  # Quitter l'application

def main():
    curses.wrapper(draw_menu)

if __name__ == "__main__":
    main()
