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

## Game Demonstration
When the console is turned on by the switch in the middle, it will light up and the main menu will show up.  
It displays the Highest achieved score that is saved to the Micro SD Card and also a choice to start a game.  

<img src="/assets/MainMenu.jpg" width="400">  

When you press the corresponding button to the **play** option a new game will begin.  
A question will be generated and the answers selectedd. They will either ask for a  
translation of the kanji displayed or for a reading in japanese.  

<img src="/assets/Question.jpg" width="400">  

If the chosen answer is wrong the game will deduct one life point and will display the  
correct answer for the question.  

<img src="/assets/QuestionWrong.jpg" width="400">  

Otherwise if the chosen aswer is correct, the game will give 1 point and notify the player  
that the chose answer has been correct.  

<img src="/assets/QuestionCorrect.jpg" width="400">

The game will end once the player has ran out of lives, there is no way to end a game without
losing unless you power off the console. At the end of a game, the player will be notified
about the score they managed to get. If the score is more than the recorded high score, it will  
be saved as the new highest score.  

<img src="/assets/GameOver.jpg" width="400">

## Possible improvements
As this was the first time I ever made something involving a Custom PCB and so many components, I have missed to think about
certain things that could prove beneficial for future projects.

### Cleaner PCB
For a first time design I feel like this one is OK, but for future projects the design should incorporate less prototyping boards and more individual components, like SMD components. These may require more complicated soldering methods but the final product would have a much more professional and clean look.  
Major imporvement would be the use of an independent ESP32 chip that has been flashed with the project.
Another big improvement would come from the TFT display. Instead of using one with a board and pins, an alternative variant that utilises a connector would be better in terms of how thin the console could be.

### Better software
This was also my first time programming on an ESP32 and ESP-IDF, so a lot was figured out through the examples given by Espressif, but the code was not well suited for this project.  
The most improvement that could be done is in the rendering, because the current rendering method is so slow that the FPS is lower than 1, but it can most certainly be at least as good as 10FPS.

### Mounting holes
Mounting holes designed into the PCB could allow for a case to be made for the console, giving it a more solid look and making it a lot more comfortable and safer than the current one, where you hold the PCB and have to take care of what touches the board.
