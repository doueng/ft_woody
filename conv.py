#!/usr/bin/env python3

s = input()
for i,c in enumerate(s[::-1]):
    if (i % 8 == 0):
        print('\npush 0x', end='')
    print('{0:x}'.format(ord(c)), end='')
print('')

