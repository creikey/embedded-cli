project('embcli', 'c')

inc = include_directories('include')

subdir('src') # embcli_sources
subdir('test') # test_sources

pkg_mod = import('pkgconfig')
pkg_mod.generate(libraries: embclilib,
version: '1.0',
name : 'libembcli',
filebase : 'embcli',
description : 'A cli library between an stm32 and a raspberry pi')