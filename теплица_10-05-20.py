from tkinter import *
from tkinter import ttk
from time import sleep
import threading, serial
import sys, glob

def read():
    """Function for reading data from Arduino
    :return: data string from Arduino"""
    byte = b''
    while byte == b'':
        byte = ser.read()
    current = ord(byte.decode())
    print("Receive: " + str(current))
    #mas.append(current)
    return current

def data():
    root.update_idletasks()
    send(chr(1)) #Получаем значение температуры 1го датчика
    print('send 1')
    sleep(0.01)
    temp=read()
    #print(type(temp), end=' ')
    label_temp_1['text']=temp

    send(chr(2)) #Получаем значение температуры 2го датчика
    print('send 2')
    sleep(0.01)
    temp=read()
    #print(type(temp),end=' ')
    label_temp_2['text']=temp

    send(chr(3)) #Получаем значение влажности 1го датчика
    print('send 3')
    sleep(0.01)
    hum=read()
    #print(type(hum),end=' ')
    label_hum_1['text']=hum*10

    send(chr(4)) #Получаем значение влажности 2го датчика
    print('send 4')
    sleep(0.01)
    hum=read()
    #print(type(hum),end=' ')
    label_hum_2['text']=hum*10

    """send(chr(5)) #Получаем значение давления 1го датчика
    sleep(0.01)
    pres=read()
    #print(type(pres))
    label_pres_1['text']=pres*10

    send(chr(6)) #Получаем значение давления 1го датчика
    sleep(0.01)
    pres=read()
    #print(type(pres))
    label_pres_2['text']=pres*10"""

    root.after(500,data) #Перезапускаем функцию каждые 500 мил.сек.
    

def send(message):
    """Function for sending data to Arduino"""
    ser.write(message.encode("utf-8"))

#Функция формирует список доступных COM портов в ОС--------------------------------------
def serial_ports():
    """Формирует список доступных COM портов в системе"""

    global ser #Определяем глобальную переменную для работы с COM портом
    
    #Определяем операционную систему
    if sys.platform.startswith('win'): #ОС Winows
        #Генератор списка-создаём список из всех возможных портов для ОС Windows
        ports = ['COM%s' % (i + 1) for i in range(256)] 
    elif sys.platform.startswith('linux'): #ОС Linux
        # this excludes your current terminal "/dev/tty"
        ports = glob.glob('/dev/tty[A-Za-z]*')
    else: #Неизвестная ОС
        raise OSError('Неизвестная ОС')

    result = [] #Создаём пустой список для COM портов

    #Перебираем все возможные варианты COM портов и пробуем их открыть
    for port in ports:
        try: 
            s = serial.Serial(port) #Инициализируем порт
            s.close() #Закрываем порт
            result.append(port) #Если порт нормально открылся (без исключений) заносим его в список
        except (OSError, serial.SerialException): 
            pass #Ошибки с инициализацией порта игнорируем и не заносим его номер в список

    win = Toplevel(root,bg='yellow',bd=5) # создать новое окно верхнего уровня
    win.title('Выбор COM порта') #Заголовок окна
    win.minsize(150,100) #Определение размера окна
    com_var = IntVar()#Создание переменной для радиокнопок
    com_var.set(0) #Присвоение начального значения переменной
    #Созжание и размещение виджета заголовка для выбора COM порта 
    Label(win,text='Выберите COM порт', font="Arial 20",bg='yellow').pack(padx=10,pady=10)

    n=0 #Создание переменной по которой будет присваиваться значение переменной для радиокнопок
    for com in result: #Создание и размещение виджетов радиокнопок для выбора COM порта
        Radiobutton(win,text=com, variable=com_var, value=n,bg='yellow').pack(anchor=W,padx=10,pady=10)
        n+=1 #Увеличение значения переменной
    Button(win, text='Выбрать', command=win.destroy).pack() # установить обработчик

    win.focus_set()    # передать фокус на созданное окно,
    win.grab_set()     # запретить доступ к др. окнам, пока открыт диалог
    win.wait_window()  # ждать, пока win не будет уничтожен
   
    bm_print.focus_set() # передаём фокус на закладку чтения показания датчиков

    #Открываем выбранный COM порт на обмен
    ser = serial.Serial(result[com_var.get()], 9600, timeout=0) 
#END serial_ports()-----------------------------------------------------------------------


#Основная программа-----------------------------------------------------------------------

#Создание основного окна
root = Tk()
root.title('Управление теплицей') #Заголовок окна
root.geometry('800x480') #Определение размера окна
root.minsize(800,400) #Определение минимального размера окна

#Создание в окне закладок
bookmark = ttk.Notebook(root)  #Создание виджета Notebook
bm_print = ttk.Frame(bookmark) #Создание виджета Frame для первой закладки
bm_input = ttk.Frame(bookmark) #Создание виджета Frame для второй закладки
bm_settings = ttk.Frame(bookmark)

#Добавление закладок 
bookmark.add(bm_print, text='Вывод данных')
bookmark.add(bm_input, text = "Ввод данных")
bookmark.add(bm_settings, text='Настройки')

#Загрузка картинок для лэйблов
photo_temp=PhotoImage(file=r'Температура1.png')
photo_hum=PhotoImage(file=r'Влажность1.png')
photo_pres=PhotoImage(file=r'Давление1.png')

#Размещение заголовков для данных по датчикам температуры-влажности-давления
label_1 = Label(bm_print, text = 'Датчик №1',font='Arial 24')
label_1.grid(row=0,column=1, padx=20, pady=20)
label_2 = Label(bm_print, text = 'Датчик №2', font='Arial 24')
label_2.grid(row=0,column=2,padx=20,pady=20)
label_3 = Label(bm_print,text = 'Температура',font='Arial 24',fg='blue',
                image=photo_temp,compound=LEFT)
label_3.grid(row=1,column=0,padx=20,pady=20,sticky=W)
label_4 = Label(bm_print,text = 'Влажность',font='Arial 24',fg='blue',
                image=photo_hum,compound=LEFT)
label_4.grid(row=2,column=0,padx=20,pady=20,sticky=W)
label_5 = Label(bm_print,text = 'Давление',font='Arial 24',fg='blue',
                image=photo_pres,compound=LEFT)
label_5.grid(row=3,column=0,padx=20,pady=20,sticky=W)

#Размещение данных по датчикам температуры-влажности-давления в первой закладке
#Датчика №1
label_temp_1 = Label(bm_print,font='Arial 24')
label_temp_1.grid(row=1,column=1,padx=20,pady=20)
label_hum_1 = Label(bm_print,font='Arial 24')
label_hum_1.grid(row=2,column=1,padx=20,pady=20)
label_pres_1 = Label(bm_print,font='Arial 24')
label_pres_1.grid(row=3,column=1,padx=20,pady=20)
#Датчика №2
label_temp_2 = Label(bm_print,font='Arial 24')
label_temp_2.grid(row=1,column=2,padx=20,pady=20)
label_hum_2 = Label(bm_print,font='Arial 24')
label_hum_2.grid(row=2,column=2,padx=20,pady=20)
label_pres_2 = Label(bm_print,font='Arial 24')
label_pres_2.grid(row=3,column=2,padx=20,pady=20)

bookmark.pack(expand=1, fill='both')  #Размещение Notebook в главном окне

bm_print.bind('<FocusIn>',lambda event: data())
bm_input.bind('<FocusIn>',lambda event: print("2"))
#Вызов функции выбора COM порта
root.after(10, serial_ports)

root.mainloop() #Запуск обработчика событий
