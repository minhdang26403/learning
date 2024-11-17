import time
from clockdeco_cls import clock


@clock("{name}: {elapsed}s")
def snooze1(seconds):
    time.sleep(seconds)


for i in range(3):
    snooze1(0.123)


@clock("{name}({args}) dt={elapsed:0.3f}s")
def snooze2(seconds):
    time.sleep(seconds)


for i in range(3):
    snooze2(0.123)
