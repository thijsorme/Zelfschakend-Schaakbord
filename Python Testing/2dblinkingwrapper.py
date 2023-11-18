from serial import Serial
import time


arduino = Serial(port='COM5', baudrate=9600, timeout=0)

       

def write_read(x):
    arduino.write(bytes(x, 'utf-8'))
    while True:
        time.sleep(0.2)
        data = arduino.read()
        if data != b"":
            print(data)
            return data

def is_valid_move(move):
    if not isinstance(move,str): return False
    if len(move) != 4: return False
    if not move.isnumeric(): return False
    if "0" in move or "2" in move: return False
    return True

while True:
    num = input("Enter a move: ")
    if is_valid_move(num):
        value = int(write_read(num))
        print(value) # printing the value
    else:
        print("Invalid move\n")