import os

####################################################################################
# Записать символы в файл
# fileName - файл, куда будут записаны данные
# nameFont - имя шрифта
# bmpData - данные файла .bmp
# offset - смещение начала картинки от 
####################################################################################
def WriteFile(fileName, nameFont, bmpData):
    output = open(fileName, "w")
    output.write("unsigned char data" + nameFont)
    output.close()
