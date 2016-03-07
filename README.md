Version 1.1 of the TuneEditor, a simple ABC notation editor, viewer, and printer.

Written using Qt 5.5.1.

Build instructions:

Step 1: Make sure that you have Qt 5 installed somewhere. This is developed with Qt 5.5.1.
   - You can get Qt from downloads.qt.io/qt.
Step 2: Get the ABCShots library.

Clone the ABCShots library and the TuneEditor library into the same directory, ie

<somedir>
    |-ABCShots
    |-TuneEditor

Step 3: Build ABCShots with qmake

i. mkdir abcShotsBuildDir
ii. cd abcShotsBuildDir
iii. /path/to/qmake ../ABCShots/ABCShots.pro
iv. make
v. make install

ABCShots installs itself into a directory called "ShotGlass" - it's expected by the TuneEditor. You can change this in the .pro files if you don't like it.

vi. cd .. 
vii. mkdir tuneEditorBuildDir
viii. cd tuneEditorBuildDir
ix. /path/to/qmake ../TuneEditor/TuneEditor.pro
x. make

Tada!! You have a finished TuneEditor application. At least, that's the Unix way of doing it.

Compilation on Windows: I have no idea. The Windows version is cross compiled on a Linux machine using MXE, and the Mac version is, well, Mac is Unix, so it works the same.



Tested and working OK on Linux Mint, Macintosh, and Windows machines [cross compiled with MXE from Linux. MXE is awesome].



The parsing is handled by ABCShot, which pumps out an SVG file to the tmp directory. This is then loaded again and displayed in an SVG window. Same kind of thing for the PDF export feature. It would be neat if ABCShots could send an SVG stream back instead, well, maybe one day. More likely I move it to QtWebKit, the pending cross-platform webview that uses a native WebView, and then use HTML on all platforms because abcjs [abcjs.net] is lovely.



Using the application:

-Select a directory to save the files.

-The filename will be generated from the name of the tune, unless you enter a different one.

-Enter the tune details in the boxes to the left, and the ABC in the box on the right. Don't enter all the ABC here, just the actual music notes. You should see the large panel at the bottom update with the tune - wow!!

-Data entered into the fields is stored when you close and reopen the application. It's so that you don't need to enter everything each time.

-Hit 'New' to clear the fields and start a new tune. Save directory, transcriber name, and note length is not removed.

-If you are copying a piece of abc which was _not_ originally transcribed by you, please don't use your own name as the transcriber. That's plagiarism, and it's not nice.


TODO
-It would be neat if you could enter the type in the type field - say it's a reel or a jig, for example - and then see the meter added automatically. Yup, that would be fun, and not so very hard to do, if there weren't so many different types of tune in the world.


Further info:

-If you are new to ABC notation, we recommend reading the ABC Primer at:
http://trillian.mit.edu/~jc/doc/doc/ABCprimer.html

-The ABC fields in this application mirror the fields used on http://thesession.org. 

-You should only enter one tune for each file - that's what the application is intended to for. Adding a second tune in the box doesn't work. Sorry.


Have fun!

Monkey&Wolf, California.
