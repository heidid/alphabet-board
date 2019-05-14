import serial
import time
import tkinter

arrowkeys_data = {'Up': '1', 'Down': '2', 'Left': '3', 'Right': '4'}

ArduinoSerial = serial.Serial('COM6', 9600) 
time.sleep(2)

root = tkinter.Tk()
prompt = 'READY'
label = tkinter.Label(root, text=prompt, width=len(prompt), font=("Roboto Mono", 120))
label.pack(padx=40, pady=60)

def keypress(event):
	if len(event.char) == 1:
		data = event.char
	else:
		data = arrowkeys_data[event.keysym]
	ArduinoSerial.write(data.encode())
	label.config(text=event.keysym)
	
root.bind_all('<Key>', keypress)
root.mainloop()
