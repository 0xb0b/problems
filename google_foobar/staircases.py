"""
count number of different staircases that can be made of n bricks.
staircase has to have minimum 2 steps, each next step has to be lower than the
previous one.
test cases:
    solution(3) = 1
    solution(200) = 487067745
"""


def count_staircases(n, complete_chunks, staircases_data, max_height=None):
    """
    count number of staircases that can be built from n bricks
    and have height at most max_height
    """
    if staircases_data[n] is None:
        # count all staircases for n
        k = (n - 1) // 2
        for m in range(8, k + 1):
            if staircases_data[m] is None:
                count_staircases(m, complete_chunks, staircases_data)
        for m in range(len(complete_chunks), k + 1):
            complete_chunks.append(complete_chunks[m - 1]
                                  + staircases_data[m][0] + 1)
        total = complete_chunks[k]
        partial_chunks = []
        height = n // 2
        while height > 3:
            num_staircases = count_staircases(n - height, complete_chunks,
                                              staircases_data,
                                              max_height=(height - 1))
            if num_staircases is None:
                break
            partial_chunks.append(num_staircases)
            height -= 1
        if partial_chunks:
            i = len(partial_chunks) - 2
            for x in reversed(partial_chunks):
                if i < 0:
                    break
                partial_chunks[i] += x
                i -= 1
            total += partial_chunks[0]
        staircases_data[n] = (total, partial_chunks)

    total, partial_parts = staircases_data[n]
    if max_height is None:
        return total
    min_body_height = n // 2 + 1
    if max_height < min_body_height:
        index = min_body_height - max_height - 1
        if index < len(partial_parts):
            return partial_parts[index]
        return None
    else:
        return total - complete_chunks[n - max_height - 1]


def solution(n):
    complete_chunks = [0, 1, 2, 4]
    # list of (total staircases count, partial parts) for each index as n
    staircases_data = [(0, []), (0, []), (0, []), (1, []),
                       (1, []), (2, []), (3, [1]), (4, [])]
    staircases_data.extend([None] * (n - 7))
    if staircases_data[n] is not None:
        return staircases_data[n][0]
    else:
        return count_staircases(n, complete_chunks, staircases_data)


for n in range(3, 19):
    print(f'{n}: {solution(n)}')
print(f'200: {solution(200)}')
