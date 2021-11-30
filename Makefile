SRC      = ./src

.PHONY = all clean

all:
	cd $(SRC) && $(MAKE) all

hex: image.png
	xxd image.png > dumb

clean:
	@(rm -rf image.png dump rig)
	cd $(SRC) && $(MAKE) clean
