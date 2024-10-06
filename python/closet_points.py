def distance(p1, p2):
    return abs(p1[0] - p2[0]) + abs(p1[1] - p2[1])


def min_distance(P):
    smallest = float("inf")

    for i in range(len(P) - 1):
        for j in range(i + 1, len(P)):
            d = distance(P[i], P[j])
            if d < smallest:
                smallest = d
                p1 = P[i]
                p2 = P[j]

    print(p1, p2)
    print(smallest)


P = [(17, 2), (16, 15), (7, 11), (6, 10), (16, 19), (16, 6), (2, 13), (10, 17)]
Py = sorted(P, key=lambda p: p[1])
print(Py)

# Divide

Qx = [(2, 13), (6, 10), (7, 11), (10, 17)]
Qy = [(6, 10), (7, 11), (2, 13), (10, 17)]
Rx = [(16, 15), (16, 19), (16, 6), (17, 2)]
Ry = [(17, 2), (16, 6), (16, 15), (16, 19)]


# min_distance(P)
# min_distance(Qx)
# min_distance(Rx)

x = 10
Sy = []
for p in Py:
    if abs(p[0] - x) <= 2:
        Sy.append(p)

print(Sy)
