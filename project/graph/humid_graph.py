import matplotlib.pyplot as plt
import pymysql

conn=pymysql.connect(host="localhost",user="raspi_user",passwd="1234",db="new")

cur=conn.cursor()

sql="SELECT * from weather ORDER BY value2 DESC" #상위 top5 시간 및 습도 값 추출하기

time=[]
humid=[]
cur.execute(sql)

for row in cur.fetchall():
    time.append(row[0]+' '+row[1])
    humid.append(row[3])


humid=humid[:5]
time=time[:5]

plt.style.use('ggplot')
plt.rcParams["figure.figsize"] = (14,8)
plt.title('Top5 in High Humidity Period')
plt.xlabel('Humid value')
plt.barh(time,humid,color='orange')


plt.savefig('/home/pi/LJH/flask/static/images/humids.png',dpi=50)

