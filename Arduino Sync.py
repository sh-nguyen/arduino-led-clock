from time import *
from Tkinter import *
from re import findall
from urllib import urlopen
import serial

weather_feed = "http://rss.weather.com.au/qld/brisbane"
temperature = 00
weather = ""
##Sunny
##Possible Shower
##Showers

def timeKeep():
    timeLabel['text'] = "Current time is : " + strftime("%I:%M:%S %p", localtime());
    root.after(1000,timeKeep)
    
def request_page(url):
    try:
        source = urlopen(url)
        feed_source = source.read()
        source.close()
        return feed_source
        print 'done'
    except IOError:
        return False

def weather_search():
    global temperature
    global weather

    feed_source = request_page(weather_feed)
    temperature = findall('Temperature:</b> (.+?)&deg', feed_source, re.DOTALL)[0]
    weather = findall('description="(.+?)\.', feed_source, re.DOTALL)[0]
   
    
def syncTime():
    global temperature
    global weather
    
    ser = serial.Serial('COM9', 9600)
    weather_search()

    sleep(2)
    ser.write(str(int(time() + 36000)))
    sleep(2)
    ser.write(str(int(float(temperature))))
    sleep(2)
    ser.write(weather)
    ser.close()
    
root = Tk();
root.title("Arduino Sync")
timeLabel = Label(root, text = "Initializing...",
                  font=("Helvetica", 16))
timeLabel.grid(row = 0, column = 0)
syncBtn = Button(root, text = "Sync", command = syncTime, font=("Helvetica", 16))
syncBtn.grid(row = 0, column = 1)

root.after(1000,timeKeep);
root.mainloop()

