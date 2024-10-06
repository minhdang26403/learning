def match_tuple(t):
    match t:
        case []:  # Empty tuple
            return "Empty tuple"
        case [
            x,
        ]:  # Single element tuple (note the comma)
            return f"Single element tuple: {x}"
        case (x, y):  # Two element tuple
            return f"Two elements: {x}, {y}"
        case (x, *rest):  # Tuple with at least one element
            return f"First element: {x}, remaining: {rest}"
        case _:
            return "Other tuple"


print(match_tuple(()))  # Output: Empty tuple
print(match_tuple((42,)))  # Output: Single element tuple: 42
print(match_tuple((1, 2)))  # Output: Two elements: 1, 2
print(match_tuple((1, 2, 3)))  # Output: First element: 1, remaining: (2, 3)


def evaluate(exp, env):
    "Evaluate an expression in an environment."
    match exp:
        # ... lines omitted
        case ["quote", x]:
            return x
        case ["if", test, consequence, alternative]:
            if evaluate(test, env):
                return evaluate(consequence, env)
            else:
                return evaluate(alternative, env)
        case ["lambda", [*parms], *body] if body:
            print(parms)
            print(body)
        case _:
            raise SyntaxError()


evaluate(["lambda", [], "abc", "xyz", "asdasd"], None)

l = [1, 2, 3, 4]
sl = l[:2]
print(f"object id: {id(l[0])}")
print(f"object id: {id(sl[0])}")
sl[0] = 2
print(f"object id: {id(l[0])}")
print(f"object id: {id(sl[0])}")

d = dict(a=10, b=20, c=30)
values = d.values()
print(values)
d['z'] = 9
print(values)
