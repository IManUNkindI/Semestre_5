import time
import serial
import threading
from tkinter import *

#Funciones
def centrar_ventana(Window):  
    Window.update_idletasks()
    ancho_ventana = (frame1.winfo_width()+frame2.winfo_width())
    alto_ventana = frame1.winfo_height()
    x = (Window.winfo_screenwidth() // 2) - (ancho_ventana // 2)
    y = (Window.winfo_screenheight() // 2) - (alto_ventana // 2)
    Window.geometry('{}x{}+{}+{}'.format(ancho_ventana, alto_ventana, x, y))

def BaudRate():
    BDRT = CTextBD.get()
    text = BDRT
    LabelBDT.config(text=text)

    STM32.baudrate = BDRT

def recepcion():
    try:
        j = 0
        while True:
            cadena[j] = STM32.read().decode('ascii')
            if cadena[j] == 'x':
                break
            j+=1
        muestra = ''.join(cadena[:j])
        CTextIN.config(state="normal")
        CTextIN.delete(0, "end")
        CTextIN.insert("end", muestra + '\n')
        CTextIN.config(state="disabled")
        j = 0
    except:
        CTextIN.config(state="normal")
        CTextIN.delete(0, "end")
        CTextIN.insert("end", "Error en el puerto" + '\n')
        CTextIN.config(state="disabled")
def Fhilo():
    while True:
        recepcion()

def Enviar():
    cadenaS = CTextOUT.get()
    STM32.write(cadenaS.encode('ascii'))

STM32 = serial.Serial('COM4', 1)
cadena = [None]*32
cadenaS = [None]*32


Window = Tk()
Window.title("Interfaz de comunicacion")
Window.iconbitmap("Kirby.ico")
Window.resizable(False,False)

frame1 = Frame(Window, borderwidth=2, relief="ridge")
frame2 = Frame(Window, borderwidth=2, relief="ridge")
frame1.grid(row=0, column=0)
frame2.grid(row=0, column=1, sticky="N")


LabelIN = Label(frame1, borderwidth=2, text="Mensaje entrante:")
LabelOUT = Label(frame1, borderwidth=2, text="Mensaje de salida:")
LabelAUX = Label(frame1, borderwidth=2, text="   ")
LabelBD = Label(frame1, borderwidth=2, text="Baud-Rate:")
LabelAUX1 = Label(frame2, borderwidth=2, text="   ")
LabelBDT1 = Label(frame2, borderwidth=2, text="Baud-Rate:")
LabelBDT = Label(frame2, borderwidth=2, text="Configurar")
LabelIN.grid(row=0, column=0, padx=5, pady=5)
LabelOUT.grid(row=1, column=0, padx=5, pady=5)
LabelAUX.grid(row=2, column=0, padx=5, pady=5)
LabelBD.grid(row=3, column=0, padx=5, pady=2)
LabelAUX1.grid(row=2, column=0, padx=5, pady=5)
LabelBDT1.grid(row=4, column=0,padx=5, pady=5, sticky="W")
LabelBDT.grid(row=5, column=0,padx=5, pady=5, sticky="W") 

CTextIN = Entry(frame2, borderwidth=2, width=50, state="readonly")
CTextOUT = Entry(frame2, borderwidth=2, width=50)
CTextBD = Entry(frame1, borderwidth=2, justify="center")
CTextIN.grid(row=0, column=0, padx=5, pady=5)
CTextOUT.grid(row=1, column=0, padx=5, pady=5)
CTextBD.grid(row=4, column=0, padx=5, pady=5)

SEND = Button(frame2, borderwidth=2, text="Enviar", command=Enviar, cursor="hand2", width=10)
BD = Button(frame1, borderwidth=2, text="Configurar", command=BaudRate, cursor="hand2")
SEND.grid(row=1, column=1, padx=5, pady=5)
BD.grid(row=5, column=0, padx=5, pady=5)


centrar_ventana(Window)

hilo = threading.Thread(target=Fhilo)
hilo.daemon = True
hilo.start()


Window.mainloop()
STM32.close()
