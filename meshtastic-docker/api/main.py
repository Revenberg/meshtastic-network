from fastapi import FastAPI, Request
import psycopg2
import meshtastic.serial_interface

app = FastAPI()

# connect to meshtastic gateway
iface = meshtastic.serial_interface.SerialInterface("/dev/ttyUSB0")

def get_db_conn():
    return psycopg2.connect(
        host="db", dbname="meshtastic", user="user", password="password"
    )

@app.on_event("startup")
async def startup_event():
    # Use a single connection so the CREATE TABLE runs on the same connection we commit
    conn = get_db_conn()
    cur = conn.cursor()
    cur.execute("""
        CREATE TABLE IF NOT EXISTS messages (
            id SERIAL PRIMARY KEY,
            from_id TEXT,
            text TEXT,
            timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP
        );
    """)
    conn.commit()
    cur.close()
    conn.close()

def on_receive(packet, iface):
    print("Got packet:", packet)
    decoded = packet.get("decoded", {}).get("text")
    sender = packet.get("fromId")
    conn = get_db_conn()
    cur = conn.cursor()
    cur.execute("INSERT INTO messages (from_id, text) VALUES (%s, %s)", (sender, decoded))
    conn.commit()
    cur.close()
    conn.close()

iface.onReceive = on_receive

@app.get("/")
async def root():
    return {"message": "Meshtastic API running"}

@app.get("/messages")
async def list_messages():
    conn = get_db_conn()
    cur = conn.cursor()
    cur.execute("SELECT id, from_id, text, timestamp FROM messages ORDER BY id DESC LIMIT 50;")
    rows = cur.fetchall()
    cur.close()
    conn.close()
    return [{"id": r[0], "from": r[1], "text": r[2], "time": r[3]} for r in rows]

@app.post("/send")
async def send_message(req: Request):
    data = await req.json()
    text = data.get("text", "")
    iface.sendText(text)
    return {"status": "sent", "text": text}