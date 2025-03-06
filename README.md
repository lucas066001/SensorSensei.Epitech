# SensorSensei.Epitech
Study repository that holds content for IoT project

List of the steps we made to setup gateway and esp modules :

* Plug radio antenna on both modules.

* Follow [Meshtastic Get Started page](https://meshtastic.org/docs/getting-started/) :
    - Install ESP32 drivers : [Drivers](https://www.silabs.com/developer-tools/usb-to-uart-bridge-vcp-drivers?tab=downloads)

    - Reboot your computer and plug modules onto it. Test the connextion following this [page](https://meshtastic.org/docs/getting-started/serial-drivers/test-serial-driver-installation/).

    - Flash the ESP32 module via the [Web UI](https://flasher.meshtastic.org/). (Chrome or Microsoft edge browers only)  
    **WARN** : You need to press the BOOT button  while plugging the USB-C cable. (button named **PRG** for Heltech V3)

    - Flash the gateway module via the [Web UI](https://flasher.meshtastic.org/). (Chrome or Microsoft edge browers only)

    - Configure region on ESP32 module. It has been done via Serial connection and Python CLI. Open terminal on root folder of the project and execute this :
        ```bash
        python -m venv venv
        .\venv\Scripts\activate
        pip install -r requirements.txt
        meshtastic --set lora.region <REGION-CODE>
        ```
        The region code we used is EU_433.

    - Configure region code on gateway module. It as been done via [Web UI](https://client.meshtastic.org/) and serial connection. (Chrome or Microsoft edge browers only)  
    Chose region in **Config > LoRa > Region**. The region code we used is EU_433.
    - **WARN**: To be able to upload code on heltec boards you'll need to add this url ```https://resource.heltec.cn/download/package_heltec_esp32_index.json``` to your ```Arduino IDE > File > Preferences > Additional boards manager url``` 