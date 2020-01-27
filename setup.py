from setuptools import setup, Extension
setup(name="pyrt", ext_modules=[Extension("pyrt", sources=['pyrt.c'])])