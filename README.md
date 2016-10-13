# Dissertation Experiment

This project contains the source code for my dissertation experiment, which consists of an approach / avoidance video game, 
word categorization tasks (with masked primes and precisely timed responses), and various questionnaires.  The experiment was
administered in two different ways -- in person, on the experimenter's computer; and online, in which the subject downloaded
the program onto their computer and ran the experiment, and following which the resulting data was uploaded to a server via FTP.

It is written almost entirely in C++ / DirectX, with Python scripts for "decrypting" / analyzing data.  It consists of a 
Visual Studio Project, and runs on Windows machines running DirectX 11 or higher.  The code is entirely my own, except for the
ftp client (in the FTPClient directory), which was written by Thomas Oswald (see that directory for license information).

