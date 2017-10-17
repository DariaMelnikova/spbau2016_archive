#! /usr/bin/env python3

#Написать упрощенный вариант утилиты tail  скрипт, выводящий в stdout последние 10 строк заданного первым параметром файла.



import sys

result = []
n = 0
for line in reversed(list(open(sys.argv[1]))):
    if n < 10:
        result.append(line)
        n += 1
    else:
        break


for line in reversed(result):
    print(line, end='')
