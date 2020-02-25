..  PlatformIO Copyright 2014-present PlatformIO <contact@platformio.org>
    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at
       http://www.apache.org/licenses/LICENSE-2.0
    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.

..  Current project Copyright 2020 Vladimir Drobyshevskiy <v.heathen@gmail.com>
    Licensed under the same conditions as above

*****************************
Esp32 Simple Control Firmware
*****************************

Based on Espressif Esp-idf examamples.

More details about the project is in `the control panel repo <https://github.com/vheathen/esp32-simple-control-panel>`_.

How to build and run the firmware project
=========================================

1. `Install PlatformIO Core <http://docs.platformio.org/page/core.html>`_
2. Download `development platform with examples <https://github.com/platformio/platform-espressif32/archive/develop.zip>`_
3. Extract ZIP archive
4. Run these commands:

.. code-block:: bash

    # Change directory to example
    > cd esp32-simple-control-firmware

    # Edit planformio.ini and set you wifi SSID and password!

    # Build project
    > platformio run

    # Upload firmware
    > platformio run --target upload

    # Clean build files
    > platformio run --target clean


P.S. Alternatively you can use `PlatformIO IDE for VSCode plugin <https://docs.platformio.org/en/latest/ide/vscode.html>`_