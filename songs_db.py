import sqlite3
songs_db = '__HOME__/final_project/songs.db'

def request_handler(request):
    conn = sqlite3.connect(songs_db)  # connect to that database (will create if it doesn't already exist)
    c = conn.cursor()  # make cursor into database (allows us to execute commands)
    outs = ""
    c.execute('''CREATE TABLE IF NOT EXISTS songs_table (name text, song_index int, note_list text);''') # run a CREATE TABLE command

    if request['method'] == 'POST':
        form = request['form']
        # c.execute('''INSERT OR REPLACE INTO test_table VALUES (?,?);''', (form['user'], form['score']))
        c.execute('''INSERT OR IGNORE INTO songs_table VALUES (?, ?, ?)''', (form['name'], form['song_index'], form['note_list']))
        c.execute('''UPDATE songs_table SET note_list = ? WHERE name=? AND song_index=?''', (form['note_list'], form['name'], form['song_index']))

        things = c.execute('''SELECT * FROM songs_table;''').fetchall()
        outs = "Things:\n"
        for x in things:
            outs+=str(x)+"\n"
        conn.commit() # commit commands
        conn.close() # close connection to database
        return outs
        # for x in things:
        #     if x[0] == form['user']:
        #         return (str(x[1])+','+str(x[2])+'.');
        # return "0,0."
    if request['method'] == 'GET':
        # if request['values'][song_index] == -1:
        #     things = c.execute('''SELECT name FROM songs_table;''').fetchall()
        #     return things
        # else:
        if 'name' not in request['values']:
            if request['values']['song_index'] == str(-1):
                things = c.execute('''SELECT DISTINCT song_index FROM songs_table ORDER BY song_index ASC;''').fetchall()
                outs = ""
                conn.commit() # commit commands
                conn.close() # close connection to database
                for x in things:
                    outs+=str(x[0])
                    outs+=" "
                return outs
            elif int(request['values']['song_index'])>-1: #a specific song index
                things = c.execute('''SELECT DISTINCT name FROM songs_table WHERE song_index=? ORDER BY name ASC;''', (int(request['values']['song_index']),)).fetchall()
                outs = ""
                conn.commit() # commit commands
                conn.close() # close connection to database
                for x in things:
                    outs+=str(x[0])
                    outs+=" "
                return outs
        else:
            things = c.execute('''SELECT note_list FROM songs_table WHERE song_index=? AND name=?;''', (int(request['values']['song_index']), request['values']['name'])).fetchall()
            conn.commit() # commit commands
            conn.close() # close connection to database
            for x in things:
                return str(x[0])