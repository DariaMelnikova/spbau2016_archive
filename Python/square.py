#! /usr/bin/env python3


#3. Выпуклая оболочка и площадь многоугольника
#1. Написать функцию, умеющую генерировать точки на плоскости, нормально распределенные вокруг заданного центра с заданным стандартным отклонением по каждой из осей.
#2. Написать функцию, возвращающую для заданного набора точек выпуклую оболочку в виде списка вершин в порядке обхода против часовой стрелки.
#3. Написать функцию, считающую площадь заданного n точками (xi, yi) (в формате, описанном в предыдущем пункте) выпуклого многоугольника по формуле S =1/2∑(xi − xi+1)(yi + yi+1) где (xn+1, yn+1) ≡ (x1, y1).
#4. Написать функцию, умеющую рисовать заданный многоугольник.


import numpy as np
import matplotlib.pyplot as plt
from matplotlib.patches import Polygon
from scipy.spatial import ConvexHull


# 1
def gen_points(n, x, y, scale):
    '''
    >>> gen_points(1, 0, 0, 1).shape
    (1, 2)
    >>> ps = gen_points(5, 0, 0, 1)
    >>> ps.shape
    (5, 2)
    >>> ps.dtype
    dtype('float64')
    '''

    result = np.zeros((2, n))
    result[0] = np.random.normal(x, scale, n)
    result[1] = np.random.normal(y, scale, n)
    return np.transpose(result)



# 2
def build_convex_hull(points):
    '''
    >>> ch = build_convex_hull(gen_points(3, 0, 0, 1))
    >>> ch.shape
    (3, 2)
    >>> np.cross(ch[1] - ch[0], ch[2] - ch[0]) > 0
    True
    '''
    hull = ConvexHull(points)
    hull_indices = hull.vertices
    hull_pts = points[hull_indices, :]
    return hull_pts

# 3
def calc_square(polygon):
    '''
    >>> calc_square(np.array([[0., 0], [1, 0], [0, 1]]))
    0.5
    >>> calc_square(np.array([[0., 0], [1, 0], [1, 1], [0, 1]]))
    1.0
    '''
    '''assert len(polygon) >= 3
    l = polygon.shape[0]
    sum = 0
    curr_x = polygon[l-1][0]
    curr_y = polygon[l-1][1]
    for i in range(0, l):
        sum += (curr_x - polygon[i][0]) * (curr_y + polygon[i][1])
        curr_x = polygon[i][0]
        curr_y = polygon[i][1]
    return sum/2'''
    x = np.transpose(polygon)[0]
    y = np.transpose(polygon)[1]
    return 0.5 * np.abs(np.dot(x, np.roll(y, 1)) - np.dot(y, np.roll(x, 1)))


# 4
def plot(polygon):
    fig = plt.figure()
    ax = plt.subplot()
    ax.axhline(y=0, color='k')
    ax.axvline(x=0, color='k')
    ax1 = fig.add_subplot(111)
    ax1.add_patch(Polygon(polygon, facecolor='#F8E0EC'))
    ax.autoscale_view()
    h = np.transpose(polygon)
    plt.vlines(h[0], h[1], 0, linestyles='dashed')
    plt.show(fig)


if __name__ == '__main__':
    import doctest
    doctest.testmod()

    plot(build_convex_hull(gen_points(50, 0, 0, 10)))
