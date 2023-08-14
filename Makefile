# Foxnit - Wrapper for runit init system
# Copyright (C) 2022 Ján Gajdoš 

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

PREFIX ?= /usr/local

CC = gcc
CFLAGS = -Wall -Wextra -g

all:
	$(MAKE) -C src all
	$(CC) $(CFLAGS) -o foxnit main.c -L. -lfoxnit

clean:
	$(MAKE) -C src clean
	rm foxnit

install:
	mkdir -p $(PREFIX)/bin
	install -m 755 foxnit $(PREFIX)/bin
	rm foxnit
	rm libfoxnit.a

uninstall:
	rm $(PREFIX)/local/bin/foxnit
