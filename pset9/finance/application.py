import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

from collections import defaultdict
import datetime

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    user_id = session["user_id"]
    balance = db.execute("SELECT cash FROM users WHERE id=?", user_id)

    # Get complete history of user transactions
    rows = db.execute("SELECT * FROM history WHERE personID=? ORDER BY symbol", user_id)

    # Transfer history to list of dicts
    portfolio = []
    for row in rows:
        dic = {}
        dic['symbol'] = row['symbol']
        dic['name'] = row['stock_name']
        dic['amount'] = row['amount']

        portfolio.append(dic)

    dic = {'symbol': "NULL"}
    portfolio.append(dic)

    # Adjust amounts to give total
    for i in range(len(portfolio)):

        if i == (len(portfolio) - 1):
            break

        elif portfolio[i]['symbol'] == portfolio[i+1]['symbol']:
            portfolio[i+1]['amount'] += portfolio[i]['amount']

    # Remove dicts that aren't final amount
    for i in range(len(portfolio)):

        if i == (len(portfolio) - 1):
            break

        while portfolio[i]['symbol'] == portfolio[i + 1]['symbol']:
            portfolio.remove(portfolio[i])

    # Remove null value
    portfolio.pop()

    # Remove any with amount == 0
    portfolio = [x for x in portfolio if x['amount'] != 0]

    # Determine total money
    total = 0
    for i in range(len(portfolio)):
        stock = lookup(portfolio[i]['symbol'])
        portfolio[i]['price'] = stock['price']
        total = portfolio[i]['price'] * portfolio[i]['amount']
        portfolio[i]['total'] = total
        total += portfolio[i]['total']

    total += (balance[0]['cash'])
    balance = usd(balance[0]['cash'])

    return render_template("index.html", balance=balance, portfolio=portfolio, total=total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == 'POST':

        share_value = request.form.get("shares")

        # Ensure all inputs are suitable and of correct type
        try:
            int(share_value)
            if int(share_value) < 0:
                return apology("Please input a valid number")
        except:
            return apology("Please input a valid number")

        if not request.form.get("symbol"):
            return apology("Please input a symbol")

        elif lookup(request.form.get("symbol")) == None:
            return apology("Please input a valid symbol")

        elif not request.form.get("shares"):
            return apology("Please specify the number of shares")

        # If inputs are ok:
        else:
            user_id = session["user_id"]

            symbol = request.form.get("symbol")
            stock = lookup(symbol)

            rows = db.execute("SELECT cash FROM users WHERE id=?", user_id)
            balance = rows[0]['cash']

            number = int(request.form.get("shares"))
            cost = number * stock["price"]

            # Check if balance is sufficient
            if balance < cost:
                flash("Insufficient Funds")

            else:

                # update balance
                balance -= cost
                db.execute("UPDATE users SET cash=? WHERE id=?", balance, user_id)

                # update history
                now = datetime.datetime.now()
                now = now.strftime("%Y-%m-%d %H:%M:%S")
                price = stock['price']
                name = stock["name"]
                symbol = stock['symbol']

                db.execute("INSERT INTO history (price, time, personID, stock_name, symbol, amount) VALUES(?, ?, ?, ?, ?, ?)",
                           price, now, user_id, name, symbol, number)

                return redirect("/")

    return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Access history table and print all transactions
    user_id = session["user_id"]
    rows = db.execute("SELECT * FROM history WHERE personID=?", user_id)

    return render_template("history.html", rows=rows)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


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

    if request.method == 'GET':
        return render_template("quote.html")

    # Get symbol input and lookup price
    else:
        symbol = request.form.get("symbol")

        if not symbol:
            return apology("Please enter a valid symbol.")

        stock = lookup(symbol)

        if stock == None:
            return apology("Please enter a valid symbol.")

        return render_template("quoted.html", stock=stock)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # if user is submitting a form
    if request.method == 'POST':

        # if there is no name input, return no input error
        if not request.form.get("username"):

            return apology("No username input!")

        # if no password inserted or if passwords dont match
        elif not request.form.get("password"):

            return apology("Password error!")

        elif request.form.get("password") != request.form.get("confirmation"):

            return apology("Passwords do not match")

        else:
            # query db for username
            rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

            # if username is in db, return "already taken" error
            if len(rows) != 0:

                return apology("Sorry! Username is already taken")

            # if username not taken and passwords match
            else:
                name = request.form.get("username")
                password = generate_password_hash(request.form.get("password"))
                db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", name, password)

                return redirect("/")

    return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    user_id = session["user_id"]
    rows = db.execute("SELECT * FROM history WHERE personID=? ORDER BY symbol", user_id)
    portfolio = []
    for row in rows:
        dic = {}
        dic['symbol'] = row['symbol']
        dic['name'] = row['stock_name']
        dic['amount'] = row['amount']

        portfolio.append(dic)

    dic = {'symbol': "NULL"}
    portfolio.append(dic)
    for i in range(len(portfolio)):

        if i == (len(portfolio) - 1):
            break

        elif portfolio[i]['symbol'] == portfolio[i+1]['symbol']:
            portfolio[i+1]['amount'] += portfolio[i]['amount']

    for i in range(len(portfolio)):

        if i == (len(portfolio) - 1):
            break

        while portfolio[i]['symbol'] == portfolio[i + 1]['symbol']:
            portfolio.remove(portfolio[i])

    portfolio.pop()

    for i in range(len(portfolio)):
        stock = lookup(portfolio[i]['symbol'])
        portfolio[i]['price'] = stock['price']
        total = portfolio[i]['price'] * portfolio[i]['amount']
        portfolio[i]['total'] = total

    if request.method == 'POST':

        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        if not symbol:
            return apology("Not a valid symbol!")

        elif not shares:
            return apology("Please input a valid amount!")

        else:
            for i in range(len(portfolio)):
                if portfolio[i]['symbol'] == symbol:
                    amount = portfolio[i]['amount']

        if int(shares) > amount:
            return apology("You don't own enough shares!")

        else:
            stock = lookup(symbol)
            price = stock['price']
            name = stock['name']

            now = datetime.datetime.now()
            now = now.strftime("%Y-%m-%d %H:%M:%S")
            amount = int(shares) * -1

            db.execute("INSERT INTO history (price, time, personID, stock_name, symbol, amount) VALUES(?, ?, ?, ?, ?, ?)",
                       price, now, user_id, name, symbol, amount)

            balance = db.execute("SELECT cash FROM users WHERE id=?", user_id)
            balance = balance[0]['cash']
            balance += ((amount * -1) * price)

            db.execute("UPDATE users SET cash=? WHERE id=?", balance, user_id)

        return redirect("/")

    return render_template("sell.html", portfolio=portfolio)


@app.route("/funds", methods=["GET", "POST"])
def funds():
    """Add funds"""

    # Take an input amount
    if request.method == 'POST':
        amount = int(request.form.get("amount"))

        # Get current balance and update it
        current = db.execute("SELECT cash FROM users WHERE id=?", session['user_id'])
        balance = current[0]['cash']
        balance += amount

        # Replace balance with new balance in db
        db.execute("UPDATE users SET cash=? WHERE id=?", balance, session['user_id'])
        return redirect("/")

    return render_template("funds.html")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)