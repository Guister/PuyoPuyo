This project consists of a PuyoPuyo game using c++ and the SDL library.
It was my first time using the SDL library so I had a lot to learn. I want to talk about my experience making this project.
Setting it up was an absolute nightmare and I had several problems that seemed beyond my control, where I would just go
"I guess this doesn't work". Documentation online was sometimes a bit scarce. I found outdated or straight up now deleted wikis
and documentation pages, specially for the secondary libraries. The SDL library itself was ok+ and I didn't have much
problems with it. Setting aside these negatives, it's great that a library like this exists and makes it so much easier
to build games in C++. I was definitely surprised by how several games I knew were made with SDL.

#Architecture
I decided to have a Game Manager that served as an engine for all the main actions the game would have to go through.
It contains the Window Renderer, the Sound Mixer and all game objects. I decided to make those 3 singletons because it
didn't make sense to me having instances of those classes. It's not a pattern I tipically use but I think it made sense
here. It didn't perfectly fit, as I was debating a lot if the game objects should have a render function, or if that was
responsability of the Window Renderer. There was some tension there, as I didn't want a Game Object to have access to
the renderer and sometimes needed it. Making the Window Renderer draw game objects by passing them as argument would
create cycic dependencies and I didn't want to pollute this small project with forward declarations.

#Game Objects
I struggled with the assets, and how I would make/obtain them. When found this format of several similar objects
that would differ on color on the same image file I decided to build the code around that. I think it ended up
pretty good. I also think that making most game objects tile based was benefitial. At first, I was going to use
collision a lot more but it was getting messy quickly. So I restrained the collision only for the balls and added
borders around the play field to limit the possibilities. The math with tiles really helped here.

#Sound:
I tried for a long time to play sounds using Mix_Chunk and Mix_LoadWAV with no success, with the error message
"Mix_PlayChannel: Tried to play a chunk with a bad frame". I searched and found no answers to this problem.
I figured out I could use Mix_Music and Mix_LoadMUS instead, and was planning on using them for all sounds in the
project. Then I noticed I was playing .mp3 files (like I've seen a ton of examples online) and decided to convert them
to .wav to see if it would fix the issue. It did. To this moment I still don't know exactly what was the problem, but
this solution is satisfactory.

#Text
I wanted also to use the SDL_ttf library to include a score for the game but it always failed me on startup.
TTF_Init() was always returning an error and I could not discover why. The library was linked successfuly so that was
not the problem. It's somewhat a shame because I would feel satisfied with this inclusion. Without it the project feels
at 95% completed to me.

#Logic
This was my favourite part of the project and I think it made me evolve as a programmer. I am specifically proud of
my solution to check the ball destruction as I think it is the first time I had to genuinely use recursion to solve a problem.
Movement and rotation was fun to code as well.
There are some booleans throught the project that I think didn't necessarly need to exist but they made things easier.
I would look at this for the first improvement I would make to the game logic.

#Compiling for windows
This was an insanely dreadful task that took me 10+ hours until everything was ok. Everything that could go wrong was
going wrong. I had to make small changes in the code to compile successful in Windows, that are not present in this
current version of the project. One of them was the rng was not working properly and was creating objects always with
the same color, so I had to use time for the rng seed generation. Others are small examples like this. I have included
the SDL dll's in the root folder for the executable to run without problems. I managed to run the executable successfully
on 1 Windows computer, but on another I couldn't because it was missing some dll's, that I ruled as outside the scope
of the task as they seemed not related to the project itself.
Commands I used to compile and link:
compiling: x86_64-w64-mingw32-g++ -Wall -c src/*.cpp src/gameObjects/*.cpp -Iinclude -I/home/guilhermelarga/Downloads/SDL2-2.0.22/x86_64-w64-mingw32/include -std=c++17 -DCMAKE_CXX_COMPILER=17 -m64
linking: x86_64-w64-mingw32-g++ *.o -o PuyoPuyo.exe -s -L/home/guilhermelarga/Downloads/SDL2-2.0.22/x86_64-w64-mingw32/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer


#Credits
balls -> https://opengameart.org/content/ball12
tilesetOpenGameBackground.png -> https://opengameart.org/content/sky-background
blocks.png -> https://opengameart.org/content/block-game-in-msx-palette
bg music -> https://opengameart.org/content/casual-classic-loop-8-bit
bounce -> https://opengameart.org/content/funny-comic-cartoon-bounce-sound
pop -> https://opengameart.org/content/bubbles-pop
font -> https://www.dafont.com/pt/theme.php?cat=115