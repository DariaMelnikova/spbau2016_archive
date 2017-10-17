#! /usr/bin/env python3

#Написать упрощенный вариант утилиты nl  скрипт, который выдает в stdout пронумерованные строки из stdin. Он должен работать так же, как nl -b a.


from string import Template


n = 1
while(1):
    line = input()
    print('{:>6}  {}'.format(n, line))
    n += 1
