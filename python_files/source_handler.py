from binascii import unhexlify
import re

import serial

import time


class InvalidFrame(Exception):
    pass


class SourceHandler:
    """Base class for classes reading CAN messages.
    This serves as a kind of interface for all classes reading CAN messages,
    whatever the source of these messages: serial port, text file etc.
    """

    def open(self):
        raise NotImplementedError

    def close(self):
        raise NotImplementedError

    def get_message(self):
        """Get CAN id and CAN data.
        Returns:
            A tuple containing the id (int) and data (bytes)
        Raises:
            InvalidFrame
        """
        raise NotImplementedError


class SerialHandler(SourceHandler):

    def __init__(self, device_name, baudrate=1000000):
        self.device_name = device_name
        self.baudrate = baudrate
        self.serial_device = None

    def open(self):
        self.serial_device = serial.Serial(self.device_name, self.baudrate)

    def close(self):
        if self.serial_device:
            self.serial_device.close()

    def get_message(self):
        line = self._read_until_newline()
        return self._parse(line)

    def _read_until_newline(self):
        """Read data from `serial_device` """
        msg = ""
        while True:
            a = self.serial_device.read(1)
            a = a.hex()

            if a == '55':
                b = self.serial_device.read(1).hex()
                if b == 'a8':
                    break
            msg += a
        return msg

    @staticmethod
    def _parse(line):
        # Sample frame from Arduino: FRAME:ID=246:LEN=8:8E:62:1C:F6:1E:63:63:20

        # Split it into an array
        # (e.g. ['FRAME', 'ID=246', 'LEN=8', '8E:62:1C:F6:1E:63:63:20'])
        frame = line
        try:
            frame_id = int(frame[0:4], 16)  # get the ID from the 'ID=246' string

            frame_length = int(frame[4:6],16)  # get the length from the 'LEN=8' string

            data = unhexlify(frame[6:])
        except (IndexError, ValueError) as exc:
            raise InvalidFrame("Invalid frame {}".format(line))

        if len(data) != frame_length:
            raise InvalidFrame("Wrong frame length or invalid data: {}".format(line))
        return frame_id, data


class CandumpHandler(SourceHandler):
    """Parser for text files generated by candump."""

    MSG_RE = r"\(([.0-9]+)\).* ([0-9A-F]+)\#([0-9A-F]*)"
    MSG_RGX = re.compile(MSG_RE)

    def __init__(self, file_path, speed_scale=1.0):
        self.file_path = file_path
        self.file_object = None
        if not speed_scale or speed_scale <= 0.0:
            speed_scale = 1.0
        self.speed_scale = speed_scale
        self.clock = 0

    def open(self):
        # interface name in candump file may contain non-ascii chars so we need utf-8
        self.file_object = open(self.file_path, 'rt', encoding='utf-8')

    def close(self):
        if self.file_object:
            self.file_object.close()

    def get_message(self):
        line = self.file_object.readline()
        if line == '':
            raise EOFError
        return self._parse_from_candump(line)

    def _parse_from_candump(self, line):
        line = line.strip('\n')

        msg_match = self.MSG_RGX.match(line)
        if msg_match is None:
            raise InvalidFrame("Wrong format: '{}'".format(line))

        abstime, hex_can_id, hex_can_data = msg_match.group(1, 2, 3)
        abstime = float(abstime)
        time.sleep((abstime - self.clock) / self.speed_scale)
        self.clock = abstime

        can_id = int(hex_can_id, 16)

        try:
            can_data = bytes.fromhex(hex_can_data)
        except ValueError as err:
            raise InvalidFrame("Can't decode message '{}': '{}'".format(line, err))

        return can_id, can_data