Version 1 of the TuneEditor, a simple ABC notation editor and viewer.

Written using Qt 5.5.1, and uses QtWebKit. Will not build with Qt 5.6. RIP QtWebKit, it's been a blast.

Build instructions:

Track 1: Open in QtCreator, and click 'build'.

Track 2: Run qmake, then make. make install if you dare.

Tested so far only on Linux Mint, although I see no reason why it would not work on any other platform - there is nothing Linux specific in the code.

The actual parsing of the ABC is handled by the abcjs JavaScript library, running inside QtWebKit. It's kind of overkill to use QtWebKit just for this, but it works well, and we were not able to find any libraries as good as abcjs. We are working on a port of abcjs to the QtQuick runtime, but no promises. ABCJS lives at http://abcjs.net/.


Using the application:

-Select a directory to save the files.

-The filename will be generated from the name of the tune, unless you enter a different one.

-Seelcting 'Autosave' will save the file each time the contents are changed, except if it's the filename, because then you would have a LOT of files being created, and that would be annoying.

-Enter the tune details in the boxes to the left, and the ABC in the big box in the middle. You should see the even larger panel on the right update with the parsed sheet music on the right - wow!!

-Data entered into the fields is stored when you close and reopen the application. It's so that you don't need to enter everything each time.

-Hit 'New' to clear the fields and start a new tune. Save directory, transcriber name, and note length is not removed.

-If you are copying a piece of abc which was _not_ originally transcribed by you, please don't use your own name as the transcriber. That's plagiarism, and it's not nice.


TODO
-It's currently not possible to print the sheetmusic, and is not required by the project we're working on. But it's not a huge job so may come one day.
-It would be neat if you could enter the type in the type field - say it's a reel or a jig, for example - and then see the meter added automatically. Yup, that would be fun, and not so very hard to do, if there weren't so many different types of tune in the world.


Further info:

-If you are new to ABC notation, we recommend reading the ABC Primer at:
http://trillian.mit.edu/~jc/doc/doc/ABCprimer.html

-The ABC fields in this application mirror the fields used on http://thesession.org. 

-You should only enter one tune for each file - that's what the application is intended to for. Adding a second tune in the box doesn't work. Sorry.


Have fun!

Monkey&Wolf, California.
