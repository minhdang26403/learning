class Bus:
    def __init__(self, passengers=None):
        if passengers is None:
            self.passengers = []
        else:
            self.passengers = list(passengers)

    def pick(self, name):
        self.passengers.append(name)

    def drop(self, name):
        self.passengers.remove(name)


import copy

bus1 = Bus(["Alice", "Bill", "Claire", "David"])
bus2 = copy.copy(bus1)
bus3 = copy.deepcopy(bus1)
print(id(bus1), id(bus2), id(bus3))

bus1.drop("Bill")
print(bus2.passengers)

print(id(bus1), id(bus2), id(bus3))
print(bus3.passengers)
