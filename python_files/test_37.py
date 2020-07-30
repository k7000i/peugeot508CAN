import serial
import sys
import time

port_dest = "/dev/cu.wchusbserial1420"
print("Starting port %s" % port_dest)
port = serial.Serial(port_dest, baudrate=1000000)

msg = ""

period = 0

while True:
    a = port.read(1)
    a = a.hex()

    if a == '55':
        b = port.read(1).hex()
        if b == 'a8':
            c = port.read(1).hex()
            if c == 'bb':
                #End Packet
                print(msg)
            msg = ""

    msg += a


while True:
    a = port.read(1)
    a = a.hex()

    if a == '55':
        b = port.read(1).hex()
        if b == 'a8':
            #End Packet
            asd = msg[2:]
            if asd[0:4] == '01a1':
                print(asd, time.time() - period)
            msg = ""
            period = time.time()
    msg += a

    #d = int(a.encode('hex'),16)
    #sys.stderr.write('{:#010b}({:02X}) '.format(d, d))
    #print('{:02X}'.format(d))
#sys.stderr.write('%s' % a.encode('hex'))