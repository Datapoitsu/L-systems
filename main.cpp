#ifndef _ENGINEH_
#define _ENGINEH_

//Windows libraries.
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <vector>
#include <string>
#include <map>
#include <math.h>

//SDL2
#include <SDL2/SDL.h>

//My library.
#include <tools/input.h>

SDL_Window *Window;
SDL_Renderer *RenderInformation;
const int screenWidth = 1000;
const int screenHeigth = 1000;
int backgroundColour[3] = {125,125,125};

//Function declaration
void Start();
void Update();
bool endApp = false;

const double PI = 3.141592653589793;

struct Lsystem
{
    std::string name;
    float moveChange;
    float angleChange;
    std::string row;
    std::map<char, std::string> rules;
    std::map<char, std::string> actionMap;
};

std::vector<Lsystem> lsysVec
{
    {
        "binaryTree",
        5.0f,
        45.0f,
        "0",
        {
            {'0',"1[0]0"},
            {'1',"11"}
        },
        {
            {'0',"F"},
            {'1',"F"},
            {'[',"S-"},
            {']',"R+"},
        }
    },
    {
        "quadraticType1Curve",
        25.0f,
        90.0f,
        "F",
        {
            {'F',"F+F-F-F+F"},
        },
        {
            {'F',"F"},
            {'+',"+"},
            {'-',"-"},
        }
    },
    {
        "kochSnowflake",
        1.0f,
        60.0f,
        "F--F--F",
        {
            {'F',"F+F--F+F"},
        },
        {
            {'F',"F"},
            {'+',"+"},
            {'-',"-"},
        }
    },
    {
        "iceFractal",
        5.0f,
        90.0f,
        "F+F+F+F",
        {
            {'F',"FF+F++F+F"},
        },
        {
            {'F',"F"},
            {'+',"+"},
        }
    },
    {
        "gosperCurve",
        5.0f,
        60.0f,
        "XF",
        {
            {'X',"X+YF++YF-FX--FXFX-YF+"},
            {'Y',"-FX+YFYF++YF+FX--FX-Y"},
        },
        {
            {'F',"F"},
            {'+',"+"},
            {'-',"-"},
        }
    },
    {
        "peanoCurve",
        5.0f,
        90.0f,
        "X",
        {
            {'X',"XFYFX+F+YFXFY-F-XFYFX"},
            {'Y',"YFXFY-F-XFYFX+F+YFXFY"},
        },
        {
            {'F',"F"},
            {'+',"+"},
            {'-',"-"},
        }
    },
    {
        "peanoCurve2",
        5.0f,
        90.0f,
        "F",
        {
            {'F',"F+F-F-FF-F-F-FF"},
        },
        {
            {'F',"F"},
            {'+',"+"},
            {'-',"-"},
        }
    },
    {
        "hilbertCurve",
        25.0f,
        90.0f,
        "A",
        {
            {'A',"+BF-AFA-FB+"},
            {'B',"-AF+BFB+FA-"},
        },
        {
            {'F',"F"},
            {'+',"+"},
            {'-',"-"},
        }
    },
    {
        "sierpinskiCurve",
        5.0f,
        45.0f,
        "F--XF--F--XF",
        {
            {'X',"XF+G+XF--F--XF+G+X"},
        },
        {
            {'F',"F"},
            {'G',"F"},
            {'+',"+"},
            {'-',"-"},
        }
    },
    {
        "sierpinskiCurve2",
        5.0f,
        90.0f,
        "F+XF+F+XF",
        {
            {'X',"XF-F+F-XF+F+XF-F+F-X"},
        },
        {
            {'F',"F"},
            {'+',"+"},
            {'-',"-"},
        }
    },
    {
        "sierpinskiArrowheadCurve",
        5.0f,
        60.0f,
        "XF",
        {
            {'X',"YF+XF+Y"},
            {'Y',"XF-YF-X"},
        },
        {
            {'F',"F"},
            {'+',"+"},
            {'-',"-"},
        }
    },
    {
        "sierpinskiTriangle",
        25.0f,
        120.0f,
        "F-G-G",
        {
            {'F',"F-G+F+G-F"},
            {'G',"GG"}
        },
        {
            {'F',"F"},
            {'G',"F"},
            {'+',"+"},
            {'-',"-"},
        }
    },
    {
        "sierpinskiTriangleApprox",
        10.0f,
        60.0f,
        "A",
        {
            {'A',"B-A-B"},
            {'B',"A+B+A"}
        },
        {
            {'A',"F"},
            {'B',"F"},
            {'+',"+"},
            {'-',"-"},
        }
    },
    {
        "gilbertCurve",
        10.0f,
        90.0f,
        "X",
        {
            {'X',"-YF+XFX+FY-"},
            {'Y',"+XF-YFY-FX+"}
        },
        {
            {'F',"F"},
            {'+',"+"},
            {'-',"-"},
        }
    },
    {
        "dragonCurve",
        10.0f,
        90.0f,
        "F",
        {
            {'F',"F+G"},
            {'G',"F-G"}
        },
        {
            {'F',"F"},
            {'G',"F"},
            {'+',"+"},
            {'-',"-"},
        }
    },
    {
        "levyCurve",
        10.0f,
        45.0f,
        "F++F++F++F",
        {
            {'F',"-F++F-"},
        },
        {
            {'F',"F"},
            {'+',"+"},
            {'-',"-"},
        }
    },
    {
        "pentaplexity",
        5.0f,
        36.0f,
        "F++F++F++F++F",
        {
            {'F',"F++F++F+++++F-F++F"},
        },
        {
            {'F',"F"},
            {'+',"+"},
            {'-',"-"},
        }
    },
    {
        "pentigree",
        5.0f,
        72.0f,
        "F-F-F-F-F",
        {
            {'F',"F-F++F+F-F-F"},
        },
        {
            {'F',"F"},
            {'+',"+"},
            {'-',"-"},
        }
    },
    {
        "fractalPlant",
        4.0f,
        25.0f,
        "-X",
        {
            {'X',"F+[[X]-X]-F[-FX]+X"},
            {'F',"FF"}
        },
        {
            {'F',"F"},
            {'[',"S"},
            {']',"R"},
            {'+',"+"},
            {'-',"-"},
        }
    },
    {
        "hex7b",
        4.0f,
        30.0f,
        "X",
        {
            {'X',"-F++F-X-F--F+Y---F--F+Y+F++F-X+++F++F-X-F++F-X+++F--F+Y--"},
            {'Y',"+F++F-X-F--F+Y+F--F+Y---F--F+Y---F++F-X+++F++F-X+++F--F+Y"},
        },
        {
            {'F',"F"},
            {'+',"+"},
            {'-',"-"},
        }
    },
    {
        "doily",
        5.0f,
        30.0f,
        "F--F--F--F--F--F",
        {
            {'F',"-F[--F--F]++F--F+"},
        },
        {
            {'F',"F"},
            {'[',"S"},
            {']',"R"},
            {'+',"+"},
            {'-',"-"},
        }
    },
    {
        "maze01",
        5.0f,
        120.0f,
        "F+F+F",
        {
            {'F',"F+FF-F"},
        },
        {
            {'F',"F"},

            {'+',"+"},
            {'-',"-"},
        }
    },
    {
        "sierpinskiCarpet",
        4.0f,
        90.0f,
        "F",
        {
            {'F',"FFF[+FFF+FFF+FFF]"},
        },
        {
            {'F',"F"},
            {'[',"S"},
            {']',"R"},
            {'+',"+"},
        }
    },
    {
        "Maze&Fractal1",
        4.0f,
        120.0f,
        "X",
        {
            {'X',"FY+FYFY-FY"},
            {'Y',"FX-FXFX+FX"},
        },
        {
            {'F',"F"},
            {'+',"+"},
            {'-',"-"},
        }
    },
    {
        "Moore",
        4.0f,
        90.0f,
        "X",
        {
            {'X',"FX+FX+FXFYFX+FXFY-FY-FY-"},
            {'Y',"+FX+FX+FXFY-FYFXFY-FY-FY"},
        },
        {
            {'F',"F"},
            {'+',"+"},
            {'-',"-"},
        }
    },
    {
        "Pentant",
        4.0f,
        72.0f,
        "X-X-X-X-X",
        {
            {'X',"FX-FX-FX+FY+FY+FX-FX"},
            {'Y',"FY+FY-FX-FX-FY+FY+FY"},
        },
        {
            {'F',"F"},
            {'+',"+"},
            {'-',"-"},
        }
    },
    {
        "Pentl",
        4.0f,
        72.0f,
        "F-F-F-F-F",
        {
            {'F',"F-F-F++F+F-F"},
        },
        {
            {'F',"F"},
            {'+',"+"},
            {'-',"-"},
        }
    },
    {
        "Tiling1",
        4.0f,
        60.0f,
        "X",
        {
            {'X',"F-F-F+F+FX++F-F-F+F+FX--F-F-F+F+FX"},
        },
        {
            {'F',"F"},
            {'+',"+"},
            {'-',"-"},
        }
    },
    {
        "ADH231a",
        4.0f,
        45.0f,
        "F++++F",
        {
            {'F',"F+F+F++++F+F+F"},
        },
        {
            {'F',"F"},
            {'+',"+"},
            {'-',"-"},
        }
    },
    {
        "ADH256a",
        4.0f,
        90.0f,
        "F+F+F+F++F-F-F-F",
        {
            {'F',"F+F++F+FF"},
        },
        {
            {'F',"F"},
            {'+',"+"},
            {'-',"-"},
        }
    },
    {
        "ADH258a",
        4.0f,
        60.0f,
        "F++F++F+++F--F--F",
        {
            {'F',"FF++F++F++FFF"},
        },
        {
            {'F',"F"},
            {'+',"+"},
            {'-',"-"},
        }
    },
};

struct Transformation
{
    float posX, posY, angle;
    bool draw = true;
};

std::string Iteration(Lsystem *lsys, int iterationRound = 1, std::string currentRow = "")
{
    if(currentRow == "")
    {
        currentRow = lsys->row;
    }
    std::string resultRow = "";
    for(int i = 0; i < currentRow.length(); i++)
    {
        std::string s = lsys->rules[currentRow[i]];
        if (s == "")
        {
            s = currentRow[i];
        }
        resultRow += s;
    }
    if(iterationRound > 1)
    {
        return Iteration(lsys, iterationRound - 1, resultRow);
    }
    return resultRow;
}

std::vector<Transformation> CalculateIteration(Lsystem *lsys, int iterations)
{
    std::string row = Iteration(lsys, iterations);
    std::string actionRow = "";
    for(int i = 0; i < row.length(); i++)
    {
        actionRow.append(lsys->actionMap[row[i]]);
    }
    std::vector<Transformation> transformations = {{screenWidth / 2,screenHeigth / 2, -PI / 2}};
    std::vector<Transformation> splits;
    for(int i = 0; i < actionRow.length(); i++)
    {
        switch(actionRow[i])
        {
            case 'X':
            {
                break;
            }
            case 'F':
            {
                Transformation t = {transformations[transformations.size() - 1].posX,transformations[transformations.size() - 1].posY,transformations[transformations.size() - 1].angle};
                t.posX += cos(t.angle) * lsys->moveChange;
                t.posY += sin(t.angle) * lsys->moveChange;
                transformations.push_back(t);
                break;
            }
            case 'S':
            {
                splits.push_back(transformations[transformations.size() - 1]);
                Transformation t = {transformations[transformations.size() - 1].posX,transformations[transformations.size() - 1].posY,transformations[transformations.size() - 1].angle};
                transformations.push_back(t);
                break;
            }
            case 'R':
            {
                Transformation t = {splits[splits.size() - 1].posX,splits[splits.size() - 1].posY,splits[splits.size() - 1].angle, false};
                transformations.push_back(t);
                splits.pop_back();
                break;
            }
            case '+':
            {
                Transformation t = {transformations[transformations.size() - 1].posX,transformations[transformations.size() - 1].posY,transformations[transformations.size() - 1].angle};
                t.angle = std::fmod(t.angle + PI * 2 - PI / 180.0f * lsys->angleChange, PI * 2);
                transformations.push_back(t);
                break;
            }
            case '-':
            {
                Transformation t = {transformations[transformations.size() - 1].posX,transformations[transformations.size() - 1].posY,transformations[transformations.size() - 1].angle};
                t.angle = std::fmod(t.angle + PI / 180.0f * lsys->angleChange,PI * 2);
                transformations.push_back(t);
                break;
            }
            default:
            {
                std::cout << "ERROR in Draw Iteration with symbol: " << actionRow[i] << std::endl;
                break;
            }
        }
    }
    return transformations;
}

bool createWindow()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    Window = SDL_CreateWindow("SDL Practice",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,screenWidth,screenHeigth,SDL_WINDOW_ALLOW_HIGHDPI);
    if (Window != NULL)
    {
        RenderInformation = SDL_CreateRenderer(Window, -1, 0);
        std::cout << "Did create a window!" << std::endl; 
    }
    else
    {
        std::cout << "Could not create window: " << SDL_GetError() << std::endl;
    }
    return Window != NULL;
}

void closeWindow()
{
    SDL_DestroyWindow(Window);
    SDL_Quit();
}


int iterationCount = 4;
int lsysIndex = 0;
float offsetX = 0;
float offsetY = 0;
float zoom = 1;
float offsetSpeed = 50.0f;

void RenderFrame(std::vector<Transformation> lines)
{
    // ----- Draw background ----- //
    SDL_SetRenderDrawColor(RenderInformation, backgroundColour[0], backgroundColour[1], backgroundColour[2], 255);
    SDL_RenderClear(RenderInformation); //Fills the screen with the background colour
    SDL_SetRenderDrawColor(RenderInformation,125,0,0,255);

    for(int i = 1; i < lines.size(); i++)
    {
        if(lines[i].draw)
        {
            SDL_RenderDrawLine(RenderInformation,(lines[i-1].posX + offsetX) * zoom,(lines[i-1].posY + offsetY) * zoom,(lines[i].posX + offsetX) * zoom,(lines[i].posY + offsetY) * zoom);
        }
    }

    SDL_RenderPresent(RenderInformation);
}

std::vector<Transformation> lines;

// ----- Time ----- //
double deltaTime;
timeval t1, t2; //Time at start and end of the frame
double elapsedTime;
double sessionTime = 0; //Total time the session has been on.
int fpsLimiter = 60;

void Restart()
{
    offsetX = 0;
    offsetY = 0;
    zoom = 1;
}

int main(int argc, char *argv[])
{
    srand(time(NULL));

    std::cout << "Starting" << std::endl;
    if(createWindow() == false)
    {
        return 1;
    }
    Start();

    //Begining of calculating time.
    mingw_gettimeofday(&t1, NULL); 
    
    std::cout << "Start completed succesfully" << std::endl;

    while (true)
    {
        SDL_Event Event;
        if (SDL_PollEvent(&Event))
        {
            if (Event.type == SDL_QUIT) //Ending application.
            {
                endApp = true;
            }
            if(Event.type == SDL_MOUSEMOTION)
            {
                if(Event.button.button == 1)
                {
                    if(Event.motion.xrel != 0 || Event.motion.yrel != 0)
                    {
                        offsetX += Event.motion.xrel / zoom;
                        offsetY += Event.motion.yrel / zoom;
                        RenderFrame(lines);
                    }
                }                
            }
            if(Event.type == SDL_MOUSEWHEEL)
            {
                zoom += Event.wheel.y * 0.1;
                if(zoom <= 0)
                {
                    zoom = 0.1f;
                }
                RenderFrame(lines);
            }
        }
        if(endApp)
        {
            break;
        }

        UpdateInputs(Event);

        Update();

        UpdatePreviousInputs(Event); //Updates previousinputs, used for keyUp and keyDown functions

        //fps limiter
        Sleep(std::max(0.0,(1000 / fpsLimiter) - (deltaTime * 1000)));

        //Calculating passing time.
        mingw_gettimeofday(&t2, NULL);
        elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;
        elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;
        deltaTime = elapsedTime / 1000;
        t1 = t2;
        sessionTime += deltaTime;
    }

    closeWindow();
    return 1;
}

void QuitApplication(){
    endApp = true;
}

void Start()
{
    lines = CalculateIteration(&lsysVec[lsysIndex],iterationCount);
    RenderFrame(lines);
}

void Update()
{
    if(GetActionDownByName("Left"))
    {
        lsysIndex = (lsysIndex + lsysVec.size() - 1) % lsysVec.size();
        iterationCount = 3;
        lines = CalculateIteration(&lsysVec[lsysIndex],iterationCount);
        Restart();
        RenderFrame(lines);
    }
    else if(GetActionDownByName("Right"))
    {
        lsysIndex = (lsysIndex + 1) % lsysVec.size();
        iterationCount = 3;
        lines = CalculateIteration(&lsysVec[lsysIndex],iterationCount);
        Restart();
        RenderFrame(lines);
    }
    else if(GetActionDownByName("Up"))
    {
        iterationCount++;
        lines = CalculateIteration(&lsysVec[lsysIndex],iterationCount);
        RenderFrame(lines);
    }
    else if(GetActionDownByName("Down"))
    {
        if(iterationCount > 1)
        {
            iterationCount--;
            lines = CalculateIteration(&lsysVec[lsysIndex],iterationCount);
            RenderFrame(lines);
        }
    }

    if(GetActionByName("Up2"))
    {
        offsetY += offsetSpeed * deltaTime;
        RenderFrame(lines);
    }
    else if(GetActionByName("Down2"))
    {
        offsetY -= offsetSpeed * deltaTime;
        RenderFrame(lines);
    }

    if(GetActionByName("Left2"))
    {
        offsetX += offsetSpeed * deltaTime;
        RenderFrame(lines);
    }
    else if(GetActionByName("Right2"))
    {
        offsetX -= offsetSpeed * deltaTime;
        RenderFrame(lines);
    }
}

#endif