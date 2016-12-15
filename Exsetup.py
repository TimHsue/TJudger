from distutils.core import setup, Extension
import os

os.system("sudo apt-get install python-dev")
os.system("sudo apt-get install libseccomp-dev")
os.system("gcc ExactRunner/runner.c -o runner -lseccomp -DEXACT_MOD")

setup (
	name='ExTJudger',
	version='1.0 Exbeta',
	packages=['ExTJudger'],
	package_dir={'ExTJudger':'ExactPyAPI'})

