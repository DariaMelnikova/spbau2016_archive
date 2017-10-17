#! /usr/bin/env python3


#Написать декоратор prof, который можно было бы использовать следующим образом:
#@prof('fname.prof')
#def foo():...


import cProfile


def prof(filename):
    def func_n(func):
        def wrapp(*args, **kwargs):
            proff = cProfile.Profile()
            result = proff.runcall(func, *args, **kwargs)
            proff.dump_stats(filename)
            return result
        return wrapp
    return func_n
