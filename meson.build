# -*- Meson -*-

project('decat', ['cpp'],
        license         : 'GNU GLP3',
        version         : '1',
        default_options : [
          'warning_level=3',
          'cpp_std=c++2a', # g++-11 does not support c++20
          'buildtype=debugoptimized',
          'werror=false'
        ],
       )

if meson.get_compiler('cpp').get_id() == 'clang'
  add_project_arguments(
    '-Weverything',
    '-Wno-weak-vtables', '-Wno-padded', '-Wno-c++98-compat', '-Wno-c++98-c++11-compat', '-Wno-c++98-compat-pedantic',
    '-Wno-ambiguous-reversed-operator', # ISO C++20 considers use of overloaded operator '<=' to be ambiguous despite there being a unique best viable function
    '-Wno-zero-as-null-pointer-constant', # clang13 somtimes with operator<=>
    language : 'cpp')
endif

# ----------------------------------------------------------------------
# external dependencies
# ----------------------------------------------------------------------

brotli_dec = dependency('libbrotlidec')
zlib = dependency('zlib', version : '>=1.2.8')
xz = dependency('liblzma')
bzip2 = meson.get_compiler('cpp').find_library('bz2', required : false)
zstd = dependency('libzstd')

# ----------------------------------------------------------------------
# decat
# ----------------------------------------------------------------------

sources = [
  'decat.cc',
  'read.cc',
  'process.cc',
  'brotli.cc',
  'gzip.cc',
]

decat = executable(
  'decat',
  sources : sources,
  dependencies : [brotli_dec, zlib, xz, bzip2, zstd],
  install : true)

# ----------------------------------------------------------------------
