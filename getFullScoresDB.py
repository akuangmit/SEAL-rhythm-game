import sqlite3
scores_db = '__HOME__/final_project/scores.db'

def request_handler(request):
    conn = sqlite3.connect(scores_db)  # connect to that database (will create if it doesn't already exist)
    c = conn.cursor()  # make cursor into database (allows us to execute commands)
    outs = ""
    c.execute('''CREATE TABLE IF NOT EXISTS scores_table (user text, song_index int, high_score int, song_author text);''') # run a CREATE TABLE command
    if request['method'] == 'GET':
    	things = c.execute('''SELECT * FROM scores_table;''').fetchall()
    	outs+="user, song_index, high_score, song_author"
    	outs+='\n'
    	for x in things:
    		outs+=str(x)
    		outs+='\n'
    	return outs