import tkinter as tk
import socket
import json

BOARD_SIZE = 19
CELL_SIZE = 40
HOST = 'localhost'
PORT = 65432

class GomokuGUI:
    def __init__(self, root, mode):
        self.root = root
        self.mode = mode  # "human" ou "ai"
        self.root.title("Gomoku Frontend")

        canvas_size = (BOARD_SIZE - 1) * CELL_SIZE + CELL_SIZE
        self.canvas = tk.Canvas(root, width=canvas_size, height=canvas_size, bg="burlywood3")
        self.canvas.pack()

        self.draw_board()

        self.canvas.bind("<Button-1>", self.click_handler)
        self.stones = {}
        self.current_player = "black"


    def draw_board(self):
        for i in range(BOARD_SIZE):
            pos = i * CELL_SIZE + CELL_SIZE // 2
            self.canvas.create_line(CELL_SIZE // 2, pos, (BOARD_SIZE - 1) * CELL_SIZE + CELL_SIZE // 2, pos)
            self.canvas.create_line(pos, CELL_SIZE // 2, pos, (BOARD_SIZE - 1) * CELL_SIZE + CELL_SIZE // 2)


    def click_handler(self, event):
        x = int(round((event.x - CELL_SIZE / 2) / CELL_SIZE))
        y = int(round((event.y - CELL_SIZE / 2) / CELL_SIZE))
        if (x, y) in self.stones or not (0 <= x < BOARD_SIZE and 0 <= y < BOARD_SIZE):
            return
        self.place_stone(x, y, self.current_player)

        if self.mode == "ai" and self.current_player == "black":
            # Le joueur est noir, l'IA joue en blanc
            ai_data = self.get_ai_move(x, y)
            self.handle_backend_data(ai_data)


    def handle_backend_data(self, ai_data):
        if ai_data:
            # placer les pierres indiquées
            for stone in ai_data.get("to_place", []):
                self.place_stone(stone["x"], stone["y"], stone["color"])

            # supprimer les pierres indiquées
            for stone in ai_data.get("to_remove", []):
                pos = (stone["x"], stone["y"])
                if pos in self.stones:
                    self.stones.pop(pos)
                    # Supprimer visuellement : ici, pour simplifier, on peut redraw canvas entier
                    self.redraw_stones()


    def place_stone(self, x, y, color):
        self.stones[(x, y)] = color
        px = x * CELL_SIZE + CELL_SIZE // 2
        py = y * CELL_SIZE + CELL_SIZE // 2
        radius = CELL_SIZE // 2 - 2
        self.canvas.create_oval(px - radius, py - radius, px + radius, py + radius, fill=color)

        # Après avoir placé une pierre, changer de joueur sauf si IA joue
        if self.mode == "human":
            self.current_player = "white" if color == "black" else "black"
        elif self.mode == "ai" and color == "white":
            self.current_player = "black"


    def get_ai_move(self, player_x, player_y):
        try:
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
                s.connect((HOST, PORT))  # 1. Connexion au backend
                move = {"x": player_x, "y": player_y}
                s.sendall(json.dumps(move).encode())  # 2. Envoi du coup joué
                response = s.recv(1024)  # 3. Lecture de la réponse IA
                ai_move = json.loads(response.decode())  # 4. Décodage
                return ai_move  # {'x': ..., 'y': ...}
        except Exception as e:
            print("Erreur de communication :", e)
            return None

    def redraw_stones(self):
        self.canvas.delete("all")
        self.draw_board()
        for (x, y), color in self.stones.items():
            px = x * CELL_SIZE + CELL_SIZE // 2
            py = y * CELL_SIZE + CELL_SIZE // 2
            radius = CELL_SIZE // 2 - 2
            self.canvas.create_oval(px - radius, py - radius, px + radius, py + radius, fill=color)

class StartMenu:
    def __init__(self, root):
        self.root = root
        self.root.title("Gomoku - Choix du mode")
        self.frame = tk.Frame(root, padx=50, pady=50)
        self.frame.pack()

        self.label = tk.Label(self.frame, text="Choisissez un mode de jeu", font=("Arial", 14))
        self.label.pack(pady=10)

        self.btn_human = tk.Button(self.frame, text="Joueur vs Joueur", width=20, command=self.start_human)
        self.btn_human.pack(pady=5)

        self.btn_ai = tk.Button(self.frame, text="Joueur vs IA", width=20, command=self.start_ai)
        self.btn_ai.pack(pady=5)

    def start_human(self):
        self.frame.destroy()
        GomokuGUI(self.root, "human")

    def start_ai(self):
        self.frame.destroy()
        GomokuGUI(self.root, "ai")

def main():
    root = tk.Tk()
    app = StartMenu(root)
    root.mainloop()

if __name__ == "__main__":
    main()
