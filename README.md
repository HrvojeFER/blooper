# blooper
Loopstation designed for beatboxers with minimal equipment and hardware requirements running on Microsoft Windows,. Written in C++ using the JUCE framework. Used for my final thesis at FER (Faculty of Electrical Engineering and Computing, University of Zagreb).

# Setup guide
1. Visual Studio
2019 Edition along with the "Desktop development with C++" workload (or more specifically MSVC v142) and Microsoft 10 SDK v10.0.18362.0 is needed for opening and running the project in Visual Studio.
The solution file (blooper.sln) is located in "Projucer/builds/VisualStudio2019".

2. Projucer
v5 and above and the above mentioned Visual Studio setup is needed for opening and running the project in Projucer.
Projucer doesn't have an installer, so after it's been downloaded, it is recommended to put it in "Program Files (x86)" along with other programs, create a shortcut to "Projucer.exe" on the desktop, taskbar or stat menu and edit the Global Path of JUCE to the folder where "Projucer.exe" is located. Global Paths are edited in Projucer in "File" -> "Global Paths..."
