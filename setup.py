"""Setup module."""

from setuptools import setup, find_packages, Extension


extension = Extension("pgf_decode", sources=["pillow_pgf/pgf_decode.c"])


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
