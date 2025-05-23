# Эволюция магнемайта

**Ограничение времени**: 5 секунд  
**Ограничение памяти**: 64Mb  
**Ввод**: стандартный ввод или `input.txt`  
**Вывод**: стандартный вывод или `output.txt`

## Условие задачи
Необходимо найти количество троек точек, образующих:
- Равнобедренные треугольники (теория 1)
- Правильные треугольники (теория 2)

## Формат ввода
1. N T - количество точек и номер теории (3 ≤ N ≤ 3000, T ∈ {1,2})
2. N пар координат (xᵢ, yᵢ)

## Формат вывода
Количество подходящих троек точек

## Примеры
**Пример 1**  
Ввод:  
```
3 2
1 1
0 0
1 0
```
Вывод:  
```
0
```

**Пример 2**  
Ввод:  
```
4 1
1 1
0 1
0 0
1 0
```  
Вывод:  
```
4
```

## Решение
### Для теории 1 (равнобедренные треугольники)
1. Для каждой точки как вершины:
   - Считаем квадраты расстояний до других точек
   - Группируем точки по расстоянию
   - Для групп размера ≥2: количество троек = C(k,2) * (кол-во других точек)

2. Оптимизация:
   - Используем хэш-таблицу для подсчета расстояний
   - Избегаем дублирования через упорядочивание точек

### Для теории 2 (правильные треугольники)
1. Для каждой пары точек:
   - Находим две возможные третьи вершины для правильного треугольника
   - Проверяем их наличие в множестве точек

2. Оптимизация:
   - Используем хэш-таблицу для быстрого поиска точек
   - Учитываем каждую тройку только один раз

**Сложность**:
- Теория 1: O(N²)
- Теория 2: O(N² log N)

**Примечание**: Для N=3000 сложность O(N²) = 9·10⁶ операций, что укладывается в ограничения.