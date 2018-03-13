from distutils.core import setup
from Cython.Build import cythonize
from distutils.extension import Extension
from Cython.Build import cythonize

extensions = [
    Extension("adlink", ["adlink.pyx"],
        include_dirs = ["C:/ADLINK/PCIS-DASK/include"
        ,"D:/Program Files/Microsoft Visual Studio 14.0/VC/include"
		,"D:/Program Files/Microsoft Visual Studio 14.0/VC/atlmfc/include"
		,"C:/Program Files/Windows Kits/10/Include/10.0.10150.0/ucrt"
		,"C:/Program Files/Windows Kits/8.1/Include/um"
		,"C:/Program Files/Windows Kits/8.1/Include/shared"
		,"C:/Program Files/Windows Kits/8.1/Include/winrt"],
        libraries = ["PCI-Dask"],
        library_dirs = ['C:/ADLINK/PCIS-DASK/lib']),
    # Everything but primes.pyx is included here.
    # Extension("*", ["*.pyx"],
    #     include_dirs = [...],
    #     libraries = [...],
    #     library_dirs = [...]),
]
setup(
  name = 'Hello world app',
  ext_modules = cythonize(extensions)
)