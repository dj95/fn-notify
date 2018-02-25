fn-notify
----------------------

A simple notification pop-up that displays the current volume(for now).

There are plans to display battery, brightness and touchpad status either.


![Screenshot](/img/music.png)


**Warning: This project is work in progress and may cause instability or something else!**


### Requirements

- `Xlib`
- `libxext`
- `libxinerama`


### Installation

- Install the requirements.
- Run `make all` in `src/`.
- Copy the binary `fn-notify` to the place you need it.


### Usage

- Just run it with `./fn-notify $LEVEL` or `fn-notify $LEVEL` from the `$PATH`.
  `$LEVEL` ois a value between `0 and 100`. For a mute-popup run `fn-notify $LEVEL --mute`.


### TODO

- config file
- further information
- code refactoring and optimization
- brightness, battery and touchpad status


### License 

(c) 2018 - Daniel Jankowski

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
