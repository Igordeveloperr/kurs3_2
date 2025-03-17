from str_modul import number_of_words, number_of_wordsC
import random
import string
import time

def generate_random_text(num_words):
    # Список случайных слов
    words = []
    for _ in range(num_words):
        # Генерация случайного слова длиной от 3 до 10 символов
        word_length = random.randint(3, 10)
        word = ''.join(random.choice(string.ascii_lowercase) for _ in range(word_length))
        words.append(word)
    
    # Объединение слов в текст с пробелами
    text = ' '.join(words)
    return text

text = generate_random_text(1000000)

start_time = time.time()
number_of_words(text)
end_time = time.time()

execution_time = end_time - start_time  # Вычисляем время выполнения
print(f"Функция C++ за {execution_time:.4f} c")

start_time = time.time()
# number_of_wordsC(text)
end_time = time.time()

execution_time = end_time - start_time  # Вычисляем время выполнения
print(f"Функция Python за {execution_time:.4f} c")