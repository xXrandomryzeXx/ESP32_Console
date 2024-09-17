# ESP32_Console
## Instalation
Following the official documentation from Espressif, you have to setup the standard toolchain for your system:  
[Linux/MacOS](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/linux-macos-setup.html)  
[Windows](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/windows-setup.html)  
### Linux/MacOS
After following the documentation you should have the standard toolchain setup.  
Now you can clone the repository. Once inside it, run the command  

`. $HOME/esp/esp-idf/export.sh`  

or for Fish  

`. $HOME/esp/esp-idf/export.fish`  

This will setup the ESP-IDF environment for the project.

Inside the cloned repository should be a directory called `SD_CARD_FILES`.  
Copy its contents to a **Micro SD Card** and format the card to the **FAT32 Format**

After inserting the card into the Micro SD Card Reader, you can build and flash the project to the ESP32  
This is done by performing the following command inside the cloned repository.  

`idf.py build flash monitor`

This will  
1. Build the project
2. Flash it to the ESP
3. Print the debug information to the terminal

### Windows
As I was not developing this on Windows, I don't know how the setup would go for it.

## About
The project was developed as a final assigment for high school. It utilizes the ESP32 to make a console-like game for learning Japanese Kanji.
It consists of multiple components:
1. Custom PCB
2. Node MCU32S board
3. 3.2 Inch TFT SPI Display board
4. Micro SD Card Reader board
5. 4x Push Button
6. 4x 10K resistor
7. Power Swtich
8. Battery box for 3 AAA 1.5V batteries

## The PCB Board
The board is two sided THT. Each component is labeled where it's supposed to be soldered on the board.  
The clean board looks like this  

<img src="/assets/CleanFrontPCB.jpg" width="400"><img src="/assets/CleanBackPCB.jpg" width="400">   

When attaching the components you have to match their orientation with the text on the silkscreen.  
The populated PCB should look like this

<img src="/assets/FrontPCB.jpg" width="400"><img src="/assets/BackPCB.jpg" width="400">   
