import tkinter as tk
import serial

def move_forward():
    ser.write(b'1')
    status.set("Carro moviéndose hacia adelante")

def move_backward():
    ser.write(b'2')
    status.set("Carro moviéndose hacia atrás")

def turn_left():
    ser.write(b'3')
    status.set("Carro girando hacia la izquierda")

def turn_right():
    ser.write(b'4')
    status.set("Carro girando hacia la derecha")

def execute_sequence():
    ser.write(b'5')
    status.set("Recogiendo...")
    # Implementar aquí la lógica de la secuencia que deseas ejecutar
    pass

# Establecer conexión con el carro mediante puerto serie
ser = serial.Serial('COM5', 9600) # Reemplaza 'COM3' con el puerto correcto en tu sistema

# Crear la ventana principal de la aplicación
root = tk.Tk()
root.title("Control de Carro")

# Crear los botones y el cuadro de texto
move_forward_button = tk.Button(root, text="Adelante", command=move_forward)
move_backward_button = tk.Button(root, text="Atrás", command=move_backward)
turn_left_button = tk.Button(root, text="Izquierda", command=turn_left)
turn_right_button = tk.Button(root, text="Derecha", command=turn_right)
execute_sequence_button = tk.Button(root, text="Ejecutar secuencia", command=execute_sequence)
status = tk.StringVar()
status_label = tk.Label(root, textvariable=status)

# Posicionar los widgets en la ventana
move_forward_button.pack()
move_backward_button.pack()
turn_left_button.pack()
turn_right_button.pack()
execute_sequence_button.pack()
status_label.pack()

# Iniciar el bucle principal de la aplicación
root.mainloop()
