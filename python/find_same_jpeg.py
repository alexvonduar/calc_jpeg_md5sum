# !/usr/bin/python

import os
import hashlib
import Image
import subprocess
import time

start_time = time.time()

subp = []
num_images = 0
for root, dirs, files in os.walk(".", topdown=False):
    for name in files:
        if name.endswith('jpg'):
            fname = os.path.join(root, name)
            p = subprocess.Popen(
                ['./calc_jpeg_md5sum', fname], stdout=subprocess.PIPE)
            subp.append([p, fname])
            num_images += 1
            if num_images % 1000 == 0:
                while len(subp) > 0:
                    p, fname = subp.pop(0)
                    out = p.communicate()[0]
                    print(fname, p.returncode, out)

while len(subp) > 0:
    p, fname = subp.pop(0)
    out = p.communicate()[0]
    p.wait()
    print(fname, p.returncode, out)


print((time.time() - start_time) / len(subp))
