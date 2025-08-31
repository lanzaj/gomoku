from time import sleep
import tkinter as tk
import socket
import json


BOARD_SIZE = 19
CELL_SIZE = 40
HOST = 'localhost'
PORT = 65433

class GomokuGUI:
    def __init__(self, root, mode):
        self.exit = False
        self.root = root
        self.root.title("Gomoku Frontend")

        # Centrer la fenêtre après l’affichage
        self.root.after(10, lambda: self.center_window(root))

        canvas_size = (BOARD_SIZE - 1) * CELL_SIZE + CELL_SIZE
        self.canvas = tk.Canvas(root, width=canvas_size, height=canvas_size, bg="burlywood3")
        self.canvas.pack()

        self.draw_board()

        # Bind le click gauche
        self.canvas.bind("<Button-1>", self.click_handler)

        # Socket
        self.sock = self.sock_conn()

        # Mode
        self.send({"mode":mode})


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
        root.update_idletasks()  # force le calcul de la taille réelle de la fenêtre
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
            print("Socket non connecté")
            return None
        try:
            response = self.sock.recv(4096)
            if response:
                return json.loads(response.decode())
            else:
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
        """
        Display the winner at the end of the game.
        winner: can be 'black', 'white', or a player name depending on backend
        """

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

        #print(response)

        if not response["authorized"]:
            return  # coup refusé, on ne fait rien
        if response["win"]:
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
    def __init__(self, root):
        self.root = root
        self.root.title("Gomoku - Choix du mode")
        self.frame = tk.Frame(root, padx=200, pady=100)
        self.frame.pack()
        root.overrideredirect(True)
        root.after(10, lambda: self.center_window(root))  # centrer après l’affichage

        self.title_label = tk.Label(
            self.frame,
            text="GOMOKU",
            font=("Arial", 60, "bold"),
            fg="white",
        )
        self.title_label.pack(pady=(0, 20))


        self.label = tk.Label(self.frame, text="Choisissez un mode de jeu", font=("Arial", 14))
        self.label.pack(pady=10)

        self.btn_human = tk.Button(self.frame, text="Joueur vs Joueur", width=20, command=self.start_human)
        self.btn_human.pack(pady=5)

        self.btn_ai = tk.Button(self.frame, text="Joueur vs IA", width=20, command=self.start_ai)
        self.btn_ai.pack(pady=5)

        self.btn_ai = tk.Button(self.frame, text="Demo : IA vs IA", width=20, command=self.start_demo)
        self.btn_ai.pack(pady=5)

    def center_window(self, root):
        root.update_idletasks()  # force le calcul de la taille réelle de la fenêtre
        width = root.winfo_width()
        height = root.winfo_height()
        screen_width = root.winfo_screenwidth()
        screen_height = root.winfo_screenheight()
        x = 0 #(screen_width // 2) - (width // 2)
        y = (screen_height // 2) - (height // 2)
        root.geometry(f"+{x}+{y}")


    def start_human(self):
        self.frame.destroy()
        GomokuGUI(self.root, "human")

    def start_ai(self):
        self.frame.destroy()
        GomokuGUI(self.root, "ai")
    
    def start_demo(self):
        self.frame.destroy()
        GomokuGUI(self.root, "demo")

def main():
    root = tk.Tk()
    app = StartMenu(root)
    root.mainloop()

if __name__ == "__main__":
    main()
