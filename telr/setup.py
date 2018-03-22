import os
from setuptools import setup


def read(fname):
    return open(os.path.join(os.path.dirname(__file__), fname)).read()


setup(
        name='telr',
        version='1.0.0',
        install_requires=['gspread>=2.0.0', 'oauth2client>=4.1.2'],
        scripts=['bin/telr'],
        author='Jarred Parr',
        author_email='jparr721@gmail.com',
        description='Stream an ip from a tiny pc',
        license='MIT',
        long_description=read('README.md')
)
