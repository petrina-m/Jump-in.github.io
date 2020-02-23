#include <iostream>
#include <fstream>
#include <winbgim.h>
#include <stdlib.h>
#include <graphics.h>
#include <cstring>
#include<time.h>
#include<mmsystem.h>

#define LUNGIME 1200
#define LATIME 600
#define IEPURE 1
#define GROAPA 5
#define CIUPERCA 4
#define LIBER 0
#define VERDE_DESCHIS COLOR(0,172,79)
#define VERDE_INCHIS COLOR(0,152,74)
#define MARO COLOR(69,50,20)
using namespace std;

///generale
void afisare_tabla();
void incepere_nivel();
void coordonate_butoane();
void pregatire_joc();
///tablou cu numere
void bordare();
///tabla desenata/joc
void deseneazaTabla();
void adauga_personaje();
///reguli joc
void castigare_nivel();
void JOC();
void care_patrat(int &x, int &y);
void stergePiesa(int x_initial,int y_initial);
void deseneazaPiesa(int x_final,int y_final,int codPiesa);
void pune_iepure_gaura();
void scoate_iepure_gaura(int i,int j);
//bool verificare_castigare_nivel(int nr_iepuri_ok,int nr_iepuri);
bool pot_muta_iepure_acolo(int x_initial, int y_initial, int x_final, int y_final);
bool pot_muta_vulpe_acolo(int &x_initial, int &y_initial, int x_final, int y_final, int &codPiesa);
void conversie_numar_in_caractere(int n, char numar[]);
void random_level();
void meniu();
void selecteaza_nivel();
void outtextxy(int x, int y, char *textstring);
void buton_meniu();
bool apasat_buton_meniu(int x, int y);

///structuri de date
int TABLA[7][7]; /// tabou cu numere
struct coordonate  ///memorez coodonatele pt fiecare patratel
{
    int x1,y1; ///coord punct din stanga sus
    int x2,y2; ///coord punct din dreapta jos
} TABLA_COORD[7][7],butoane_meniu[4],butoane_meniu_start[5],nivel[30],meniu_coordonate;


void initializeaza_TABLA_COORD();
void citire_nivel();


int MARGINE=(LUNGIME-LATIME)/2;
int latura_patrat=LATIME/5;
bool MATRICE_GROPI[8][8];
int nr_iepuri,nr_iepuri_ok;
int nrfisier=1;
int nrtest;
void incepere_nivel_random();
int main()
{
    pregatire_joc();
    meniu();
    //incepere_nivel();

    //JOC();
    getch();
    closegraph();
    return 0;
}
void meniu()
{

    setfillstyle(SOLID_FILL,BLACK);
    bar(meniu_coordonate.x1,meniu_coordonate.y1,meniu_coordonate.x2,meniu_coordonate.y2);

    readimagefile("FUNDAL_MENIU.jpg",MARGINE,0,MARGINE+LATIME,LATIME);
    readimagefile("MENIU.jpg",TABLA_COORD[1][2].x1,TABLA_COORD[1][2].y1,TABLA_COORD[1][4].x2,TABLA_COORD[1][4].y2);
    readimagefile("JOC_NOU.jpg",TABLA_COORD[2][2].x1,TABLA_COORD[2][2].y1,TABLA_COORD[2][4].x2,TABLA_COORD[2][4].y2);
    readimagefile("ALEGE_NIVEL.jpg",TABLA_COORD[3][2].x1,TABLA_COORD[3][2].y1,TABLA_COORD[3][4].x2,TABLA_COORD[3][4].y2);
    readimagefile("NIVEL_RANDOM.jpg",TABLA_COORD[4][2].x1,TABLA_COORD[4][2].y1,TABLA_COORD[4][4].x2,TABLA_COORD[4][4].y2);
    readimagefile("INSTRUCTIUNI_BUTON.jpg",TABLA_COORD[5][2].x1,TABLA_COORD[5][2].y1,TABLA_COORD[5][4].x2,TABLA_COORD[5][4].y2);


    for(int i=1; i<=4; i++)
    {
        butoane_meniu_start[i].x1=TABLA_COORD[i+1][2].x1;
        butoane_meniu_start[i].y1=TABLA_COORD[i+1][2].y1;
        butoane_meniu_start[i].x2=TABLA_COORD[i+1][4].x2;
        butoane_meniu_start[i].y2=TABLA_COORD[i+1][4].y2;
    }
    int x_click,y_click;
    int buton=0,ruleaza=1,n=4;
    while(ruleaza)
    {
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            getmouseclick(WM_LBUTTONDOWN,x_click,y_click);
            for(int i=1; i<=n; i++)
            {
                if(x_click>=butoane_meniu_start[i].x1 && x_click<=butoane_meniu_start[i].x2 && y_click>=butoane_meniu_start[i].y1 && y_click<=butoane_meniu_start[i].y2)
                {
                    PlaySound("buton.wav",GetModuleHandle(NULL),SND_FILENAME|SND_ASYNC|SND_NODEFAULT);
                    buton=i;
                    ruleaza=0;
                    break;
                }
            }
        }
    }

    if(buton==1)
    {
        PlaySound("buton.wav",GetModuleHandle(NULL),SND_FILENAME|SND_ASYNC|SND_NODEFAULT);
        nrfisier=1;
        buton_meniu();
        //citire_nivel();
        incepere_nivel();
        JOC();
    }
    if(buton==2)
    {
        PlaySound("buton.wav",GetModuleHandle(NULL),SND_FILENAME|SND_ASYNC|SND_NODEFAULT);
        buton_meniu();
        selecteaza_nivel();
    }
    if(buton==3)
    {
        PlaySound("buton.wav",GetModuleHandle(NULL),SND_FILENAME|SND_ASYNC|SND_NODEFAULT);
        buton_meniu();
        random_level();
        incepere_nivel_random();
        JOC();
    }
    ruleaza=1;
    if(buton==4)
    {
        PlaySound("buton.wav",GetModuleHandle(NULL),SND_FILENAME|SND_ASYNC|SND_NODEFAULT);
        buton_meniu();
        readimagefile("INSTRUCTIUNI.jpg",MARGINE,0,MARGINE+LATIME,LATIME);
        while(ruleaza)
        {
            if(ismouseclick(WM_LBUTTONDOWN))
            {
                getmouseclick(WM_LBUTTONDOWN,x_click,y_click);
                if(apasat_buton_meniu(x_click,y_click))
                {
                    ruleaza=0;
                    meniu();
                }
            }
        }
    }
}

void buton_meniu()
{
    readimagefile("MENIU.jpg",MARGINE+LATIME,0,MARGINE+LATIME+latura_patrat,latura_patrat/3);
    meniu_coordonate.x1=MARGINE+LATIME;
    meniu_coordonate.y1=0;
    meniu_coordonate.x2=MARGINE+LATIME+latura_patrat;
    meniu_coordonate.y2=latura_patrat/3;
}

bool apasat_buton_meniu(int x, int y)
{
    if(x>=meniu_coordonate.x1 && x<=meniu_coordonate.x2 && y>=meniu_coordonate.y1 && y<=meniu_coordonate.y2)
        return 1;
    return 0;
}

void selecteaza_nivel()
{
    setfillstyle(SOLID_FILL,VERDE_DESCHIS);
    bar(MARGINE,0,MARGINE+LATIME,LATIME);
    int k=1;
    char nr[2]= {'\0','\0'};
    for(int i=1; i<=5; i++)
        for(int j=1; j<=5; j++)
        {
            if(k>9)
            {
                nr[1]='0'+k%10;
                nr[0]='0'+k/10;
            }
            else
            {
                nr[0]='0'+k;
                nr[1]='\0';
            }
            nr[2]='\0';
            nivel[k].x1=TABLA_COORD[i][j].x1;
            nivel[k].y1=TABLA_COORD[i][j].y1;
            nivel[k].x2=TABLA_COORD[i][j].x2;
            nivel[k].y2=TABLA_COORD[i][j].y2;
            if((i+j)%2==0)
            {
                setfillstyle(SOLID_FILL,WHITE);
                bar(TABLA_COORD[i][j].x1+1,TABLA_COORD[i][j].y1+1,TABLA_COORD[i][j].x2-1,TABLA_COORD[i][j].y2-1);
                setfillstyle(SOLID_FILL,WHITE);
                settextstyle(DEFAULT_FONT,HORIZ_DIR,latura_patrat/3);
                outtextxy(TABLA_COORD[i][j].x1+latura_patrat/5,TABLA_COORD[i][j].y1+latura_patrat/5,nr);
            }
            else
            {
                setfillstyle(SOLID_FILL,BLACK);
                bar(TABLA_COORD[i][j].x1+1,TABLA_COORD[i][j].y1+1,TABLA_COORD[i][j].x2-1,TABLA_COORD[i][j].y2-1);
                setcolor(WHITE);
                settextstyle(DEFAULT_FONT,HORIZ_DIR,latura_patrat/3);
                outtextxy(TABLA_COORD[i][j].x1+latura_patrat/5,TABLA_COORD[i][j].y1+latura_patrat/5,nr);
            }
            k++;
        }

    int x_click,y_click;
    int buton=0,ruleaza=1,n=25;
    while(ruleaza)
    {
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            getmouseclick(WM_LBUTTONDOWN,x_click,y_click);

            if(apasat_buton_meniu(x_click,y_click))
            {
                PlaySound("buton.wav",GetModuleHandle(NULL),SND_FILENAME|SND_ASYNC|SND_NODEFAULT);
                meniu();
            }

            for(int i=1; i<=n; i++)
            {
                if(x_click>=nivel[i].x1 && x_click<=nivel[i].x2 && y_click>=nivel[i].y1 && y_click<=nivel[i].y2)
                {
                    PlaySound("buton.wav",GetModuleHandle(NULL),SND_FILENAME|SND_ASYNC|SND_NODEFAULT);
                    buton=i;
                    ruleaza=0;
                    break;
                }
            }
        }
    }
    if(buton!=0)
    {
        PlaySound("buton.wav",GetModuleHandle(NULL),SND_FILENAME|SND_ASYNC|SND_NODEFAULT);
        nrfisier=buton;
        citire_nivel();
        incepere_nivel();
        JOC();
    }
}



void pregatire_joc()
{
    initwindow(LUNGIME,LATIME,"JUMP IN");
    bordare();
    initializeaza_TABLA_COORD();
    coordonate_butoane();
}
void bordare()
{
    for(int i=0; i<=6; i++)
    {
        TABLA[0][i]=-1;
        TABLA[6][i]=-1;
        TABLA[i][0]=-1;
        TABLA[i][6]=-1;
    }
}
void initializeaza_TABLA_COORD()
{
    ///calculez coordonatele pt fiecare patrat

    for(int i=1; i<=5; i++)
        for(int j=1; j<=5; j++)
        {
            TABLA_COORD[i][j].x1=MARGINE+latura_patrat*(j-1);
            TABLA_COORD[i][j].y1=latura_patrat*(i-1);
            TABLA_COORD[i][j].x2=MARGINE+latura_patrat*j;
            TABLA_COORD[i][j].y2=latura_patrat*i;
        }
}
void incepere_nivel()
{
    citire_nivel();
    //afisare_tabla();
    deseneazaTabla();
    adauga_personaje();

}
void citire_nivel()
{
    char calefisiernivel[]="nivel\\nivel",numar[3]= {0};

    conversie_numar_in_caractere(nrfisier,numar);
    strcat(calefisiernivel,numar);
    strcat(calefisiernivel,".txt");


    string vect;
    ifstream nivelfile;

    nivelfile.open(calefisiernivel);

    if(nivelfile.is_open())
    {
        int i=1,j;
        /// citesc iepuri/ciuperci/vulpi
        while(i<=5)
        {
            getline(nivelfile,vect);
            for(j=0; j<=4; j++)
            {
                TABLA[i][j+1]=vect[j]-'0';
            }
            i++;
        }
        ///citesc gropile
        i=1;
        while(i<=5)
        {
            getline(nivelfile,vect);
            for(j=0; j<=4; j++)
            {
                MATRICE_GROPI[i][j+1]=vect[j]-'0';
            }
            i++;
        }


        nivelfile.close();
    }
}
void deseneazaTabla()
{
    setfillstyle(SOLID_FILL,VERDE_DESCHIS);
    bar(MARGINE,0,MARGINE+LATIME,LATIME);

    for(int i=1; i<=5; i++)
        for(int j=1; j<=5; j++)
        {
            if(i%2==1 && j%2==1)
            {
                setcolor(WHITE);
                line(TABLA_COORD[i][j].x1,TABLA_COORD[i][j].y1,TABLA_COORD[i][j].x1,TABLA_COORD[i][j].y2);
                line(TABLA_COORD[i][j].x1,TABLA_COORD[i][j].y1,TABLA_COORD[i][j].x2,TABLA_COORD[i][j].y1);
                line(TABLA_COORD[i][j].x1,TABLA_COORD[i][j].y2,TABLA_COORD[i][j].x2,TABLA_COORD[i][j].y2);
                line(TABLA_COORD[i][j].x2,TABLA_COORD[i][j].y2,TABLA_COORD[i][j].x2,TABLA_COORD[i][j].y1);
            }
            setfillstyle(SOLID_FILL,VERDE_INCHIS);
            fillellipse((TABLA_COORD[i][j].x1+TABLA_COORD[i][j].x2)/2,(TABLA_COORD[i][j].y1+TABLA_COORD[i][j].y2)/2,latura_patrat/3.5,latura_patrat/3.5);
            setcolor(VERDE_INCHIS);
            circle((TABLA_COORD[i][j].x1+TABLA_COORD[i][j].x2)/2,(TABLA_COORD[i][j].y1+TABLA_COORD[i][j].y2)/2,latura_patrat/3.5);
        }
}
void adauga_personaje()
{
///asocierea numerelor din tabloul cu numere c personajele care tb afisate pe ecran in timpul jocului
    nr_iepuri = 0; ///numar cati iepuri exista intr-un nivel;
    nr_iepuri_ok=0;
    for(int i=1; i<=5; i++)
        for(int j=1; j<=5; j++)
        {
            if (MATRICE_GROPI[i][j]==1) /// se adauga groapa IN LOCUL UNDE EXISTA IN MATRICEA DOAR CU GROPI
            {
                if(TABLA[i][j]==1)
                    nr_iepuri_ok++;
                setfillstyle(SOLID_FILL,MARO);
                fillellipse((TABLA_COORD[i][j].x1+TABLA_COORD[i][j].x2)/2,(TABLA_COORD[i][j].y1+TABLA_COORD[i][j].y2)/2,latura_patrat/3.5,latura_patrat/3.5);
            }
            if (TABLA[i][j]==1) ///se adauga iepure
            {
                nr_iepuri++;
                readimagefile("iepure.jpg",TABLA_COORD[i][j].x1+1,TABLA_COORD[i][j].y1+1,TABLA_COORD[i][j].x2-1,TABLA_COORD[i][j].y2-1);
            }

            if (TABLA[i][j]==2) /// se adauga vulpe orizontala
            {
                readimagefile("vulpe_orizontala.jpg",TABLA_COORD[i][j].x1-latura_patrat+1,TABLA_COORD[i][j].y1+1,TABLA_COORD[i][j].x2-1,TABLA_COORD[i][j].y2-1);
            }
            if (TABLA[i][j]==3) /// se adauga vulpe verticala
            {
                readimagefile("vulpe_verticala.jpg",TABLA_COORD[i][j].x1+1,TABLA_COORD[i][j].y1+1,TABLA_COORD[i][j].x2-1,TABLA_COORD[i][j].y2+latura_patrat-1);
            }
            if (TABLA[i][j]==4) /// se adauga ciuperca
            {
                readimagefile("ciuperca.jpg",TABLA_COORD[i][j].x1+1,TABLA_COORD[i][j].y1+1,TABLA_COORD[i][j].x2-1,TABLA_COORD[i][j].y2-1);
            }
        }
}
void JOC()
{
    //  nr_iepuri_ok = 0;
    int x_initial, y_initial, x_final,y_final,codPiesa;
    bool ruleaza=1;
    while(ruleaza)
    {
        if(ismouseclick(WM_LBUTTONDOWN))   //primul click
        {
            getmouseclick(WM_LBUTTONDOWN,y_initial,x_initial);

            if(apasat_buton_meniu(y_initial,x_initial))
            {
                PlaySound("buton.wav",GetModuleHandle(NULL),SND_FILENAME|SND_ASYNC|SND_NODEFAULT);
                meniu();
            }

            care_patrat(x_initial, y_initial);
            while(ruleaza && (TABLA[x_initial][y_initial]==1 || TABLA[x_initial][y_initial]==2 || TABLA[x_initial][y_initial]==3 || TABLA[x_initial][y_initial]==7 || TABLA[x_initial][y_initial]==8))
            {

                if(ismouseclick(WM_LBUTTONDOWN) )   //al doilea click
                {
                    getmouseclick(WM_LBUTTONDOWN,y_final,x_final);

                    if(apasat_buton_meniu(y_final,x_final))
                    {
                        PlaySound("buton.wav",GetModuleHandle(NULL),SND_FILENAME|SND_ASYNC|SND_NODEFAULT);
                        meniu();
                    }
                    care_patrat(x_final, y_final);
                    break;
                }
            }
            if(TABLA[x_initial][y_initial]==1)
            {
                if (pot_muta_iepure_acolo(x_initial,y_initial,x_final,y_final))
                {
                    PlaySound("jump.wav",GetModuleHandle(NULL),SND_FILENAME|SND_ASYNC|SND_NODEFAULT);
                    codPiesa=TABLA[x_initial][y_initial];
                    TABLA[x_initial][y_initial]=0;

                    TABLA[x_final][y_final]=codPiesa;
                    stergePiesa(x_initial,y_initial);
                    deseneazaPiesa(x_final,y_final,codPiesa);
                    if(MATRICE_GROPI[x_final][y_final]==1)
                        pune_iepure_gaura();
                    if(MATRICE_GROPI[x_initial][y_initial]==1)
                        scoate_iepure_gaura(x_initial,y_initial);
                }
            }
            else if(TABLA[x_initial][y_initial]==2 || TABLA[x_initial][y_initial]==3 || TABLA[x_initial][y_initial]==7 || TABLA[x_initial][y_initial]==8)
            {
                codPiesa=TABLA[x_initial][y_initial];
                if(pot_muta_vulpe_acolo(x_initial,y_initial,x_final,y_final,codPiesa))
                {
                    deseneazaPiesa(x_final,y_final,codPiesa);
                }
            }
        }
    }
}
void care_patrat(int &x, int &y)
{
    y=y-MARGINE;
    x=x/latura_patrat+1;
    y=y/latura_patrat+1;
}
void deseneazaPiesa(int i,int j,int codPiesa)
{
    if (codPiesa==1) ///se adauga iepure
    {
        readimagefile("iepure.jpg",TABLA_COORD[i][j].x1+1,TABLA_COORD[i][j].y1+1,TABLA_COORD[i][j].x2-1,TABLA_COORD[i][j].y2-1);
    }
    else if (codPiesa==2) /// se adauga vulpe orizontala
    {
        readimagefile("vulpe_orizontala.jpg",TABLA_COORD[i][j].x1-latura_patrat+1,TABLA_COORD[i][j].y1+1,TABLA_COORD[i][j].x2-1,TABLA_COORD[i][j].y2-1);
    }
    else if (codPiesa==3) /// se adauga vulpe verticala
    {
        readimagefile("vulpe_verticala.jpg",TABLA_COORD[i][j].x1+1,TABLA_COORD[i][j].y1+1,TABLA_COORD[i][j].x2-1,TABLA_COORD[i][j].y2+latura_patrat-1);
    }
}
void stergePiesa(int i,int j)
{
    setfillstyle(SOLID_FILL,VERDE_DESCHIS);
    bar(TABLA_COORD[i][j].x1+1,TABLA_COORD[i][j].y1+1,TABLA_COORD[i][j].x2-1,TABLA_COORD[i][j].y2-1);
    setfillstyle(SOLID_FILL,VERDE_INCHIS);
    fillellipse((TABLA_COORD[i][j].x1+TABLA_COORD[i][j].x2)/2,(TABLA_COORD[i][j].y1+TABLA_COORD[i][j].y2)/2,latura_patrat/3.5,latura_patrat/3.5);
    setcolor(VERDE_INCHIS);
    circle((TABLA_COORD[i][j].x1+TABLA_COORD[i][j].x2)/2,(TABLA_COORD[i][j].y1+TABLA_COORD[i][j].y2)/2,latura_patrat/3.5);
}
void pune_iepure_gaura()
{
    nr_iepuri_ok++;
    if(nr_iepuri_ok==nr_iepuri)
    {
        castigare_nivel();
        nr_iepuri_ok=0;
    }

}
void scoate_iepure_gaura(int i, int j)
{
    nr_iepuri_ok--;
    setfillstyle(SOLID_FILL,MARO);
    fillellipse((TABLA_COORD[i][j].x1+TABLA_COORD[i][j].x2)/2,(TABLA_COORD[i][j].y1+TABLA_COORD[i][j].y2)/2,latura_patrat/3.5,latura_patrat/3.5);
}
void coordonate_butoane()
{
    int i=(TABLA_COORD[2][2].x1+latura_patrat/5+TABLA_COORD[2][2].x2)/2;
    int j=(TABLA_COORD[2][2].y1+latura_patrat/5+TABLA_COORD[2][2].y2)/2;
    int n=1;
    while(n<4)
    {
        butoane_meniu[n].x1=i-latura_patrat/4;
        butoane_meniu[n].y1=j-latura_patrat/4;
        butoane_meniu[n].x2=i+latura_patrat/4;
        butoane_meniu[n].y2=j+latura_patrat/4;
        j+=latura_patrat/1.8;
        n++;
    }
}
void castigare_nivel()
{
    int x_click,y_click;
    readimagefile("bravo.jpg",MARGINE+LATIME/5,LATIME/5,MARGINE+(LATIME/5)*4,(LATIME/5)*4);
    setfillstyle(SOLID_FILL,BLACK);
    readimagefile("NEXT.jpg",butoane_meniu[1].x1,butoane_meniu[1].y1,butoane_meniu[1].x2,butoane_meniu[1].y2);
    readimagefile("REPLAY.jpg",butoane_meniu[2].x1,butoane_meniu[2].y1,butoane_meniu[2].x2,butoane_meniu[2].y2);
    readimagefile("BUTON_MENIU.jpg",butoane_meniu[3].x1,butoane_meniu[3].y1,butoane_meniu[3].x2,butoane_meniu[3].y2);
    int buton=0,ruleaza=1,n=3;
    while(ruleaza)
    {
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            getmouseclick(WM_LBUTTONDOWN,x_click,y_click);
            for(int i=1; i<=n; i++)
            {
                if(x_click>=butoane_meniu[i].x1 && x_click<=butoane_meniu[i].x2 && y_click>=butoane_meniu[i].y1 && y_click<=butoane_meniu[i].y2)
                {
                    PlaySound("buton.wav",GetModuleHandle(NULL),SND_FILENAME|SND_ASYNC|SND_NODEFAULT);
                    buton=i;
                    ruleaza=0;
                    break;
                }
            }
        }
    }
    if(buton==1)
    {
        PlaySound("buton.wav",GetModuleHandle(NULL),SND_FILENAME|SND_ASYNC|SND_NODEFAULT);
        nrfisier++;
        citire_nivel();
        incepere_nivel();
        JOC();
    }
    if(buton==2)
    {
        PlaySound("buton.wav",GetModuleHandle(NULL),SND_FILENAME|SND_ASYNC|SND_NODEFAULT);
        citire_nivel();
        incepere_nivel();
        JOC();
    }
    if(buton==3)
    {
        PlaySound("buton.wav",GetModuleHandle(NULL),SND_FILENAME|SND_ASYNC|SND_NODEFAULT);
        //nr_iepuri_ok=0;
        meniu();
    }

}
void random_level()
{
    srand(time(NULL));
    // int nrtest;
    nrtest=rand()%25+1;
    char calefisiernivel[]="nivel\\nivel",numar2[3]= {0};

    conversie_numar_in_caractere(nrtest,numar2);
    strcat(calefisiernivel,numar2);
    strcat(calefisiernivel,".txt");


    string vect;
    ifstream nivelfile;

    nivelfile.open(calefisiernivel);

    if(nivelfile.is_open())
    {
        int i=1,j;
        /// citesc iepuri/ciuperci/vulpi
        while(i<=5)
        {
            getline(nivelfile,vect);
            for(j=0; j<=4; j++)
            {
                TABLA[i][j+1]=vect[j]-'0';
            }
            i++;
        }
        ///citesc gropile
        i=1;
        while(i<=5)
        {
            getline(nivelfile,vect);
            for(j=0; j<=4; j++)
            {
                MATRICE_GROPI[i][j+1]=vect[j]-'0';
            }
            i++;
        }


        nivelfile.close();
    }
}

void conversie_numar_in_caractere(int n, char numar[])
{
    int nrcifre=0;
    int copie_n=n;
    while(copie_n)
    {
        copie_n/=10;
        nrcifre++;
    }
    int cfr;

    numar[nrcifre]=0;
    while (n)
    {
        cfr=n%10;
        numar[nrcifre-1]='0'+ cfr;
        nrcifre--;
        n=n/10;
    }
}
bool pot_muta_iepure_acolo(int x_initial, int y_initial, int x_final, int y_final)
{
    int i;
    if(x_initial==x_final && (y_initial==y_final|| y_initial==y_final+1 || y_final==y_initial+1))
        return 0;

    if(y_initial==y_final && (x_initial==x_final|| x_initial==x_final+1 || x_final==x_initial+1))
        return 0;

    if(TABLA[x_final][y_final]==0)
    {
        if(x_initial==x_final)
        {
            if(y_initial<y_final)
            {
                for(i=y_initial+1; i<y_final; i++)
                {
                    if(TABLA[x_initial][i]==0)
                        return 0;
                }
            }
            else
            {
                for(i=y_final+1; i<y_initial; i++)
                {
                    if(TABLA[x_initial][i]==0)
                        return 0;
                }
            }
            return 1;
        }
        else if(y_initial==y_final)
        {
            if(x_initial<x_final)
                for(i=x_initial+1; i<x_final; i++)
                {
                    if(TABLA[i][y_initial]==0)
                        return 0;
                }
            else
            {
                for(i=x_final+1; i<x_initial; i++)
                {
                    if(TABLA[i][y_initial]==0)
                        return 0;
                }
            }
            return 1;
        }
    }

    return 0;

}
bool pot_muta_vulpe_acolo(int &x_initial, int &y_initial, int x_final, int y_final, int &codPiesa)
{
    if(x_initial==x_final  && (codPiesa%7)%2==0)
    {
        if(TABLA[x_initial][y_initial]==7)
            y_initial++;
        if(TABLA[x_initial][y_final-1]==-1)
            return 0;
        if(TABLA[x_initial][y_initial]==TABLA[x_initial][y_final])
            return 0;
        if(y_initial<y_final)
        {
            for(int i=y_initial+1; i<=y_final; i++)
                if(TABLA[x_initial][i]!=0  || (TABLA[x_initial][i]==0 && MATRICE_GROPI[x_initial][i]==1))
                    return 0;
        }
        else
        {
            for(int i=y_initial-2; i>=y_final-1; i--)
                if(TABLA[x_initial][i]!=0  || (TABLA[x_initial][i]==0 && MATRICE_GROPI[x_initial][i]==1))
                    return 0;
        }
        codPiesa=TABLA[x_initial][y_initial];
        TABLA[x_initial][y_initial]=0;
        TABLA[x_initial][y_initial-1]=0;
        TABLA[x_final][y_final]=2;
        TABLA[x_final][y_final-1]=7;
        stergePiesa(x_initial,y_initial);
        stergePiesa(x_initial,y_initial-1);

        setcolor(VERDE_DESCHIS);
        line(TABLA_COORD[x_initial][y_initial].x1,TABLA_COORD[x_initial][y_initial].y1+1,TABLA_COORD[x_initial][y_initial].x1,TABLA_COORD[x_initial][y_initial].y2-1);
        line(TABLA_COORD[x_initial][y_initial].x1+1,TABLA_COORD[x_initial][y_initial].y1+1,TABLA_COORD[x_initial][y_initial].x1+1,TABLA_COORD[x_initial][y_initial].y2-1);
        line(TABLA_COORD[x_initial][y_initial].x1-1,TABLA_COORD[x_initial][y_initial].y1+1,TABLA_COORD[x_initial][y_initial].x1-1,TABLA_COORD[x_initial][y_initial].y2-1);
        return 1;
    }
    if(y_initial==y_final && (codPiesa%7)%2==1)
    {
        if(TABLA[x_initial][y_initial]==8)
            x_initial--;
        if(TABLA[x_final+1][y_initial]==-1)
            return 0;
        if(TABLA[x_initial][y_initial]==TABLA[x_final][y_initial])
            return 0;
        if(x_initial<x_final)
        {
            for(int i=x_initial+2; i<=x_final+1; i++)
                if(TABLA[i][y_initial]!=0  || (TABLA[i][y_initial]==0 && MATRICE_GROPI[i][y_initial]==1))
                    return 0;
        }
        else
        {
            for(int i=x_initial-1; i>=x_final; i--)
                if(TABLA[i][y_initial]!=0 || (TABLA[i][y_initial]==0 && MATRICE_GROPI[i][y_initial]==1))
                    return 0;
        }
        codPiesa=TABLA[x_initial][y_initial];
        TABLA[x_initial][y_initial]=0;
        TABLA[x_initial+1][y_initial]=0;
        TABLA[x_final][y_final]=3;
        TABLA[x_final+1][y_final]=8;
        stergePiesa(x_initial,y_initial);
        stergePiesa(x_initial+1,y_initial);

        setcolor(VERDE_DESCHIS);
        line(TABLA_COORD[x_initial][y_initial].x1+1,TABLA_COORD[x_initial][y_initial].y2,TABLA_COORD[x_initial][y_initial].x2-1,TABLA_COORD[x_initial][y_initial].y2);
        line(TABLA_COORD[x_initial][y_initial].x1+1,TABLA_COORD[x_initial][y_initial].y2-1,TABLA_COORD[x_initial][y_initial].x2-1,TABLA_COORD[x_initial][y_initial].y2-1);
        line(TABLA_COORD[x_initial][y_initial].x1+1,TABLA_COORD[x_initial][y_initial].y2+1,TABLA_COORD[x_initial][y_initial].x2-1,TABLA_COORD[x_initial][y_initial].y2+1);
        return 1;
    }
    return 0;
}
void afisare_tabla()
{
    for(int i=0; i<=6; i++)
    {
        for(int j=0; j<=6; j++)
            cout<<TABLA[i][j]<<" ";
        cout<<endl;
    }
}
void incepere_nivel_random()
{
    deseneazaTabla();
    adauga_personaje();
}


