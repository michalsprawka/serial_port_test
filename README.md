# Electronjs app for displaying data from serial

This program is indented to run on raspberry pi 5 with arduino R4 connected through usb. Arduino is connected to Arduino IOT Cloud

Pattern serial - electron app is on this [site](https://github.com/serialport/electron-serialport/tree/master)

## Remeber

when changes are made with html file run `npx tailwindcss -i ./input.css -o ./output.css --watch`

to run on wayland architecture add this line to ~/.bashrc file `export DISPLAY=:0`

to run program print `npm run start` in terminal

when you do not want to see a console comment this lines in `main.js`:

```
setTimeout(() => {
      mainWindow.webContents.openDevTools();
      }, 1000) // wait long enough
```

## How to create executable on Rpi5

See this [site](https://www.electronjs.org/docs/latest/tutorial/tutorial-packaging)

Executable will be created in out/make ....deb directory

For installing info see this [site](https://chipwired.com/install-deb-file-raspberry-pi/)

## Details

App has several kinds of input
- message input is a string with '#' as fist character, 
- temperature input is a string with $ as first character,
- pressure input is a string with '&' as first character
- console input - other strings
