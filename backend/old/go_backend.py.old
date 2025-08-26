import socket
import threading
import json

HOST = 'localhost'
PORT = 65432

def handle_client(conn):
    with conn:
        while True:
            data = conn.recv(1024)
            if not data:
                break
            move = json.loads(data.decode())
            print("Reçu du client :", move)

            # template du json :
            # ai_response = {
            #   "to_place": [ { "x": 1, "y":2 }, "color": "white" ]
            #   "to_remove": [ { "x":2, "y":5 }, { "x": 1, "y":2 } ]
            # }

            # Simule un coup IA (à droite du dernier et supprime le deux a gauche)
            ai_response = {
                "to_place": [
                    {"x": move["x"] + 1 if move["x"] + 1 < 19 else move["x"], "y": move["y"], "color": "white"}
                ],
                "to_remove": [
                    {"x": move["x"] - 1, "y": move["y"]},
                    {"x": move["x"] - 2, "y": move["y"]}
                ]
            }
            conn.sendall(json.dumps(ai_response).encode())

def run_server():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind((HOST, PORT))
        s.listen()
        print(f"Serveur IA en écoute sur {HOST}:{PORT}")
        while True:
            conn, _ = s.accept()
            threading.Thread(target=handle_client, args=(conn,), daemon=True).start()

if __name__ == "__main__":
    run_server()
