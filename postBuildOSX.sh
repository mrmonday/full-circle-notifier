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

if [ "`dirname $0`" != "." -o ! -e "fcnotify.pro" ]; then
	echo "Script must be run from the source directory"
	exit 1
fi

if [ -z "$1" ]; then
	echo "Usage: $0 <path to .app>"
	exit 1	
fi

# Generate necessary files for distribution
macdeployqt-4.6 $1

# Remove unused ppc code, cuts the resulting app's size in half
cd $1/Content
for f in `find Frameworks -name Qt\* -type f`; do
	ditto --arch i386 $f ${f}2
	mv ${f}2 $f
done
for f in `find . -name \*.dylib -type f`; do
	ditto --arch i386 $f ${f}2
	mv ${f}2 $f
done

# Update Info.plist file
# Change info string
perl -pi -e 's|Created by Qt/QMake|Notifier for Full Circle Magazine|' Info.plist

# Update the bundle identifier
perl -pi -e 's|yourcompany|octarineparrot|' Info.plist

# Don't leave an icon in the dock
perl -pi -e 's|</dict>|<key>LSUIElement</key><true/></dict>|' Info.plist

# Rename the .app
mv $1 `dirname $1`/Full\ Circle\ Notifier.app
