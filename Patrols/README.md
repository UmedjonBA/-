# Патрули

**Ограничение времени**: 2 секунды  
**Ограничение памяти**: 256Mb  
**Ввод**: стандартный ввод или `input.txt`  
**Вывод**: стандартный вывод или `output.txt`  

**В этой задаче запрещено использовать `map`, `set` и аналогичные встроенные структуры.**

## Условие задачи

Хоббс организовал патрулирование улицы с помощью N группировок, каждая из которых контролирует свой отрезок улицы. Отрезки не пересекаются и покрывают всю улицу. Необходимо обрабатывать два типа событий:

1. **Увольнение группировки** (тип 1)  
   - Управляемый отрезок переходит к соседям
   - Если сосед один — он получает весь отрезок
   - Если соседей два — отрезок делится пополам (с учетом четности длины) и распределяется между соседями

2. **Разделение участка** (тип 2)  
   - Отрезок делится на две части:
     - Четная длина: две равные половины
     - Нечетная длина: части различаются на 1 (меньшая ближе к началу улицы)

После каждого события нужно вычислить сумму квадратов длин всех текущих отрезков.

## Формат ввода
- Первая строка: N (количество патрулей) и p (номер подзадачи, игнорируется)
- Вторая строка: a₁, a₂, ..., aₙ — исходные длины отрезков
- Третья строка: Q (количество событий)
- Следующие Q строк: eᵢ и vᵢ (тип события и номер участка)

## Формат вывода
- Q+1 строк: исходная сумма квадратов и результаты после каждого события

## Пример
**Ввод**  
```
4 0
3 5 5 4
5
1 1
2 1
1 3
2 2
1 3
```

**Вывод**  
```
75
105
73
101
83
113
```

## Примечание
> «Невозможно заставить любить (этот контест).» (с) *Форсаж 7. Месть не знает границ.*

**Решение требует:**
1. Эффективной структуры данных для хранения отрезков (например, связный список с быстрым доступом)
2. Быстрого пересчета суммы квадратов при изменениях
3. Обработки операций разделения и слияния за O(1) или O(log n)