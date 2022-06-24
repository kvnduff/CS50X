import os
import re

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, jsonify
from flask_session import Session
from tempfile import mkdtemp
from random import randint

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///recryptex.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
  """Ensure responses aren't cached"""

  response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
  response.headers["Expires"] = 0
  response.headers["Pragma"] = "no-cache"
  return response


@app.route("/")
def index():
  """Display recryptex game, loading slave regex, master regex and solution"""

  # Declare arrays
  match = []
  nomatch = []

  # Generate random number for selection of random database row
  num = db.execute("SELECT COUNT(*) FROM slave")
  rand = randint(1,int(num[0]["COUNT(*)"]))

  # Select id and match/nomatch lists from slave table
  sid = db.execute("SELECT id FROM slave WHERE id = ?", rand)[0]["id"]
  for i in range(8):
    match.append(db.execute("SELECT m? FROM slave WHERE id = ?", i, sid)[0]["m" + str(i)])
    nomatch.append(db.execute("SELECT n? FROM slave WHERE id = ?", i, sid)[0]["n" + str(i)])

  # Add id and match/nomatch lists to s_active table
  db.execute("UPDATE s_active SET id = ? WHERE first_row = 1", sid)
  for i in range(8):
    db.execute("UPDATE s_active SET m? = ? WHERE first_row = 1", i, match[i])
    db.execute("UPDATE s_active SET n? = ? WHERE first_row = 1", i, nomatch[i])

  # Declare arrays
  master = []
  cypher = []
  solution_el = []

  # Generate random number for selection of random database row
  num = db.execute("SELECT COUNT(*) FROM master")
  rand = randint(1,int(num[0]["COUNT(*)"]))

  # Select id, master regex, cyphertext and solution from master table
  mid = db.execute("SELECT id FROM master WHERE id = ?", rand)[0]["id"]
  solution = db.execute("SELECT solution FROM master WHERE id = ?", mid)[0]["solution"]
  for i in range(5):
    master.append(db.execute("SELECT m? FROM master WHERE id = ?", i, mid)[0]["m" + str(i)])
    cypher.append(db.execute("SELECT c? FROM master WHERE id = ?", i, mid)[0]["c" + str(i)])
    solution_el.append(db.execute("SELECT s? FROM master WHERE id = ?", i, mid)[0]["s" + str(i)])

  # Add id, master regex, cyphertext and solution to m_active table
  db.execute("UPDATE m_active SET id = ? WHERE first_row = 1", mid)
  db.execute("UPDATE m_active SET solution = ? WHERE first_row = 1", solution)
  for i in range(5):
    db.execute("UPDATE m_active SET m? = ? WHERE first_row = 1", i, master[i])
    db.execute("UPDATE m_active SET c? = ? WHERE first_row = 1", i, cypher[i])
    db.execute("UPDATE m_active SET s? = ? WHERE first_row = 1", i, solution_el[i])

  # Create array of master element lengths
  master_length = []
  for i in range(5):
    master_length.append(len(master[i]))

  # Create variable of max length of master elements
  master_max = max([len(i) for i in master])

  # Render game template with required inputs
  return render_template("index.html", sid=sid, match=match, nomatch=nomatch, mid=mid, master=master, cypher=cypher, solution_el=solution_el, solution=solution, master_length=master_length, master_max=master_max)


@app.route("/regex")
def regex():
  """Process regex input"""

  # Retrieve regex from fetch
  regex = request.args.get("x")

  # Declare arrays
  match = []
  nomatch = []

  # Retrieve id and match/nomatch lists from s_active table
  for i in range(8):
    match.append(db.execute("SELECT m? FROM s_active WHERE first_row = 1", i)[0]["m" + str(i)])
    nomatch.append(db.execute("SELECT n? FROM s_active WHERE first_row = 1", i)[0]["n" + str(i)])

  # Use Python regex search function to test if regex matches match elements (true) and doesn't match nomatch elements (false)
  match_true = []
  nomatch_false = []
  for i in range(8):
    match_true.append(bool(re.search(regex, match[i])))
    nomatch_false.append(bool(re.search(regex, nomatch[i])))

  # Return required inputs
  return jsonify(regex, match, nomatch, match_true, nomatch_false)


@app.route("/reload")
def reload():
  """Reload new slave regex"""

  # Declare arrays
  match_new = []
  nomatch_new = []

  # Generate random number for selection of random database row
  num = db.execute("SELECT COUNT(*) FROM slave")
  rand = randint(1,int(num[0]["COUNT(*)"]))

  # Select id and match/nomatch lists from slave table
  id = db.execute("SELECT id FROM slave WHERE id = ?", rand)[0]["id"]
  for i in range(8):
    match_new.append(db.execute("SELECT m? FROM slave WHERE id = ?", i, id)[0]["m" + str(i)])
    nomatch_new.append(db.execute("SELECT n? FROM slave WHERE id = ?", i, id)[0]["n" + str(i)])

  # Add id and match/nomatch lists to s_active table
  db.execute("UPDATE s_active SET id = ? WHERE first_row = 1", id)
  for i in range(8):
    db.execute("UPDATE s_active SET m? = ? WHERE first_row = 1", i, match_new[i])
    db.execute("UPDATE s_active SET n? = ? WHERE first_row = 1", i, nomatch_new[i])

  # Return required inputs
  return jsonify(match_new, nomatch_new)


@app.route("/help", methods=["GET", "POST"])
def help():
    """Display recryptex help"""

    # Render help template
    return render_template("help.html")