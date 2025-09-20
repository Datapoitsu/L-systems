#ifndef _ENGINEH_
#define _ENGINEH_

//Windows libraries.
#include <iostream>
#include <fstream>
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

    void PrintData()
    {
        std::cout << "# ----- " << name << " ----- #" << std::endl;
        std::cout << "Move amount: " << moveChange << std::endl;
        std::cout << "Angle amount: " << angleChange << std::endl;
        std::cout << "Axiom: " << row << std::endl;
        std::cout << "Rules: " << std::endl;
        for(auto i: rules)
        {
            std::cout << "\t" << i.first << ": " << i.second << std::endl;
        }
        std::cout << "Action map: " << std::endl;
        for(auto i: actionMap)
        {
            std::cout << "\t" << i.first << ": " << i.second << std::endl;
        }
    }
};

std::vector<Lsystem> lsysVec;

void LoadData(std::string path, std::vector<Lsystem> *lsysVec)
{
    std::string rowText;
    std::ifstream MyReadFile(path);
    while (std::getline (MyReadFile, rowText))
    {
        std::cout << "RowTEXT: " << rowText << std::endl;
        Lsystem lsys = {};
        char arr[rowText.length() + 1]; //char arr version of the string.
        strcpy(arr, rowText.c_str());

        char *tokenPtr = strtok(arr, ";");
        lsys.name = tokenPtr;
        tokenPtr = strtok(NULL, ";");
        lsys.moveChange = std::stof(tokenPtr);
        tokenPtr = strtok(NULL, ";");
        lsys.angleChange = std::stof(tokenPtr);
        tokenPtr = strtok(NULL, ";");
        lsys.row = tokenPtr;

        char *ruleToken = strtok(NULL, ";");
        char *actionToken = strtok(NULL, ";");

        char *rulePtr = strtok(ruleToken,",");
        while(rulePtr != NULL)
        {
            char *rulePtr2 = strtok(NULL,",");
            lsys.rules.insert(std::make_pair(*rulePtr,rulePtr2));
            rulePtr = strtok(NULL,",");
        }

        if(actionToken != NULL)
        {
            char *actionPtr = strtok(actionToken,",");
            while(actionPtr != NULL)
            {
                char *actionPtr2 = strtok(NULL,",");
                lsys.actionMap.insert(std::make_pair(*actionPtr,actionPtr2));
                actionPtr = strtok(NULL,",");
            }
        }

        lsysVec->push_back(lsys);
    }
}

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
    std::string actionRow;

    for (char c : row) {
        auto it = lsys->actionMap.find(c);
        if (it != lsys->actionMap.end()) {
            actionRow += it->second;
        } else {
            actionRow += c;           
        }
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
                std::cout << "ERROR in Draw Iteration with symbol: " << row[i] << std::endl;
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
    LoadData("data.txt", &lsysVec);
    for(int i = 0; i < lsysVec.size(); i++)
    {
        lsysVec[i].PrintData();
    }
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