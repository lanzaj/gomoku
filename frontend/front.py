from time import sleep
import tkinter as tk
import socket
import json

BOARD_SIZE = 19
CELL_SIZE = 40
HOST = 'localhost'
PORT = 65433

class GomokuGUI:
    def __init__(self, root, mode, board_size, player_color, start_option):
        self.exit = False
        self.root = root
        self.root.title("Gomoku Frontend")
        self.root.overrideredirect(True)  # enlève la barre Windows

        global BOARD_SIZE
        BOARD_SIZE = board_size  # applique la taille choisie

        self.root.after(10, lambda: self.center_window(root))

        canvas_size = (BOARD_SIZE - 1) * CELL_SIZE + CELL_SIZE
        self.canvas = tk.Canvas(root, width=canvas_size, height=canvas_size, bg="burlywood3")
        self.canvas.pack()

        self.draw_board()

        self.canvas.bind("<Button-1>", self.click_handler)

        self.sock = self.sock_conn()
        self.send({"mode": mode, "player_color": player_color, "start_option": start_option})


        # Dessine une croix en haut à droite
        size = 20
        margin = 10
        x1 = (BOARD_SIZE - 1) * CELL_SIZE + CELL_SIZE - margin - size
        y1 = margin
        x2 = x1 + size
        y2 = y1 + size

        self.quit_button = self.canvas.create_rectangle(x1, y1, x2, y2, fill="red", outline="red")
        self.canvas.create_line(x1+4, y1+4, x2-4, y2-4, fill="white", width=2)
        self.canvas.create_line(x1+4, y2-4, x2-4, y1+4, fill="white", width=2)

        # Associe l’action quitter
        self.canvas.tag_bind(self.quit_button, "<Button-1>", lambda e: self.root.destroy())

    def sock_conn(self):
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        try:
            sock.connect((HOST, PORT))
            print("Connexion au backend établie")
        except Exception as e:
            print("Erreur connexion au backend :", e)
            sock = None
        return sock

    def center_window(self, root):
        root.update_idletasks()
        width = root.winfo_width()
        height = root.winfo_height()
        screen_width = root.winfo_screenwidth()
        screen_height = root.winfo_screenheight()
        x = 0 #(screen_width // 2) - (width // 2)
        y = (screen_height // 2) - (height // 2)
        root.geometry(f"+{x}+{y}")

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

    def draw_board(self):
        for i in range(BOARD_SIZE):
            pos = i * CELL_SIZE + CELL_SIZE // 2
            self.canvas.create_line(CELL_SIZE // 2, pos, (BOARD_SIZE - 1) * CELL_SIZE + CELL_SIZE // 2, pos)
            self.canvas.create_line(pos, CELL_SIZE // 2, pos, (BOARD_SIZE - 1) * CELL_SIZE + CELL_SIZE // 2)

    def display_winner(self):
        self.canvas.create_text(
            self.canvas.winfo_width() // 2,
            self.canvas.winfo_height() // 2,
            text=f"Game Over!",
            font=("Arial", 32, "bold"),
            fill="red"
        )

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
        if response.get("win"):
            self.redraw(response)
            self.exit = True
            self.display_winner()
            return

        self.redraw(response)

    def redraw(self, response):
        self.canvas.delete("all")
        self.draw_board()
        for stone in response["game_state"]:
            x, y, color = stone["x"], stone["y"], stone["color"]
            px = x * CELL_SIZE + CELL_SIZE // 2
            py = y * CELL_SIZE + CELL_SIZE // 2
            radius = CELL_SIZE // 2 - 2
            self.canvas.create_oval(px - radius, py - radius, px + radius, py + radius, fill=color)


class StartMenu:
    BG = "#F0E6D2"
    def __init__(self, root):
        self.root = root
        self.root.title("Gomoku - Choix du mode")
        self.root.overrideredirect(True)  # enlève la barre Windows
        self.frame = tk.Frame(root, padx=200, pady=100, bg=self.BG)
        self.frame.pack()
        root.after(10, lambda: self.center_window(root))

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

    def center_window(self, root):
        root.update_idletasks()
        width = root.winfo_width()
        height = root.winfo_height()
        screen_width = root.winfo_screenwidth()
        screen_height = root.winfo_screenheight()
        x = (screen_width // 2) - (width // 2)
        y = (screen_height // 2) - (height // 2)
        root.geometry(f"+{x}+{y}")

    def go_options(self, mode):
        self.frame.destroy()
        OptionsMenu(self.root, mode)
    
    def exit_button(self):
        close_btn = tk.Button(self.frame, text="✕", command=self.root.destroy,
                            bg=self.BG, fg="#4F4F4F", bd=0, relief="flat",
                            highlightthickness=0,
                            font=("Arial", 14, "bold"), activebackground="#2b2b3a")
        
        close_btn.place(x=600, y=-100)

class OptionsMenu:
    BG = "#F0E6D2"
    def __init__(self, root, mode):
        self.root = root
        self.mode = mode
        self.frame = tk.Frame(root, padx=200, pady=100, bg=self.BG)
        self.frame.pack()
        root.after(10, lambda: self.center_window(root))

        tk.Label(self.frame, text="Choisissez vos options", font=("Arial", 16)).pack(pady=10)

        # Choix couleur
        self.color_var = tk.StringVar(value="black")
        tk.Label(self.frame, text="Couleur du joueur").pack()
        tk.Radiobutton(self.frame, text="Noir", variable=self.color_var, value="black").pack()
        tk.Radiobutton(self.frame, text="Blanc", variable=self.color_var, value="white").pack()

        # Taille plateau
        self.size_var = tk.IntVar(value=19)
        tk.Label(self.frame, text="Taille du plateau").pack()
        for size in [19, 15, 13]:
            tk.Radiobutton(self.frame, text=f"{size} x {size}", variable=self.size_var, value=size).pack()

        # Option départ
        self.start_var = tk.StringVar(value="standard")
        tk.Label(self.frame, text="Règle de départ").pack()
        for opt in ["standard", "pro", "swap", "swap2"]:
            tk.Radiobutton(self.frame, text=opt.capitalize(), variable=self.start_var, value=opt).pack()

        tk.Button(self.frame, text="Valider", command=self.start_game).pack(pady=20)

        self.exit_button()


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
                            font=("Arial", 14, "bold"), activebackground="#2b2b3a")
        
        close_btn.place(x=380, y=-100)


def main():
    root = tk.Tk()
    StartMenu(root)
    root.mainloop()

if __name__ == "__main__":
    main()
