# CNN
Convolutional Neural Network

I am in progress for this project, so current aim is to
make difference between ZERO and ONE on a picture you drawn

You can use this tool only if you have linux
First of all you need to install something to run my tool

  sudo apt-get install freeglut3 freeglut3-dev libglew1.5 libglew1.5-dev libglu1-mesa libglu1-mesa-dev libgl1-mesa-glx libgl1-mesa-dev mesa-common-dev
  sudo apt-get install libboost-dev

When you've done it you should make some files executable

  sudo chmod +x build
  sudo chmod +x buildCNNTool
  sudo chmod +x buildPerceptronTool
  sudo chmod +x buildImageTool

You need to build project

  ./buildImageTool
  ./buildPerceptronTool
  ./buildCNNTool
  ./build

You can just run binary file to make sure that it works(but not so perfect)

  ./bin

Draw number(0 or 1) and close PAINT TOOL window.
You can see answer of my CNN which number it is in terminal
where you run application.
This is loop, so you can draw while you want.
You can quit my tool (CTRL + C).
