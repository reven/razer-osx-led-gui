# razer-osx-led-gui

This is an agent status bar app that controls keyboard leds on a Razer Blade laptop running on MacOS.

![screenshot.png](screenshot.png?raw=true "Razer-OSX-LED-GUI")

It's basically a wrapper for the command-line application [osx-razer-led](https://github.com/DocSystem/osx-razer-led) originally developed by [@DocSystem](https://github.com/DocSystem), building upon work by [@kprinssu](https://github.com/kprinssu) and [@dylanparker](https://github.com/dylanparker), and the linux driver code by [@terrycain](https://github.com/terrycain). With some _special_ glue.

## Usage
Download the [latest release](https://github.com/Reven/razer-osx-led-gui/releases/latest/download/Razer-OSX-LED.GUI.zip). Just drop the app into your applications folder. You don't need to install the original `razer-osx-led` script, it's baked in. You can add it to your startup items to have the saved settings apply after startup.

__NOTE__: You can force the color pickers to appear using `Alt+click` on the color wells. I don't know why the color wells don't always bring up the color pickers. Not closing the color picker before dismissing the window causes this to happen immediately, but no idea how to force a new instance directly from the code. If anyone can help, it would be awesome.

The currently supported effects are the same as in the original command line app, with some adaptations. Your device might not support all of them.

* __info__: Used internally to show detected device
* __static__: Accepts optional color. Set all keys to color (or green default if not specified)
* __breathe__: Accepts 2 optional colors. Pulse keys with color, or between both colors specified (or random colors if none specified)
* __starlight__: Accepts optional color. Twinkle effect with custom color. Speed not implemented, defaults to `2`. __NT__
* __reactive__: Accepts optional color. Keys light up with color and fade when pressed. Defaults to green. Speed not implemented, defaults to `2`. __NT__
* __spectrum__: Cycles through color spectrum
* __wave__: Spectrum colored wave accross keyboard. Direction not implemented, defaults to `right`. __NT__
* __brightness__: Slider passes an integer (0-255) that sets the brightness value.

__NT__: Haven't been able to test, as this effect is not supported by my hardware.

## To do
* Add contraints to views and clean up layout

### BUGS:
* Color pickers don't always appear. Is there a way to integrate them in the view? Help!
* Dismissing 'About' view without clicking on 'Back' leaves the app in an undetermined state, with no way to quit.
* Driver not always responds correctly; this isn't handled at all. Should retry?

### Nice to haves:
* Some kind of error management?
* Check selected things to see if they form a compatible combination for command.

## Acknowledgements
These are the repos where most of the actual work was done:
* https://github.com/openrazer/openrazer
* https://github.com/kprinssu/osx-razer-blade
* https://github.com/dylanparker/osx-razer-led
* https://github.com/DocSystem/osx-razer-led

## License
This project unless otherwise stated in the file is licensed under the GPLv2 license.
