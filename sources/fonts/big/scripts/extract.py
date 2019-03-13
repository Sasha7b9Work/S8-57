import os

# Записать символы в файл
def WriteFile(fileName):
    output = open(fileName, "w")
    output.write("test")
    output.close()
