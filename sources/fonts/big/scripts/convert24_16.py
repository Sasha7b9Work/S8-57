import os
from struct import *

import extract as extractor

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


###### Start here ######



_symbols = ReadFile()

_offset = CalculateOffset()

_numColors = CalculateNumColors(_offset)

_width = GetWidthPicture()

_height = GetHeightPicture()

_dX = CalculateOffsetX()

_dY = CalculateOffsetY()

print("offset ", _offset)

print("number colors ", _numColors)

print("size picture ", _width, " x ", _height)

print("colors - ", _colors[0], " ", _colors[1], " ", _colors[2])

print("dX = ", _dX, ", dY = ", _dY)

Dump(70, 151)

extractor.WriteFile("../out/fontDigits16.cpp")
