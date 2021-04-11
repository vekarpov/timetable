from sys import argv
from subprocess import run

_, cfg = argv
run(['fet-build/fet-cl', f'--inputfile={cfg}', '--outputdir=fet-build', '--language=ru', '--exportcsv=true'])
