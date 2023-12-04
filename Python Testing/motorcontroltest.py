from serial import Serial
import time
import keyboard


arduino = Serial(port='COM5', baudrate=9600, timeout=0)
pressedLastFrame = {"up": False, "right": False, "down": False, "left": False, "q": False, "w": False}

def write_read(x):
    arduino.write(bytes(x, 'ascii'))
    print(x)
    return
    # while True:
    #     time.sleep(0.2)
    #     data = arduino.read()
    #     if data != b"":
    #         print(data)
    #         return data

while True:
    for i,key in enumerate(pressedLastFrame.keys()):
        if keyboard.is_pressed(key):
            pressedLastFrame[key] = True
        elif pressedLastFrame[key]:
            pressedLastFrame[key] = False
            print(i+35)
            write_read(chr(i + 35))
    time.sleep(0.001)
    data = arduino.read()
    if data != b"":
        print(data,end=" ")
        print(" ")
    else:
        print(" ", end="")