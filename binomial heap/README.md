# Биномиальная куча

## Условие задачи
Требуется реализовать структуру данных "Биномиальная куча" с поддержкой следующих операций:

### Формат ввода
Первая строка содержит:
- `n` - количество куч (1 ≤ n ≤ 1000)
- `q` - количество запросов (1 ≤ q ≤ 10^6)

Последующие `q` строк содержат запросы в формате:
1. `0 a x` - добавить элемент `x` в кучу `a`
2. `1 a b` - переместить все элементы из кучи `a` в кучу `b` (куча `a` станет пустой)
3. `2 i` - удалить элемент с индексом `i`
4. `3 i x` - изменить значение элемента `i` на `x`
5. `4 a` - вывести минимальный элемент в куче `a`
6. `5 a` - удалить минимальный элемент из кучи `a`

### Формат вывода
Для каждого запроса типа `4` вывести значение минимального элемента.

## Ограничения
- Время: 1 секунда (Java - 3 секунды)
- Память: 256 МБ (Java - 512 МБ)
- Значения элементов: 1 ≤ x ≤ 10^9

## Пример
### Ввод:
```
3 19
0 1 10
4 1
0 2 5
0 2 7
4 2
3 2 20
4 2
1 2 1
4 1
5 1
4 1
3 2 3
4 1
2 2
4 1
0 1 9
1 1 3
0 3 8
4 3
```

### Вывод:
```
10
5
7
7
10
3
10
8
```

## Требования к реализации
1. **Структура данных**:
   - Реализовать биномиальную кучу с поддержкой всех указанных операций
   - Не использовать встроенные библиотечные реализации

2. **Производительность**:
   - Операция поиска минимума (`4`) должна выполняться за O(1)
   - Операции слияния куч (`1`) должны выполняться за O(log N)

3. **Дополнительные условия**:
   - При равных значениях элементов выбирать с минимальным индексом
   - Гарантируется корректность запросов (например, при удалении элемент существует)

## Рекомендации
1. Для эффективного поиска элементов по индексу использовать дополнительную хеш-таблицу
2. Реализовать уменьшение ключа (для операции `3`) с сохранением свойств кучи
3. Для операции слияния использовать стандартный алгоритм объединения биномиальных куч
4. Учитывать возможность дублирования значений элементов