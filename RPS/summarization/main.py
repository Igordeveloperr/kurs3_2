# importing libraries 
import PyPDF2
import os
import nltk
nltk.download('stopwords')
nltk.download('punkt_tab')
from nltk.corpus import stopwords 
from nltk.tokenize import word_tokenize, sent_tokenize 

# Папка с результатами
txt_folder = "./outputs/"
# Папка с PDF-файлами
pdf_folder = "./pdfs/"
# Собираем все PDF-файлы из папки
pdf_files = [f for f in os.listdir(pdf_folder) if f.endswith('.pdf')]

# Функция для извлечения текста из PDF-файла
def extract_text_from_pdf(pdf_path):
    # Открываем PDF-файл в бинарном режиме чтения ('rb')
    with open(pdf_path, 'rb') as file:
        # Создаем объект PdfReader для чтения PDF-файла
        reader = PyPDF2.PdfReader(file)
        # Инициализируем пустую строку для хранения извлеченного текста
        text = ""
        # Перебираем все страницы PDF-файла
        for page in reader.pages:
            # Извлекаем текст с текущей страницы и добавляем его к общему тексту
            text += page.extract_text()
    # Возвращаем весь извлеченный текст
    return text

# Функция для суммаризации текста на основе частоты слов
def summarize_text(text):
    # Загружаем стоп-слова для русского языка (слова, которые не влияют на смысл)
    stopWords = set(stopwords.words("russian"))
    # Разбиваем текст на отдельные слова (токенизация)
    words = word_tokenize(text)  
    # Создаем словарь для подсчета частоты слов
    freqTable = dict()  
    # Обрабатываем каждое слово в тексте
    for word in words:  
        # Приводим слово к нижнему регистру для единообразия
        word = word.lower()  
        # Пропускаем слово, если оно в списке стоп-слов
        if word in stopWords:  
            continue  
        # Если слово уже есть в таблице частот, увеличиваем счетчик
        if word in freqTable:  
            freqTable[word] += 1  
        # Иначе добавляем слово в таблицу с частотой 1
        else:  
            freqTable[word] = 1  
    # Разбиваем текст на предложения
    sentences = sent_tokenize(text)  
    # Создаем словарь для оценки значимости каждого предложения
    sentenceValue = dict()  
    # Для каждого предложения подсчитываем "вес" на основе частот слов
    for sentence in sentences:  
        for word, freq in freqTable.items():  
            # Если слово встречается в предложении, увеличиваем его значимость
            if word in sentence.lower():  
                if sentence in sentenceValue:  
                    sentenceValue[sentence] += freq  
                else:  
                    sentenceValue[sentence] = freq  
    # Вычисляем общую сумму значимостей всех предложений
    sumValues = 0  
    for sentence in sentenceValue:  
        sumValues += sentenceValue[sentence]  
    # Находим среднюю значимость предложения
    average = int(sumValues / len(sentenceValue))  
    # Формируем итоговый суммаризованный текст
    summary = ''  
    # Добавляем в результат предложения, значимость которых выше среднего на 20%
    for sentence in sentences:  
        if (sentence in sentenceValue) and (sentenceValue[sentence] > (1.2 * average)):  
            summary += " " + sentence  
    # Возвращаем суммаризованный текст
    return summary  

# запись текста в файл
def write_text_to_file(file_name, text):
    # имя файла
    txt_file = os.path.splitext(file_name)[0]
    # путь к файлу
    txt_path = os.path.join(txt_folder, f"{txt_file}.txt")
    # Записываем результат в текстовый файл
    with open(txt_path, 'w', encoding='utf-8') as f:
        f.write(out_text)

if __name__ == "__main__":
    # прогоняем все pdf файлы через суммаризацию и записываем результаты в файлы
    for pdf_file in pdf_files:
        text = extract_text_from_pdf(pdf_folder+pdf_file)
        out_text = summarize_text(text)
        write_text_to_file(pdf_file, out_text)