import sqlite3
songs_db = '__HOME__/final_project/songs.db'

def request_handler(request):
    conn = sqlite3.connect(songs_db)  # connect to that database (will create if it doesn't already exist)
    c = conn.cursor()  # make cursor into database (allows us to execute commands)
    outs = ""
    c.execute('''CREATE TABLE IF NOT EXISTS songs_table (name text, song_index int, note_list text, note_length int, song_length int);''') # run a CREATE TABLE command

    if request['method'] == 'POST':
        form = request['form']
        # c.execute('''INSERT OR REPLACE INTO test_table VALUES (?,?);''', (form['user'], form['score']))
        c.execute('''INSERT OR IGNORE INTO songs_table VALUES (?, ?, ?, ?, ?)''', (form['name'], form['song_index'], form['note_list'], form['note_length'], form['song_length']))
        c.execute('''UPDATE songs_table SET note_list = ?, note_length = ?, song_length = ? WHERE name=?''', (form['note_list'], form['note_length'], form['song_length'], form['name']))

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
        if request['values']['name'] == "ALL":
            things = c.execute('''SELECT name FROM songs_table;''').fetchall()
            return things
        else:
            things = c.execute('''SELECT * FROM songs_table;''').fetchall()
            outs = "Things:\n"
            conn.commit() # commit commands
            conn.close() # close connection to database
            for x in things:
                outs += str(x)+ "\n"
                if x[0] == request['values']['name']:
                    return (str(x[1]) + ';' + str(x[2]) + ';' + str(x[3]))
                # if x[0] == request['values']['user']:
                #     return (str(x[1])+','+str(x[2])+'.');
            # return "0,0."
        return outs