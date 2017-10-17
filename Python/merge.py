#! /usr/bin/env pytho3 

#lazy merge sort


import random


def merge(seq1, seq2):
    '''
    >>> list(merge([], []))
    []

    >>> list(merge(iter([]), [1]))
    [1]

    >>> list(merge(iter([2]), iter([1])))
    [1, 2]

    >>> list(merge([1, 1, 2], [1, 3]))
    [1, 1, 1, 2, 3]
    '''
    s1 = iter(seq1)
    s2 = iter(seq2)
    try:
        x1 = next(s1)
    except StopIteration:
        x1 = None
    try:
        x2 = next(s2)
    except StopIteration:
        x2 = None
    while x1 or x2:
        if x1 == None and x2 != None:
            yield x2
            try:
                x2 = next(s2)
            except StopIteration:
                x2 = None
        if x2 == None and x1 != None:
            yield x1
            try:
                x1 = next(s1)
            except StopIteration:
                x1 = None
        if (x1 != None and x2 != None) and x1 <= x2:
            yield x1
            try:
                x1 = next(s1)
            except StopIteration:
                x1 = None
        elif (x1 != None and x2 != None):
            yield x2
            try:
                x2 = next(s2)
            except StopIteration:
                x2 = None
  
def elem_wrapper(x):
	yield x


# 2
def merge_sort(seq):
    '''
    >>> list(merge_sort(iter([])))
    []

    >>> list(merge_sort(iter([1])))
    [1]

    >>> list(merge_sort([2, 1]))
    [1, 2]

    >>> list(merge_sort([2, 1, 1, 1, 5, 0, 10, 1]))
    [0, 1, 1, 1, 1, 2, 5, 10]
    '''
	queue = deque (map(elem_wrapper, seq))
	if not queue:
		return iter([])
	while len(queue) != 1
		queue.append(merge(queue.popleft()), queue.popleft())
    return queue.pop()


# 3
class Rnd:
    '''
    >>> len(list(Rnd(5)))
    5
    '''

    def __init__(self, N):
        self.vars = []
        self.counter = -1
        for i in range(0,N):
            self.N = N
            self.vars.append(int(random.random() * 32175))

    def __iter__(self):
        return self

    def __next__(self):
        self.counter += 1
        if self.counter < self.N:
            return self.vars[self.counter]
        else:
            raise StopIteration


# 4
def sort_random_numbers():
    pass


if __name__ == '__main__':
    import doctest
    doctest.testmod()
