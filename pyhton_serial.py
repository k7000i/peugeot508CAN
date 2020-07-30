import serial
import sys

port_dest = "/dev/cu.wchusbserial1420"
print >> sys.stderr, "Starting port %s" % port_dest
#port = serial.Serial(port_dest, baudrate=76800)
port = serial.Serial(port_dest, baudrate=19200)


while True:
	a = port.read(1)

	#d = int(a.encode('hex'),16)
	#sys.stderr.write('{:#010b}({:02X}) '.format(d, d))

	#sys.stderr.write('{:02X}'.format(d))
	sys.stderr.write(a)
