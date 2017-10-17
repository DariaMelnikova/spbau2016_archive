#! /usr/bin/env python3


#Реализовать алгоритм Винограда — Штрассена. Начиная с определенного размера матрицы (подобрать самостоятельно) нужно останавливать рекурсию и использовать обычное умножение. Измерить время работы написанного алгоритма и стандартного умножения из NumPy на матрицах различного размера, показать результаты измерений на графике.



import numpy as np
import matplotlib.pyplot as plt
import math
import time


def make_square(mat, size):
    len_hor = mat.shape[0]
    len_vert = mat.shape[1]
    b = np.zeros((size, size))
    b[:len_hor, :len_vert] = mat
    return b


def mult(mat1, mat2):
    size1 = max(mat1.shape[0], mat1.shape[1])
    size2 = max(mat2.shape[0], mat2.shape[1])
    size = 2 ** math.ceil(math.log2(max(size1, size2)))
    if(size <= 64):
        return np.dot(mat1, mat2)

    mat1_upd = make_square(mat1, size)
    mat2_upd = make_square(mat2, size)
    med = int(size/2)
    x11 = mat1_upd[0:med, 0:med].copy()
    x12 = mat1_upd[0:med, med:].copy()
    x21 = mat1_upd[med:, 0:med].copy()
    x22 = mat1_upd[med:, med:].copy()
    y11 = mat2_upd[0:med, 0:med].copy()
    y12 = mat2_upd[0:med, med:].copy()
    y21 = mat2_upd[med:, 0:med].copy()
    y22 = mat2_upd[med:, med:].copy()

    p1 = mult(np.add(x11, x22), np.add(y11, y22))
    p2 = mult(x21 + x22, y11)
    p3 = mult(x11, y12 - y22)
    p4 = mult(x22, np.subtract(y21, y11))
    p5 = mult(np.add(x11, x12), y22)
    p6 = mult(x21 - x11, y11 + y12)
    p7 = mult(np.subtract(x12, x22), np.add(y21, y22))
    result = np.zeros((size, size))
    result[:med, :med] = np.subtract(np.add(p1, p4), np.subtract(p5, p7))
    result[0:med:, med:] = np.add(p3, p5)
    result[med:, 0:med] = np.add(p2, p4)
    result[med:, med:] = np.add(np.subtract(p1, p2), np.add(p3, p6))
    return result[:mat1.shape[0], :mat2.shape[1]]


def check_mult(n, m, k):
    mat1 = np.random.uniform(0, 10.0, size=(n, m))
    mat2 = np.random.uniform(0, 10.0, size=(m, k))
    assert (np.linalg.norm(mult(mat1, mat2) - mat1.dot(mat2)) <= 1e-3)


def test_mult():
    sizes = [1, 2, 16, 128, 512, 640, 1024, 4000]
    for n in sizes:
        for m in sizes:
            for k in sizes:
                yield check_mult, n, m, k


def measure_time_and_show_plots():
    ax = plt.subplot()
    plt.xlabel('Matrix size')
    plt.ylabel('Time, ms')
    points_my = []
    points_dot = []
    counts = []
    gen = test_mult()

    while (True):
        try:
            f, n, m, k = next(gen)
            if n == m and m == k:
                t = time.time() * 1000
                f(n, m, k)
                counts.append(m)
                points_my.append(time.time() * 1000 - t)
                mat1 = np.random.uniform(0, 10.0, size=(n, m))
                mat2 = np.random.uniform(0, 10.0, size=(m, k))
                d_t = time.time() * 1000
                np.dot(mat1, mat2)
                points_dot.append(time.time() * 1000 - d_t)

        except StopIteration:
            my_plot, = ax.plot(counts, points_my, 'r-..', label='mult()')
            dot_plot, = ax.plot(counts, points_dot, 'b--', label='numpy.dot()')
            plt.legend(handles=[my_plot, dot_plot], loc='best')
            ax.autoscale_view()
            plt.show(ax)
            return


if __name__ == '__main__':
    pass

measure_time_and_show_plots()
