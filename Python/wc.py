#!/usr/bin/env python3

#Написать скрипт, работающий так же, как утилита wc, вызванная без параметров.
#Обратите внимание на то, что третьим числом wc выводит не число символов, а число байт!



import sys

if __name__ == '__main__':

    data = sys.stdin.read()
    chars = len(data)
    words = len(data.split())
    lines = len(data.split('\n'))

    print("{:>7}{:>8}{:>8}".format(lines-1, words, chars))
