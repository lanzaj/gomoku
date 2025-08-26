import socket
import threading
import json

HOST = 'localhost'
PORT = 65433


def handle_client(conn):
    mode = None
    game_state = {}
    current_player = "black"
    
    with conn:
        while True:
            data = received_data(conn)

            if "mode" in data:
                mode = chosen_mode(data)

            else:
                # [human vs ai] & [human vs human] : coup autorisé ?
                game_state, authorized = rules(data, game_state, current_player)

                # reaction en fonction du mode
                if mode == "human":
                    current_player = "white" if current_player == "black" else "black"
                if mode == "ai":
                    game_state = ai(data, game_state, current_player)

                # Conversion pour envoie de l'état de jeu
                json_game_state = [{"x": x, "y": y, "color": c} for (x, y), c in game_state.items()]
                response = {
                    "game_state" : json_game_state,
                    "win" : win(data),
                    "authorized": authorized
                    }
                # Envoie au front
                conn.sendall(json.dumps(response).encode())


def rules(data, game_state, current_player):
    BOARD_SIZE = 19
    x, y = data['x'], data['y']
    authorized = True

    if (x, y) in game_state or not (0 <= x < BOARD_SIZE and 0 <= y < BOARD_SIZE):
        authorized = False

    if authorized:
        game_state[(x, y)] = current_player
    return game_state, authorized


def win(data):
    return False


def ai(data, game_state, current_player): # last change 23.08
    x, y = data['x'], data['y']
    current_player = "white" if current_player == "black" else "black"

    game_state[(x - 1, y)] = current_player
    game_state.pop((x + 1, y), None)

    return game_state


def received_data(conn):
    data = conn.recv(1024)
    if not data:
        return
    data = json.loads(data.decode())
    # print("Reçu du client :", data)
    return data


def chosen_mode(data):
    if "mode" in data:
        print(f"Mode défini pour ce client : {data['mode']}")
        return data["mode"]


def run_server():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind((HOST, PORT))
        s.listen()
        print(f"Serveur en écoute sur {HOST}:{PORT}")
        while True:
            conn, _ = s.accept()
            threading.Thread(target=handle_client, args=(conn,), daemon=True).start()


if __name__ == "__main__":
    run_server()
