from setuptools import setup, Extension
setup(name="py_realtime", ext_modules=[Extension("py_realtime", sources=['pyrt.c'])])