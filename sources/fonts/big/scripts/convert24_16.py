import os
from struct import *

# import extract as extractor

#######################################################################
_symbols = []       # Здесь содержимое файла
_offset = 0         # Смщещение картинки относительно начала файла
_numColors = 0      # Количество цветов в картинкe
_width = 0          # Ширина картинки в пикселях
_height = 0         # Высота картинки в пикселях
_colors = []        # Цвета изображения. Они идут в таком порядке : цвет промежутков между глифами, цвет пустоты в глифах, цвет символа
_dX = 0             # Смещение между левыми верхними углами глефов по горизонтали
_dY = 0             # Смещение между левыми верхними углами глефов по вертикали
########################################################################



# Читать 16 бит из массива
def ReadHalfWord(index):
    return _symbols[index] + _symbols[index + 1] * 256

# Читатать 32 бита из массива
def ReadWord(index):
    return ReadHalfWord(index) + ReadHalfWord(index + 2) * 256 * 256

# Чтение изображения из файла
def ReadFile():
    fileName = "../bmp/digitals24_16.bmp"
    input = open(fileName, "rb")
    statinfo = os.stat(fileName)
    data = input.read()
    input.close()
    symbols = unpack(str(statinfo.st_size) + "B", data)
    return symbols

# Расчёт смещения начала пикселей
def CalculateOffset():
    return ReadWord(0x0a)

# Расчёт количества цветов в картинке
def CalculateNumColors(index):
    while index < len(_symbols):
        color = _symbols[index]
        if _colors.count(color) == 0:
            _colors.append(color)
        index += 1
    return len(_colors)

# Возвращает ширину изображения в пикселях
def GetWidthPicture():
    width = ReadWord(0x12)
    while width % 4 != 0:
        width += 1
    return width

# Возвращает высоту изображения в пикселях
def GetHeightPicture():
    return ReadWord(0x16)

# Возвращает значение пикселя в точке x, y
def GetPoint(x, y):
    return _symbols[_offset + x + (_height - y - 1) * _width]

# Возвращает True, если цвет в данной точке - пустой
def ColorIsEmpty(x, y):
    return GetPoint(x, y) == _colors[0]

# Возвращает True, если цвет в данной точке - фон
def ColorIsBack(x, y):
    return GetPoint(x, y) == _colors[1]

# Возвращает True, елси цвет в данной точке - закрашен
def ColorIsFill(x, y):
    return GetPoint(x, y) == _colors[2]

# Возвращает координату X верхнего левого угла первого слева глифа
def CalculateX0():
    for y in range(_height):
        for x in range(ReadWord(0x12)):
            if not ColorIsEmpty(x, y):
                return x
                break

# Возвращает координату Y верхенго левого угла первого слева глифа
def CalculateY0():
    for x in range(ReadWord(0x12)):
        for y in range(_height):
            if not ColorIsEmpty(x, y):
                return y
                break

# Расчёт смещения X между глефами
def CalculateOffsetX():
    x1 = CalculateX0()
    x2 = 0;
    for x in range(x1, _width):
        if ColorIsEmpty(x, 1):          # Вышли за глиф
            x2 = x
            break
    for x in range(x2, _width):
        if not ColorIsEmpty(x, 1):      # Вошли в следующий глиф
            x2 = x
            break
    return x2 - x1

# Расчёт смещения Y между глефами
def CalculateOffsetY():
    y1 = CalculateY0()
    y2 = 0;
    for y in range(y1, _height):
        if ColorIsEmpty(1, y):          # Вышли за глиф
            y2 = y
            break
    for y in range(y2, _height):
        if not ColorIsEmpty(1, y):      # Вошли в следующий глиф
            y2 = y
            break
    return y2 - y1


# Рисует левый верхний угол изображения
def Dump(sizeX, sizeY):
    for y in range(sizeY):
        for x in range(sizeX):
            color = GetPoint(x, y)
            if color == _colors[0]:
                print('8', end = '')
            if color == _colors[1]:
                print('*', end = '')
            if color == _colors[2]:
                print(' ', end = '')
        print()

##########################################################################################
##########################################################################################
# Возвращает абсолютную координату х левого верхнего угла глефа с относительной координатой x
def AbsoluteX(x):
    return CalculateX0() + x * _dX

##########################################################################################
# Возвращает абсолютную координату y левого верхнего угла глефа с относительной координатой y
def AbsoluteY(y):
    return CalculateY0() + y * _dY

##########################################################################################
# Возвращает длину строки символа в битах
def LengthRow(x, y):
    start = x                           # Начальная координата строки
    endFill = 0                         # Координата последней закрашенной точки
    endBack = 0                         # Координата последней глефа цвета фона
    
    while not ColorIsEmpty(x, y):
        if ColorIsFill(x, y):
            endFill = x
        if ColorIsBack(x, y):
            endBack = x
        x += 1
        
    return endBack - start

##########################################################################################
# Возвращает ширину символа в битах
def WidthSymbol(x, y):
    absX = AbsoluteX(x)
    absY = AbsoluteY(y)
    return 8

##########################################################################################
# Возвращает размер памяти в байтах, занимаемой символом с относительным координатами x, y
def CalculateSizeSymbol(x, y):
    absX = AbsoluteX(x)     # Рассчитываем абсолютные координаты глефа
    absY = AbsoluteY(y)

    maxSize = 0             # Размер самой длинной строки символа в битах

    for y in range (absY, absY + _dY):
        size = LengthRow(absX, y)
        if size > maxSize:
            maxSize = size

    while (maxSize % 8) != 0:
        maxSize += 1
    
    return int(maxSize * 16 / 8); # Такая формула потому, что сначала мы количестов бит мы должны разделить на 8, чтобы получить количество байт, а потом умножить на 16 (количество строк)

##########################################################################################
# Подсчёт размера памяти, занимаемой глефами с относительными координатами x[], y[]
def CalculateSizeDataFont(x, y):
    result = 0
    
    for i in range(len(x)):
        result += CalculateSizeSymbol(x[i], y[i])
        
    return result

##########################################################################################
# Записывает данные символа в файл
def WriteDataSymbol(code, file, data):
    file.write("  /* ")
    file.write(str(hex(code)))
    file.write(" */  ")
    for i in range(len(data)):
        
        #file.write(str(data[i]))
        file.write("{0:#0{1}x}".format(data[i], 4))
        
        if i != len(data) - 1:
            file.write(", ")

##########################################################################################
# Возвращает данные символа по абсолютным координатам x, y
def GetDataSymbol(absX, absY):
    rows = []
    for i in range(16):
        rows.append(i)
    return rows


##########################################################################################
# Записывает выходные данные в файл
# nameFile - имя файла
# nameFont - имя шрифта
# codes[] - коды символов, которые требуется сохранить
# x[] - координаты по горизонтали в файле изображения глефов, соотвествующих символам
# y[] - координаты по вертикали в файле изображения глефов, соответствующих символам
##########################################################################################
def WriteToFile(nameFile, nameFont, codes, x, y):
    output = open(nameFile, "w")

    # Пишем данные
    
    output.write("static const unsigned char data" + nameFont + "[")
    output.write(str(CalculateSizeDataFont(x, y)))
    output.write("] =\n")
    output.write("{\n")

    offsets = [0]                                                   # Здесь будут храниться смещения символов

    for i in range(len(x)):                                         # Теперь для всех глефов
        data = GetDataSymbol(AbsoluteX(x[i]), AbsoluteY(y[i]))      # Получаем данные глефа
        offsets.append(offsets[len(offsets) - 1] + len(data))
        WriteDataSymbol(codes[i], output, data)                     # И записываем их в файл
        if i != len(x) - 1:
            output.write(",\n")

    output.write("\n};\n\n");

    # Пишем символы

    output.write("static const BigSymbol simbols" + nameFont + "[" + str(len(codes)) + "] = \n{\n")

    for i in range(len(codes)):
        output.write("    { " + "{0:#0{1}x}".format(codes[i], 4) + ", " + str(WidthSymbol(x[i], y[i])) + ", " + "{0:#0{1}x}".format(offsets.pop(0), 6) + " }")
        if i != len(codes) - 1:
            output.write(",")
        else:
            output.write(" ")
        output.write("    //  \'" + chr(codes[i]) + "\'\n")

    output.write("\n};\n")

#    output.write("static const BigFont " + nameFont + " =\n{\n")
#    output.write("    16,\n")
#    output.write("    data" + nameFont + ",\n    {\n")
#
#    for i in range(256):
#        if i in codes:
#            output.write("        /* ")
#            output.write("{0:#0{1}x}".format(i, 4))
#            output.write(" */  ")
#            output.write("{0:#0{1}x}".format(offsets.pop(0), 6))
#        else:
#            output.write("                    0xFFFF")
#        if i != 255:
#            output.write(",\n")
#
#    output.write("\n    },\n    {\n")
#
#    for i in range(256):
        
    
    output.close()


##########################################################################################
#                                    Start here                                          #
##########################################################################################


_symbols = ReadFile()

_offset = CalculateOffset()

_numColors = CalculateNumColors(_offset)

_width = GetWidthPicture()

_height = GetHeightPicture()

_dX = CalculateOffsetX()

_dY = CalculateOffsetY()

#print("offset ", _offset)
#print("number colors ", _numColors)
#print("size picture ", _width, " x ", _height)
#print("colors - ", _colors[0], " ", _colors[1], " ", _colors[2])
#print("dX = ", _dX, ", dY = ", _dY)
# Dump(70, 151)

#         '0'   '1'   '2'   '3'   '4'   '5'   '6'   '7'   '8'   '9'
codes = [0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39]

x =     [  0,     1,    2,    3,    4,    5,    6,    7,    8,    9]

y =     [  1,     1,    1,    1,    1,    1,    1,    1,    1,    1]

WriteToFile("../out/fontDigits16.cpp", "fontDigits16", codes, x, y)
