from distutils.core import setup
from distutils.extension import Extension

setup(name="bonuscoin_subsidys",
    ext_modules=[
        Extension("bonuscoin_subsidy", ["Bonuscoin_GetBlockValue.cpp"],
        libraries = ["boost_python"])
    ])
