import requests
import re
import console
#from lxml import html
from bs4 import BeautifulSoup

site = 'https://www.libsdl.org/'

url = site + 'download-2.0.php'

# Здесь текст html-файла
page = requests.get(url).text

#with open('page.html', 'w') as out_file:
#    out_file.write(page)
    
soup = BeautifulSoup(page, "lxml")

# Извлекаем название zip-файла
file_name = soup.find('div', {'id': 'content'}).find('div', {'class': 'col left'}).find('blockquote').find('a').get('href')

# Полное имя файла для скачивания
full_name = site + file_name

# Сюда будем сохранять файл
zip_file = open("..\\temp\\" + re.split('/', file_name)[1], 'wb')

print('Download file ' + full_name)

# Даём запрос на чтение файла
r = requests.get(full_name, stream = True)

# Узнаём размер файла
size_file = int(r.headers['content-length'])

# Размер элементарной порции чтения
size_chunk = int(size_file / 100.0)

# Сколько загружено байт
loaded_size = 0

print()

for chunk in r.iter_content(chunk_size = size_chunk):
    zip_file.write(chunk)
    loaded_size += len(chunk)
    
    print("\rDownload " + str(int(loaded_size / 1024)) + " from " + str(int(size_file / 1024)) + " kb = " + str(int(loaded_size / size_file * 100)) + '%', end='')
    
print()


## Отсюда будем считывать
#remote_file = requests.get(full_name)
#
## Записываем файл
#zip_file.write(remote_file.content)

zip_file.close();





