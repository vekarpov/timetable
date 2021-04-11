from os import mkdir
from subprocess import run

mkdir('fet-build')
run(['qmake-qt5', '../fet-src/fet.pro', 'DEFINES+=USE_SYSTEM_LOCALE'], encoding='utf-8', cwd='fet-build')
run(['make', '-j8'], encoding='utf-8', cwd='fet-build')
