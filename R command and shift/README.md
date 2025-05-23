# Команда R и сдвиг

**Ограничение времени**: 1 секунда  
**Ограничение памяти**: 32Mb  
**Ввод**: стандартный ввод или `input.txt`  
**Вывод**: стандартный вывод или `output.txt`

## Условие задачи
Необходимо реализовать систему, которая:
1. Добавляет слова в словарь (`+ слово`)
2. Проверяет наличие похожего слова (`? слово`)

Слова считаются похожими, если их буквенные шаблоны идентичны (одинаковые буквы заменяются на одинаковые, разные - на разные).

## Формат ввода
1. q - количество запросов (1 ≤ q ≤ 10⁵)
2. q запросов:
   - `+ слово` - добавить
   - `? слово` - проверить

Суммарная длина слов L ≤ 10⁶

## Формат вывода
Для каждого `?` вывести `YES` (если есть похожее) или `NO`

## Пример
**Ввод**  
```
5
? cdc
+ aaa
? cdc
+ aba
? cdc
```

**Вывод**  
```
NO
NO
YES
```

## Решение
1. **Нормализация слов**:
   - Заменяем каждую букву на порядковый номер ее первого вхождения
   - Например: "cdc" → "0 1 0", "aba" → "0 1 0"

2. **Хранение**:
   - Используем `unordered_set` для хранения нормализованных форм

3. **Алгоритм**:
   - Для добавления: вычисляем нормализованную форму и добавляем в множество
   - Для проверки: вычисляем нормализованную форму и ищем в множестве

**Сложность**:
- O(L) на обработку всех запросов (линейная от суммарной длины слов)
- O(1) в среднем на каждый запрос

**Оптимизации**:
- Используем один проход для нормализации
- Минимизируем аллокации памяти

**Пример нормализации**:
- "engineering" → "0 1 2 3 4 5 0 2 1 2"
- "hello" → "0 1 2 2 3"
- "test" → "0 1 2 0"