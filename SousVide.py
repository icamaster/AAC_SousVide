#Thank you  ForgotHowToComputer@community.spiceworks.com
import serial
import csv
import sys
#This is where we save data
f = open('SoushVideData.csv','wb')
 
def scan():
    """scan for available ports. return a list of tuples (num, name)"""
    available = []
    for i in range(256):
        try:
            s = serial.Serial(i)
            available.append( (i, s.portstr))
            s.close()   # explicit close 'cause of delayed GC in java
        except serial.SerialException:
            pass
    return available
if __name__=='__main__':
    print ("Found ports:")
    for n,s in scan():
        print ("(%d) %s" % (n,s))
    #Use input instead of raw_input for python3
    selection = raw_input("Enter port number:")
    try:
        ser = serial.Serial(eval(selection), 9600, timeout=1)
        print("connected to: " + ser.portstr)
    except serial.SerialException:
        pass  
    while True:
        # Read a line and convert it from b'xxx\r\n' to xxx
        line = ser.readline().decode('utf-8')[:-1]
        if line:  # If it isn't a blank line
            print(line)
            f.write(line)
    f.close()
ser.close()
