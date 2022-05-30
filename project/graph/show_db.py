import pymysql


conn=pymysql.connect(host="localhost",user="raspi_user",passwd="1234",db="new")
cur=conn.cursor()
sql="select * from weather"
cur.execute(sql)
for row in cur.fetchall():
    print(row[0],row[1],row[2],row[3])
conn.close()