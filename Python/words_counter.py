#! /usr/bin/env python3


# Нужно найти десять слов, встречающихся чаще всего в двух заданных текстовых файлах.
#Числом вхождений слова в оба файла считается минимум из числа вхождений слова в первый файл и числа вхождений во второй файл. Словом считается любая непустая последовательность букв. Слова, различающиеся лишь регистром символов, следует считать одинаковыми.
#Слова нужно вывести в файл cnt.txt в порядке убывания частоты вхождений. Формат вывода:
#слово число_вхождений
#например:
#orange 10
#apple 5




import operator
import re

INPUT_FILE_1 = 'war_and_peace.txt'
INPUT_FILE_2 = 'and_quiet_flows_the_don_1.txt'
OUTPUT_FILE = 'cnt.txt'


def main():
    m = {}
    with open(INPUT_FILE_1, "r") as f:
        for j in f:
            x = re.split(r"\W", j)        
            for y in x:            
                if y in m.keys():
                    m[y] += 1
                else:
                    m[y] = 1



    n = {}
    with open(INPUT_FILE_2, "r") as f2 :
        for j2 in f2:
            x2 = re.split(r"\W", j2)
            for y2 in x2:            
                if y2 in n.keys():
                    n[y2] += 1
                else:
                    n[y2] = 1
    del m['']
    del n['']
    fst = [(k, m[k]) for k in sorted(m, key=m.get, reverse=True)]
    snd = [(k, n[k]) for k in sorted(n, key=n.get, reverse=True)]
    result = {}
    u  = fst.pop(0)
    v  = snd.pop(0)
    count = 0
    with open(OUTPUT_FILE, "w") as write:    
        while count < 10:
            while  (u[0] in n.keys()) or (v[0] in m.keys()):
                if u[0] in n.keys():
                    del m[u[0]]                
                    u  = fst.pop(0)            
                if v[0] in m.keys():
                    del n[v[0]]
                    v  = snd.pop(0)                 
            if u[1] > v [1]:
                result[u[0]]=u[1]      
                write.write(u[0] + ' ' + str(u[1]) + '\n')      
                u  = fst.pop(0)            
            else:
                write.write (v[0] + ' ' + str(v[1]) + '\n')
                v = snd.pop(0)
            count +=1


main()
#if __name__ == '__main__':
#    import timeit
#    import statistics
#    measures = timeit.repeat('main()', 'from __main__ import main',
#                             repeat=51, number=1)
#    print(statistics.median(measures))
