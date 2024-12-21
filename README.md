# Electronjs app for displaying data from serial

This program is indented to run on raspberry pi 5 with arduino R4 connected through usb. Arduino is connected to Arduino IOT Cloud

## Remeber

when changes are made with html file run `npx tailwindcss -i ./input.css -o ./output.css --watch`

to run on wayland architecture add this line to ~/.bashrc file `export DISPLAY=:0`

to run program print `npm run start` in terminal

## how to create executable on Rpi5

See this [site](https://www.electronjs.org/docs/latest/tutorial/tutorial-packaging)

Executable will be created in out/make ....deb directory

For installing info see this [site](https://chipwired.com/install-deb-file-raspberry-pi/)

