#! /usr/bin/env python3


# Lazy heap sort


# 1
def empty():
    '''
    >>> list(empty())
    []
    '''
    return
    yield


# 2
def heapify(value, heap1, heap2):
    '''
    >>> list(heapify(1, empty(), empty()))[0]
    (1, 1, 1)

    >>> list(heapify(2, heapify(1, empty(), empty()), empty()))[0]
    (1, 2, 2)

    >>> heap1 = heapify(3, empty(), empty())
    >>> heap2 = heapify(1, empty(), empty())
    >>> list(list(heapify(2, heap1, heap2))[2])[0]
    (2, 1, 1)
    '''
    h1 = next(heap1, None)
    h2 = next(heap2, None)
    if (h1 is not None) or (h2 is not None):
        if (h1 is None) and (h2 is not None):
            if h2[0] >= value:
                yield(value, h2[1] + 1, h2[2] + 1)
                yield empty()
                yield (heapify(h2[0], next(heap2), next(heap2)))
            else:
                yield(h2[0], h2[1] + 1, h2[2] + 1)
                yield empty()
                yield(heapify(value, next(heap2), next(heap2)))
        if (h1 is not None) and (h2 is None):
            if h1[0] >= value:
                yield(value, h1[1] + 1, h1[2] + 1)
                yield (heapify(h1[0], next(heap1), next(heap1)))
                yield empty()
            else:
                yield (h1[0], h1[1] + 1, h1[2] + 1)
                yield (heapify(value, next(heap1), next(heap1)))
                yield empty()
        if (h1 is not None) and (h2 is not None):
            if (value <= h1[0] and value <= h2[0]):
                yield (value, h1[1] + h2[1] + 1, max(h1[2], h2[2]) + 1)
                yield (heapify(h1[0], next(heap1), next(heap1)))
                yield (heapify(h2[0], next(heap2), next(heap2)))
            elif (value >= h1[0] and h1[0] <= h2[0]):
                yield (h1[0], h1[1] + h2[1] + 1, max(h1[2], h2[2]) + 1)
                yield heapify(value, next(heap1), next(heap1))
                yield heapify(h2[0], next(heap2), next(heap2))
            elif (value >= h2[0] and h1[0] >= h2[0]):
                yield (h2[0], h1[1] + h2[1] + 1, max(h1[2], h2[2]) + 1)
                yield heapify(h1[0], next(heap1), next(heap1))
                yield heapify(value, next(heap2), next(heap2))

    else:
        yield (value, 1, 1)
        yield empty()
        yield empty()


# 3
def print_heap(heap, prefix=''):
    '''
    >>> print_heap(empty())

    >>> print_heap(heapify(1, empty(), empty()))
    [1, 1]: 1

    >>> print_heap(heapify(2, heapify(1, empty(), empty()), empty()))
    [2, 2]: 1
        [1, 1]: 2
    >>> print_heap(heapify(2, heapify(1, empty(), empty()), empty()))
    [2, 2]: 1
        [1, 1]: 2
    '''

    h = next(heap, None)
    if h is None:
        pass
    else:
        print("{}[{}, {}]: {}".format(prefix, h[1], h[2], h[0]))
        print_heap(next(heap), '    ' + prefix)
        print_heap(next(heap), '    ' + prefix)


# 4
def build_heap(seq):
    '''
    >>> print_heap(build_heap([]))

    >>> print_heap(build_heap([1]))
    [1, 1]: 1

    >>> print_heap(build_heap([2, 1]))
    [2, 2]: 1
        [1, 1]: 2

    >>> print_heap(build_heap([3, 1, 2]))
    [3, 2]: 1
        [1, 1]: 3
        [1, 1]: 2

    >>> print_heap(build_heap(range(1, 4)))
    [3, 2]: 1
        [1, 1]: 2
        [1, 1]: 3

    >>> print_heap(build_heap([2, 4, 1, 3]))
    [4, 3]: 1
        [2, 2]: 3
            [1, 1]: 4
        [1, 1]: 2

    >>> print_heap(build_heap([2, 4, 1, 3, 5]))
    [5, 3]: 1
        [3, 2]: 3
            [1, 1]: 4
            [1, 1]: 5
        [1, 1]: 2
    '''

    seq = list(seq)
    l = len(seq)
    if seq == []:
        return empty()
    for i in range(l // 2, l):
        seq[i] = heapify(seq[i], empty(), empty())
    for i in reversed(range(0, l//2)):
        if 2 * (i + 1) < l:
            seq[i] = heapify(seq[i], seq[2 * (i + 1) - 1], seq[2 * (i + 1)])
        else:
            seq[i] = heapify(seq[i], seq[2 * (i + 1) - 1], empty())
    return seq[0]


# 5
def extract_last(heap):
    '''
    >>> last, heap = extract_last(build_heap([]))
    >>> last
    >>> print_heap(heap)

    >>> last, heap = extract_last(build_heap([1]))
    >>> last
    1
    >>> print_heap(heap)

    >>> last, heap = extract_last(build_heap([2, 1]))
    >>> last
    2
    >>> print_heap(heap)
    [1, 1]: 1

    >>> last, heap = extract_last(build_heap([3, 1, 2]))
    >>> last
    2
    >>> print_heap(heap)
    [2, 2]: 1
        [1, 1]: 3

    >>> last, heap = extract_last(build_heap([2, 4, 1, 3, 5]))
    >>> last
    5
    >>> print_heap(heap)
    [4, 3]: 1
        [2, 2]: 3
            [1, 1]: 4
        [1, 1]: 2
    '''
    h = next(heap, None)
    if h is None:
        yield None
        yield empty()
    else:
        if h[1] == 1:
            yield h[0]
            yield empty()
        h1 = next(heap)
        h2 = next(heap)
        h1head = next(h1)
        h2head = next(h2, None)
        if h2head is None:
            yield h1head[0]
            yield heapify(h[0], empty(), empty())
        else:
            if h1head[2] > h2head[2]:
                res = extract_last(
                    heapify(h1head[0], next(h1), next(h1)))
                yield next(res)
                yield heapify(h[0],
                              next(res),
                              heapify(h2head[0], next(h2), next(h2)))
            else:
                res = extract_last(
                    heapify(h2head[0], next(h2), next(h2)))
                yield next(res)
                yield heapify(h[0],
                              heapify(h1head[0], next(h1), next(h1)),
                              next(res))


# 6
def extract_min(heap):
    '''
    >>> min_value, heap = extract_min(build_heap([]))
    >>> min_value
    >>> print_heap(heap)

    >>> min_value, heap = extract_min(build_heap([1]))
    >>> min_value
    1
    >>> print_heap(heap)

    >>> min_value, heap = extract_min(build_heap([2, 1]))
    >>> min_value
    1
    >>> print_heap(heap)
    [1, 1]: 2

    >>> min_value, heap = extract_min(build_heap([3, 1, 2]))
    >>> min_value
    1
    >>> print_heap(heap)
    [2, 2]: 2
        [1, 1]: 3

    >>> min_value, heap = extract_min(build_heap([2, 4, 1, 3, 5]))
    >>> min_value
    1
    >>> print_heap(heap)
    [4, 3]: 2
        [2, 2]: 3
            [1, 1]: 4
        [1, 1]: 5
    '''
    h = next(heap, None)
    if h is None:
        yield None
        yield empty()
    elif h[1] == 1:
        yield h[0]
        yield empty()
    else:
        last, new = extract_last(
                    heapify(h[0], next(heap), next(heap)))
        yield next(new)[0]
        yield heapify(last, next(new), next(new))


# 7
def heap_sort(seq):
    '''
    >>> list(heap_sort([]))
    []

    >>> list(heap_sort(iter([1])))
    [1]

    >>> list(heap_sort([2, 1]))
    [1, 2]

    >>> list(heap_sort(iter([2, 1, 3])))
    [1, 2, 3]

    >>> list(heap_sort(range(7, 0, -1)))
    [1, 2, 3, 4, 5, 6, 7]
    '''
    heap = build_heap(seq)
    min, heap = extract_min(heap)
    while min is not None:
        yield min
        min, heap = extract_min(heap)

if __name__ == '__main__':
    import doctest
    doctest.testmod()
