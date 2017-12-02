import math


def cut_evenly(weights, threshold):
    min_weight = min(weights)
    weights = [w / min_weight for w in weights]
    intervals = [None] * len(weights)
    base_cuts = 0
    for i, weight in enumerate(weights):
        if weight > 1:
            a, b = weight * threshold, weight / threshold
            intervals[i] = (a, b)
            for coefficient in range(1, math.ceil(1 / (b - a)) + 1):
                a, b = coefficient * a, coefficient * b
                min_integer = math.ceil(a)
                if math.ceil(a) == int(a):
                    min_integer += 1
                if min_integer < b:
                    base_cuts = max(base_cuts, coefficient - 1)
                    break
    while True:
        coefficient = base_cuts + 1
        min_total_cuts = 0
        for interval in intervals:
            if interval is None:
                min_total_cuts += base_cuts
            else:
                a, b = coefficient * interval[0], coefficient * interval[1]
                min_integer = math.ceil(a)
                if min_integer == int(a):
                    min_integer += 1
                if min_integer < b:
                    min_total_cuts += min_integer - 1
                else:
                    break
        else:
            break
        base_cuts += 1
        continue
    return min_total_cuts


if __name__ == "__main__":
    threshold, _ = input().strip().split(" ")
    threshold = float(threshold)
    veggies = [int(weight) for weight in input().strip().split(" ")]
    min_cuts = cut_evenly(veggies, threshold)
    print(min_cuts)
