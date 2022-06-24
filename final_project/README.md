# RECRYPTEX

Recryptex is an online game that uses regex to decrypt cyphertext. A video 
demonstration of the game can be found at:
  [Recryptex](https://youtu.be/M8JmcAFzFuo)


## Program Features

The overall goal of Recryptex is to decrypt cyphertext. However, doing so is
easier said than done. You will get lots of regular expression practice as you
progress through the match, reveal, and decrypt stages:

1. Match: During the match stage, you will be presented with two columns of text
   labelled "match" and "^match". Each column has multiple strings. You are
   tasked with creating a regular expression that matches all of the strings in
   the match column but also doesn't match any of the strings in the ^match
   column. This regular expression is referred to as the "slave" regular
   expression. Initially, all strings in both columns will be white. As you type
   your regular expression into the "type slave regex" text box, each string
   will turn green if (1) the string is in the matched column and has been
   matched, or (2) the string is in the ^matched column and has not been
   matched. If a string hasn't been matched properly then it will turn pink. If
   you successfully match all strings in both columns (all strings are green)
   then you will advance to the reveal stage.

2. Reveal: The goal of the reveal stage is to reveal the "master" regular
   expressions (there are five in total). At the start of the game, the master
   regular expressions are completely hidden. Every time you successfully
   complete the match stage you will be required to select one character
   (letter, number, symbol, or space) and if that character is contained within
   the master regular expressions then it will be revealed. This stage is
   similar to the popular American television game show, Wheel of Fortune,
   however, rather than trying to guess a phrase, you are trying to reveal the
   master regular expressions. Characters available for guessing are listed in
   green but will turn pink once they have been selected. Revealing most, if not
   all, of the master regular expressions will make the decrypt stage much
   easier.

3. Decrypt: The decrypt stage includes a random block of text - the cyphertext.
   There are five rows of cyphertext numbered one to five. To decrypt the
   cyphertext you will need to apply each master regular expression (also
   numbered one to five) with it's corresponding row of cyphtertext (e.g. one
   with one, three with three, five with five). The solution is determined by
   concatenating the cyphertext characters matched by each of the master regular
   expressions. So for example, if the first master regular expression matches
   "Reg", the second matches "ex", the third matches " is", the fourth matches "
   ha", and the fifth matches "rd!", then the solution would be "Regex is
   hard!". Note that, capitalization and white space are important so pay
   attention to these details. When you know the overall solution, type it into
   the "type solution" text box and click the "submit solution" button. You are
   only provided three attempts so don't submit until you are confident you know
   the solution!

The rules of Recryptex can be a bit "cryptic" when you are first starting out.
That's why our robot assistant, "Clunky", will be with you every step of the
way. Clunky likes to chime in from time to time with helpful tips to keep you on
track. Unfortunately, Clunky is in need of maintenance and repairs. If you
listen closely you can here his parts squeaking and clunking. Fingers crossed
he'll last long enough for you to become a regular expression master!

Although regular expression syntax is generally compatible across languages
(having POSIX standards), different regular expression engines process regular
expressions in slightly different ways. Recryptex was implemented using the
Python regular expression engine. For comprehensive documentation about the
differences between engines please visit:
  [Regular Expression Engine Comparison Chart](https://gist.github.com/CMCDragonkai/6c933f4a7d713ef712145c5eb94a1816)


## Technical Details

Recryptex is a web application developed using HTML, CSS, JavaScript, Python,
SQL and Flask/Jinja, within the CS50 Codespace IDE. Game images were made with
GIMP. The following includes a general description of how the game was 
developed, categorized according to the web application files.

### `app.py`
This is the web application Python file. Standard libraries are imported for
CS50 and Flaks functionality, as well as the modules randint (random) for
generating random numbers, jsonify (flask) for returning json objects, as well
as the re module for testing user inputted regular expressions. This file
includes five routes:
1. after_request: ensures responses aren't cached
2. index: displays the game screen and loads data from the SQL database
3. regex: retrieves and process user inputted regex text
4. reload: reloads new game data
5. help: loads the help screen

### `layout.html`
The master html file. The index and layout html files expand the layout file.

### `index.html`
The main game screen. The file is separated into the three gameplay stages:
1. Match: The match and no match lists are populated in the left two columns
   and Clunky is added on the right side column. The game is intended to be
   played on a desktop, however, columns were created responsively for
   mobile play. There is also a text field where the user inputs text. An
   asynchronous function is used with fetch to return the user input so that
   the input can be checked with the match and nomatch lists so that the
   lists can be modified dynamically (via the regex route). If the user
   input was successful then the user is prompted to click a button in the
   reveal stage.
2. Reveal: The master regex components are populated in the left column and
   a series of character buttons are loaded in the right column. When the
   user clicks on a button, the button character is revealed if it is
   present in the master regex. A series of other events are also initiated
   including the reload route which loads new match and no match lists so
   that the user can test a new slave regular expression on the new lists.
3. Decrypt: The cyphertext components are loaded. Below the cyphertext, the
   number of attempts the user has remaining is shown, as well as a text
   field for the user to input a solution with an associated submit button.
   When the user submits a solution, the user inputted solution is checked
   against the database solution. If the user is correct then a
   congratulatory message is loaded and the user is prompted to play a new
   game. If the user is incorrect then the user is prompted to try a new
   slave regular expression and the number of attempts remaining is
   decremented by one. If the user is incorrect three times then the game
   ends and the user is prompted to try again.

### `help.html`
The help screen. General information about what the game is and how to play it
are presented. This page is static and has no associated JavaScript.

### `styles.css`
The CSS stylesheet.

### `requirements.txt`
The required libraries.

### `images.png`
Muliple images are included in the static folder and are loaded to the game
screen according to gameplay events. A favicon image is included.

### `recryptex.db`
An SQL database that includes the following tables:
1. slave: the slave regex
2. s_active: the currently active slave regex
3. master: the master regex, cyphertext and solution
4. m_active: the currently active master regex, cyphertext and solution

### `slave.csv`
A csv file that stores the slave regex data.This file is imported into the
recryptex.db, slave table.

### `master.csv`
A csv file that stores the master regex, cyphertext and solution data.This file
is imported into the recryptex.db, master table.
