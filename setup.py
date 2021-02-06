"""Setup module."""

from setuptools import setup, find_packages, Extension

# run this setup: python setup.py build
#https://docs.python.org/3/extending/building.html
extension = Extension("pgf_decode", 
                      define_macros=[('WIN64', '1')],
                      include_dirs=["../Pillow/src/libImaging"],
                      libraries=["clibpgf.lib"],
                      library_dirs=["libpgf"],
                      sources=["pillow_pgf/pgf_decode.c"])


setup(
    name="Pillow-PGF",
    version="0.1",
    url="https://github.com/DavidMStraub/Pillow-PGF",
    author="David M. Straub",
    author_email="straub@protonmail.com",
    description="A PGF (Progressive Graphics File) plugin for Pillow",
    packages=find_packages(),
    install_requires=["Pillow"],
    ext_modules=[extension],
)
