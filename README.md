libcoherent
===========
libcoherent is a header-only C++11 template library for generating coherent noise. It is first and foremost designed with effeciency in mind, favoring compile-time templates over run-time virtual functions. This makes libcoherent more versatile, but also a bit more difficult to use (see [the tutorial][1] for more information).

Dependencies
------------
* Eigen3
* Boost (also needs BOOST_RESULT_OF_USE_DECLTYPE to be defined)

In addition, the examples have the following dependencies:
* libpng
* PNG++

License
-------
libcoherent is released under the MIT license. See license.txt for more information.

Credits
-------
* Mike Pedersen, noctune9@gmail.com
* Stefan Gustavson, stegu@itn.liu.se
    * Simplex noise implementation: http://webstaff.itn.liu.se/~stegu/aqsis/aqsis-newnoise/

[1]:https://github.com/Noctune/libcoherent/wiki/Tutorial
