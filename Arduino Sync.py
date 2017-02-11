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


def sync():
    ser = serial.Serial('COM9', 9600)
    ser.readline()
    ser.write(str(int(time() + 36000)))
    ser.write(str(temperature))
    ser.write(str(weather))
    ser.close()

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
    feed_source = request_page(weather_feed)
    temperature = findall('Temperature:</b> (.+?)&deg', feed_source, re.DOTALL)[0]
    weather = findall('description="(.+?)\.', feed_source, re.DOTALL)[0]

root = Tk();
root.title("Arduino Sync")
timeLabel = Label(root, text = "Initializing...",
                  font=("Helvetica", 16))
timeLabel.grid(row = 0, column = 0)
syncBtn = Button(root, text = "Sync", command = sync, font=("Helvetica", 16))
syncBtn.grid(row = 0, column = 1)

weather_search()
    
root.after(1000,timeKeep);
root.mainloop()

