import os
import sys
from struct import *

#######################################################################
_heightFont = 0         # Высота шрифта
_deltaHeightFont = 0    # Расстояние от верха глифа до верхней точки символа. Одинаковое для всех глифов
_inFile = ""            # Имя файла с изображением
_outFile = ""           # Имя выходного файла
_nameFont = ""          # Имя шрифта

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
    fileName = _inFile
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
def AbsX(x):
    return CalculateX0() + x * _dX

##########################################################################################
# Возвращает абсолютную координату y левого верхнего угла глефа с относительной координатой y
def AbsY(y):
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
def WidthSymbol(absX, absY):   
    maxSize = 0             # Размер самой длинной строки символа в битах

    for y in range (absY, absY + _dY):
        size = LengthRow(absX, y)
        if size > maxSize:
            maxSize = size
            
#    print("width symbol ", absX, absY, " = ", maxSize)
    
    return maxSize

##########################################################################################
# Возвращает шириру символа в байтах
def WidthSymbolInBytes(absX, absY):
    width = WidthSymbol(absX, absY)
    while (width % 8) != 0:
        width += 1

    return int(width / 8)

##########################################################################################
# Возвращает размер памяти в байтах, занимаемой символом с относительным координатами x, y
def SizeSymbol(absX, absY):

    return WidthSymbolInBytes(absX, absY) * _heightFont
    
##########################################################################################
# Подсчёт размера памяти, занимаемой глефами с относительными координатами x[], y[]
def CalculateSizeDataFont(x, y):
    result = 0
    
    for i in range(len(x)):
        result += SizeSymbol(AbsX(x[i]), AbsY(y[i]))

    return result

##########################################################################################
# Записывает данные символа в файл
def WriteDataSymbol(code, file, data, offset):
    file.write("\n                         //  code " + str(hex(code)) + ", offset = " + str(hex(offset)) + "\n    ")
    
    count = 0
    
    for i in range(len(data)):
        file.write("{0:#0{1}x}".format(data[i], 4))
        count += 1
        if i != len(data) - 1:
            file.write(", ")
        if count == 24 and i != len(data) - 1:
            count = 0
            file.write("\n    ")

##########################################################################################
# Возвращает данные символа по абсолютным координатам x, y
def GetDataSymbol(absX, absY):

    rows = []

    for y in range(absY + _deltaHeightFont, absY + _deltaHeightFont + _heightFont):
        bits = []                            # Здесь будем хранить биты для каждой строки
        for x in range(absX, absX + WidthSymbol(absX, absY)):
            if ColorIsFill(x, y):
                bits.append(1)
            else:
                bits.append(0)
        rows.append(bits)
#        print(bits)

#       print("in symbol ", absX, ", ", absY, " ", len(rows), " rows")

# Сейчас в rows хранятся строки бит

    chars = []

    byte = 0                # Здесь текущий рассчитываемый байт
    leftBits = 8            # Осталось обработать бит в текущем байте
    
    while len(rows) != 0:
        row = rows.pop(0)
        byte = 0
        leftBits = 8
        while len(row) != 0:
            bit = row.pop(0)                # Извлекаем очередной бит
            value = bit << (8 - leftBits)   # Преобразуем его в маску
            byte |= value                   # И прибавляем к уже существующему значению байта
            leftBits -= 1                   # Уменьшаем количество оставшихся байт в данном байте
            if leftBits == 0:
                chars.append(byte)
                byte = 0
                leftBits = 8
        if leftBits != 8:
            chars.append(byte)
    

#    print("number bytes = ", len(chars))
            
    return chars


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

# Пишем данные **************************************************************************
    
    output.write("static const unsigned char data" + nameFont + "[")
    output.write(str(CalculateSizeDataFont(x, y)))
    output.write("] =\n")
    output.write("{")

    offset = 0
    offsets = [offset]                                              # Здесь будут храниться смещения символов

    for i in range(len(x)):                                         # Теперь для всех глефов
        data = GetDataSymbol(AbsX(x[i]), AbsY(y[i]))                # Получаем данные глефа
        WriteDataSymbol(codes[i], output, data, offset)             # И записываем их в файл
        offset = offsets[len(offsets) - 1] + len(data)
        offsets.append(offset)
        if i != len(x) - 1:
            output.write(",\n")

    output.write("\n};\n\n");

# Пишем символы **************************************************************************

    output.write("static const BigSymbol symbols" + nameFont + "[" + str(len(codes)) + "] = \n{\n")

    for i in range(len(codes)):
        output.write("    { " + "{0:#0{1}x}".format(codes[i], 4) + ", " + str(WidthSymbol(AbsX(x[i]), AbsY(y[i]))) + ", " + "{0:#0{1}x}".format(offsets.pop(0), 6) + " }")
        if i != len(codes) - 1:
            output.write(",")
        else:
            output.write(" ")
        output.write("    //  \'" + chr(codes[i]) + "\'\n")

    output.write("\n};\n")

# Пишем описание шрифта *****************************************************************

    output.write("\nconst BigFont " + nameFont + " =\n")
    output.write("{\n")
    output.write("    " + str(_heightFont) + ",\n")
    output.write("    " + str(len(codes)) + ",\n")
    output.write("    data" + nameFont + ",\n")
    output.write("    symbols" + nameFont + ",\n")
    output.write("};")      
    
    output.close()


##########################################################################################
#                                    Start here                                          #
##########################################################################################


_heightFont = int(sys.argv[1])
_deltaHeightFont = int(sys.argv[2])
_inFile = sys.argv[3]

_outFile = "../out/fontDigits" + str(_heightFont) + ".cpp"
_nameFont = "fontDigits" + str(_heightFont)

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

WriteToFile(_outFile, _nameFont, codes, x, y)
