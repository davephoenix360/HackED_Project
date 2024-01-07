import serial.tools.list_ports
import finalApplication as fA
import time

ports = serial.tools.list_ports.comports()
serialInst = serial.Serial(timeout=1)

portList = []

for port in ports:
    portList.append(str(port))
    print(str(port))
    
val = 4

for x in range(len(portList)):
    if portList[x].startswith("COM" + str(val)):
        portVar = "COM" + str(val)
        print(portList[x])

serialInst.baudrate = 9600
serialInst.port = portVar
serialInst.open()
time.sleep(3)

checker = 0

while True and checker < 1:
    serialInst.write(b'r\n')
    #time.sleep(1)
    ORIGIN = str(serialInst.readline().decode('utf')).strip() # receive ORIGIN
    #mine = str(serialInst.readline().decode('utf')).strip()
    serialInst.write(b'r\n')
    DEST = str(serialInst.readline().decode('utf')).strip()
    directions = fA.simplify_drections(fA.get_directions(ORIGIN, DEST)) + "\n"
    #test = "ad north and move forward for 27.2, Turn left a" + "\n"
    #print(test)
    #print(test.encode('utf'))
    #time.sleep(1)
    serialInst.write(directions.encode('utf-8')) #send origin
    #time.sleep(1)
    #mine = serialInst.readline().decode('utf-8').strip() # receive directions
    #print(mine)
    #serialInst.readline() # receive origin
    
    #if mine == "directions":
        #my_directions = fA.simplify_drections(fA.get_directions("9005 112 ST NW", "9120 116 St NW T6G 2V4"))
        #serialInst.write(bytes(my_directions, 'utf'))
        #serialInst.write(bytes('My name is David.', 'utf'))
        #mine = str(serialInst.readline().decode('utf')).strip()
         
    #mine = str(serialInst.readline().decode('utf')).strip()
    #print(mine)
    
    #mine = serialInst.readline().decode('utf')
    #print(mine) #count
    checker += 1
