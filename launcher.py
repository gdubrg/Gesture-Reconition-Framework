__author__ = 'Guido Borghi'
from subprocess import call

import subprocess
import sys


filetrain = "DTWMSR/trainAS1.txt"
filetest = "DTWMSR/testAS1.txt"

# for l in [x/10.0 for x in range(3, 50, 1)]:
#     for k in range(2, 100, 1):
#         command = "release\GRFrameworkBATCH.exe DTWMSR/trainALL.txt DTWMSR/testALL.txt " + str(l) + " " + str(k) + " " + "1" + " " + "dhmm train test"
#         print command
#         p = subprocess.Popen(command, stdout=subprocess.PIPE, shell=True)
#         while True:
#             out = p.stdout.read(1)
#             if out == '' and p.poll() != None:
#                 break
#             if out != '':
#                 sys.stdout.write(out)
#                 sys.stdout.flush()

#
#
command = "release\GRFramework.exe DTWMSR/trainAS1.txt DTWMSR/testAS1.txt 0.5 10 dhmm train test"

p = subprocess.Popen(command, stdout=subprocess.PIPE, shell=True)
while True:
    out = p.stdout.read(1)
    if out == '' and p.poll() != None:
        break
    if out != '':
        sys.stdout.write(out)
        sys.stdout.flush()


command = "release\GRFramework.exe DTWMSR/trainAS2.txt DTWMSR/testAS2.txt 0.5 10 dhmm train test"

p = subprocess.Popen(command, stdout=subprocess.PIPE, shell=True)
while True:
    out = p.stdout.read(1)
    if out == '' and p.poll() != None:
        break
    if out != '':
        sys.stdout.write(out)
        sys.stdout.flush()


command = "release\GRFramework.exe DTWMSR/trainAS3.txt DTWMSR/testAS3.txt 0.5 10 dhmm train test"

p = subprocess.Popen(command, stdout=subprocess.PIPE, shell=True)
while True:
    out = p.stdout.read(1)
    if out == '' and p.poll() != None:
        break
    if out != '':
        sys.stdout.write(out)
        sys.stdout.flush()

command = "release\GRFramework.exe DTWMSR/trainALL.txt DTWMSR/testALL.txt 0.5 10 dhmm train test"

p = subprocess.Popen(command, stdout=subprocess.PIPE, shell=True)
while True:
    out = p.stdout.read(1)
    if out == '' and p.poll() != None:
        break
    if out != '':
        sys.stdout.write(out)
        sys.stdout.flush()