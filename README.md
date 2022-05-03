# Assembler
The next program is an assembler (although with data and values different from the regular assembler, but works just like it)

[compilation instructions.pdf](https://github.com/Adir15/Assembler/files/8609763/compilation.instructions.pdf)

We will first insert the source file containing the assembly language

![Annotation 2022-05-03 1231541](https://user-images.githubusercontent.com/104766812/166434508-a6923d56-fd56-4875-b8c0-f06b361c79e2.jpg)

We will then receive a number of files, below is a brief description of each file we receive (for a detailed explanation - written in Hebrew [assembler.pdf](https://github.com/Adir15/Assembler/files/8609626/assembler.pdf) )

am. file -
containing the source file after the pre-assembler stage. Notice the macro from the previous image

![Annotation 2022-05-03 1240252](https://user-images.githubusercontent.com/104766812/166434539-6614f92e-73d8-42ca-93de-458f4292e51a.jpg)

ob. file- 
contains the machine code

![Annotation 2022-05-03 1242023](https://user-images.githubusercontent.com/104766812/166434574-7084284c-8ca6-409b-a56e-cb72b11629c6.jpg)

ext. file-
contains details about the addresses where there is a symbol declared as external

![Annotation 2022-05-03 1246334](https://user-images.githubusercontent.com/104766812/166434646-1f009ebf-da92-4ca2-a969-a655988699a6.jpg)

ent. file -
it contains details about each symbol declared as an entry

![Annotation 2022-05-03 125539](https://user-images.githubusercontent.com/104766812/166434909-4bed76a4-547f-4028-857e-84c6198eedf6.jpg)
