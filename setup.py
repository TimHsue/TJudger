from distutils.core import setup, Extension
import os

os.system("sudo apt-get install python-dev")
os.system("sudo apt-get install libseccomp-dev")

setup (
	name='TJudger',
	version='1.0 bate',
	ext_modules=[Extension(name='TJudger', sources=['PythonAPI/PyAPI.cpp'], libraries=['seccomp'])])

