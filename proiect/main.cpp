#include <iostream>
#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <mmsystem.h>
#include <bits/stdc++.h>
#include <math.h>
#include <winbgim.h>
#include <string.h>
using namespace std;
#define NoduriMax 300
#define MAX_Line_chars 150
#define latime 800
#define lungime 1000
vector<int> muchii[NoduriMax];
struct chenar
{
    int x1,y1,x2,y2;
};
struct desen
{
    chenar x,y,z;
    int mijloc;
};
struct pereche
{
    char c[301];
    int tab;
    bool accesat;
    int tipst;
    int cost;
    int linieChenarInceput;
    int linieChenarSfarsit;
    desen d;
};
int linii;
vector<pereche> linieFisier;
vector <pair<int,int> >perechiIfElse;
bool liniiAccesate[300];
int cost = 0;
int maxim = -1;
int spatii;
bool gasit = false;
//////////<------>ACCESARE FISIER CU NUMARARE DE LINII COD<------>//////////
int linescount()
{
    char line[MAX_Line_chars];
    int linecount=0;
    FILE *fptr;
    fptr = fopen("pseudocode.txt","r");
    fseek(fptr,0,SEEK_SET);
    int for_count = 0;
    while(!feof(fptr))
    {
        fgets(line,MAX_Line_chars,fptr);
        if(strstr(line,"for"))linecount++;
        linecount++;
    }
    return linecount;
}
//////////<------>CREARE PERECHI IF-ELSE<------>//////////
void crearePerechi()
{
    for(int i = 1; i < linieFisier.size(); ++i)
    {
        bool gasit = false;
        if(linieFisier[i].tipst == 2 && !linieFisier[i].accesat)
        {
            linieFisier[i].accesat = true;
            for(int j = 1; j < linieFisier.size(); j++)
                if(j != i && linieFisier[j].tipst == 2 && muchii[j][0] == muchii[i][0] && !linieFisier[j].accesat)
                {
                    perechiIfElse.push_back(make_pair(i,-1));
                    gasit = true;
                    break;
                }

                else if(j != i && linieFisier[j].tipst == 3 && muchii[j][0] == muchii[i][0] && !linieFisier[j].accesat)
                {
                    perechiIfElse.push_back(make_pair(i,j));
                    gasit = true;
                    linieFisier[j].accesat = true;
                    break;
                }
            if(!gasit)
                perechiIfElse.push_back(make_pair(i,-1)),gasit = true;
        }
    }
}
//////////<------>FUNCTIE DE CREARE A ARBORELUI<------>//////////
void crearearbore()
{
    for(int i=1; i<linieFisier.size(); i++)
    {
        if(strstr(linieFisier[i].c,"until")) linieFisier[i].tipst = -1;
        else if(strstr(linieFisier[i].c,"if")) linieFisier[i].tipst=2;
        else if(strstr(linieFisier[i].c,"else")) linieFisier[i].tipst=3;
        else if(strstr(linieFisier[i].c,"for")) linieFisier[i].tipst=4;
        else if(strstr(linieFisier[i].c,"while")) linieFisier[i].tipst=5;
        else if(strstr(linieFisier[i].c,"repeat")) linieFisier[i].tipst=6;
        else linieFisier[i].tipst = 1;

        if(strstr(linieFisier[i].c,"for") || strstr(linieFisier[i].c,"repeat")) linieFisier[i].cost = 2;
        else linieFisier[i].cost = 1;

    }
    linieFisier[0].tipst=0;
    linieFisier[0].cost=0;

    for(int i=1; i<linieFisier.size(); i++)
    {
        if(linieFisier[i].accesat==false && linieFisier[i].tipst != -1)
        {
            if (linieFisier[i].tab==1)
            {
                linieFisier[i].accesat==true;
                muchii[0].push_back(i);
                muchii[i].push_back(0);
            }
            else
            {
                int linienoua=i;
                while(linieFisier[linienoua].tab!=linieFisier[i].tab-1)
                {
                    linienoua--;
                }
                muchii[linienoua].push_back(i);
                muchii[i].push_back(linienoua);
                linieFisier[i].accesat==true;
            }
        }
    }
}
//////////<------>RESTART LA STAREA LINIILOR PENTRU ACCES<------>//////////
void restart()
{
    for(int i = 0; i < linieFisier.size(); ++i)
        linieFisier[i].accesat = false;
}
//////////<------>REGASIREA STRAMOSULUI PENTRU UN NOD<------>//////////
bool RegasescTatal(int j,int i)
{
    while(j != 0)
    {
        if(i == j)
            return true;
        j = muchii[j][0];
    }
    return false;
}
//////////<------>ACCESUL SUBARBORELUI PRIN DFS<------>//////////
void dfs(int i)
{
    linieFisier[i].accesat = true;
    cost += linieFisier[i].cost;
    for(int j = 1; j < muchii[i].size(); ++j)
        if(!linieFisier[muchii[i][j]].accesat)
            dfs(muchii[i][j]);
}
//////////<------>MODIFICAREA SUBARBORILOR IN FUNCTIE DE LINII<------>//////////
void dfsModificare(int i,int l)
{
    linieFisier[i].accesat = true;
    linieFisier[i].linieChenarInceput -= l;
    linieFisier[i].linieChenarSfarsit -= l;
    for(int j = 1; j < muchii[i].size(); ++j)
        if(!linieFisier[muchii[i][j]].accesat)
            dfsModificare(muchii[i][j],l);
}
//////////<------>GASIREA PERECHII LUI ELSE<------>//////////
int gasirePerecheElse(int k)
{
    for(int i = 0; i < perechiIfElse.size(); ++i)
        if(perechiIfElse[i].second == k)
            return perechiIfElse[i].first;
    return -1;
}
//////////<------>GASIREA PERECHII LUI IF<------>//////////
int gasirePerecheIf(int k)
{
    for(int i = 0; i < perechiIfElse.size(); ++i)
        if(perechiIfElse[i].first == k && perechiIfElse[i].second != -1)
            return perechiIfElse[i].second;
    return -1;
}
//////////<------>MAXIMUL IN SUBARBORE<------>//////////
void maximDfs(int i)
{
    linieFisier[i].accesat = true;
    maxim = max(maxim,linieFisier[i].linieChenarSfarsit);
    for(int j = 1; j < muchii[i].size(); ++j)
        if(!linieFisier[muchii[i][j]].accesat)
            maximDfs(muchii[i][j]);
}
//////////<------>GASIREA ELEMENTULUI IN SUBARBORE<------>//////////
bool gasitInDFS(int i,int el)
{
    linieFisier[i].accesat = true;
    if(i == el) gasit = true;
    for(int j = 1; j < muchii[i].size(); ++j)
        if(!linieFisier[muchii[i][j]].accesat)
            gasitInDFS(muchii[i][j],el);
}
//////////<------>GASIRE LINIE NEACCESATA<------>//////////
int gasireLinie(int l)
{
    for(int i = 1; i < l; ++i)
        if(liniiAccesate[i] == false)
            return i;
}
//////////<------>GASIRE LINIE PENTRU REPEAT UNTIL<------>//////////
int gasireRepeat(int l)
{
    for(int i = l; i < linieFisier.size(); ++i)
        if(linieFisier[i].tipst == -1 && linieFisier[i].tab == linieFisier[l].tab)
            return i;
}
//////////<------>RESTART LA STARI ALE LINIILOR<------>//////////
void restartTot()
{
    for(int i = 1; i <= linii; ++i)
        liniiAccesate[i] = false;
    for(int i = 1; i < linieFisier.size(); ++i)
    {
        if(linieFisier[i].tipst == 1 || linieFisier[i].tipst == 2  || linieFisier[i].tipst == 5)
            liniiAccesate[linieFisier[i].linieChenarInceput] = true;
        else if(linieFisier[i].tipst == 4)
            liniiAccesate[linieFisier[i].linieChenarInceput] = liniiAccesate[linieFisier[i].linieChenarSfarsit] = true;
        else if(linieFisier[i].tipst == 6)
            liniiAccesate[linieFisier[i].linieChenarSfarsit] = true;
    }
}
//////////<------>DFS CU RESTARTURI LA LINII ACCESATE<------>//////////

void maximalLinie()
{
    for(int i = 1; i < linieFisier.size(); ++i)
    {
        maxim = -1;
        maximDfs(i);
        if(linieFisier[i].tipst != 4 || linieFisier[i].tipst != 6)
            linieFisier[i].linieChenarSfarsit = maxim;
        else
            linieFisier[i].linieChenarSfarsit = maxim + 1;
    }
}
//////////<------>ALGORITMUL DIN SPATELE CREEARI CHENARELOR CAT SI POZITIONAREA ACESTORA CAT MAI CORECTA<------>//////////
void dfsAccesate(int i)
{
    linieFisier[i].accesat = true;
    if(linieFisier[i].tipst == 1 || linieFisier[i].tipst == 2  || linieFisier[i].tipst == 5)
        liniiAccesate[linieFisier[i].linieChenarInceput] = true;
    else if(linieFisier[i].tipst == 4)
        liniiAccesate[linieFisier[i].linieChenarInceput] = liniiAccesate[linieFisier[i].linieChenarSfarsit] = true;
    else if(linieFisier[i].tipst == 6)
        liniiAccesate[linieFisier[i].linieChenarSfarsit] = true;
    for(int j = 1; j < muchii[i].size(); ++j)
        if(!linieFisier[muchii[i][j]].accesat)
            dfsAccesate(muchii[i][j]);
}
void accesLinii()
{
    for(int i = 1; i < linieFisier.size(); ++i)
        linieFisier[i].linieChenarInceput = i;
    cout << endl;
    for(int i = 1; i < linieFisier.size(); ++i)
    {
        if(linieFisier[i].tipst != -1)
        {
            restart();
            cost = 0;
            dfs(i);
            linieFisier[i].linieChenarSfarsit = linieFisier[i].linieChenarInceput - 1 + cost;
        }
    }
    for(int i = 1; i < linieFisier.size(); ++i)
    {
        if(linieFisier[i].tipst == 4 || linieFisier[i].tipst == 6)
        {
            int element = i;
            for(int j = i + 1; j < linieFisier.size(); ++j)
            {
                gasit = false;
                restart();
                gasitInDFS(i,j);
                if(!gasit)
                    linieFisier[j].linieChenarInceput++,linieFisier[j].linieChenarSfarsit++;
            }
        }
    }
    for(int i = 1; i < linieFisier.size(); ++i)
    {
        if(gasirePerecheElse(i) != -1 && linieFisier[i].tipst != -1)
        {
            restart();
            int diferenta = linieFisier[gasirePerecheElse(i)].linieChenarSfarsit - linieFisier[gasirePerecheElse(i)].linieChenarInceput + 1;
            if(diferenta > 0)
                dfsModificare(i,diferenta);
            else
                dfsModificare(gasirePerecheElse(i),abs(diferenta));
        }
    }
    for(int i = 1; i < linieFisier.size(); ++i)
        if(linieFisier[i].tipst == 6)
            linieFisier[i].linieChenarInceput++;
    for(int i = 1; i < linieFisier.size(); ++i)
        if(linieFisier[i].tipst == 2 && gasirePerecheIf(i) != -1)
            linieFisier[i].linieChenarSfarsit = max(linieFisier[i].linieChenarSfarsit,linieFisier[gasirePerecheIf(i)].linieChenarSfarsit);
    restartTot();
    cout << endl;
    cout << "INITIAL: ";
    for(int i = 1; i < linieFisier.size(); ++i)
        cout << "LINIA " << i << " este intre " << linieFisier[i].linieChenarInceput << ' ' << linieFisier[i].linieChenarSfarsit << ' ' << endl;
    cout << endl;
    maximalLinie();

    for(int i = 1; i < linieFisier.size(); ++i)
    {
        if(linieFisier[i].tipst != -1)
        {
            if(linieFisier[i].tipst != -1)
            {
                if(gasireLinie(linieFisier[i].linieChenarInceput))
                {
                    int diferenta = linieFisier[i].linieChenarInceput - gasireLinie(linieFisier[i].linieChenarInceput);
                    for(int j = 1; j < linieFisier.size(); ++j)
                    {
                        if(RegasescTatal(i,j) == true && i != j)
                        {
                            linieFisier[j].linieChenarSfarsit -= diferenta;
                            cout << j << ' ';
                        }
                        else if(j >= i)
                        {
                            linieFisier[j].linieChenarInceput -= diferenta;
                            linieFisier[j].linieChenarSfarsit -= diferenta;
                        }
                    }
                    restartTot();
                }
            }
        }
    }
    for(int i = 1; i < linieFisier.size(); ++i)
    {
        if(linieFisier[i].tipst == 2 && gasirePerecheIf(i) != -1)
        {
            linieFisier[i].linieChenarSfarsit = max(linieFisier[i].linieChenarSfarsit,linieFisier[gasirePerecheIf(i)].linieChenarSfarsit);
            linieFisier[gasirePerecheIf(i)].linieChenarSfarsit = max(linieFisier[i].linieChenarSfarsit,linieFisier[gasirePerecheIf(i)].linieChenarSfarsit);
            int dif = linieFisier[i].linieChenarInceput - linieFisier[gasirePerecheIf(i)].linieChenarInceput;
            restart();
            dfsModificare(gasirePerecheIf(i),-dif);
        }
    }
}
//////////<------>DESENAREA LOGICA A DIAGRAMEI PE INTERFATA<------>//////////
void desenLinii()
{
    setcolor(WHITE);
    char s[10];
    for(int i = 0; i < linieFisier.size(); ++i)
    {
        sprintf(s,"%d",i-1);
        if(linieFisier[i].tipst == 1)
        {
            setcolor(WHITE);
            rectangle(linieFisier[i].d.x.x1,linieFisier[i].d.x.y1,linieFisier[i].d.x.x2,linieFisier[i].d.x.y2);
        }
        else if(linieFisier[i].tipst == 2)
        {
            rectangle(linieFisier[i].d.x.x1,linieFisier[i].d.x.y1,linieFisier[i].d.x.x2,linieFisier[i].d.x.y2);
            line(linieFisier[i].d.x.x1,linieFisier[i].d.x.y1,linieFisier[i].d.mijloc,linieFisier[i].d.x.y2);
            line(linieFisier[i].d.mijloc,linieFisier[i].d.x.y2,linieFisier[i].d.x.x2,linieFisier[i].d.x.y1);
            rectangle(linieFisier[i].d.y.x1,linieFisier[i].d.y.y1,linieFisier[i].d.y.x2,linieFisier[i].d.y.y2);
            rectangle(linieFisier[i].d.z.x1,linieFisier[i].d.z.y1,linieFisier[i].d.z.x2,linieFisier[i].d.z.y2);
            outtextxy(linieFisier[i].d.x.x1 + 20,linieFisier[i].d.x.y2 - 20,"T");
            outtextxy(linieFisier[i].d.x.x2 - 20,linieFisier[i].d.x.y2 - 20, "F");
        }
        else if(linieFisier[i].tipst == 4)
        {
            rectangle(linieFisier[i].d.x.x1,linieFisier[i].d.x.y1,linieFisier[i].d.x.x2,linieFisier[i].d.x.y2);
            rectangle(linieFisier[i].d.y.x1,linieFisier[i].d.y.y1,linieFisier[i].d.y.x2,linieFisier[i].d.y.y2);
            rectangle(linieFisier[i].d.z.x1,linieFisier[i].d.z.y1,linieFisier[i].d.z.x2,linieFisier[i].d.z.y2);
            setcolor(BLACK);
            line(linieFisier[i].d.y.x1 + 1,linieFisier[i].d.y.y1,linieFisier[i].d.y.x2,linieFisier[i].d.y.y1);
            line(linieFisier[i].d.y.x1 + 1,linieFisier[i].d.y.y2,linieFisier[i].d.y.x2,linieFisier[i].d.y.y2);
            setcolor(WHITE);
        }
        else if(linieFisier[i].tipst == 5)
        {
            rectangle(linieFisier[i].d.x.x1,linieFisier[i].d.x.y1,linieFisier[i].d.x.x2,linieFisier[i].d.x.y2);
            rectangle(linieFisier[i].d.y.x1,linieFisier[i].d.y.y1,linieFisier[i].d.y.x2,linieFisier[i].d.y.y2);
            setcolor(BLACK);
            line(linieFisier[i].d.x.x1 + 1,linieFisier[i].d.x.y2,linieFisier[i].d.x.x1 + spatii/2,linieFisier[i].d.x.y2);
            setcolor(WHITE);
        }
        else if(linieFisier[i].tipst == 6)
        {
            rectangle(linieFisier[i].d.x.x1,linieFisier[i].d.x.y1,linieFisier[i].d.x.x2,linieFisier[i].d.x.y2);
            rectangle(linieFisier[i].d.y.x1,linieFisier[i].d.y.y1,linieFisier[i].d.y.x2,linieFisier[i].d.y.y2);
            setcolor(BLACK);
            line(linieFisier[i].d.y.x1 + 1,linieFisier[i].d.y.y2,linieFisier[i].d.y.x2,linieFisier[i].d.y.y2);
            setcolor(WHITE);
        }
    }
    setcolor(WHITE);
}
//////////<------>PUNEREA IN DIAGRAMA A ALGORITMULUI REPRODUS<------>//////////
void punereChenare()
{
    linieFisier[0].d.x = {0,0,1000,1000};
    spatii = 800/linii;
    cout << spatii << endl;
    for(int i = 1; i < linieFisier.size(); ++i)
    {
        if(linieFisier[i].tipst == 1)
        {
            int tatal = muchii[i][0];
            if(tatal == 0)
                linieFisier[i].d.x= {0,(linieFisier[i].linieChenarInceput - 1)*spatii,1000,linieFisier[i].linieChenarSfarsit*spatii};
            else if(linieFisier[tatal].tipst == 2)
                linieFisier[i].d.x= {linieFisier[tatal].d.x.x1,(linieFisier[i].linieChenarInceput-1)*spatii,linieFisier[tatal].d.mijloc,linieFisier[i].linieChenarSfarsit*spatii};
            else if(linieFisier[tatal].tipst == 3)
                linieFisier[i].d.x= {linieFisier[gasirePerecheElse(tatal)].d.mijloc,(linieFisier[i].linieChenarInceput-1)*spatii,linieFisier[gasirePerecheElse(tatal)].d.x.x2,linieFisier[i].linieChenarSfarsit*spatii};
            else if(linieFisier[tatal].tipst == 4)
                linieFisier[i].d.x= {linieFisier[tatal].d.y.x2,(linieFisier[i].linieChenarInceput-1)*spatii,linieFisier[tatal].d.x.x2,linieFisier[i].linieChenarSfarsit*spatii};
            else if(linieFisier[tatal].tipst == 5)
                linieFisier[i].d.x= {linieFisier[tatal].d.y.x2,(linieFisier[i].linieChenarInceput-1)*spatii,linieFisier[tatal].d.x.x2,linieFisier[i].linieChenarSfarsit*spatii};
            else if(linieFisier[tatal].tipst == 6)
                linieFisier[i].d.x= {linieFisier[tatal].d.y.x2,(linieFisier[i].linieChenarInceput-1)*spatii,linieFisier[tatal].d.x.x2,linieFisier[i].linieChenarSfarsit*spatii};

        }
        else if(linieFisier[i].tipst == 2)
        {
            int tatal = muchii[i][0];
            if(tatal == 0)
            {
                linieFisier[i].d.x= {0,(linieFisier[i].linieChenarInceput - 1)*spatii,1000,linieFisier[i].linieChenarInceput*spatii};
                linieFisier[i].d.mijloc = (linieFisier[i].d.x.x1 + linieFisier[i].d.x.x2)/2;
                linieFisier[i].d.y= {linieFisier[i].d.x.x1,linieFisier[i].d.x.y2,linieFisier[i].d.mijloc,linieFisier[i].linieChenarSfarsit*spatii};
                linieFisier[i].d.z= {linieFisier[i].d.mijloc,linieFisier[i].d.x.y2,linieFisier[i].d.x.x2,linieFisier[i].linieChenarSfarsit*spatii};
            }
            else if(linieFisier[tatal].tipst == 2)
            {
                linieFisier[i].d.x= {linieFisier[tatal].d.x.x1,(linieFisier[i].linieChenarInceput - 1)*spatii,linieFisier[tatal].d.mijloc,linieFisier[i].linieChenarInceput*spatii};
                linieFisier[i].d.mijloc = (linieFisier[i].d.x.x1 + linieFisier[i].d.x.x2)/2;
                linieFisier[i].d.y= {linieFisier[i].d.x.x1,linieFisier[i].d.x.y2,linieFisier[i].d.mijloc,linieFisier[i].linieChenarSfarsit*spatii};
                linieFisier[i].d.z= {linieFisier[i].d.mijloc,linieFisier[i].d.x.y2,linieFisier[i].d.x.x2,linieFisier[i].linieChenarSfarsit*spatii};
            }
            else if(linieFisier[tatal].tipst == 3)
            {
                linieFisier[i].d.x= {linieFisier[gasirePerecheElse(tatal)].d.mijloc,(linieFisier[i].linieChenarInceput - 1)*spatii,linieFisier[gasirePerecheElse(tatal)].d.x.x2,linieFisier[i].linieChenarInceput*spatii};
                linieFisier[i].d.mijloc = (linieFisier[i].d.x.x1 + linieFisier[i].d.x.x2)/2;
                linieFisier[i].d.y= {linieFisier[i].d.x.x1,linieFisier[i].d.x.y2,linieFisier[i].d.mijloc,linieFisier[i].linieChenarSfarsit*spatii};
                linieFisier[i].d.z= {linieFisier[i].d.mijloc,linieFisier[i].d.x.y2,linieFisier[i].d.x.x2,linieFisier[i].linieChenarSfarsit*spatii};
            }
            else if(linieFisier[tatal].tipst == 4)
            {
                linieFisier[i].d.x= {linieFisier[tatal].d.y.x2,(linieFisier[i].linieChenarInceput - 1)*spatii,linieFisier[tatal].d.x.x2,linieFisier[i].linieChenarInceput*spatii};
                linieFisier[i].d.mijloc = (linieFisier[i].d.x.x1 + linieFisier[i].d.x.x2)/2;
                linieFisier[i].d.y= {linieFisier[i].d.x.x1,linieFisier[i].d.x.y2,linieFisier[i].d.mijloc,linieFisier[i].linieChenarSfarsit*spatii};
                linieFisier[i].d.z= {linieFisier[i].d.mijloc,linieFisier[i].d.x.y2,linieFisier[i].d.x.x2,linieFisier[i].linieChenarSfarsit*spatii};
            }
            else if(linieFisier[tatal].tipst == 5)
            {
                linieFisier[i].d.x= {linieFisier[tatal].d.y.x2,(linieFisier[i].linieChenarInceput - 1)*spatii,linieFisier[tatal].d.x.x2,linieFisier[i].linieChenarInceput*spatii};
                linieFisier[i].d.mijloc = (linieFisier[i].d.x.x1 + linieFisier[i].d.x.x2)/2;
                linieFisier[i].d.y= {linieFisier[i].d.x.x1,linieFisier[i].d.x.y2,linieFisier[i].d.mijloc,linieFisier[i].linieChenarSfarsit*spatii};
                linieFisier[i].d.z= {linieFisier[i].d.mijloc,linieFisier[i].d.x.y2,linieFisier[i].d.x.x2,linieFisier[i].linieChenarSfarsit*spatii};
            }
            else if(linieFisier[tatal].tipst == 6)
            {
                linieFisier[i].d.x= {linieFisier[tatal].d.y.x2,(linieFisier[i].linieChenarInceput - 1)*spatii,linieFisier[tatal].d.x.x2,linieFisier[i].linieChenarInceput*spatii};
                linieFisier[i].d.mijloc = (linieFisier[i].d.x.x1 + linieFisier[i].d.x.x2)/2;
                linieFisier[i].d.y= {linieFisier[i].d.x.x1,linieFisier[i].d.x.y2,linieFisier[i].d.mijloc,linieFisier[i].linieChenarSfarsit*spatii};
                linieFisier[i].d.z= {linieFisier[i].d.mijloc,linieFisier[i].d.x.y2,linieFisier[i].d.x.x2,linieFisier[i].linieChenarSfarsit*spatii};
            }
        }
        else if(linieFisier[i].tipst == 4)
        {
            int tatal = muchii[i][0];
            if(tatal == 0)
            {
                linieFisier[i].d.x= {0,(linieFisier[i].linieChenarInceput - 1)*spatii,1000,linieFisier[i].linieChenarInceput*spatii};
                linieFisier[i].d.z= {linieFisier[i].d.x.x1,(linieFisier[i].linieChenarSfarsit - 1)*spatii,1000,linieFisier[i].linieChenarSfarsit*spatii};
                linieFisier[i].d.y= {linieFisier[i].d.x.x1,linieFisier[i].d.x.y2,linieFisier[i].d.z.x1 + spatii/2,linieFisier[i].d.z.y1};

            }
            else if(linieFisier[tatal].tipst == 2)
            {
                linieFisier[i].d.x= {linieFisier[tatal].d.x.x1,(linieFisier[i].linieChenarInceput - 1)*spatii,linieFisier[tatal].d.mijloc,linieFisier[i].linieChenarInceput*spatii};
                linieFisier[i].d.z= {linieFisier[i].d.x.x1,(linieFisier[i].linieChenarSfarsit - 1)*spatii,linieFisier[tatal].d.mijloc,linieFisier[i].linieChenarSfarsit*spatii};
                linieFisier[i].d.y= {linieFisier[i].d.x.x1,linieFisier[i].d.x.y2,linieFisier[i].d.z.x1 + spatii/2,linieFisier[i].d.z.y1};
            }
            else if(linieFisier[tatal].tipst == 3)
            {
                linieFisier[i].d.x= {linieFisier[gasirePerecheElse(tatal)].d.mijloc,(linieFisier[i].linieChenarInceput - 1)*spatii,linieFisier[gasirePerecheElse(tatal)].d.x.x2,linieFisier[i].linieChenarInceput*spatii};
                linieFisier[i].d.z= {linieFisier[i].d.x.x1,(linieFisier[i].linieChenarSfarsit - 1)*spatii,linieFisier[gasirePerecheElse(tatal)].d.x.x2,linieFisier[i].linieChenarSfarsit*spatii};
                linieFisier[i].d.y= {linieFisier[i].d.x.x1,linieFisier[i].d.x.y2,linieFisier[i].d.z.x1 + spatii/2,linieFisier[i].d.z.y1};
            }
            else if(linieFisier[tatal].tipst == 4)
            {
                linieFisier[i].d.x= {linieFisier[tatal].d.y.x2,(linieFisier[i].linieChenarInceput - 1)*spatii,linieFisier[tatal].d.x.x2,linieFisier[i].linieChenarInceput*spatii};
                linieFisier[i].d.z= {linieFisier[i].d.x.x1,(linieFisier[i].linieChenarSfarsit - 1)*spatii,linieFisier[tatal].d.x.x2,linieFisier[i].linieChenarSfarsit*spatii};
                linieFisier[i].d.y= {linieFisier[i].d.x.x1,linieFisier[i].d.x.y2,linieFisier[i].d.z.x1 + spatii/2,linieFisier[i].d.z.y1};
            }
            else if(linieFisier[tatal].tipst == 5)
            {
                linieFisier[i].d.x= {linieFisier[tatal].d.y.x2,(linieFisier[i].linieChenarInceput - 1)*spatii,linieFisier[tatal].d.x.x2,linieFisier[i].linieChenarInceput*spatii};
                linieFisier[i].d.z= {linieFisier[i].d.x.x1,(linieFisier[i].linieChenarSfarsit - 1)*spatii,linieFisier[tatal].d.x.x2,linieFisier[i].linieChenarSfarsit*spatii};
                linieFisier[i].d.y= {linieFisier[i].d.x.x1,linieFisier[i].d.x.y2,linieFisier[i].d.z.x1 + spatii/2,linieFisier[i].d.z.y1};
            }
            else if(linieFisier[tatal].tipst == 6)
            {
                linieFisier[i].d.x= {linieFisier[tatal].d.y.x2,(linieFisier[i].linieChenarInceput - 1)*spatii,linieFisier[tatal].d.x.x2,linieFisier[i].linieChenarInceput*spatii};
                linieFisier[i].d.z= {linieFisier[i].d.x.x1,(linieFisier[i].linieChenarSfarsit - 1)*spatii,linieFisier[tatal].d.x.x2,linieFisier[i].linieChenarSfarsit*spatii};
                linieFisier[i].d.y= {linieFisier[i].d.x.x1,linieFisier[i].d.x.y2,linieFisier[i].d.z.x1 + spatii/2,linieFisier[i].d.z.y1};
            }
        }
        else if(linieFisier[i].tipst == 5)
        {
            int tatal = muchii[i][0];
            if(tatal == 0)
            {
                linieFisier[i].d.x = {0,(linieFisier[i].linieChenarInceput-1)*spatii,1000,linieFisier[i].linieChenarInceput*spatii};
                linieFisier[i].d.y = {linieFisier[i].d.x.x1,linieFisier[i].d.x.y2,linieFisier[i].d.x.x1 + spatii/2,linieFisier[i].linieChenarSfarsit*spatii};
            }
            else if(linieFisier[tatal].tipst == 2)
            {
                linieFisier[i].d.x = {linieFisier[tatal].d.x.x1,(linieFisier[i].linieChenarInceput-1)*spatii,linieFisier[tatal].d.mijloc,linieFisier[i].linieChenarInceput*spatii};
                linieFisier[i].d.y = {linieFisier[i].d.x.x1,linieFisier[i].d.x.y2,linieFisier[i].d.x.x1 + spatii/2,linieFisier[i].linieChenarSfarsit*spatii};
            }
            else if(linieFisier[tatal].tipst == 3)
            {
                linieFisier[i].d.x = {linieFisier[gasirePerecheElse(tatal)].d.mijloc,(linieFisier[i].linieChenarInceput-1)*spatii,linieFisier[gasirePerecheElse(tatal)].d.x.x2,linieFisier[i].linieChenarInceput*spatii};
                linieFisier[i].d.y = {linieFisier[i].d.x.x1,linieFisier[i].d.x.y2,linieFisier[i].d.x.x1 + spatii/2,linieFisier[i].linieChenarSfarsit*spatii};
            }
            else if(linieFisier[tatal].tipst == 4)
            {
                linieFisier[i].d.x = {linieFisier[tatal].d.y.x2,(linieFisier[i].linieChenarInceput-1)*spatii,linieFisier[tatal].d.x.x2,linieFisier[i].linieChenarInceput*spatii};
                linieFisier[i].d.y = {linieFisier[i].d.x.x1,linieFisier[i].d.x.y2,linieFisier[i].d.x.x1 + spatii/2,linieFisier[i].linieChenarSfarsit*spatii};
            }
            else if(linieFisier[tatal].tipst == 5)
            {
                linieFisier[i].d.x = {linieFisier[tatal].d.y.x2,(linieFisier[i].linieChenarInceput-1)*spatii,linieFisier[tatal].d.x.x2,linieFisier[i].linieChenarInceput*spatii};
                linieFisier[i].d.y = {linieFisier[i].d.x.x1,linieFisier[i].d.x.y2,linieFisier[i].d.x.x1 + spatii/2,linieFisier[i].linieChenarSfarsit*spatii};
            }
            else if(linieFisier[tatal].tipst == 6)
            {
                linieFisier[i].d.x = {linieFisier[tatal].d.y.x2,(linieFisier[i].linieChenarInceput-1)*spatii,linieFisier[tatal].d.x.x2,linieFisier[i].linieChenarInceput*spatii};
                linieFisier[i].d.y = {linieFisier[i].d.x.x1,linieFisier[i].d.x.y2,linieFisier[i].d.x.x1 + spatii/2,linieFisier[i].linieChenarSfarsit*spatii};
            }
        }
        else if(linieFisier[i].tipst == 6)
        {
            int tatal = muchii[i][0];
            if(tatal == 0)
            {
                linieFisier[i].d.x = {0,(linieFisier[i].linieChenarSfarsit-1)*spatii,1000,linieFisier[i].linieChenarSfarsit*spatii};
                linieFisier[i].d.y = {0,(linieFisier[i].linieChenarInceput-1)*spatii,linieFisier[i].d.x.x1 + spatii/2,(linieFisier[i].linieChenarSfarsit-1)*spatii};
            }
            else if(linieFisier[tatal].tipst == 2)
            {
                linieFisier[i].d.x = {linieFisier[tatal].d.x.x1,(linieFisier[i].linieChenarSfarsit-1)*spatii,linieFisier[tatal].d.mijloc,linieFisier[i].linieChenarSfarsit*spatii};
                linieFisier[i].d.y = {linieFisier[tatal].d.x.x1,(linieFisier[i].linieChenarInceput-1)*spatii,linieFisier[i].d.x.x1 + spatii/2,(linieFisier[i].linieChenarSfarsit-1)*spatii};
            }
            else if(linieFisier[tatal].tipst == 3)
            {
                linieFisier[i].d.x = {linieFisier[gasirePerecheElse(tatal)].d.mijloc,(linieFisier[i].linieChenarSfarsit-1)*spatii,linieFisier[gasirePerecheElse(tatal)].d.x.x2,linieFisier[i].linieChenarSfarsit*spatii};
                linieFisier[i].d.y = {linieFisier[gasirePerecheElse(tatal)].d.mijloc,(linieFisier[i].linieChenarInceput-1)*spatii,linieFisier[i].d.x.x1 + spatii/2,(linieFisier[i].linieChenarSfarsit-1)*spatii};
            }
            else if(linieFisier[tatal].tipst == 4)
            {
                linieFisier[i].d.x = {linieFisier[tatal].d.y.x2,(linieFisier[i].linieChenarSfarsit-1)*spatii,linieFisier[tatal].d.x.x2,linieFisier[i].linieChenarSfarsit*spatii};
                linieFisier[i].d.y = {linieFisier[tatal].d.y.x2,(linieFisier[i].linieChenarInceput-1)*spatii,linieFisier[i].d.x.x1 + spatii/2,(linieFisier[i].linieChenarSfarsit-1)*spatii};
            }
            else if(linieFisier[tatal].tipst == 5)
            {
                linieFisier[i].d.x = {linieFisier[tatal].d.y.x2,(linieFisier[i].linieChenarSfarsit-1)*spatii,linieFisier[tatal].d.x.x2,linieFisier[i].linieChenarSfarsit*spatii};
                linieFisier[i].d.y = {linieFisier[tatal].d.y.x2,(linieFisier[i].linieChenarInceput-1)*spatii,linieFisier[i].d.x.x1 + spatii/2,(linieFisier[i].linieChenarSfarsit-1)*spatii};
            }
            else if(linieFisier[tatal].tipst == 6)
            {
                linieFisier[i].d.x = {linieFisier[tatal].d.y.x2,(linieFisier[i].linieChenarSfarsit-1)*spatii,linieFisier[tatal].d.x.x2,linieFisier[i].linieChenarSfarsit*spatii};
                linieFisier[i].d.y = {linieFisier[tatal].d.y.x2,(linieFisier[i].linieChenarInceput-1)*spatii,linieFisier[i].d.x.x1 + spatii/2,(linieFisier[i].linieChenarSfarsit-1)*spatii};
            }
        }
    }
}
//////////<------>CULOARE CASUTA SI SCRIS<------>//////////
struct culoare
{
    int r,g,b;
};
//////////<------>INTERFATA MENIU<------>//////////
struct meniu
{
    bool accesat;
    struct chenare
    {
        int x1,y1,x2,y2;
        bool accesat;
    } c[5];
} meniu;
//////////<------>STRUCTURA PENTRU PRELUAREA INFORMATIILOR DIN PSEUDOCOD<------>//////////
struct pseudocod
{
    int liniiCod;
    struct linie_pseudocod
    {
        char c[256];
        int tab;
    } y[1001];
} p;
//////////<------>CHENAR PENTRU SETARI<------>//////////
struct chenareSettings
{
    struct chenar
    {
        int x1,y1,x2,y2;
        bool op1, op2;
        culoare y;
    } a[5];
} z;
int audio = 1;
int audio2 = 0;
//////////<------>BEEP DE INRARE SI IESIRE DINTR-UN CHENAR<------>//////////
void beepChenar()
{
    if(audio == 1)
        Beep(440,500);
    delay(0.2);
}

//////////<------>LOADING SCREEN<------>//////////
void loading()
{
    cleardevice();
    int x = 30;
    for(int i = 0; i < 1000; ++i)
    {
        int r = rand() % 150;
        if(x >= latime-30)
            break;
        outtextxy(850,730,"LOADING...");
        if(x + r >= latime-30)
            rectangle(x,750,latime-30,770);
        else
            rectangle(x,750,x + r,770);
        floodfill(x + 1,751,WHITE);
        delay(90);
        x+=r;
    }
    Sleep(100);
}
//////////<------>INTERFATA MENIULUI<------>//////////
void interfataMeniu()
{
    setcolor(WHITE);
    setlinestyle(0,0,0);
    readimagefile("fisier.jpg",meniu.c[1].x1+5,meniu.c[1].y1,meniu.c[1].x2,meniu.c[1].y2-5);
    readimagefile("diagrama.jpg",meniu.c[2].x1+5,meniu.c[2].y1+30,meniu.c[2].x2,meniu.c[2].y2+30-5);
    readimagefile("setari.jpg",meniu.c[3].x1+5,meniu.c[3].y1+60,meniu.c[3].x2,meniu.c[3].y2+60-5);
    readimagefile("iesire.jpg",meniu.c[4].x1+5,meniu.c[4].y1+90,meniu.c[4].x2,meniu.c[4].y2+90-5);
    cout<<endl<<"c1:"<<meniu.c[1].y1<<", "<<meniu.c[1].y2-5;
    cout<<endl<<"c2:"<<meniu.c[2].y1+30<<", "<<meniu.c[2].y2+30-5;
    cout<<endl<<"c3:"<<meniu.c[3].y1+60<<", "<<meniu.c[3].y2+60-5;
    cout<<endl<<"c4:"<<meniu.c[4].y1+90<<", "<<meniu.c[4].y2+90-5;
}
//////////<------>CALCUL TABURI<------>//////////
int calculareTab(int lungimeInitiala,pseudocod &x, int indice)
{
    for (int i = 0; i < lungimeInitiala; ++i)
        if(isalpha(x.y[indice].c[i]))
            return i;
}
//////////<------>INTERFATA CHENARULUI FISIERULUI<------>//////////
void interfataPseudo(pseudocod &p)
{
    beepChenar();
    loading();
    cleardevice();
    system("pseudocode.txt");
    //////////<------>DESCHIDE FISIERUL<------>//////////
    FILE *f;
    f = fopen("pseudocode.txt","r");
    if(f == NULL)
    {
        bgiout << "can not open";
        outstreamxy(100,100);
    }

    //////////<------>PRELUARE DATE DIN FISIER<------>//////////
    int indice = 0;
    char linie[256];
    char *linieStatus;
    while(linieStatus)
    {
        linieStatus = fgets(linie,sizeof(linie),f);
        char l[256];
        strcpy(l,linie);
        strcpy(p.y[indice++].c,linie);
    }
    cleardevice();
    //////////<------>ACCESARE LINII DE PSEUDOCOD IN INTERFATA<------>//////////
    //////////<------>ARATA CARE E ULTIMA LINIE DE COD DIN FIECARE PAGINA<------>//////////
    int ultimaLinieCod[21];
    for(int i = 1; i <= 21; ++i)
        ultimaLinieCod[i] = 37*i - 1;
    int paginaCurenta = 1;
    while(1)
    {
        //////////<------>PUNEREA DATELOR IN INTERFATA<------>//////////
        int yspatiu = 10;
        for(int i = 1*(paginaCurenta-1)*37; i <= paginaCurenta*37-1; ++i)
        {
            if(p.liniiCod < i)
                break;
            int xspatiu = 60;
            int tab = calculareTab(strlen(p.y[i].c),p,i);
            bgiout << i;
            outstreamxy(xspatiu,yspatiu);
            bgiout << p.y[i].c;
            outstreamxy(xspatiu + 60 + tab*30,yspatiu);
            yspatiu += 20;
        }
        p.liniiCod = indice-2;
        readimagefile("butoane.jpg",30,750,50,770);
        outtextxy(70,750, "SUS SAU JOS");
        outtextxy(800,750, "R = REFRESH LA PAGINA");
        outtextxy(380,750,"APASATI E PENTRU A SELECTA MENIUL");
        char c = (char)getch();

        if(c == KEY_DOWN)
        {
            if(paginaCurenta*37 - 1 <= p.liniiCod)
                paginaCurenta++;
        }
        if(c == KEY_UP)
        {
            if(paginaCurenta != 1)
                paginaCurenta--;
        }
        else if(c == 'e')
        {
            break;
        }
        cleardevice();
    }
    cleardevice();
}
//////////<------>DEPLASAREA DIAGRAMEI IN FUNCTIE DE COMENZILE ACCESATE<------>//////////
void deplasare(int i,int j)
{
    for(int k = 0; k < linieFisier.size(); ++k)
    {
        if(linieFisier[k].tipst == 0)
        {
            linieFisier[k].d.x.x1 += i;
            linieFisier[k].d.x.x2 += i;
            linieFisier[k].d.x.y1 += j;
            linieFisier[k].d.x.y2 += j;
        }
        else if(linieFisier[k].tipst == 1)
        {
            linieFisier[k].d.x.x1 += i;
            linieFisier[k].d.x.x2 += i;
            linieFisier[k].d.x.y1 += j;
            linieFisier[k].d.x.y2 += j;
        }
        else if(linieFisier[k].tipst == 2)
        {
            linieFisier[k].d.x.x1 += i;
            linieFisier[k].d.x.x2 += i;
            linieFisier[k].d.x.y1 += j;
            linieFisier[k].d.x.y2 += j;
            linieFisier[k].d.y.x1 += i;
            linieFisier[k].d.y.x2 += i;
            linieFisier[k].d.y.y1 += j;
            linieFisier[k].d.y.y2 += j;
            linieFisier[k].d.z.x1 += i;
            linieFisier[k].d.z.x2 += i;
            linieFisier[k].d.z.y1 += j;
            linieFisier[k].d.z.y2 += j;
            linieFisier[k].d.mijloc += i;
        }
        else if(linieFisier[k].tipst == 4)
        {
            linieFisier[k].d.x.x1 += i;
            linieFisier[k].d.x.x2 += i;
            linieFisier[k].d.x.y1 += j;
            linieFisier[k].d.x.y2 += j;
            linieFisier[k].d.y.x1 += i;
            linieFisier[k].d.y.x2 += i;
            linieFisier[k].d.y.y1 += j;
            linieFisier[k].d.y.y2 += j;
            linieFisier[k].d.z.x1 += i;
            linieFisier[k].d.z.x2 += i;
            linieFisier[k].d.z.y1 += j;
            linieFisier[k].d.z.y2 += j;

        }
        else if(linieFisier[k].tipst == 5)
        {
            linieFisier[k].d.x.x1 += i;
            linieFisier[k].d.x.x2 += i;
            linieFisier[k].d.x.y1 += j;
            linieFisier[k].d.x.y2 += j;
            linieFisier[k].d.y.x1 += i;
            linieFisier[k].d.y.x2 += i;
            linieFisier[k].d.y.y1 += j;
            linieFisier[k].d.y.y2 += j;
        }
        else if(linieFisier[k].tipst == 6)
        {
            linieFisier[k].d.x.x1 += i;
            linieFisier[k].d.x.x2 += i;
            linieFisier[k].d.x.y1 += j;
            linieFisier[k].d.x.y2 += j;
            linieFisier[k].d.y.x1 += i;
            linieFisier[k].d.y.x2 += i;
            linieFisier[k].d.y.y1 += j;
            linieFisier[k].d.y.y2 += j;
        }
    }
}
//////////<------>RESETARE INFORMATII DESPRE DIAGRAMA<------>//////////
void golireTOT()
{
    linieFisier.clear();
    perechiIfElse.clear();
    for(int i = 0; i < NoduriMax; ++i)
        muchii[i].clear();
    linii = 0;
    liniiAccesate[300] = {false};
    cost = 0;
    maxim = -1;
    gasit = false;
}
//////////<------>CREAREA DIAGRAMEI<------>//////////
void ProgramInSine()
{
    golireTOT();
    FILE *f;
    f = fopen("pseudocode.txt","r");
    int linie = 1;
    pereche n;
    linieFisier.push_back(n);
    while(!feof(f))
    {
        char m[301];
        fgets(m,MAX_Line_chars,f);
        pereche s;
        strcpy(s.c,m);
        linieFisier.push_back(s);
        linie++;
    }
    cout << endl;
    for (int i = 1; i < linieFisier.size(); ++i)
        cout << i << ' ' << linieFisier[i].c << endl;
    for (int i = 1; i < linieFisier.size(); ++i)
    {
        int nrtabs=0,ok=1,j=0;
        linieFisier[i].accesat=false;
        while(ok==1)
        {
            if(linieFisier[i].c[j]==9) nrtabs++;
            else ok=0;
            j++;
        }
        linieFisier[i].tab=nrtabs;
    }
    crearearbore();
    for (int i = 0; i < linieFisier.size(); ++i)
    {
        cout<<"Vecinii lui "<< i << " sunt : ";
        for(int j = 0; j < muchii[i].size() ; ++j)
            cout<<muchii[i][j]<< "" <<"("<<"tip:"<<linieFisier[muchii[i][j]].tipst<<") ";
        if(i)
            cout << "costul este " << linieFisier[i].cost;
        else
            cout << "costul este " << 0;
        cout<<endl;
    }
    restart();
    crearePerechi();
    for(int i = 0; i < perechiIfElse.size(); ++i)
        cout << "Perechea If-Else: " << perechiIfElse[i].first << ' ' << perechiIfElse[i].second << endl;
    linii = linescount();
    cout << "Numarul total de linii initial este: " << linescount() << endl;
    accesLinii();
    for(int i = 1; i < linieFisier.size(); ++i)
        cout << "Linia lui " << i << " incepe de la " << linieFisier[i].linieChenarInceput << " si se termina la " << linieFisier[i].linieChenarSfarsit << endl;
    maxim = -1;
    for(int i = 1; i < linieFisier.size(); ++i)
    {
        if(linieFisier[i].tipst != -1)
        {
            maxim = max(linieFisier[i].linieChenarSfarsit,maxim);
            linii = maxim;
        }
    }
    cout << "Numarul total de linii actualizat este: " << linii << endl;
    punereChenare();
}
//////////<------>SELECTARE CHENAR<------>//////////
int selectare(int x,int y)
{
    for(int i = 1; i < linieFisier.size(); ++i)
    {
        if(linieFisier[i].tipst == 1)
        {
            if(linieFisier[i].d.x.x1 <= x && linieFisier[i].d.x.x2 >= x && linieFisier[i].d.x.y1 <= y && linieFisier[i].d.x.y2 >= y)
                return i;
        }
        else if(linieFisier[i].tipst == 2)
        {
            if(linieFisier[i].d.x.x1 <= x && linieFisier[i].d.x.x2 >= x && linieFisier[i].d.x.y1 <= y && linieFisier[i].d.x.y2 >= y)
                return i;
        }
        else if(linieFisier[i].tipst == 4)
        {
            if(linieFisier[i].d.x.x1 <= x && linieFisier[i].d.x.x2 >= x && linieFisier[i].d.x.y1 <= y && linieFisier[i].d.x.y2 >= y)
                return i;
            if(linieFisier[i].d.y.x1 <= x && linieFisier[i].d.y.x2 >= x && linieFisier[i].d.y.y1 <= y && linieFisier[i].d.y.y2 >= y)
                return i;
            if(linieFisier[i].d.z.x1 <= x && linieFisier[i].d.z.x2 >= x && linieFisier[i].d.z.y1 <= y && linieFisier[i].d.z.y2 >= y)
                return i;
        }
        else if(linieFisier[i].tipst == 5)
        {
            if(linieFisier[i].d.x.x1 <= x && linieFisier[i].d.x.x2 >= x && linieFisier[i].d.x.y1 <= y && linieFisier[i].d.x.y2 >= y)
                return i;
            if(linieFisier[i].d.y.x1 <= x && linieFisier[i].d.y.x2 >= x && linieFisier[i].d.y.y1 <= y && linieFisier[i].d.y.y2 >= y)
                return i;
        }
        else if(linieFisier[i].tipst == 6)
        {
            if(linieFisier[i].d.x.x1 <= x && linieFisier[i].d.x.x2 >= x && linieFisier[i].d.x.y1 <= y && linieFisier[i].d.x.y2 >= y)
                return i;
            if(linieFisier[i].d.y.x1 <= x && linieFisier[i].d.y.x2 >= x && linieFisier[i].d.y.y1 <= y && linieFisier[i].d.y.y2 >= y)
                return i;
        }
    }
    return -1;
}
//////////<------>EVIDENTIERE LINIE COD SELECTAT<------>//////////
void interfataLinieEvidentiata(int selectat1,int selectat2)
{
    FILE *f;
    f = fopen("pseudocode.txt","r");
    if(f == NULL)
    {
        bgiout << "can not open";
        outstreamxy(100,100);
    }
    //////////<------>PRELUARE DATE DIN FISIER<------>//////////
    int indice = 0;
    char linie[256];
    char *linieStatus;
    while(linieStatus)
    {
        linieStatus = fgets(linie,sizeof(linie),f);
        char l[256];
        strcpy(l,linie);
        strcpy(p.y[indice++].c,linie);
    }
    cleardevice();
    //////////<------>ACCESARE LINII DE PSEUDOCOD IN INTERFATA<------>//////////
    //////////<------>ARATA CARE E ULTIMA LINIE DE COD DIN FIECARE PAGINA<------>//////////
    int ultimaLinieCod[21];
    for(int i = 1; i <= 21; ++i)
        ultimaLinieCod[i] = 37*i - 1;
    int paginaCurenta = 1;
    while(1)
    {
        //////////<------>PUNEREA DATELOR IN INTERFATA<------>//////////
        int yspatiu = 10;
        for(int i = 1*(paginaCurenta-1)*37; i <= paginaCurenta*37-1; ++i)
        {
            if(p.liniiCod < i)
                break;
            if(i == selectat1 -1 || i == selectat2 -1)
                setcolor(GREEN);
            int xspatiu = 60;
            int tab = calculareTab(strlen(p.y[i].c),p,i);
            bgiout << i;
            outstreamxy(xspatiu,yspatiu);
            bgiout << p.y[i].c;
            outstreamxy(xspatiu + 60 + tab*30,yspatiu);
            yspatiu += 20;
            if(i == selectat1 -1 || i == selectat2 -1)
                setcolor(WHITE);
        }
        p.liniiCod = indice-2;
        readimagefile("butoane.jpg",30,750,50,770);
        outtextxy(70,750, "SUS SAU JOS");
        outtextxy(800,750, "R = REFRESH LA PAGINA");
        outtextxy(380,750,"APASATI E PENTRU A SELECTA MENIUL");
        char c = (char)getch();

        if(c == KEY_DOWN)
        {
            if(paginaCurenta*37 - 1 <= p.liniiCod)
                paginaCurenta++;
        }
        if(c == KEY_UP)
        {
            if(paginaCurenta != 1)
                paginaCurenta--;
        }
        else if(c == 'e')
        {
            break;
        }
        cleardevice();
    }
    getch();
}
//////////<------>INTERFATA DIAGRAMA<------>//////////
void interfataDiagrama()
{
    loading();
    ProgramInSine();
    //////////<------>COMENZI PENTRU SCHIMBAREA STARII DIAGRAMEI<------>//////////
    while(1)
    {
        cleardevice();
        char c;
        desenLinii();
        c = (char)getch();
        if(c == 's')
        {
            clearmouseclick(WM_LBUTTONDBLCLK);
            while(1)
            {
                if(ismouseclick(WM_LBUTTONDBLCLK))
                {
                    int select = selectare(mousex(),mousey());
                    if(select != -1)
                    {
                        if(linieFisier[select].tipst == 6)
                            interfataLinieEvidentiata(select,gasireRepeat(select));
                        else if(linieFisier[select].tipst == 2 && gasirePerecheIf(select) != -1)
                            interfataLinieEvidentiata(select,gasirePerecheIf(select));
                        else
                            interfataLinieEvidentiata(select,select);
                    }
                    break;
                }
            }
        }
        else if(c == KEY_RIGHT)
            deplasare(50,0);
        else if(c == KEY_LEFT)
            deplasare(-50,0);
        else if(c == KEY_UP)
            deplasare(0,-50);
        else if(c == KEY_DOWN)
            deplasare(0,50);
        else if(c == 'e')
        {
            return;
        }
    }
}
//////////<------>INTERFATA SETARI<------>//////////
//////////<------>STAREA CHENARELOR DIN SETARII<------>//////////
void stareSchimbataSetari(chenareSettings &x,int indice)
{
    x.a[indice].y= {255,255,255};
    for(int i = 0; i < 4; ++i)
        if(i != indice)
            x.a[i].y= {115,115,91};
}

//////////<------>CREAREA CHENARELOR DIN SETARI<------>//////////
void creareChenareSetari(chenareSettings &x)
{
    //////////<------>AUDIO<------>//////////
    // x1 = 550, y1 = 200, x2 = 950, y2 = 350
    setcolor(COLOR(x.a[1].y.r,x.a[1].y.g,x.a[1].y.b));
    outtextxy(475,275,"SUNET");
    if(audio == 1)
        outtextxy(475,300,"PORNIT");
    else
        outtextxy(475,300,"OPRIT");
    x.a[1].x1 = 300;
    x.a[1].x2 = 700;
    x.a[1].y1 = 200;
    x.a[1].y2 = 350;
    rectangle(x.a[1].x1,x.a[1].y1,x.a[1].x2,x.a[1].y2);

    //////////<------>MUSIC<------>//////////
    // x1 = 50, y1 = 450, x2 = 450, y2 = 600
    setcolor(COLOR(x.a[2].y.r,x.a[2].y.g,x.a[2].y.b));
    outtextxy(215,525,"MUZICA");
    if(audio2 == 1)
        outtextxy(215,550,"PORNIT");
    else
        outtextxy(215,550,"OPRIT");
    x.a[2].x1 = 50;
    x.a[2].x2 = 450;
    x.a[2].y1 = 450;
    x.a[2].y2 = 600;
    rectangle(x.a[2].x1,x.a[2].y1,x.a[2].x2,x.a[2].y2);

    //////////<------>BACK<------>//////////
    // x1 = 550, y1 = 450, x2 = 950, y2 = 600
    setcolor(COLOR(x.a[3].y.r,x.a[3].y.g,x.a[3].y.b));
    outtextxy(725,525,"INAPOI");
    x.a[3].x1 = 550;
    x.a[3].x2 = 950;
    x.a[3].y1 = 450;
    x.a[3].y2 = 600;
    rectangle(x.a[3].x1,x.a[3].y1,x.a[3].x2,x.a[3].y2);

    setcolor(WHITE);
}

//////////<------>STARE INITIALA SETARI<------>//////////
void stareInitialaSetari(chenareSettings &x)
{
    for(int i = 0; i < 2; ++i)
    {
        x.a[i].op1 = true;
        x.a[i].op2 = false;
    }
}
//////////<------>INTERFATA PENTRU SETARI<------>//////////
void interfataSettings()
{
    int indice = 1;
    int x,y;
    beepChenar();
    while(1)
    {
        cleardevice();
        //////////<------>ACTUALIZARE STARE CHENARE START<------>//////////
        setbkcolor(BLACK);
        setcolor(WHITE);
        cleardevice();
        stareSchimbataSetari(z,indice);
        creareChenareSetari(z);
        setcolor(COLOR(115,115,91));
        if(indice == 3 )
        {
            delay(50);
            return ;
        }
        clearmouseclick(WM_LBUTTONDOWN);
        while(!ismouseclick(WM_LBUTTONDOWN))
        {
            x=mousex();
            y=mousey();
            if(x>300&&x<700&&y>200&&y<350)
            {
                stareSchimbataSetari(z,1);
                indice = 1;
                if(ismouseclick(WM_LBUTTONDOWN))
                {
                    clearmouseclick(WM_LBUTTONDOWN);
                    audio = !audio;
                    break;
                }
            }
            if(x>50&&x<450&&y>450&&y<600)
            {
                stareSchimbataSetari(z,2);
                indice = 2;
                if(ismouseclick(WM_LBUTTONDOWN))
                {
                    clearmouseclick(WM_LBUTTONDOWN);
                    audio2 = !audio2;
                    if(audio2 == 1)
                        PlaySoundA((LPCSTR)"music.wav", NULL, SND_FILENAME | SND_ASYNC);
                    else
                        PlaySoundA((LPCSTR)"gol.wav", NULL, SND_FILENAME | SND_ASYNC);
                    break;
                }
            }
            if(x>550&&x<950&&y>450&&y<600)
            {
                stareSchimbataSetari(z,3);
                if(ismouseclick(WM_LBUTTONDOWN))
                {
                    indice = 3;
                    break;
                }
            }
        }
    }
    setcolor(WHITE);
}
//////////<------>STAREA INITIALA A MENIULUI<------>//////////
void stareInitialaMeniu()
{
    meniu.accesat = true;
    for(int i = 1; i <= 4; ++i)
        meniu.c[i] = {900,i * 100,1000,i * 100 + 100,false};
}
//////////<------>INTERFATA PRINCIPALA A MENIULUI<------>//////////
void interfataPrincipala(int indice)
{
    if(indice == 1)
        interfataPseudo(p);
    else if(indice == 2)
        interfataDiagrama();
    else if(indice == 3)
        interfataSettings();

}
int main()
{
    stareInitialaMeniu();
    initwindow(lungime,latime,"INTERFATA DIAGRAMA NS");
    interfataMeniu();
    int x,y;
    int indice = 0;
    while(1)
    {
        cleardevice();
        beepChenar();
        interfataMeniu();
        clearmouseclick(WM_LBUTTONDOWN);
        while(!ismouseclick(WM_LBUTTONDOWN))
        {
            x = mousex(),y = mousey();
            if(x >= 900 && y <= 100 && !meniu.accesat)
                outtextxy(700,50,"MENIU");
            else if(x >= 920 && meniu.accesat)
            {
                if(y <= 200)
                    outtextxy(930,200,"FISIER");
                else if(y <= 330)
                    outtextxy(920,330,"DIAGRAMA");
                else if(y <= 460)
                    outtextxy(930,460,"SETARI");
                else if(y <= 590)
                    outtextxy(930,590,"IESIRE");
            }
        }
        if(x >= 920)
        {
            if(y>100&&y<195) indice = 1;
            if(y>230&&y<325) indice = 2;
            if(y>360&&y<455) indice = 3;
            if(y>490&&y<585) indice = 4;
        }
        if(indice == 4)
            return 0;
        interfataPrincipala(indice);
    }
    getch();
}
