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

SDL_Window *Window;
SDL_Renderer *RenderInformation;
const int screenWidth = 1000;
const int screenHeigth = 1000;
int backgroundColour[3] = {125,125,125};

//Function declaration
void Start();
void Update();
bool endApp = false;

std::string currentRow = "0";
std::string newRow;
const double PI = 3.141592653589793;
float moveChange = 5.0f;
float angleChange = PI / 180 * 45;

std::map<char, std::string> rules =
{
    {'0',"1[0]0"},
    {'1',"11"}
};

struct Transformation
{
    float posX, posY, angle;
    bool draw = true;
};

std::string Iteration(std::map<char, std::string> rules, std::string currentRow, int interationRound = 1)
{
    std::string resultRow = "";
    for(int i = 0; i < currentRow.length(); i++)
    {
        std::string s = rules[currentRow[i]];
        if (s == "")
        {
            s = currentRow[i];
        }
        resultRow += s;
    }
    if(interationRound > 1)
    {
        return Iteration(rules, resultRow, interationRound - 1);
    }
    return resultRow;
}

std::vector<Transformation> DrawIteration(std::string row)
{
    std::vector<Transformation> transformations = {{screenWidth / 2,screenHeigth,-PI / 2}};
    std::vector<Transformation> splits;
    for(int i = 0; i < row.length(); i++)
    {
        switch (row[i])
        {
            case '0':
            {
                Transformation t = {transformations[transformations.size() - 1].posX,transformations[transformations.size() - 1].posY,transformations[transformations.size() - 1].angle};
                t.posX += cos(t.angle) * moveChange;
                t.posY += sin(t.angle) * moveChange;
                transformations.push_back(t);
                break;
            }
            case '1':
            {
                Transformation t = {transformations[transformations.size() - 1].posX,transformations[transformations.size() - 1].posY,transformations[transformations.size() - 1].angle};
                t.posX += cos(t.angle) * moveChange;
                t.posY += sin(t.angle) * moveChange;
                transformations.push_back(t);
                break;
            }
            case '[':
            {
                splits.push_back(transformations[transformations.size() - 1]);
                Transformation t = {transformations[transformations.size() - 1].posX,transformations[transformations.size() - 1].posY,transformations[transformations.size() - 1].angle};
                t.angle += angleChange;
                transformations.push_back(t);
                break;
            }
            case ']':
            {
                Transformation t = {splits[splits.size() - 1].posX,splits[splits.size() - 1].posY,splits[splits.size() - 1].angle, false};
                t.angle -= angleChange;
                transformations.push_back(t);
                splits.pop_back();
                break;
            }
            default:
            {
                std::cout << "ERROR!" << std::endl;
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
            SDL_RenderDrawLine(RenderInformation,lines[i-1].posX,lines[i-1].posY,lines[i].posX,lines[i].posY);
        }
    }

    SDL_RenderPresent(RenderInformation);
}

// ----- Time ----- //
double deltaTime;
timeval t1, t2; //Time at start and end of the frame
double elapsedTime;
double sessionTime = 0; //Total time the session has been on.
int fpsLimiter = 60;

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
        //Ending application.
        SDL_Event Event;
        if (SDL_PollEvent(&Event))
        {
            if (SDL_QUIT == Event.type)
            {
                endApp = true;
            }
        }
        if(endApp)
        {
            break;
        }

        //UpdateInputs(Event);

        Update();

        //UpdatePreviousInputs(Event); //Updates previousinputs, used for keyUp and keyDown functions

        //PhysicsEngine();
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
    newRow = Iteration(rules,currentRow,5);
    RenderFrame(DrawIteration(newRow));
}

void Update()
{

}

#endif