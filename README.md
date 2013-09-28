Generative
===========
Generative is a header-only C++11 template library for compositioning coherent noise.

Most C++ coherent noise libraries use a very object-heavy approach where you string objects together to create different kinds of coherent noise. Generative, on the other hand, uses a more function-oriented approach.

Generative is first and foremost designed with effeciency in mind, favoring compile-time templates over run-time virtual functions. This makes libcoherent more versatile, but also requires you to make more considerations on how to make your noise functions (see [the tutorial][1] for more information).

Dependencies
------------
* OpenGL Mathematics (GLM)
* Boost (also needs BOOST_RESULT_OF_USE_DECLTYPE to be defined)

License
-------
Generative is released under the MIT license. See LICENSE for more information.

[1]:https://github.com/Noctune/libcoherent/wiki/Tutorial
