import curses
import subprocess


def execute_c_program(args):
    """
    Exécute le programme C avec les arguments spécifiés.
    """
    command = ["./fi"] + args
    try:
        result = subprocess.run(command, capture_output=True, text=True, check=True)
        return result.stdout.strip()
    except subprocess.CalledProcessError as e:
        return f"Erreur lors de l'exécution : {e.stderr.strip()}"


def safe_addstr(stdscr, y, x, string, *args):
    """
    Ajoute du texte à l'écran sans dépasser les limites.
    """
    try:
        stdscr.addstr(y, x, string, *args)
    except curses.error:
        pass  # Ignore les erreurs dues aux dépassements


def check_terminal_size(stdscr):
    """
    Vérifie si la taille du terminal est suffisante pour afficher correctement.
    """
    height, width = stdscr.getmaxyx()
    if height < 15 or width < 40:
        stdscr.clear()
        stdscr.addstr(0, 0, "!!! TERMINAL TROP PETIT !!!", curses.color_pair(1))
        stdscr.addstr(1, 0, "Agrandissez votre terminal et relancez.", curses.color_pair(1))
        stdscr.refresh()
        stdscr.getch()
        return False
    return True


def draw_ascii_border(stdscr, title=""):
    """
    Dessine une bordure ASCII rétro adaptée à la taille actuelle du terminal.
    """
    height, width = stdscr.getmaxyx()
    border_top = "+" + "=" * (width - 2) + "+"
    border_middle = "|" + " " * (width - 2) + "|"

    safe_addstr(stdscr, 0, 0, border_top, curses.color_pair(2))
    for row in range(1, height - 1):
        safe_addstr(stdscr, row, 0, border_middle, curses.color_pair(2))
    safe_addstr(stdscr, height - 1, 0, border_top, curses.color_pair(2))

    if title:
        title_str = f" {title} "
        title_x = (width - len(title_str)) // 2
        safe_addstr(stdscr, 0, title_x, title_str, curses.color_pair(1) | curses.A_BOLD)


def read_file_content(file_path):
    """
    Lit le contenu d'un fichier texte et retourne une chaîne unique.
    """
    try:
        with open(file_path, 'r') as f:
            return "".join(f.readlines()).strip()
    except FileNotFoundError:
        return "Erreur : Fichier introuvable."
    except Exception as e:
        return f"Erreur lors de la lecture : {str(e)}"


def display_comparison(stdscr, before_file, after_file, operation):
    """
    Affiche une comparaison entre le contenu avant et après l'opération.
    """
    before_content = read_file_content(before_file)
    after_content = read_file_content(after_file)

    title = f"RÉSULTAT DU {operation.upper()}"
    content = [
        "=== TEXTE AVANT ===",
        before_content,
        "",
        "=== TEXTE APRÈS ===",
        after_content,
    ]
    display_boxed_window(stdscr, content, title=title)


def display_boxed_window(stdscr, content, title=""):
    """
    Affiche une fenêtre avec une bordure ASCII.
    """
    stdscr.clear()
    height, width = stdscr.getmaxyx()

    # Taille de la fenêtre
    box_width = min(80, width - 4)
    box_height = min(len(content) + 6, height - 4)

    start_y = (height - box_height) // 2
    start_x = (width - box_width) // 2

    # Dessiner le cadre ASCII
    safe_addstr(stdscr, start_y, start_x, "+" + "=" * (box_width - 2) + "+", curses.color_pair(2))
    for y in range(1, box_height - 1):
        safe_addstr(stdscr, start_y + y, start_x, "|" + " " * (box_width - 2) + "|", curses.color_pair(2))
    safe_addstr(stdscr, start_y + box_height - 1, start_x, "+" + "=" * (box_width - 2) + "+", curses.color_pair(2))

    if title:
        title_str = f" {title} "
        title_x = start_x + (box_width - len(title_str)) // 2
        safe_addstr(stdscr, start_y, title_x, title_str, curses.color_pair(1) | curses.A_BOLD)

    # Ajouter le contenu
    for idx, line in enumerate(content[:box_height - 4]):
        safe_addstr(stdscr, start_y + 2 + idx, start_x + 2, line[:box_width - 4], curses.color_pair(1))

    stdscr.refresh()
    stdscr.getch()


def encrypt_message(stdscr):
    """
    Permet de chiffrer un message et affiche le texte avant et après chiffrement.
    """
    curses.echo()
    stdscr.clear()
    height, width = stdscr.getmaxyx()

    safe_addstr(stdscr, height // 2 - 2, (width - 30) // 2, "Entrez le fichier clair :", curses.color_pair(1))
    input_file = stdscr.getstr(height // 2 - 1, (width - 50) // 2, 50).decode("utf-8")

    safe_addstr(stdscr, height // 2 + 1, (width - 40) // 2, "Entrez le fichier chiffré :", curses.color_pair(1))
    output_file = stdscr.getstr(height // 2 + 2, (width - 50) // 2, 50).decode("utf-8")

    curses.noecho()
    execute_c_program(["-e", input_file, output_file])
    display_comparison(stdscr, input_file, output_file, "chiffrement")


def decrypt_message(stdscr):
    """
    Permet de déchiffrer un message et affiche le texte avant et après déchiffrement.
    """
    curses.echo()
    stdscr.clear()
    height, width = stdscr.getmaxyx()

    safe_addstr(stdscr, height // 2 - 2, (width - 30) // 2, "Entrez le fichier chiffré :", curses.color_pair(1))
    input_file = stdscr.getstr(height // 2 - 1, (width - 50) // 2, 50).decode("utf-8")

    safe_addstr(stdscr, height // 2 + 1, (width - 40) // 2, "Entrez le fichier déchiffré :", curses.color_pair(1))
    output_file = stdscr.getstr(height // 2 + 2, (width - 50) // 2, 50).decode("utf-8")

    curses.noecho()
    execute_c_program(["-d", input_file, output_file])
    display_comparison(stdscr, input_file, output_file, "déchiffrement")


def main_menu(stdscr):
    """
    Menu principal.
    """
    if not check_terminal_size(stdscr):
        return

    curses.curs_set(0)
    curses.start_color()
    curses.init_pair(1, curses.COLOR_RED, curses.COLOR_BLACK)  # Rouge pour un effet URSS
    curses.init_pair(2, curses.COLOR_YELLOW, curses.COLOR_BLACK)  # Jaune pour les bordures

    menu_items = [
        "Chiffrer un message",
        "Déchiffrer un message",
        "Quitter"
    ]
    current_row = 0

    while True:
        stdscr.clear()
        draw_ascii_border(stdscr, title="МАШИНА ФИАЛКА - СИМУЛЯТОР")

        height, width = stdscr.getmaxyx()

        # Options du menu
        for idx, item in enumerate(menu_items):
            x = (width - len(item)) // 2
            y = 6 + idx
            if idx == current_row:
                safe_addstr(stdscr, y, x, f"> {item}", curses.color_pair(1) | curses.A_BOLD)
            else:
                safe_addstr(stdscr, y, x, f"  {item}", curses.color_pair(1))

        # Instructions en bas de l'écran
        footer = "Utilisez les flèches pour naviguer et Entrée pour sélectionner"
        safe_addstr(stdscr, height - 2, (width - len(footer)) // 2, footer, curses.color_pair(1))

        stdscr.refresh()

        key = stdscr.getch()

        if key == curses.KEY_UP and current_row > 0:
            current_row -= 1
        elif key == curses.KEY_DOWN and current_row < len(menu_items) - 1:
            current_row += 1
        elif key in [curses.KEY_ENTER, 10, 13]:
            if current_row == 0:
                encrypt_message(stdscr)
            elif current_row == 1:
                decrypt_message(stdscr)
            elif current_row == 2:
                break


def main():
    curses.wrapper(main_menu)


if __name__ == "__main__":
    main()
