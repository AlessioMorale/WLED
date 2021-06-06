# Matrix Clock
This clock uses a matrix of 7x10 leds like the [Lazy Grid Clock v2](https://www.thingiverse.com/thing:4263318) design from the Thingiverse user [parallyze](https://www.thingiverse.com/parallyze)

## Installation 

### Building the code

1. Copy `wled00/my_config_sample.h` as `wled00/my_config.h` (or edit the existing file if there is one)
1. Add the following line at the end of the file


``` C
#define USERMOD_CLOCK_MATRIX
```

1. compile and upload (follow the standard WLED documentation for up-to-date instructions).

### Configuring the software.

#### Base WLED configuration

##### Network

Follow the standard WiFI setup procedure (the clock needs an internet connection to sync the time as it has no rtc on board). Ensure you can access the configuration UI from your wifi.

##### LEDs configuration

1. Select `Config` > `LED Preferences`
2. Set `Total LED count` to the total number of leds connected (i.e. 77 if you have individual strips for each column or 83 if you have a single strip with two unused leds connecting columns 1&2, 3&4 and 5&6 as shown in the Thingiverse page).
3. Under Hardware setup select `WS281x` the pin used to connect the light under `Pin:` and the same number of LEDs set above under `Count:`
4. Click `save`

##### Time configuration

1. Select `Time & Macros` (within the `Config` page)
2. Check `Get time from NTP server`
3. In the textbox below either leave the default one or set your favourite NTP server
4. Select the right Timezone/Offset for your area 
5. Leave anything else unchanged and click `Save`

##### Finalize & reboot

1. Click `Security & Update` (within the `Config` page)
2. Click `Save & Reboot` and wait until the page is reloaded

#### Basic segments and effect configuration

Create a preset with the following base configuration:

``` json
{
    "on": true,
    "bri": 139,
    "transition": 7,
    "mainseg": 0,
    "seg": [
        {
            "id": 0,
            "start": 0,
            "stop": 11,
            "grp": 1,
            "spc": 0,
            "on": true,
            "bri": 255,
            "col": [
                [
                    255,
                    0,
                    0
                ],
                [
                    0,
                    0,
                    0
                ],
                [
                    250,
                    139,
                    30
                ]
            ],
            "fx": 0,
            "sx": 128,
            "ix": 128,
            "pal": 8,
            "sel": true,
            "rev": true,
            "mi": false
        },
        {
            "id": 1,
            "start": 11,
            "stop": 22,
            "grp": 1,
            "spc": 0,
            "on": true,
            "bri": 255,
            "col": [
                [
                    255,
                    0,
                    0
                ],
                [
                    0,
                    0,
                    0
                ],
                [
                    250,
                    139,
                    30
                ]
            ],
            "fx": 0,
            "sx": 128,
            "ix": 128,
            "pal": 8,
            "sel": true,
            "rev": false,
            "mi": false
        },
        {
            "id": 2,
            "start": 22,
            "stop": 33,
            "grp": 1,
            "spc": 0,
            "on": true,
            "bri": 255,
            "col": [
                [
                    255,
                    0,
                    0
                ],
                [
                    0,
                    0,
                    0
                ],
                [
                    250,
                    139,
                    30
                ]
            ],
            "fx": 0,
            "sx": 128,
            "ix": 128,
            "pal": 8,
            "sel": true,
            "rev": true,
            "mi": false
        },
        {
            "id": 3,
            "start": 33,
            "stop": 44,
            "grp": 1,
            "spc": 0,
            "on": true,
            "bri": 255,
            "col": [
                [
                    255,
                    0,
                    0
                ],
                [
                    0,
                    0,
                    0
                ],
                [
                    250,
                    139,
                    30
                ]
            ],
            "fx": 0,
            "sx": 128,
            "ix": 128,
            "pal": 8,
            "sel": true,
            "rev": false,
            "mi": false
        },
        {
            "id": 4,
            "start": 44,
            "stop": 55,
            "grp": 1,
            "spc": 0,
            "on": true,
            "bri": 255,
            "col": [
                [
                    255,
                    0,
                    0
                ],
                [
                    0,
                    0,
                    0
                ],
                [
                    250,
                    139,
                    30
                ]
            ],
            "fx": 0,
            "sx": 128,
            "ix": 128,
            "pal": 8,
            "sel": true,
            "rev": true,
            "mi": false
        },
        {
            "id": 5,
            "start": 55,
            "stop": 66,
            "grp": 1,
            "spc": 0,
            "on": true,
            "bri": 255,
            "col": [
                [
                    255,
                    0,
                    0
                ],
                [
                    255,
                    0,
                    255
                ],
                [
                    250,
                    139,
                    30
                ]
            ],
            "fx": 0,
            "sx": 128,
            "ix": 128,
            "pal": 8,
            "sel": true,
            "rev": false,
            "mi": false
        },
        {
            "id": 6,
            "start": 66,
            "stop": 77,
            "grp": 1,
            "spc": 0,
            "on": true,
            "bri": 255,
            "col": [
                [
                    255,
                    0,
                    0
                ],
                [
                    0,
                    0,
                    0
                ],
                [
                    250,
                    139,
                    30
                ]
            ],
            "fx": 0,
            "sx": 128,
            "ix": 128,
            "pal": 8,
            "sel": true,
            "rev": true,
            "mi": false
        },
        {
            "stop": 0
        },
        {
            "stop": 0
        },
        {
            "stop": 0
        },
        {
            "stop": 0
        },
        {
            "stop": 0
        },
        {
            "stop": 0
        },
        {
            "stop": 0
        },
        {
            "stop": 0
        },
        {
            "stop": 0
        }
    ]
}
```

Adjust segments `start`/`stop` and `reverse` values according to your wiring.

the Third colour in the palette selection is used for the digits (next improvement will be moving this to some proper setting).

Play with effect/colour as you wish.

Under `Config` > `LED Preferences` set `Apply preset` as the number of this preset so it is applied at startup. 

Enjoy!
