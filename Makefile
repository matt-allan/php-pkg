php_version = php-7.3.5


$(php_version).tar.gz:
	@echo "Downloading php-src..."
	curl -O -L -C - https://www.php.net/distributions/$(php_version).tar.gz

$(php_version): $(php_version).tar.gz
	@echo "Extracting php-src..."
	tar -xzf $(php_version).tar.gz

$(php_version)/sapi/embed/php_embed.o: $(php_version)
	@echo "Building php..."
	cd $(php_version) && ./configure --enable-static --disable-all --disable-cli --enable-embed=static --prefix=$(dirname $(pwd))/php
	cd $(php_version) && make -j 2
	cd $(php_version) && make install

embed: php
	gcc -o embed embed.c -I php/include/php/ -I php/include/php/main -I php/include/php/Zend -I php/include/php/TSRM -L php/lib -lphp7 -lresolv
