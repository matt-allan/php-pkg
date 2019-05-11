php_version = php-7.3.5

all: build/embed

.PHONY: all

build/$(php_version).tar.gz:
	@echo "Downloading php-src..."
	cd build && curl -O -L -C - https://www.php.net/distributions/$(php_version).tar.gz

build/$(php_version): build/$(php_version).tar.gz
	@echo "Extracting php-src..."
	cd build && tar -xzf $(php_version).tar.gz

build/php: build/$(php_version)
	@echo "Building php..."
	cd build/$(php_version) && ./configure \
	--enable-static \
	--disable-all \
	--disable-cli \
	--enable-embed=static \
	--prefix=$(shell pwd)/build/php
	cd build/$(php_version) && make -j 2
	cd build/$(php_version) && make install

build/embed: build/php embed.c
	gcc -o build/embed embed.c -I build/php/include/php/ -I build/php/include/php/main -I build/php/include/php/Zend -I build/php/include/php/TSRM -L build/php/lib -lphp7 -lresolv

clean:
	rm -fr ./build/*
