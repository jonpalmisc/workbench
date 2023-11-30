#!/usr/bin/env python3

import random

BASE32_CHARSET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567"

attempts = 0
while True:
    serial = "".join(random.choice(BASE32_CHARSET) for _ in range(16))

    even_sum = 0
    odd_sum = 0

    for i in range(16):
        if i % 2 == 0:
            even_sum += ord(serial[i])
        else:
            odd_sum += ord(serial[i])

    if odd_sum % 23 == 0 and even_sum % 37 == 0 and (even_sum + odd_sum) % 31 == 0:
        print(serial)
        # print(attempts)
        break

    attempts += 1
