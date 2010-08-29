#!/bin/sh
#
# fcnotifier: Notify the user of a new full circle issue/podcast/news item
# Copyright (C) 2010  Robert Clipsham <robert@octarineparrot.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

dlls="QtCore4.dll QtGui4.dll QtNetwork4.dll libgcc_s_dw2-1.dll mingwm10.dll qjson0.dll"
mkdir -p build
for f in $dlls; do
	echo $f
	cp /c/Qt/2010.04/qt/bin/$f build/
done
cp ../fcnotify-build-desktop/release/fcnotifier.exe build/ 
cp installer.nsi build/
cp COPYING build/
cd build
makensis installer.nsi

