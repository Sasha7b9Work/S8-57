import requests
import re
from bs4 import BeautifulSoup

#-----------------------------------------------------------------------------------
def ExtractName(soup):
    return soup.find('div', {'id': 'content'}).find('div', {'class': 'col left'}).find('blockquote').find('a').get('href')


#-----------------------------------------------------------------------------------
def GetNameFile(url):
    
    # Здесь текст html-файла
    page = requests.get(url).text
    
    #with open('page.html', 'w') as out_file:
    #    out_file.write(page)
        
    soup = BeautifulSoup(page, "lxml")
    
    # Извлекаем название zip-файла
    file_name = ExtractName(soup)
    
    return file_name

#-----------------------------------------------------------------------------------    
def GetFile(src, dest):

    # Сколько загружено байт
    loaded_size = 0
    
    # Сюда будем сохранять файл
    zip_file = open(dest, 'wb')
    
    # Даём запрос на чтение файла
    r = requests.get(src, stream = True)
    
    # Узнаём размер файла
    size_file = int(r.headers['content-length'])
    
    # Размер элементарной порции чтения
    size_chunk = int(size_file / 100.0)
      
    print()
    
    for chunk in r.iter_content(chunk_size = size_chunk):
        zip_file.write(chunk)
        loaded_size += len(chunk)
        
        print("\rDownloaded " + str(int(loaded_size / 1024)) + " kb from " + str(int(size_file / 1024)) + " kb = " + str(int(loaded_size / size_file * 100)) + '%', end='')
        
    print()
    
    zip_file.close();




#-----------------------------------------------------------------------------------
site = 'https://www.libsdl.org/'

url = site + 'download-2.0.php'

file_name = GetNameFile(url)

# Полное имя файла для скачивания
full_name = site + file_name

print('Download file ' + full_name)

GetFile(full_name, "..\\temp\\" + re.split('/', file_name)[1])




