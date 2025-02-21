# telescope

Work on ESP32 with A4988 stepper drivers.
RTS2 server is working with Stellarium telescope plugin. Never tested with others softwares.
Parameters are working with an HEQ5 equatorial mount. uSteps are set at 1/16

[Projet Fusion 360](https://gmail330021.autodesk360.com/g/projects/20200813323185226/data/dXJuOmFkc2sud2lwcHJvZDpmcy5mb2xkZXI6Y28uZnl3eEFBUC1SUFc4RGFCX3Q2WFNFdw)

[PDF mécanique](https://github.com/jujax/telescope/blob/master/m%C3%A9canique.pdf)

Remote debug => https://github.com/JoaoLopesF/RemoteDebug

## RTS2 Server and Endpoints

The RTS2 server provides several endpoints for interacting with the telescope. These endpoints include:

- `/reboot`: Reboots the ESP32.
- `/settings`: Allows updating the WiFi SSID and password.
- `/api/devbytype`: Returns the type of device (e.g., "telescope").
- `/api/deviceinfo`: Returns information about the device.
- `/api/get`: Returns the current state of the telescope, including longitude, latitude, altitude, and target distance.
- `/api/cmd`: Allows sending commands to the telescope, such as moving to a specific right ascension (RA) and declination (DEC).

## WiFi Initialization Process

The WiFi initialization process involves connecting to a specified WiFi network using the provided SSID and password. If the connection is successful, the ESP32 will be assigned an IP address, and the ArduinoOTA service will be started for over-the-air updates. If the connection fails, the ESP32 will attempt to connect to a default network with the SSID and password set to "telescope".

## Motor Control Functionality

The motor control functionality is implemented using A4988 stepper drivers. The code initializes two stepper motors for the right ascension (RA) and declination (DEC) axes. The motors are configured with a specific RPM and microstepping mode. The code also includes tasks for controlling the motors and handling movement commands. The motors can be commanded to move to specific positions based on the received RA and DEC values.
