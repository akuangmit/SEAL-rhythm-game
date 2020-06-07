import sqlite3
scores_db = '__HOME__/final_project/scores.db'

def request_handler(request):
    conn = sqlite3.connect(scores_db)  # connect to that database (will create if it doesn't already exist)
    c = conn.cursor()  # make cursor into database (allows us to execute commands)
    outs = ""
    c.execute('''CREATE TABLE IF NOT EXISTS scores_table (user text, song text, high_score int);''') # run a CREATE TABLE command

    if request['method'] == 'POST':
        form = request['form']
        currentHighScore = int(form['high_score'])
        result= c.execute('''SELECT high_score FROM scores_table WHERE user = ?''',(form['user'],));
        oldHighScore = None
        for x in result:
            oldHighScore = int(x[0])
        if oldHighScore is None:
            c.execute('''INSERT OR IGNORE INTO scores_table VALUES (?, ?, ?)''', (form['user'], form['song'], form['high_score']))
        elif currentHighScore>oldHighScore:
            c.execute('''UPDATE scores_table SET high_score = ? WHERE user =?''', (currentHighScore, form['user']))

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
        things = c.execute('''SELECT * FROM scores_table;''').fetchall()
        outs = "Things:\n"
        conn.commit() # commit commands
        conn.close() # close connection to database
        for x in things:
            outs += str(x)+ "\n"
            # if x[0] == request['values']['user']:
            #     return (str(x[1])+','+str(x[2])+'.');
        # return "0,0."
        return outs