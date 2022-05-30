import matplotlib.pyplot as plt
import pymysql

conn=pymysql.connect(host="localhost",user="raspi_user",passwd="1234",db="new")

cur=conn.cursor()

sql="SELECT * from weather ORDER BY value1 DESC" #상위 top5 시간 및 습도 값 추출하기

time=[]
temp=[]
cur.execute(sql)

for row in cur.fetchall():
    time.append(row[0]+' '+row[1])
    temp.append(row[2])


temp=temp[:5]
time=time[:5]

plt.style.use('ggplot')
plt.rcParams["figure.figsize"] = (14,8)
plt.title('Top5 in High Temperature Period')
plt.xlabel('Temp value')
plt.barh(time,temp,color='salmon')


plt.savefig('/home/pi/LJH/flask/static/images/temp.png',dpi=50)

