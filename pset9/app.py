import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Add transaction history table to finance.db
db.execute("CREATE TABLE IF NOT EXISTS history ( \
        history_id INTEGER UNIQUE NOT NULL, \
        user_id INTEGER NOT NULL, \
        date TEXT NOT NULL, \
        type TEXT NOT NULL, \
        symbol TEXT NOT NULL, \
        name TEXT NOT NULL, \
        shares INTEGER NOT NULL, \
        price INTEGER NOT NULL, \
        cost INTEGER NOT NULL, \
        PRIMARY KEY(history_id), \
        FOREIGN KEY(user_id) REFERENCES users(id))")

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
@login_required
def index():
    """Show portfolio of stocks"""

    # Retrieve stock information from history table
    user_id = session["user_id"]
    symbols = db.execute("SELECT symbol FROM history WHERE user_id = ? GROUP BY symbol", user_id)
    names = db.execute("SELECT name FROM history WHERE user_id = ? GROUP BY symbol", user_id)
    shares = db.execute("SELECT SUM(shares) FROM history WHERE user_id = ? GROUP BY symbol", user_id)
    number = len(db.execute("SELECT symbol, COUNT(symbol) FROM history WHERE user_id = ? GROUP BY symbol", user_id))

    # Determine current prices and values
    prices = []
    values = []
    for i in range(number):
        price = lookup(symbols[i]["symbol"])["price"]
        values.append(price * shares[i]["SUM(shares)"])
        prices.append(price)

    # Determine cash balance, stock value and grand total
    cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]
    value = 0
    for i in range(number):
        int(values[i])
        value += values[i]
    total = cash + value

    # Convert dollar amounts to USD format
    cash = usd(cash)
    value = usd(value)
    total = usd(total)
    for i in range(number):
        values[i] = usd(values[i])
        prices[i] = usd(prices[i])

    # Render portfolio template
    return render_template("index.html", symbols=symbols, names=names, shares=shares, prices=prices, values=values, number=number, cash=cash, value=value, total=total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # User reached route via GET (as by clicking a link or via redirect)
    if request.method == "GET":
        return render_template("buy.html")

    # User reached route via POST (as by submitting a form via POST)
    else:

        # Store input in variables
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")
        result = lookup(symbol)

        # Store date/time in variables
        temp = datetime.now()
        date = temp.strftime("%d/%m/%Y %H:%M:%S")

        # Ensure stock symbol submitted
        if symbol == "":
            return apology("must provide symbol", 400)

        # Ensure stock symbol exists
        elif result == None:
            return apology("symbol does not exist", 400)

        # Ensure shares submitted
        elif shares == "":
            return apology("must provide number of shares", 400)

        # Ensure shares is an integer
        elif not shares.isnumeric():
            return apology("must provide number of shares as a whole number", 400)

        # Ensure shares > 0
        elif int(shares) <= 0:
            return apology("must provide number of shares greater than zero", 400)

        # Ensure user has more cash than purchase price
        else:

            # Determine stock price and user cash
            price = result["price"]
            name = result["name"]
            cost = price * int(shares)
            user_id = session["user_id"]
            cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]['cash']
            balance = cash - cost
            type = "purchase"

            # Check if user has more cash than purchase price
            if balance <= 0:
                return apology("insufficent cash to purchase stocks")

            else:
                # Save transaction to history table
                db.execute("INSERT INTO history (user_id, date, type, symbol, name, shares, price, cost) VALUES (?, ?, ?, ?, ?, ?, ?, ?)",
                user_id, date, type, symbol, name, shares, price, cost)

                # Update user cash
                cash = db.execute("UPDATE users SET cash = ? WHERE id = ?", balance, user_id)

            # Redirect user to home page
            return redirect("/")


@app.route("/load", methods=["GET", "POST"])
@login_required
def load():
    """Load cash to account"""

    # User reached route via GET (as by clicking a link or via redirect)
    if request.method == "GET":
        return render_template("load.html")

    # User reached route via POST (as by submitting a form via POST)
    else:

        # Store input in variables
        load = request.form.get("load")
        user_id = session["user_id"]

        # Ensure amount submitted
        if load == "":
            return apology("must provide cash amount", 400)

        # Ensure load is an integer
        elif not load.isnumeric():
            return apology("must provide cash amount as a whole number", 400)

        # Ensure load > 0
        elif int(load) <= 0:
            return apology("must provide cash amount greater than zero", 400)

        # Update user cash
        else:

            # Update user cash
            cash_old = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]
            cash_new = cash_old + int(load)
            db.execute("UPDATE users SET cash = ? WHERE id = ?", cash_new, user_id)

            # Redirect user to home page
            return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Retrieve stock information from history table
    user_id = session["user_id"]
    symbols = db.execute("SELECT symbol FROM history WHERE user_id = ?", user_id)
    names = db.execute("SELECT name FROM history WHERE user_id = ?", user_id)
    shares = db.execute("SELECT shares FROM history WHERE user_id = ?", user_id)
    prices = db.execute("SELECT price FROM history WHERE user_id = ?", user_id)
    values = db.execute("SELECT cost FROM history WHERE user_id = ?", user_id)
    types = db.execute("SELECT type FROM history WHERE user_id = ?", user_id)
    dates = db.execute("SELECT date FROM history WHERE user_id = ?", user_id)
    number = len(db.execute("SELECT symbol FROM history WHERE user_id = ?", user_id))

    # Convert dollar amounts to USD format
    for i in range(number):
        prices[i] = usd(prices[i]["price"])
        values[i] = usd(values[i]["cost"])

    # Render history template
    return render_template("history.html", symbols=symbols, dates=dates, types=types, names=names, shares=shares, prices=prices, values=values, number=number)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via GET (as by clicking a link or via redirect)
    if request.method == "GET":
        return render_template("login.html")

    # User reached route via POST (as by submitting a form via POST)
    else:

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 400)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # User reached route via GET (as by clicking a link or via redirect)
    if request.method == "GET":
        return render_template("quote.html")

    # User reached route via POST (as by submitting a form via POST)
    else:

        # Store input in variable
        symbol = request.form.get("symbol")
        result = lookup(symbol)

        # Ensure stock symbol submitted
        if symbol == "":
            return apology("must provide symbol", 400)

        # Ensure stock symbol exists
        elif result == None:
            return apology("symbol does not exist", 400)

        # Render quoted template
        else:
            return render_template("quoted.html", symbol=result["symbol"], name=result["name"], price=usd(result["price"]))


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Store inputs in variables
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # Ensure username was submitted
        if username == "":
            return apology("must provide username", 400)

        # Ensure username is available
        elif len(db.execute("SELECT username FROM users WHERE username = ?", username)) > 0:
            return apology("username not available", 400)

        # Ensure initial password was submitted
        elif password == "":
            return apology("must provide password", 400)

        # Ensure confirmation password was submitted
        elif confirmation == "":
            return apology("must provide confirmation password", 400)

        # Ensure initial and confirmation passwords match
        elif password != confirmation:
            return apology("password fields do not match", 400)

        # Add user to users database
        db.execute('INSERT INTO users (username, hash) VALUES(?, ?)', username, generate_password_hash(password))

        # Log user in
        session["user_id"] = db.execute("SELECT * FROM users WHERE username = ?", username)[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # User reached route via GET (as by clicking a link or via redirect)
    if request.method == "GET":

        # Retrieve stock information from history table
        user_id = session["user_id"]
        symbols = db.execute("SELECT symbol FROM history WHERE user_id = ? GROUP BY symbol", user_id)
        number = len(db.execute("SELECT symbol, COUNT(symbol) FROM history WHERE user_id = ? GROUP BY symbol", user_id))

        # Render sell template
        return render_template("sell.html", symbols=symbols, number=number)

    # User reached route via POST (as by submitting a form via POST)
    else:

        # Store inputs in variables
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")
        user_id = session["user_id"]
        type = "sale"

        # Store date/time in variables
        temp = datetime.now()
        date = temp.strftime("%d/%m/%Y %H:%M:%S")

        # Ensure stock symbol submitted
        if symbol == []:
            return apology("must provide symbol", 400)

        # Ensure shares submitted
        elif shares == "":
            return apology("must provide number of shares", 400)

        # Ensure shares is an integer
        elif not shares.isnumeric():
            return apology("must provide number of shares as a whole number", 400)

        # Ensure shares > 0
        elif int(shares) <= 0:
            return apology("must provide number of shares greater than zero", 400)

        # Ensure user owns any stock
        elif db.execute("SELECT SUM(shares) FROM history WHERE user_id = ? AND symbol = ? GROUP BY symbol", user_id, symbol) == []:
            return apology("stock not owned", 400)

        # Ensure user has equal to or more shares than trying to sell
        elif db.execute("SELECT SUM(shares) FROM history WHERE user_id = ? AND symbol = ? GROUP BY symbol", user_id, symbol)[0]["SUM(shares)"] < int(shares):
            return apology("not permitted to sell more stock than owned", 400)

        else:

            # Update variables to record transaction details
            shares = int(shares) * -1
            result = lookup(symbol)
            price = result["price"]
            name = result["name"]
            cost = price * shares
            cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]['cash']
            balance = cash - cost

            # Save transaction to history table
            db.execute("INSERT INTO history (user_id, date, type, symbol, name, shares, price, cost) VALUES (?, ?, ?, ?, ?, ?, ?, ?)",
            user_id, date, type, symbol, name, shares, price, cost)

            # Update user cash
            db.execute("UPDATE users SET cash = ? WHERE id = ?", balance, user_id)

            # Redirect user to home page
            return redirect("/")
