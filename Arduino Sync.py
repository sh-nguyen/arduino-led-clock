from time import *
from Tkinter import *
import serial


def sync():
    ser = serial.Serial('COM9', 9600)
    ser.readline()
    ser.write(str(int(time() + 36000)))
    ser.close()

root = Tk();
root.title("Arduino Sync")
timeLabel = Label(root, text = "Initializing...",
                  font=("Helvetica", 16))
timeLabel.grid(row = 0, column = 0)
syncBtn = Button(root, text = "Sync", command = sync, font=("Helvetica", 16))
syncBtn.grid(row = 0, column = 1)

def timeKeep():
    timeLabel['text'] = "Current time is : " + strftime("%I:%M:%S %p", localtime());
    root.after(1000,timeKeep)
    
root.after(1000,timeKeep);
root.mainloop()

