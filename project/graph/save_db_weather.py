import time
import board
import adafruit_dht
import pymysql

conn=pymysql.connect(host="localhost",user="raspi_user",passwd="1234",db="new")
cur=conn.cursor()
dhtDevice=adafruit_dht.DHT11(board.D20) #GPIO 20에연결

sql="insert into weather values(%s,%s,%s,%s)"

while True:
    temp,humid=dhtDevice.temperature,dhtDevice.humidity
    print(temp,humid)
    cur.execute(sql,(time.strftime("%Y-%m-%d",time.localtime()),time.strftime("%H:%M-%S",time.localtime()),str(temp),str(humid)))
    conn.commit()
    
    time.sleep(2) #2초마다 온도 습도값 read

