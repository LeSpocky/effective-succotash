# ♥ 2018 by Alexander Dahl <post@lespocky.de>.
# Copying is an act of love. Please copy.

cmake_minimum_required(VERSION 3.1)
project(effective-succotash
	VERSION 0.0.1
	LANGUAGES C
)

option(OPENSSL_EXTERNAL "Download and build OpenSSL by ourselves")

unset(OPENSSL_INCLUDE_DIR CACHE)
unset(OPENSSL_CRYPTO_LIBRARY CACHE)
unset(OPENSSL_SSL_LIBRARY CACHE)

if(OPENSSL_EXTERNAL)
	message("Building OpenSSL by ourselves!")
	include(ExternalProject)

	ExternalProject_Add(openssl
		URL https://www.openssl.org/source/old/1.0.2/openssl-1.0.2l.tar.gz
		URL_MD5 f85123cd390e864dfbe517e7616e6566
		CONFIGURE_COMMAND ./config --prefix=<INSTALL_DIR>/usr --openssldir=<INSTALL_DIR>/usr/lib/ssl shared
		BUILD_COMMAND make
		BUILD_IN_SOURCE 1
		INSTALL_COMMAND make install_sw
		TEST_BEFORE_INSTALL 1
		TEST_COMMAND make test
	)

	ExternalProject_Get_Property(openssl INSTALL_DIR)
	find_path(OPENSSL_INCLUDE_DIR
		NAMES
			openssl/ssl.h
		PATHS
			"${INSTALL_DIR}/usr"
		PATH_SUFFIXES
			include
		NO_DEFAULT_PATH
	)
	find_library(OPENSSL_CRYPTO_LIBRARY
		NAMES
			crypto
		PATHS
			"${INSTALL_DIR}/usr"
		PATH_SUFFIXES
			lib
		NO_DEFAULT_PATH
	)
	find_library(OPENSSL_SSL_LIBRARY
		NAMES
			ssl
		PATHS
			"${INSTALL_DIR}/usr"
		PATH_SUFFIXES
			lib
		NO_DEFAULT_PATH
	)
	mark_as_advanced(OPENSSL_INCLUDE_DIR OPENSSL_CRYPTO_LIBRARY OPENSSL_SSL_LIBRARY)

	add_library(OpenSSL::Crypto SHARED IMPORTED)
	set_target_properties(OpenSSL::Crypto PROPERTIES
		INTERFACE_INCLUDE_DIRECTORIES "${OPENSSL_INCLUDE_DIR}"
		IMPORTED_LINK_INTERFACE_LANGUAGES "C"
		IMPORTED_LOCATION "${OPENSSL_CRYPTO_LIBRARY}"
	)

	add_library(OpenSSL::SSL SHARED IMPORTED)
	set_target_properties(OpenSSL::SSL PROPERTIES
		INTERFACE_INCLUDE_DIRECTORIES "${OPENSSL_INCLUDE_DIR}"
		IMPORTED_LINK_INTERFACE_LANGUAGES "C"
		IMPORTED_LOCATION "${OPENSSL_SSL_LIBRARY}"
		INTERFACE_LINK_LIBRARIES OpenSSL::Crypto
	)

	add_dependencies(OpenSSL::Crypto openssl)
	add_dependencies(OpenSSL::SSL openssl)
else(OPENSSL_EXTERNAL)
	message("Using OpenSSL from system!")
	find_package(OpenSSL REQUIRED)
endif(OPENSSL_EXTERNAL)

# my stuff
add_executable(effective-succotash
	effective-succotash.c
)

target_link_libraries(effective-succotash
	OpenSSL::Crypto
)
