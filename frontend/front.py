from time import sleep
import tkinter as tk
import socket
import json
import time

BOARD_SIZE = 19
CELL_SIZE = 40
HOST = 'localhost'
PORT = 65433

class GomokuGUI:
    BG = "burlywood3"
    def __init__(self, root, mode, board_size, player_color, rules):
        self.rules = rules
        self.exit = False
        self.root = root
        self.root.title("Gomoku Frontend")
        self.root.overrideredirect(True)  # enlève la barre Windows
        self.mode = mode

        global BOARD_SIZE
        BOARD_SIZE = board_size  # applique la taille de board choisie

        self.root.after(10, lambda: self.center_window(root))

        canvas_size = (BOARD_SIZE - 1) * CELL_SIZE + CELL_SIZE
        self.canvas = tk.Canvas(root, width=canvas_size, height=canvas_size, bg=self.BG)
        self.canvas.pack()

        self.draw_board()
        self.bar(root)

        self.sock = self.sock_conn()
        self.send({"mode": mode, "player_color": player_color, "start_option": rules})

        self.rules_choice()

        if not self.mode == 'demo':
            self.canvas.bind("<Button-1>", self.click_handler)
        else:
            while 1:
                print('demo')
                self.send({'waiting':True})
                self.root.after(10, lambda: self.center_window(root))
                response = self.receive()
                self.handle_move(response)
                self.root.update_idletasks()
                time.sleep(0.05)
                if response.get('win'):
                    time.sleep(2)
                    exit(0)

    ############## Window  ##############

    def bar(self, root):
        # Barre de titre custom
        self.title_bar = tk.Frame(root, bg="#D2B48C", height=30)  # couleur "bois clair"
        self.title_bar.pack(fill="x", side="top")

        exit_btn = tk.Button(
            self.title_bar,
            text="Exit",               # croix
            command=root.destroy,
            bg="#D2B48C",           # fond assorti à la barre
            fg="black",
            bd=0,                   # pas de bordure
            relief="flat",
            font=("Arial", 12, "bold"),
            highlightthickness=0,
            activebackground="#C19A6B"  # couleur au survol
        )
        exit_btn.pack(side="right", padx=5)

        self.make_draggable(self.title_bar) # fenetre movible

    def make_draggable(self, widget):
        widget.bind("<Button-1>", self.start_move)
        widget.bind("<B1-Motion>", self.do_move)

    def start_move(self, event):
        self._x = event.x
        self._y = event.y

    def do_move(self, event):
        x = self.root.winfo_x() + (event.x - self._x)
        y = self.root.winfo_y() + (event.y - self._y)
        self.root.geometry(f"+{x}+{y}")

    def center_window(self, root):
        root.update_idletasks()
        width = root.winfo_width()
        height = root.winfo_height()
        screen_width = root.winfo_screenwidth()
        screen_height = root.winfo_screenheight()
        x = (screen_width // 2) - (width // 2)
        y = (screen_height // 2) - (height // 2)
        root.geometry(f"+{x}+{y}")

    ##############   Socket   ##############

    def sock_conn(self):
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        try:
            sock.connect((HOST, PORT))
            print("Connexion au backend établie")
        except Exception as e:
            print("Erreur connexion au backend :", e)
            sock = None
        return sock

    def send(self, data):
        if not self.sock:
            print("Socket non connecté")
            return
        try:
            message = json.dumps(data).encode()
            self.sock.sendall(message)
        except Exception as e:
            print("Erreur envoi :", e)

    def receive(self):
        if not self.sock:
            return None
        try:
            response = self.sock.recv(4096)
            if response:
                return json.loads(response.decode())
            return None
        except Exception as e:
            print("Erreur réception :", e)
            return None

    ##############   Draw   ##############

    def draw_board(self):
        for i in range(BOARD_SIZE):
            pos = i * CELL_SIZE + CELL_SIZE // 2
            self.canvas.create_line(CELL_SIZE // 2, pos, (BOARD_SIZE - 1) * CELL_SIZE + CELL_SIZE // 2, pos)
            self.canvas.create_line(pos, CELL_SIZE // 2, pos, (BOARD_SIZE - 1) * CELL_SIZE + CELL_SIZE // 2)

    def redraw(self, response):
        self.canvas.delete("all")
        self.draw_board()
        for stone in response["game_state"]:
            x, y, color = stone["x"], stone["y"], stone["color"]
            px = x * CELL_SIZE + CELL_SIZE // 2
            py = y * CELL_SIZE + CELL_SIZE // 2
            radius = CELL_SIZE // 2 - 2
            self.canvas.create_oval(px - radius, py - radius, px + radius, py + radius, fill=color)

    def display_winner(self):
        self.canvas.create_text(
            self.canvas.winfo_width() // 2,
            self.canvas.winfo_height() // 2,
            text=f"Game Over!",
            font=("Arial", 32, "bold"),
            fill="red"
        )

    ##############   Click   ##############

    def click_handler(self, event):
        if self.exit == True:
            exit()

        x = int(round((event.x - CELL_SIZE / 2) / CELL_SIZE))
        y = int(round((event.y - CELL_SIZE / 2) / CELL_SIZE))

        self.send({"x": x, "y": y})
        response = self.receive()

        if not response:
            return
        
        if not response.get("authorized", True):
            return  # coup refusé

        self.handle_move(response)
        self.root.update_idletasks()

        if self.rules == 'swap' and response.get('game_state') and len(response['game_state']) == 3:
            self.swap_choice()

        if self.rules =='swap2' and response.get('game_state') and len(response['game_state']) == 3:
            self.swap2_choice()
        

        if self.mode == 'ai':
            response = self.receive()
            self.handle_move(response)


    def handle_move(self, response):
        self.redraw(response)

        if response.get("win"):
            self.exit = True
            self.display_winner()
            return

    ##############   Rules   ##############

    def rules_choice(self):
        if self.rules == "swap":
            self.show_swap_popup("Règle Swap", "Le premier joueur place 2 pierre noires et 1 blanche\n Le second joueur choisira ensuite sa couleur", '400x200')
        if self.rules == "swap2":
            self.show_swap_popup("Règle Swap2", "Le premier joueur place 2 pierre noires et 1 blanche\n Le second joueur aura 3 choix :\n   -Garder les Blancs\n   -Changer de couleur\n   -Placer 2 pierres de plus et laisser\nle premiers joueur choisir sa couleur", '400x300')
        

    def show_swap_popup(self, title_, text_, geo):
        popup = tk.Toplevel(self.root)
        popup.title("Règle Swap")
        popup.geometry(geo)
        popup.transient(self.root)  # fenêtre liée à la principale
        popup.grab_set()  # empêche d’interagir avec la fenêtre principale
        popup.overrideredirect(True)
        self.center_window(popup)

        popup_bg = "#F0E6D2"  # beige clair
        popup.configure(bg=popup_bg)

        tk.Label(popup, text=title_, font=("Arial", 14, "bold"), bg=popup_bg).pack(pady=20)
        tk.Label(popup, text=text_, font=("Arial", 12), bg=popup_bg).pack(pady=10)

        btn_style = {
            "font": ("Arial", 14, "bold"),
            "bg": popup_bg,
            "fg": "#A17C5A",
            "activebackground": "#5a5a8a",
            "activeforeground": "white",
            "relief": "flat",
            "bd": 0,
            "width": 20,
            "height": 2,
            "highlightthickness": 0
        }

        ok_btn = tk.Button(popup, text="OK", command=popup.destroy, **btn_style)
        ok_btn.pack(pady=20)

    def swap_choice(self):
        popup = tk.Toplevel(self.root)
        popup.title("Règle Swap")
        popup.geometry("400x250")
        popup.transient(self.root)  # fenêtre liée à la principale
        popup.overrideredirect(True)
        self.center_window(popup)

        popup_bg = "#F0E6D2"  # beige clair
        popup.configure(bg=popup_bg)

        tk.Label(popup, text="Règle Swap", font=("Arial", 14, "bold"), bg=popup_bg).pack(pady=20)
        tk.Label(popup, text="Le second joueur choisie sa couleur", font=("Arial", 12), bg=popup_bg).pack(pady=10)

        btn_style = {
            "font": ("Arial", 14, "bold"),
            "bg": popup_bg,
            "fg": "#A17C5A",
            "activebackground": "#5a5a8a",
            "activeforeground": "white",
            "relief": "flat",
            "bd": 0,
            "width": 10,
            "height": 1,
            "highlightthickness": 0
        }

        btn_frame = tk.Frame(popup, bg=popup_bg)
        btn_frame.pack(pady=20)

        black_btn = tk.Button(btn_frame, text="Noirs", command=popup.destroy, **btn_style)
        black_btn.pack(pady=5, fill="x")

        white_btn = tk.Button(btn_frame, text="Blancs", command=popup.destroy, **btn_style)
        white_btn.pack(pady=5, fill="x")

        popup.after(10, lambda: popup.grab_set())

    def swap2_choice(self):
        popup = tk.Toplevel(self.root)
        popup.title("Règle Swap2")
        popup.geometry("400x300")
        popup.transient(self.root)  # fenêtre liée à la principale
        popup.overrideredirect(True)
        self.center_window(popup)

        popup_bg = "#F0E6D2"  # beige clair
        popup.configure(bg=popup_bg)

        tk.Label(popup, text="Règle Swap", font=("Arial", 14, "bold"), bg=popup_bg).pack(pady=20)
        tk.Label(popup, text="Le second joueur choisi : ", font=("Arial", 12), bg=popup_bg).pack(pady=10)

        btn_style = {
            "font": ("Arial", 14, "bold"),
            "bg": popup_bg,
            "fg": "#A17C5A",
            "activebackground": "#5a5a8a",
            "activeforeground": "white",
            "relief": "flat",
            "bd": 0,
            "width": 15,
            "height": 1,
            "highlightthickness": 0
        }

        btn_frame = tk.Frame(popup, bg=popup_bg)
        btn_frame.pack(pady=25)

        black_btn = tk.Button(btn_frame, text="Noirs", command=popup.destroy, **btn_style)
        black_btn.pack(pady=5, fill="x")

        white_btn = tk.Button(btn_frame, text="Blancs", command=popup.destroy, **btn_style)
        white_btn.pack(pady=5, fill="x")

        white_btn = tk.Button(btn_frame, text="Placer 2 pierres", command=popup.destroy, **btn_style)
        white_btn.pack(pady=5, fill="x")

        popup.after(10, lambda: popup.grab_set())


class StartMenu:
    BG = "#F0E6D2"
    def __init__(self, root):
        self.root = root
        self.root.title("Gomoku - Choix du mode")
        self.root.overrideredirect(True)  # enlève la barre Windows
        self.frame = tk.Frame(root, padx=200, pady=100, bg=self.BG)
        self.frame.pack()
        root.after(10, lambda: self.center_window(root))

        self.make_window_draggable(self.frame) # fenetre movible

        # Gros titre
        self.title_label = tk.Label(
            self.frame,
            text="GOMOKU",
            font=("Arial", 72, "bold"),
            fg="#8B5E3C",
            bg=self.BG
        )
        self.title_label.pack(pady=(0, 40))

        btn_style = {
            "font": ("Arial", 14, "bold"),
            "bg": self.BG,
            "fg": "#A17C5A",
            "activebackground": "#5a5a8a",
            "activeforeground": "white",
            "relief": "flat",
            "bd": 0,
            "width": 20,
            "height": 2,
            "highlightthickness": 0, 
        }

        tk.Button(self.frame, text="Joueur vs Joueur", command=lambda: self.go_options("human"), **btn_style).pack(pady=5)
        tk.Button(self.frame, text="Joueur vs IA", command=lambda: self.go_options("ai"), **btn_style).pack(pady=5)
        tk.Button(self.frame, text="Demo : IA vs IA", command=lambda: self.go_options("demo"), **btn_style).pack(pady=5)

        self.exit_button()

    ############## Window ##############

    def center_window(self, root):
        root.update_idletasks()
        width = root.winfo_width()
        height = root.winfo_height()
        screen_width = root.winfo_screenwidth()
        screen_height = root.winfo_screenheight()
        x = (screen_width // 2) - (width // 2)
        y = (screen_height // 2) - (height // 2)
        root.geometry(f"+{x}+{y}")

    def make_window_draggable(self, widget):
        widget.bind("<Button-1>", self._start_move_window)
        widget.bind("<B1-Motion>", self._do_move_window)

    def _start_move_window(self, event):
        # décalage entre le curseur et le coin de la fenêtre
        self._dx = event.x_root - self.root.winfo_x()
        self._dy = event.y_root - self.root.winfo_y()

    def _do_move_window(self, event):
        x = event.x_root - self._dx
        y = event.y_root - self._dy
        self.root.geometry(f"+{x}+{y}")

    def exit_button(self):
        close_btn = tk.Button(self.frame, text="✕", command=self.root.destroy,
                            bg=self.BG, fg="#4F4F4F", bd=0, relief="flat",
                            highlightthickness=0,
                            font=("Arial", 14, "bold"), activebackground=self.BG)
        
        close_btn.place(x=600, y=-100)
        
    ##############                 ##############

    def go_options(self, mode):
        self.frame.destroy()
        OptionsMenu(self.root, mode)

    

class OptionsMenu:
    BG = "#F0E6D2"
    BTN_ACTIVE = "#A67B5B"  # marron clair quand sélectionné
    BTN_TEXT = "#2B2B2B"

    def __init__(self, root, mode):
        self.root = root
        self.mode = mode
        self.frame = tk.Frame(root, padx=200, pady=100, bg=self.BG)
        self.frame.pack()
        root.after(10, lambda: self.center_window(root))
        
        self.make_window_draggable(self.frame) # fenetre movible
        tk.Label(self.frame, text="Choisissez vos options", font=("Arial", 16), bg=self.BG).pack(pady=10)

        # Choix couleur
        tk.Label(self.frame, text="Couleur du joueur", bg=self.BG).pack()
        self.color_var = tk.StringVar(value="black")
        self.color_buttons = {}
        for color, label in [("black", "Noir"), ("white", "Blanc")]:
            btn = tk.Button(
                self.frame, text=label, width=12,
                bg=self.BG, fg=self.BTN_TEXT,
                relief="flat", bd=0, pady=5,
                command=lambda c=color: self.select_option(self.color_var, c, self.color_buttons)
            )
            btn.pack(pady=2)
            self.color_buttons[color] = btn
        self.select_option(self.color_var, "black", self.color_buttons)  # valeur par défaut

        # Taille plateau
        tk.Label(self.frame, text="Taille du plateau", bg=self.BG).pack()
        self.size_var = tk.IntVar(value=19)
        self.size_buttons = {}
        for size in [19, 15, 13]:
            btn = tk.Button(
                self.frame, text=f"{size} x {size}", width=12,
                bg=self.BG, fg=self.BTN_TEXT,
                relief="flat", bd=0, pady=5,
                command=lambda s=size: self.select_option(self.size_var, s, self.size_buttons)
            )
            btn.pack(pady=2)
            self.size_buttons[size] = btn
        self.select_option(self.size_var, 19, self.size_buttons)

        # Option départ
        tk.Label(self.frame, text="Règle de départ", bg=self.BG).pack()
        self.start_var = tk.StringVar(value="standard")
        self.start_buttons = {}
        for opt in ["standard", "pro", "swap", "swap2"]:
            btn = tk.Button(
                self.frame, text=opt.capitalize(), width=12,
                bg=self.BG, fg=self.BTN_TEXT,
                relief="flat", bd=0, pady=5,
                command=lambda o=opt: self.select_option(self.start_var, o, self.start_buttons)
            )
            btn.pack(pady=2)
            self.start_buttons[opt] = btn
        self.select_option(self.start_var, "standard", self.start_buttons)

        # Valider
        tk.Button(self.frame, text="Valider", command=self.start_game,
                  bg="#8FBC8F", fg="white", font=("Arial", 12, "bold"), relief="flat",
                  activebackground="#6B8E23", pady=5, width=14).pack(pady=20)

        self.exit_button()

    ############## Fenetre movible ##############

    def make_window_draggable(self, widget):
        widget.bind("<Button-1>", self._start_move_window)
        widget.bind("<B1-Motion>", self._do_move_window)

    def _start_move_window(self, event):
        # décalage entre le curseur et le coin de la fenêtre
        self._dx = event.x_root - self.root.winfo_x()
        self._dy = event.y_root - self.root.winfo_y()

    def _do_move_window(self, event):
        x = event.x_root - self._dx
        y = event.y_root - self._dy
        self.root.geometry(f"+{x}+{y}")

    ##############                   ##############

    def select_option(self, var, value, buttons):
        """Met à jour la sélection et change la couleur des boutons"""
        var.set(value)
        for v, btn in buttons.items():
            if str(v) == str(value):
                btn.config(bg=self.BTN_ACTIVE, fg="white")
            else:
                btn.config(bg=self.BG, fg=self.BTN_TEXT)

    def start_game(self):
        self.frame.destroy()
        GomokuGUI(
            self.root,
            self.mode,
            self.size_var.get(),
            self.color_var.get(),
            self.start_var.get()
        )

    def center_window(self, root):
        root.update_idletasks()
        width = root.winfo_width()
        height = root.winfo_height()
        screen_width = root.winfo_screenwidth()
        screen_height = root.winfo_screenheight()
        x = (screen_width // 2) - (width // 2)
        y = (screen_height // 2) - (height // 2)
        root.geometry(f"+{x}+{y}")

    def exit_button(self):
        close_btn = tk.Button(self.frame, text="✕", command=self.root.destroy,
                            bg=self.BG, fg="#4F4F4F", bd=0, relief="flat",
                            highlightthickness=0,
                            font=("Arial", 14, "bold"), activebackground=self.BG)
        close_btn.place(x=380, y=-100)



def main():
    root = tk.Tk()
    StartMenu(root)
    root.mainloop()

if __name__ == "__main__":
    main()
