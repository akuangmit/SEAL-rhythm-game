import sqlite3
scores_db = '__HOME__/final_project/scores.db'

def request_handler(request):
    conn = sqlite3.connect(scores_db)  # connect to that database (will create if it doesn't already exist)
    c = conn.cursor()  # make cursor into database (allows us to execute commands)
    outs = ""
    c.execute('''CREATE TABLE IF NOT EXISTS scores_table (user text, song_index int, high_score int, song_author text);''') # run a CREATE TABLE command

    if request['method'] == 'POST':
        form = request['form']
        if form['user'] == "overwrite":
            c.execute('''DELETE FROM scores_table WHERE song_index = ? AND song_author = ?''', (int(form['song_index']), form['song_author']))
            conn.commit() # commit commands
            conn.close() # close connection to database
        else:
            currentHighScore = int(form['high_score'])
            result= c.execute('''SELECT high_score FROM scores_table WHERE user = ? AND song_index = ? AND song_author = ?''',(form['user'], form['song_index'], form['song_author']))
            oldHighScore = None
            for x in result:
                oldHighScore = int(x[0])
            if oldHighScore is None:
                c.execute('''INSERT OR IGNORE INTO scores_table VALUES (?, ?, ?, ?)''', (form['user'], form['song_index'], form['high_score'], form['song_author']))
            elif currentHighScore>oldHighScore:
                c.execute('''UPDATE scores_table SET high_score = ? WHERE user =? AND song_index = ? AND song_author = ?''', (currentHighScore, form['user'], form['song_index'], form['song_author']))

            # c.execute('''INSERT OR IGNORE INTO scores_table VALUES (?, ?, ?)''', (form['user'], form['song'], form['high_score']))
            
            # print(oldHighScore)
            # print(currentHighScore)
            # print(currentHighScore>oldHighScore)
            

            
            # if currentHighScore>oldHighScore:
            #     c.execute('''UPDATE scores_table SET high_score = ? WHERE user =?''', (currentHighScore, user))
            # c.execute('''INSERT OR IGNORE INTO scores_table VALUES (?, ?, ?)''', (form['user'], form['song'], form['high_score']))

            # c.execute('''UPDATE songs_table SET correct = ?  WHERE user=?''', (form['correct'], form['user']))
            # c.execute('''UPDATE songs_table SET wrong = ?  WHERE user=?''', (form['wrong'], form['user']))

            things = c.execute('''SELECT * FROM scores_table;''').fetchall()
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
        querySongIndex = int(request['values']['song_index'])
        songAuthor = request['values']['song_author']
        # print(type(querySongIndex))
        things = c.execute('''SELECT * FROM scores_table WHERE song_index = ? AND song_author = ? ORDER BY high_score DESC LIMIT 1''', (querySongIndex, songAuthor)).fetchall()
        outs = ""
        conn.commit() # commit commands
        conn.close() # close connection to database
        for x in things:
            outs += str(x)+ "\n"
            if x[1] == querySongIndex:
                return str(x[2]) + " by player " + str(x[0])  
            # if x[0] == request['values']['user']:
            #     return (str(x[1])+','+str(x[2])+'.');
        # return "0,0."
        if outs == "":
            return "0"
        return outs