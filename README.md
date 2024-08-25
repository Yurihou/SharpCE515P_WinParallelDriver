# SharpCE515P_WinParallelDriver
Sharp CE-515P is an "ancient" plotter which is used for Sharp PC-1500. It can be communicated with parallel or serial port. This programme is for Windows to drive CE-515P. With graphical mode is used, we can save our command into txt file and send it to this programme.

![/ce-515p.jpg](/ce-515p.jpg)

This programme is based on Windows printer API given by Microsoft Learn. [Link(English)](https://learn.microsoft.com/zh-cn/previous-versions/troubleshoot/windows/win32/win32-raw-data-to-printer) [Chinese](https://learn.microsoft.com/zh-cn/previous-versions/troubleshoot/windows/win32/win32-raw-data-to-printer)

## Command Format
The command format of this program is

```
ce515p [printer name] [command text file]
```

Where

+ [printer name] is the plotter name in your computer;
+ [command text file] is the plotter command file.
 
## Printer Setup

This programme is based on Windows printer API. To add your printer port to printer list, please follow section "Installing a local printer manually" of [this link](https://www.windowscentral.com/how-install-older-printer-windows-10), and notice when it comes to "Install the printer driver" step, please choose "Generic - Generic / Text Only". Then remember the printer name and send it to "printer name" parameter above.

## Printer Command

The printer command can be found in [Sharp CE-515P Manual.pdf
](https://github.com/Yurihou/SharpCE515P_WinParallelDriver/blob/main/Sharp%20CE-515P%20Manual.pdf) or [Original link from manualslib.com](https://www.manualslib.com/manual/3383039/Sharp-Ce-515p.html). Here comes the brief introduction.

### General knowledge

+ **Each command (except TEXT mode raw words) ends with CR (0x0D).**
+ Spaces (0x20) will be ignored.
+ Only integer in command can be accepted. dot (0x2E) and digits followed will be ignored.

Here are some general commands for both TEXT and GRAPH mode:
+ TEXT mode and GRAPH mode is available. to set mode, use [ESC] + "a" (0x1B 0x61) to TEXT mode or [ESC] + "b" (0x1B 0x62) to GRAPH mode.
+ Use [ESC] + "?" + "a" to "o" (0x1B 0x3F 0x61 to 0x1b 0x3F 0x6F) to set text size.
+ USE [ESC] + "0" to "3" to change the pen with different slot on the printer.

### TEXT mode

Text mode will let printer plot raw text into paper. ASCII which could be displayed in computer can also be plotted by printer.

Besides, these symbol are functional to the printer (almost a few ASCII functional codes):
+ Backspace (0x08) to let pen backspace by one character. If the pen is at the most left place, this symbol will be ignored.
+ Line Feed (0x0A) to move paper down to the next line without moving the pen.
+ Line Up (0x0B) to move paper up to previous line without pen moving.
+ Return (0x0D) to return pen to most left place without paper moving.


### GRAPH mode

When the printer is set to GRAPH mode, graph command can be used for plotting lines into the paper. Here is the commands

#### L -- Line shape
"L0"-"L15" for different line shape. "L0" for default solid line, "L1" for dotted line, "L2"-"L14" for different wide dashed line, and "L15" for solid line again.
#### A -- Back to TEXT mode
Without any parameter. The same function as [ESC]+"a"
#### Q -- Character rotation
"Q0" for default no rotation. "Q1", "Q2", "Q3" for 90, 180, 270 degrees rotation. This command is only for GRAPH mode. When set to TEXT mode, the rotation is cancelled.
#### M -- Absolute movement with pen up
Examples:
```
M100,-150,200,-300
```
For this command, the pen will move through straight line with pen up from current position to X=100, Y=-150 (100,-150), then to (200,-300). 

The number here for X and Y is same to Cartesian coordinate system (X positive for right, Y negative for down). Unit is 0.2mm, thus, (200,-300) means 40mm right and 60mm down from origin.
#### R -- Relative movement with pen up
Examples:
```
R100,-150,200,-300
```
Coordinate and unit is the same as command "M". So are the following "D" and "J" commands.
#### D -- Absolute movement with pen down
Examples:
```
D100,-150,200,-300
```
That means to draw a line from current position to (100,-150), then to (200,-300).
#### J -- Relative movement with pen down
#### I -- Set current (pen and paper) position as origin (X=0, Y=0)
#### H -- Return to origin with pen up
#### P -- Print letters
The letters will be aligned from left bottom to current position.
