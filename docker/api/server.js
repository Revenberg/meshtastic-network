const express = require('express');
const mysql = require('mysql2');
const app = express();
app.use(express.json());

// Retry logic for MySQL connection
function connectWithRetry(retries = 10, delay = 3000) {
  const db = mysql.createConnection({
    host: 'mysql',
    user: 'meshtastic',
    password: 'example_pw',
    database: 'meshtastic',
    port: 3306, // Default MySQL port
  });

  db.connect((err) => {
    if (err) {
      if (retries > 0) {
        console.error('MySQL connection failed, retrying in', delay / 1000, 'seconds...');
        setTimeout(() => connectWithRetry(retries - 1, delay), delay);
      } else {
        console.error('MySQL connection failed after retries:', err);
        process.exit(1);
      }
    } else {
      console.log('Connected to MySQL database.');
      startServer(db);
    }
  });

  db.on('error', (err) => {
    console.error('MySQL error:', err);
  });
}

function startServer(db) {
  app.post('/api/register', (req, res) => {
    const { teamName, captainName } = req.body;
    if (!teamName || !captainName) return res.status(400).send('Missing fields');
    db.query(
      'INSERT INTO teams (team_name, captain_name) VALUES (?, ?)',
      [teamName, captainName],
      (err) => {
        if (err) return res.status(500).send('DB error');
        res.status(200).send('OK');
      }
    );
  });

  app.get('/api/teams', (req, res) => {
    db.query('SELECT id, team_name FROM teams', (err, results) => {
      if (err) return res.status(500).send('DB error');
      res.json(results);
    });
  });

  // Add user registration endpoint
  app.post('/api/add_user', (req, res) => {
    const { userName, teamId } = req.body;
    if (!userName || !teamId) return res.status(400).send('Missing fields');
    db.query(
      'INSERT INTO users (user_name, team_id) VALUES (?, ?)',
      [userName, teamId],
      (err) => {
        if (err) return res.status(500).send('DB error');
        res.status(200).send('OK');
      }
    );
  });

  // List users with team name
  app.get('/api/users', (req, res) => {
    db.query(
      `SELECT users.user_name, teams.team_name, users.registered_at
       FROM users
       JOIN teams ON users.team_id = teams.id
       ORDER BY users.registered_at DESC`,
      (err, results) => {
        if (err) return res.status(500).send('DB error');
        res.json(results);
      }
    );
  });

  app.listen(3001, () => console.log('API server running on 3001'));
}

connectWithRetry();
