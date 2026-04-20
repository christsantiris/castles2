.PHONY: build run test dmg

APP      = dist/Castles3.app
BINARY   = $(APP)/Contents/MacOS/castle3_bin
LAUNCHER = $(APP)/Contents/MacOS/castle3

define PLIST_CONTENT
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
	<key>CFBundleExecutable</key>
	<string>castle3</string>
	<key>CFBundleIdentifier</key>
	<string>com.castles3.siege</string>
	<key>CFBundleName</key>
	<string>Castles III</string>
	<key>CFBundlePackageType</key>
	<string>APPL</string>
	<key>CFBundleShortVersionString</key>
	<string>1.0</string>
	<key>NSHighResolutionCapable</key>
	<true/>
</dict>
</plist>
endef
export PLIST_CONTENT

build:
	cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build

run: build
	./build/castle3

test: build
	./build/castles3_tests

clean:
	rm -rf build dist

debug:
	cmake -B build -DCMAKE_BUILD_TYPE=Debug
	cmake --build build
	./build/castle3

dmg: build
	rm -rf $(APP) dist/Castles3.dmg
	mkdir -p $(APP)/Contents/MacOS $(APP)/Contents/Resources $(APP)/Contents/Frameworks
	cp build/castle3 $(BINARY)
	printf '#!/bin/bash\ncd "$$(dirname "$$0")/../Resources"\nexec "$$(dirname "$$0")/castle3_bin"\n' > $(LAUNCHER)
	chmod +x $(LAUNCHER)
	cp -r data fonts music $(APP)/Contents/Resources/
	echo "$$PLIST_CONTENT" > $(APP)/Contents/Info.plist
	dylibbundler -od -b -x $(BINARY) -d $(APP)/Contents/Frameworks/ -p @executable_path/../Frameworks/
	hdiutil create -volname "Castles III" -srcfolder $(APP) -ov -format UDZO dist/Castles3.dmg
	@echo "DMG ready: dist/Castles3.dmg"
