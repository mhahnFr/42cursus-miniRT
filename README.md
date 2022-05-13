# Welcome to 42cursus-miniRT!
This repository contains a group project of the [42 school]. The task for the two students is to write a small raytracer
using the C programming language and the graphics library of the [42 school], the MiniLibX.  
This project has been done by **[kyomawolf]** and **[mhahnFr]**.

### Screenshots:
<p><img src="https://www.github.com/mhahnFr/42cursus-miniRT/raw/main/screenshots/the_green_man_low.png" alt="The green man" width="400"/>
<img src="https://www.github.com/mhahnFr/42cursus-miniRT/raw/main/screenshots/reflection_room_wide_low.png" alt="Reflection room" width="400"/></p>
<p><img src="https://www.github.com/mhahnFr/42cursus-miniRT/raw/main/screenshots/hall.png" alt="Temple" width="400"/>
<img src="https://www.github.com/mhahnFr/42cursus-miniRT/raw/main/screenshots/the_violent.png" alt="The violent" width="400"/></p>

### Approach
_Description coming soon!_

### Usage
In order to get started, clone and build the project:
- Clone: ``git clone https://www.github.com/mhahnFr/42cursus-miniRT``
- and build: ``cd 42cursus-miniRT && make``.

The application can either be started with only a file as parameter: ``./miniRT <file>``. The scene will be rendered
using the default settings. Using the following options, the settings can be adjusted:
- ``-h`` Prints the help screen. If launched without any parameters or the given parameter is not recognized, this
option is invoked.
- ``-v`` Displays the currently used rendering configuration.
- ``-m <bounces>`` Sets how often a ray that is rendered can bounce off surfaces.
- ``-a <factor>`` Sets the factor of antialiasing. Each pixel is rendered ``1 + factor`` times.
- ``-r <width> <height>`` Sets the resolution of the window.
- ``-f <file>`` Specifies the scene file to be rendered.

> _Please note:_ using the above described options requires the scene file to be specified using the ``-f`` option.

### Platforms
This project has been written for and was tested on _macOS 10.15.7_.

© Copyright 2022 [mhahnFr], [kyomawolf]

[42 school]: https://www.42heilbronn.de/learncoderepeat
[kyomawolf]: https://www.github.com/kyomawolf
[mhahnFr]: https://www.github.com/mhahnFr