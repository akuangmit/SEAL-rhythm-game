import sqlite3
songs_db = '__HOME__/final_project/songs.db'

def request_handler(request):
    conn = sqlite3.connect(songs_db)  # connect to that database (will create if it doesn't already exist)
    c = conn.cursor()  # make cursor into database (allows us to execute commands)
    outs = ""
    c.execute('''CREATE TABLE IF NOT EXISTS songs_table (name text, song_index int, note_list text);''')
    if request['method'] == 'GET':
    	things = c.execute('''SELECT * FROM songs_table;''').fetchall()
    	outs+="name, song_index, note_list"
    	outs+='\n'
    	for x in things:
    		outs+=str(x)
    		outs+='\n'
    	return outs