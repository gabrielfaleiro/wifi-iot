# wifi-iot
This repo contains a low power wifi communication iot device code based on the Wemos D1 mini board.

## Development IDE - Platform IO
https://docs.platformio.org/en/latest/boards/espressif8266/d1_mini_lite.html

Set-up IDE: https://docs.platformio.org/en/latest/integration/ide/vscode.html#quick-start

## Design decisions
- For powering the Board from a set of batteries, they shall be connected to the 5V pin of the board. It has the ME6211 integrated circuit, which outputs 3.3V when input ranges 3V to 6V.
- Deep sleep mode:
    - RST must be set to LOW value for waking up the micro again. Only the 33.V -> 0V transition triggers the wake up of the micro.
    - D0 pin shall be connected to RST pin for auto reboot.
- Reed switch
    - Shall be connected to pin RST. Ground signal must be send to RST for wake up. 
- MQTT Client
- Power consumptions
    - Deep Sleep: 0.6mA
    - Sending MQTT messages: <80mA (70-80mA)

## TODO
 in normal and deep sleep operations
- Use internal permanent memory for credentials
- Create a wifi hotspot and a webserver when a valid configuration is not found.
    - How can this be forced for reconfiguration?
- Attach a temperature and humidity sensors