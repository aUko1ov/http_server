from flask import Flask, request, jsonify
import sqlite3
import json

app = Flask(__name__)

def execute_sql(query, params=None, one=False):

    with sqlite3.connect('database.db') as conn:
        cur = conn.cursor()
        cur.execute(query, params or [])
        result = cur.fetchone() if one else cur.fetchall()
    return result


@app.route('/<table>/<int:id>', methods=['GET', 'POST', 'PUT'])
def handle_request(table, id):

    if request.method == 'GET':
        data = execute_sql(f'SELECT {table} FROM players_db WHERE id = ?', [id], one=True)
        return jsonify(json.loads(data[0])) if data else (jsonify({'error': 'Not found'}), 404)

    player_info = request.json
    if request.method == 'POST':
        execute_sql('INSERT INTO players_db (id, {0}) VALUES (?, ?)'.format(table), [id, json.dumps(player_info)])
        return jsonify({'status': 'Created'}), 201

    elif request.method == 'PUT':
        execute_sql('UPDATE players_db SET {0} = ? WHERE id = ?'.format(table), [json.dumps(player_info), id])
        return jsonify({'status': 'Updated'}), 201


if __name__ == '__main__':
    app.run(host='localhost', port=5000)
