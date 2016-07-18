#include <winbgim.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define REN 30 /*Renglones de la malla */
#define COL 55 /*Columnas de la malla*/
#define TAM_NODO 15 /*Tamaño que tendrá el nodo*/
#define PTLLAX 1200 /*Tamaño de la pantalla en x*/
#define PTLLAY 635 /*Tamaño de la pantalla en y*/
#define ESQY 30
#define ESQX 30
#define POSX 30
#define POSY 150
#define POSFX 320
#define POSFY 190
#define INTER 40
#define CENT 10
#define BOTONES 4

//-lbgi -lgdi32 -luser32

typedef struct nodo
{      
        int xi, yi, xf, yf;
        int estado; /*Indica si se encuentra disponible o esta ocupado*/
        int id;
        struct nodo *up, *down, *left, *right;
}MALLA;

typedef struct
{
  int puntos;
  char name[20];
}RECORDS;

typedef struct
{
        int color;
        int existe;
}IMAGEN;

typedef struct
{
        IMAGEN **imagen;
        int dimx, dimy;
}IMAGENES;

typedef struct ataque
{
        int indice;
        int direccion;
        MALLA *posicion;
        struct ataque *ant,*sig;
}ATAQUE;

typedef struct personaje
{
        int indice;
        int vidas;
        ATAQUE *ataque;
        MALLA *posicion; /*Posicion que ocupa en la malla*/
        personaje *ant,*sig;
}PERSONAJE;

typedef struct nodoM
{
        char nom[40];
        int x,y,posx,posy,num;
        struct nodoM *sig;
}MENU;

void portada(int, int, int, int);
int leerNombre(int, int, RECORDS * );
void programa(RECORDS *);
int juego(RECORDS *, int, int, char *);
void perdiste(RECORDS *,char *, int, char *);

int crearMalla(MALLA **);
MALLA *insertarInicio(MALLA **, int, int, int, int);
MALLA *crearNodo(int, int, int, int);
void imprimeMalla(MALLA *);
void liberarNodo(MALLA **);
void liberarMalla(MALLA **);

MENU *crea_NodoM(char *,int,int,int,int,int);
int insertBotonM(MENU **,char *,int,int,int,int,int);
int creaMenu(MENU **);
void dibujaMenu(MENU *);
int liberaMenu(MENU **);

IMAGENES *cargarArchivosImagenes(char *);  
int abrirImagen(IMAGENES *, char *);
IMAGEN **crearImagen(int dimx, int dimy);
void liberarImagenes(IMAGENES *);

void ordenaBurbuja( RECORDS *, int );
void leerRecords();
int nJugadores();
void imprimeRecords( RECORDS *a, int nJ );
void guardaRecords( RECORDS );
void leerArchivoText( char *, int , int  , int);
void lee_texto(int,int, char *);

void moverPosicionImagen(MALLA **, int, int);
int cargarEnemigo(PERSONAJE **, MALLA *, int);
void posicionarJugador(PERSONAJE **, MALLA *, int, int, int);
void visualizaJuego(IMAGENES *, PERSONAJE *, PERSONAJE *, ATAQUE *, PERSONAJE *, PERSONAJE *);
void ambienteLV1(RECORDS *, int, int);
int dispararAtaqueHeroe(PERSONAJE **, int, int);
int insertarAtaque(MALLA *, ATAQUE **, int, int);
ATAQUE *crearAtaque(MALLA *, int, int);
void moverAtaqueA(PERSONAJE *, PERSONAJE **, int *);
void moverAtaqueN(PERSONAJE *, PERSONAJE **, int *);
int insertarEnemigo(MALLA *posicion, PERSONAJE **enemigos, int direccion, int indice, int vidas, int n);
PERSONAJE *crearEnemigo(MALLA *posicion, int direccion, int indice, int vidas);
void AutomatizarAlien(PERSONAJE *, PERSONAJE *);
void reduceVida(int,PERSONAJE **);
void Nivel(int);
int elimAlien(PERSONAJE **,int );
int elimIniAtaque(ATAQUE **);
void visualizaImagen(MALLA *, IMAGENES *, int);
void liberaPersonaje(PERSONAJE **);
void liberaAtaque(ATAQUE **);

void limpiaPantalla();
void limpiaPag();
void waitLMC();


int main()
{
    RECORDS rec;
    // delete these lines and replace them with your own code:
    
    
    initwindow(PTLLAX,PTLLAY);
    setwindowtitle("MEXICA");
    
    strcpy( rec.name, "" );
    rec.puntos = 0;
    //juego(&rec,2,1);
    /*strcpy(cad2,"Ganaste!");
    perdiste(&rec,"hola",10,cad2);*/
    portada(350,100,4,3);
    setcolor(RED);
    outtextxy(400,500,"CLICK PARA CONTINUAR");  
    setcolor(WHITE);
    waitLMC();
    clearmouseclick(WM_LBUTTONDOWN);
    
    programa(&rec); 
    //perdiste(&rec,"tiempo");
    //getch();
    // while(!kbhit());     // wait for user to press a key
    //waitLMC(); // use one or the other of these--not both


    closegraph();        //close graphics window
	return 0;
}
void portada(int x, int y, int T, int t)
{
     //int x=350,y=100;
     
     limpiaPantalla();
     rectangle(POSX,POSX,getmaxx()-POSX,getmaxy()-38);
     settextstyle(3,0,T);
     setcolor(WHITE);
     outtextxy(x-100,y,"UNIVERSIDAD AUTONOMA DE SAN LUIS POTOSI");
     settextstyle(3,0,t);
     outtextxy(x,y+100,"Nombre: Eduardo Montes Hernandez");
     outtextxy(x,y+140,"Carrera: Ingeniero en Informatica");
     outtextxy(x,y+180,"Materia: Estructuras de Datos y Algoritmos B");
     outtextxy(x,y+220,"Profesor: Raymundo Antonio Gonzalez Grimaldo");
     outtextxy(x,y+260,"Semestre: 2011-2012/I");    
}   
void Nivel(int n)
{    
     char cad[20];
     
     cleardevice();
     settextstyle(8, 0, 6);
     setbkcolor(BLACK);
     sprintf(cad,"PREPARATE! NIVEL %d",n);
     outtextxy(getmaxx()/2-300, getmaxy()/2-50,cad);
     PlaySound(("nivel.wav"),NULL,0);
}          
int creaMenu(MENU **M)
{
     int res=1;
     
     if(insertBotonM(M,"IR AL JUEGO",POSX,POSY,POSFX,POSFY,0))
     {
                                 res+=insertBotonM(M,"AYUDA",POSX,POSY+INTER*res,POSFX,POSFY+INTER*res,res);
                                 res+=insertBotonM(M,"RECORDS",POSX,POSY+INTER*res,POSFX,POSFY+INTER*res,res);
                                 res+=insertBotonM(M,"ACERCA DEL AUTOR",POSX,POSY+INTER*res,POSFX,POSFY+INTER*res,res);
                                 res+=insertBotonM(M,"SALIR",POSX,POSY+INTER*res,POSFX,POSFY+INTER*res,res);
                                 
     }
     return(res);

}
MENU *crea_nodoM(char *nom, int x,int y,int xf,int yf,int num)
{
    MENU *n=(MENU *)malloc(sizeof(MENU));
    if(n)
    {
         n->x=x;
         n->y=y;
         n->posx=xf;
         n->posy=yf;
         n->num=num;
         strcpy(n->nom,nom);
         n->sig=NULL;
         return(n);
    }
    else
    return(NULL);
}
int insertBotonM(MENU **nodo, char *nom, int x,int y,int xf,int yf,int num)
{
    MENU *p;
    int res=0;
    if(p=crea_nodoM(nom,x,y,xf,yf,num))
    {/*
          if(!*nodo)
          {         
                    *nodo=p;
                    res=1;
          }
          else
          res=insertBotonM(&(*nodo)->sig,nom,x,y,xf,yf,num);
    }
    return(res);*/
         if(*nodo)
         {
                        p->sig=*nodo;
                        *nodo=p;
         }
         else
         *nodo=p;
    return(1);
    }
    return(0);
}
int liberaMenu(MENU **M)
{
    int res=0;
    if(*M)
    {     
          while(*M)
          {
             free(*M);
             *M=(*M)->sig;
          }
          res=1;
    }
}
void dibujaMenu(MENU *M)
{
     cleardevice();
     PlaySound(("Menu.wav"),NULL,SND_ASYNC);
     settextstyle(8,0,9);
     setbkcolor(BLACK);
     outtextxy(POSX+400,POSX+10,"MEXICA");
     rectangle(POSX,POSX,getmaxx()-POSX,getmaxy()-38);
     line(POSX,POSY,getmaxx()-POSX,POSY);
     settextstyle(8,HORIZ_DIR,1);
     setbkcolor(RED);
     setfillstyle(1,RED);
     while(M)
     {
               bar(M->x,M->y,M->posx,M->posy);
               outtextxy(M->x+CENT,M->y+CENT,M->nom);
               rectangle(M->x,M->y,M->posx,M->posy);
               M=M->sig;
     }
     setbkcolor(BLACK);
}
void programa(RECORDS *rec)
{
     
     int X=0,Y=0,page,band=1,i,gana=1;
     MENU *M=NULL,aux;
     char cad[20],cad2[20];
     
     cleardevice();
     creaMenu(&M);
     dibujaMenu(M);
     leerArchivoText("ayuda.txt", POSFX+100 , POSY-30  , 1);
          
  do{                                               
     X=mousex();
     Y=mousey();
     /*if(page)
     page=0;
     else
     page=1;
     setactivepage(page);
     cleardevice();*/

     //printf("%d\n",band2);
     if(band==2)
     { 
        setlinestyle(0,0,0);
        dibujaMenu(M);
        leerArchivoText("ayuda.txt", POSFX+100 , POSY-30  , 1);
        band=1;
     }
          if(X>POSX&&X<=POSFX&&Y>POSY&&Y<POSFY)
          {
             
             setcolor(BLUE);
             rectangle(POSX,POSY,POSFX,POSFY);
             setcolor(WHITE);
             if(ismouseclick(WM_LBUTTONDOWN))
            {
                                          if(leerNombre(POSFX+30,POSFY,rec))
                                          {
                                             for(i=1;i<6&&gana;i++)
                                             {
                                                limpiaPag();
                                                Nivel(i);
                                                gana=juego(rec,i+1,i/2,cad);
                                             }
                                             
                                             if(gana)
                                             {
                                                PlaySound(("gana.wav"),NULL,SND_ASYNC);
                                                guardaRecords(*rec);
                                                strcpy(cad2,"¡GANASTE!");
                                                perdiste(rec,cad,i-1,cad2);
                                             }
                                             else
                                             {
                                                 
                                                 PlaySound(("Game Over.wav"),NULL,SND_ASYNC);
                                                 strcpy(cad2,"¡PERDISTE!");
                                                 perdiste(rec,cad,i-1,cad2);
                                                 gana=1;
                                             }
                                                
                                             limpiaPag();
                                             band=2;
                                          }
            }
          }
          else
             rectangle(POSX,POSY,POSFX,POSFY);
          if(X>POSX&&X<=POSFX&&Y>POSFY&&Y<POSFY+INTER)
          {
             setcolor(BLUE);
             rectangle(POSX,POSY+INTER,POSFX,POSFY+INTER);
             setcolor(WHITE);
             if(ismouseclick(WM_LBUTTONDOWN))
            {
                 leerArchivoText("ayuda.txt", POSFX+100 , POSY-30  , 1);
            }
             
          }
          else
             rectangle(POSX,POSY+INTER,POSFX,POSFY+INTER);
          if(X>POSX&&X<=POSFX&&Y>POSFY+INTER&&Y<POSFY+INTER*2)
          {
             setcolor(BLUE);
             rectangle(POSX,POSY+INTER*2,POSFX,POSFY+INTER*2);
             setcolor(WHITE);
             if(ismouseclick(WM_LBUTTONDOWN))
            {
                 leerRecords();
            }
             
          }
          else
             rectangle(POSX,POSY+INTER*2,POSFX,POSFY+INTER*2); 
          if(X>POSX&&X<=POSFX&&Y>POSFY+INTER*2&&Y<POSFY+INTER*3)
          {
             setcolor(BLUE);
             rectangle(POSX,POSY+INTER*3,POSFX,POSFY+INTER*3);
             setcolor(WHITE);
             if(ismouseclick(WM_LBUTTONDOWN))
            {
                                          portada(POSFX+200,POSY+20,3,2);
            }
          }
          else
          rectangle(POSX,POSY+INTER*3,POSFX,POSFY+INTER*3);        
    /* setvisualpage(page);
     setbkcolor(BLACK);*/
     /*setfillstyle(1,BLACK);
     bar(POSX,POSX,getmaxx()-POSX,POSY);*/
     if(X>=POSX&&X<=POSFX&&Y>=POSY+INTER*BOTONES&&Y<=POSFY+INTER*BOTONES)
     {
            setcolor(BLUE);
            rectangle(POSX,POSY+INTER*BOTONES,POSFX,POSFY+INTER*BOTONES);
            setcolor(WHITE);
            if(ismouseclick(WM_LBUTTONDOWN))
            {
                                          band=0;
                                          limpiaPag();
            }
     }
     else
     rectangle(POSX,POSY+INTER*BOTONES,POSFX,POSFY+INTER*BOTONES);

     clearmouseclick(WM_LBUTTONDOWN);
     delay(10);
     }while(band);
     liberaMenu(&M);        
}
void limpiaPag()
{
       
     setvisualpage(1);
     setactivepage(1);
     setbkcolor(BLACK);
     cleardevice();
     setvisualpage(0);
     setactivepage(0);
     setbkcolor(BLACK);
     cleardevice(); 
}
int juego(RECORDS *rec,int nA, int nN, char *cad)
{
    MALLA *malla = NULL;
    IMAGENES *imagenes;
    PERSONAJE *heroe = NULL, *alien=NULL, *auxAlien=NULL,*nave=NULL,*auxNave=NULL,*lanza=NULL;
    ATAQUE *proyEnemigos = NULL;
    int n=2, direccion, a,i=0,j,page, lanzas=20,naves,r,gana=1;
    long d;
    time_t ha,hi;
    char tecla,tecla2;
    
    fflush(stdin);
    crearMalla(&malla);
    imagenes = cargarArchivosImagenes("imagenes.txt");
    //cargarEnemigo(&alien,malla,2);
    //cargarJugador(&heroe, malla, "heroe");
    insertarEnemigo(malla,&heroe,0,0,10,1);
    insertarEnemigo(malla,&alien,2,10,nA*2,nA);
    if(nN)
    insertarEnemigo(malla,&nave,2,16,nN*3,nA);
    
    posicionarJugador(&heroe,malla,REN-1,COL/2,2);
    
    for(auxAlien=alien,i=3;auxAlien;auxAlien=auxAlien->sig,i++)
    {
       r=rand()%COL-1;
       posicionarJugador(&auxAlien,malla,REN-1,r,i);
       alien->indice=8;
    }
    
    for(auxNave=nave;auxNave;auxNave=auxNave->sig,i++)
    {
       r=rand()%COL;
       posicionarJugador(&auxNave,malla,1,r,i);
    }
    //posicionarJugador(&(alien)->sig,malla,REN-1,COL-10,3); 

    alien->indice=8;
    //liberarMalla(&malla);

     time(&hi);
     srand(time(NULL));
     
    do{    
        
      if(page == 0)
        page = 1;
      else
        page = 0; 
      
      setactivepage(page);
      cleardevice(); 
      ambienteLV1(rec,lanzas,heroe->vidas);
      outtextxy(ESQX+COL*TAM_NODO+90+CENT,(ESQY+REN*TAM_NODO+100)/5*4+CENT,"Tiempo");
        time(&ha);
	    d=(long)difftime(ha, hi);
	    sprintf(cad,"%02d:%02d:%02d", d/3600, d%3600/60, d%3600%60);
      outtextxy(ESQX+COL*TAM_NODO+90+CENT,(ESQY+REN*TAM_NODO+100)/5*4+CENT*5,cad);
      
      /*if(alien)
      sprintf(cad,"%d",alien->vidas);
      outtextxy(200,200,cad);
      
      if(nave)
      sprintf(cad,"%d",nave->vidas);
      outtextxy(300,200,cad);*/
      
      
      //imprimeMalla(malla);
      
      
      
      
       
      if(kbhit())
      { 
                 
         
        

        tecla2=tecla = getch(); 
        /*if(tecla == 0) 
        tecla = getch();
        if(tecla == 27) 
          return(0);
        else*/
          switch(tecla)
          {
            
        
            case KEY_UP:
                        if(heroe->indice==0||heroe->indice==1||heroe->indice==2||heroe->indice==4||heroe->indice==5||heroe->indice==6)
                         heroe->indice=17;
                         else
                         if(heroe->indice==3)
                         heroe->indice=0;
                         else
                         if(heroe->indice==7)
                         heroe->indice=4;
                         //moverPosicionImagen(&(heroe)->posicion, 1); 
                         break;
          
            case KEY_DOWN: 
                         if(heroe->indice==0||heroe->indice==1||heroe->indice==2)
                         heroe->indice=3;
                         else
                         if(heroe->indice==4||heroe->indice==5||heroe->indice==6)
                         heroe->indice=7;
                         moverPosicionImagen(&(heroe)->posicion, 2,2); 
                         break;
           
            case KEY_LEFT: 
                           
                         if(heroe->indice==3)
                         heroe->indice=7;
                         else
                         if(heroe->indice==4)
                         heroe->indice=5;
                         else
                         heroe->indice=4;
                         
                         for(auxAlien=alien;auxAlien;auxAlien=auxAlien->sig)
                         auxAlien->indice=8; 
                         
                         if(heroe->posicion->left)
                            if(heroe->posicion->left->id==50)
                               {
                                  lanzas=20;
                                  elimAlien(&lanza,50);
                                  PlaySound(("item.wav"),NULL,SND_ASYNC);
                               }
                               
                         moverPosicionImagen(&(heroe)->posicion, 3,2); 
                         
                         //moverPosicionImagen(&(alien)->posicion, 4,3);
                         break;
          
            case KEY_RIGHT: 
                         if(heroe->indice==7)
                         heroe->indice=3;
                         else
                         if(heroe->indice==0)
                         heroe->indice=1;
                         else
                         heroe->indice=0;  
                         
                         for(auxAlien=alien;auxAlien;auxAlien=auxAlien->sig)
                         auxAlien->indice=10;    
                         
                         if(heroe->posicion->right)
                            if(heroe->posicion->right->id==50)
                            {
                               lanzas=20;
                               elimAlien(&lanza,50);
                               PlaySound(("item.wav"),NULL,SND_ASYNC);
                            }
                               
                         moverPosicionImagen(&(heroe)->posicion, 4,2); 
                         //moverPosicionImagen(&(alien)->posicion, 3,3); 
                         break;
            
            case 'z':
            case 'Z':
                      if(lanzas>0)
                      {
                         if(heroe->indice==0||heroe->indice==1||heroe->indice==3||heroe->indice==2)  
                         {
                            heroe->indice=2;
                            direccion=3;
                            dispararAtaqueHeroe(&heroe,direccion,12);
                            
                         }
                         else
                         if(heroe->indice==4||heroe->indice==5||heroe->indice==7||heroe->indice==6)  
                         {
                            heroe->indice=6;
                            direccion=2;
                            dispararAtaqueHeroe(&heroe,direccion,13);
                         }
                         else
                         if(heroe->indice==17||heroe->indice==19)
                         {
                            heroe->indice=19;
                            dispararAtaqueHeroe(&heroe,0,18);
                         }
                         lanzas--;
                      } 
                         break;

                     
          }
          
      }
      if(d%10==0)
      {
         if(!lanza)
         {
            insertarEnemigo(malla,&lanza,0,20,0,1);
            r=rand()%COL-1;
            posicionarJugador(&lanza,malla,REN-1,r,50);
         }
      }
         
      //srand(time(NULL));
      /*if(alien);
      {printf("\nsillega");elimAlien(&alien);
      }*/
      AutomatizarAlien(alien,nave);
      
      if(lanza)
      {
         lanza->posicion->id=50;
         if(lanza->posicion->left)
            lanza->posicion->left->id=50;
 
      }
             
      
      
      heroe->posicion->id=2;
      
      moverAtaqueN(heroe,&nave,&(rec)->puntos);
      moverAtaqueN(nave,&heroe,&(rec)->puntos);
      moverAtaqueA(heroe,&alien,&(rec)->puntos);
      moverAtaqueA(alien,&heroe,&(rec)->puntos);
      
      
      visualizaJuego(imagenes, heroe, alien, heroe->ataque,nave,lanza);
      setvisualpage(page);
      
            //delay(100);
    }while((alien||nave)&&heroe->vidas); 
    
    if(!heroe->vidas)
    {
       gana=0;
       liberaPersonaje(&alien);
       liberaPersonaje(&heroe);
       liberaPersonaje(&nave);
       PlaySound("link8.wav",0,0);
    }
    
    liberarImagenes(imagenes);
    liberarMalla(&malla);
    //limpiaPag();
    
    return(gana);
}
void waitLMC()
{
    clearmouseclick(WM_LBUTTONDOWN);
    const int DELAY = 50; // Milliseconds of delay between checks
    int x, y;
    while (!ismouseclick(WM_LBUTTONDOWN))
        delay(DELAY);
    getmouseclick(WM_LBUTTONDOWN, x, y);
}
int crearMalla(MALLA **lM)
{
    int i, j;
    int contA = 0, contB = 0;
    int xi, yi, xf, yf, xAux, yAux;
    MALLA **auxA, *auxB, *auxC;
    xi = ESQX; yi = ESQY; xf = xi + COL * TAM_NODO; yf = yi + REN * TAM_NODO;
    
    auxA = (MALLA **)malloc(sizeof(MALLA *) * REN);
    if(auxA == NULL) return(0);
    for(i = 0, yAux = yf; i < REN; i++, yAux -= TAM_NODO)
      for(j = 0, xAux = xf ; j < COL; j++, xAux -= TAM_NODO)
      {
        auxB = insertarInicio(&(*lM), xAux-TAM_NODO, yAux-TAM_NODO, xAux, yAux);
        if(auxB != NULL) contA++;
        else return 0;
        if(j == 0) *(auxA + contB++) = *lM;
      }
      i=0;
    while(i<contB-1)//for(i = 0; i < contB-1; i++)
    {
      auxB = auxA[i]; auxC = auxA[i+1];
      j=0;
      while(j<COL)//for(j = 0; j < COL; j++)
      {
        auxB->up = auxC; auxC->down = auxB;
        if(auxC->left != NULL)
        {
          auxB = auxB->left; auxC = auxC->left;
        }
        j++;
      }
      auxA[i+1]->right->left = NULL; auxA[i+1]->right = NULL;
    i++;
    }
    return(1);   
}

MALLA *insertarInicio(MALLA **lM, int xi, int yi, int xf, int yf)
{
      MALLA *nuevo;
      
      nuevo = crearNodo(xi, yi, xf, yf);
      if(nuevo == NULL)
        return(nuevo);
      if(*lM == NULL)
        *lM = nuevo;
      else
      {
        nuevo->right = *lM;
        nuevo->right->left = nuevo;
        *lM = nuevo;
      }
      return(nuevo);
}

MALLA *crearNodo(int xi, int yi, int xf, int yf)
{
      MALLA *aux = NULL;
      
      aux = (MALLA *)malloc(sizeof(MALLA));
      if(aux == NULL) return (aux);
      aux->xi = xi;
      aux->xf = xf;
      aux->yi = yi;
      aux->yf = yf;
      aux->estado = 0;
      aux->id = 0;
      //aux->enemigos = NULL;
      aux->up = aux->down = aux->left = aux->right = NULL;
      return(aux);
}

void liberarMalla(MALLA **malla)
{
     int i, j;
     MALLA *aux, *aux2;
     
     for( ; *malla; *malla = aux2)
     {
         aux2 = (*malla)->down;
         liberarNodo(malla);
     }
}
    
void liberarNodo(MALLA **malla)
{
     if((*malla)->right)
     {
       liberarNodo(&(*malla)->right);
       free(*malla);
     }
     else
       free(*malla);
} 

void imprimeMalla(MALLA *lM)
{
     MALLA *aux,*aux2;      
     aux = lM;
     for(aux2 = aux; aux2; aux2 = aux2->down)
     { printf("\n");
       for(aux = aux2; aux ;aux = aux->right)
       {
         printf("%d",aux->id);
       }
     }
}
int abrirImagen(IMAGENES *imagenes, char *nombre)
{
    FILE *f;
    int i, j;
    
    f = fopen(nombre, "rb");
    if(!f) return 0;
    
    
    fread(&imagenes->dimx, sizeof(int), 1, f);
    fread(&imagenes->dimy, sizeof(int), 1, f);
    imagenes->imagen = crearImagen(imagenes->dimx, imagenes->dimy);
    if(!imagenes->imagen)
     return 0;
    
    for(i = 0; i < imagenes->dimx; i++)
      for(j = 0; j < imagenes->dimy; j++)
      {
        fread(&imagenes->imagen[i][j].color, sizeof(int), 1, f);
        fread(&imagenes->imagen[i][j].existe, sizeof(int), 1, f);
      }
    /*
    fread(&imagenes->dimImg, sizeof(long), 1, f);
    imagenes->image = malloc(imagenes->dimImg);
    fread(imagenes->image, imagenes->dimImg, 1, f);
    */
    fclose(f);
    return 1;
}
IMAGENES *cargarArchivosImagenes(char *name)
{
         IMAGENES *imagenes = NULL;
         FILE *f, *g;
         int i, j, n, ret = 1;
         char nombre[50];
          
         f = fopen(name, "r");
         if(!f) return imagenes;
     
         fscanf(f, " %d ", &n);
         imagenes = (IMAGENES *)malloc(sizeof(IMAGENES) * n);
         if(!imagenes) return 0;
         for(i = 0; i < n && ret; i++)
         {
           fscanf(f, " %s ", nombre);
           ret = abrirImagen(&imagenes[i], nombre);
         }
     
         fclose(f);
         
         return imagenes;
} 
IMAGEN **crearImagen(int dimx, int dimy)
{
     IMAGEN **arr=NULL;
     int i;
     
     arr = (IMAGEN **)malloc(sizeof(IMAGEN *) * dimx);
     for(i = 0; i < dimx; i++)
       *(arr+i) = (IMAGEN *)malloc(sizeof(IMAGEN) * dimy);
     return arr;
}
void visualizaImagen(MALLA *posicion, IMAGENES *imagenes, int indice)
{
     int i, j;
     
     for(i = 0; i < imagenes[indice].dimx; i++)
       for(j = 0; j < imagenes[indice].dimy; j++)
         if(imagenes[indice].imagen[i][j].existe)
           putpixel(posicion->xi + i, posicion->yi + j, imagenes[indice].imagen[i][j].color);
    
    
    //putimage(posicion->xi, posicion->yi, imagenes[indice].image, 2)
}

void liberarImagenes(IMAGENES *imagenes)
{
     int i;
     
     for(i = 0; i < 14; i++)
       free(imagenes[i].imagen);
     free(imagenes);
}
void moverPosicionImagen(MALLA **posicion, int dir, int i)
{
     
     switch(dir)
     {
       case 1: if((*posicion)->up)
               {               
                   *posicion = (*posicion)->up;
                   (*posicion)->id=i;
                   (*posicion)->down->id=0;
                   if((*posicion)->down->left)
                      (*posicion)->down->left->id=0;
               }
               break;
               
       case 2: if((*posicion)->down)
               {
                   *posicion = (*posicion)->down;
                   (*posicion)->id=i;
                   (*posicion)->up->id=0;
                   if((*posicion)->up->left)
                      (*posicion)->up->left->id=0;
               }
               break;
               
       case 3: if((*posicion)->left)
               {
                   *posicion = (*posicion)->left;
                   (*posicion)->id=i;
                   (*posicion)->right->id=0;
                   if((*posicion)->right->left)
                      (*posicion)->right->left->id=0;
               }
               break; 
       
       case 4: if((*posicion)->right)
               {
                   *posicion = (*posicion)->right;
                   (*posicion)->id=i;
                   (*posicion)->left->id=0;
                   if((*posicion)->left->left)
                      (*posicion)->left->left->id=0;
               }
               break;
     }
}
void posicionarJugador(PERSONAJE **heroe, MALLA *malla, int x, int y, int id)
{
     int i;
    (*heroe)->posicion = malla;
    for(i = 0; i < x; i++)
      moverPosicionImagen(&(*heroe)->posicion, 2,id);
    for(i = 0; i < y; i++)
      moverPosicionImagen(&(*heroe)->posicion, 4,id);
    (*heroe)->posicion->id=id;
}
int cargarJugador(PERSONAJE **heroe, MALLA *malla, char *nombre)
{
    int i, j, tamCad;
    
    *heroe = (PERSONAJE *)malloc(sizeof(PERSONAJE));
    if(!*heroe) return 0;
    (*heroe)->indice = 0;
    (*heroe)->vidas = 3;
    (*heroe)->sig=NULL;
    (*heroe)->ataque = NULL;
    return 1;
}
void visualizaJuego(IMAGENES *imagenes, PERSONAJE *heroe, PERSONAJE *alien, ATAQUE *a, PERSONAJE *nave, PERSONAJE *lanza)
{
     PERSONAJE *aux;
     ATAQUE *auxA;

     
     for(;nave;nave=nave->sig)
     {     
        for(auxA=nave->ataque;auxA;auxA=auxA->sig)
           visualizaImagen(auxA->posicion, imagenes, auxA->indice);
        
        visualizaImagen(nave->posicion, imagenes, nave->indice);
     }

     for(auxA=heroe->ataque;auxA;auxA=auxA->sig)
     visualizaImagen(auxA->posicion, imagenes, auxA->indice);
     
     
     for(;alien;alien=alien->sig)
     {     
        for(auxA=alien->ataque;auxA;auxA=auxA->sig)
           visualizaImagen(auxA->posicion, imagenes, auxA->indice);
        
        visualizaImagen(alien->posicion, imagenes, alien->indice);
     }
    
     if(lanza)
     visualizaImagen(lanza->posicion, imagenes, lanza->indice);
    
     visualizaImagen(heroe->posicion, imagenes, heroe->indice);

}
/*int cargarEnemigo(PERSONAJE **alien, MALLA *malla, int n)
{
    int i, j;
    
    for(i=0;i<n;i++)
    {
    *alien = (PERSONAJE *)malloc(sizeof(PERSONAJE));
    if(!*alien) return 0;
    (*alien)->indice = 8;
    (*alien)->vidas = 3;
    (*alien)->sig=NULL;
    (*alien)->ataque = NULL;
    }
    return 1;
}*/
void ambienteLV1(RECORDS *rec, int lanzas, int vidas)
{
     char cad[30];
     
     
     
     setlinestyle(0,0,4);
     rectangle(ESQX,ESQY,ESQX+COL*TAM_NODO+70,ESQY+REN*TAM_NODO+100);
     setcolor(YELLOW);
     rectangle(ESQX+COL*TAM_NODO+90,ESQY,ESQX+COL*TAM_NODO+270,ESQY+REN*TAM_NODO+100);
     line(ESQX+COL*TAM_NODO+90,(ESQY+REN*TAM_NODO+100)/5,ESQX+COL*TAM_NODO+270,(ESQY+REN*TAM_NODO+100)/5);
     line(ESQX+COL*TAM_NODO+90,(ESQY+REN*TAM_NODO+100)/5*2,ESQX+COL*TAM_NODO+270,(ESQY+REN*TAM_NODO+100)/5*2);
     line(ESQX+COL*TAM_NODO+90,(ESQY+REN*TAM_NODO+100)/5*3,ESQX+COL*TAM_NODO+270,(ESQY+REN*TAM_NODO+100)/5*3);
     line(ESQX+COL*TAM_NODO+90,(ESQY+REN*TAM_NODO+100)/5*4,ESQX+COL*TAM_NODO+270,(ESQY+REN*TAM_NODO+100)/5*4);
     
     settextstyle(3,0,1);
     outtextxy(ESQX+COL*TAM_NODO+90+CENT,ESQY+CENT*3,rec->name);
     
     outtextxy(ESQX+COL*TAM_NODO+90+CENT,(ESQY+REN*TAM_NODO+100)/5+CENT,"Vidas");
     sprintf(cad,"%d",vidas);
     outtextxy(ESQX+COL*TAM_NODO+90+CENT*8,(ESQY+REN*TAM_NODO+100)/5+CENT*5,cad);
     
     outtextxy(ESQX+COL*TAM_NODO+90+CENT,(ESQY+REN*TAM_NODO+100)/5*2+CENT,"Lanzas");
     sprintf(cad,"%d",lanzas);
     outtextxy(ESQX+COL*TAM_NODO+90+CENT*8,(ESQY+REN*TAM_NODO+100)/5*2+CENT*5,cad);
     
     outtextxy(ESQX+COL*TAM_NODO+90+CENT,(ESQY+REN*TAM_NODO+100)/5*3+CENT,"Puntos");
     sprintf(cad,"%d",rec->puntos);
     outtextxy(ESQX+COL*TAM_NODO+90+CENT*8,(ESQY+REN*TAM_NODO+100)/5*3+CENT*5,cad);
     
     
     //line(ESQX+COL*TAM_NODO+90,(ESQY+REN*TAM_NODO+100),ESQX+COL*TAM_NODO+270,(ESQY+REN*TAM_NODO+100));
     setfillstyle(1,GREEN);
     bar(ESQX,ESQY+REN*TAM_NODO+85,ESQX+COL*TAM_NODO+70,ESQY+REN*TAM_NODO+100);
     setcolor(WHITE);
}
void leerArchivoText( char *name, int x, int y , int num)
{
  
  limpiaPantalla();
  
  FILE *ptrA;
  char cadena[900];
  int x1, y1, x2, y2 ;
  
  x1 = 250;
  y1 = 570;
  x2 = 300;
  y2 = 600;
  
  setcolor(WHITE ); 
  
  /*setlinestyle( SOLID_LINE,0, 1);
  setcolor( YELLOW);
  rectangle( 200, y +10, 800, y + 490 );*/
  
  ptrA = fopen(  name , "r" );
  
  if( ptrA != NULL )
  {
     while( !feof( ptrA ) )
     {
        fgets( cadena, 900 , ptrA );
         
        cadena[ strlen( cadena )  ] = '\0' ;
        setcolor( WHITE );
     
        settextstyle( 11 , 0 , 1 );
        outtextxy( x-10 , y + 40 , cadena );
        y += 16;
            
     } 
     fclose( ptrA );
  }else
       outtextxy( 300, 400, "Error" );
}
void leerRecords()
{
  RECORDS recAux;
  RECORDS *arrayRec;
  int  nJug, i  = 0;
  FILE *ptrArch;
  
  
  if( nJug = nJugadores() )// Se determina el numero de jugadores existentes en el archivo
  { 
      //Reservacion de espacio de memoria para el arreglo de tipo Records
      arrayRec = ( RECORDS*)malloc( sizeof( RECORDS ) * nJug -1 ); 
      ptrArch = fopen( "records.dat", "rb+" );
      
      // Se almacenan los datos del jugador en el arreglo de tipo Records arrayRec
      if( ptrArch != NULL )
      {
        while( !feof( ptrArch ))
        {      
         fread( &recAux, sizeof( RECORDS ), 1, ptrArch );
         
         strcpy( arrayRec[i].name , recAux.name );
         arrayRec[i].puntos = recAux.puntos;
         
         i++;
        }
      }else
         printf( "Error no se puede leer el archivo " );
  
      fclose( ptrArch );
      ordenaBurbuja( arrayRec , nJug -1 );// una ves que se capturarn los jugadores,se ordenan decrecientemente
   }
   
   imprimeRecords( arrayRec , nJug -1 );
   
   if( nJug )
       free( arrayRec );// Liberacion de memoria
}
/*
  Para saber el numero de jugadores; se abre un archivo binario ( records.dat )
  y se retorna los N jugadores, este valor retornado por la fucnion nos sera util para el momento de 
  imprimir el puntaje de los jugadores en la ventana de records*/
int nJugadores( )
{

   int nJugAux = 0;
   
   RECORDS recAux;
   FILE *ptrArch;
   
   if(( ptrArch = fopen( "records.dat", "rb" ) ) != NULL  )
       while( !feof( ptrArch ))
       { 
          fread( &recAux, sizeof( RECORDS ), 1, ptrArch );
          nJugAux++;
       }
   else
         printf( "Error no se puede leer el archivo " );
        
    fclose( ptrArch );
   
   return ( nJugAux );
   
}

// Esta funcion se encarga de limpiar la pantalla del menu, cada ves que se seleciona una opcion se ejecuta
void  limpiaPantalla()
{
   setfillstyle(SOLID_FILL, BLACK);
   bar(POSFX+1,POSY+1,getmaxx()-POSX-1,getmaxy()-39);
}
/*
 Esta funcion es para imprimir los records en la opcion del menu, recibe como parametros, un arreglo de tipo
 estructura RECORDS y el numero de jugadores */
void imprimeRecords( RECORDS *a, int nJ )
{
  
  limpiaPantalla();
  
  int x1,j, x2, y1, y2,x,y, corX,corY, bandera = -1;
  char cad[10];
  
  y = 150; 
  x = POSFX+100;
  
  setlinestyle( SOLID_LINE,0, 1);
  setcolor( YELLOW);
  //rectangle( 230, y, 770, y + 470 );
  //line(260, y + 90 , 740, y + 90 );
  
  setcolor( WHITE );
  settextstyle( 4, 0 , 3 );
  outtextxy( x + 30 , y + 30 , " Nombre             Puntos" );
  line(POSFX+350, y + 30 ,POSFX+350, y + 430);
  
  // Imprime el nombre y los puntos de cada jugador
  for( j = 0; j < nJ ; j++, y += 30  )
  {
    settextstyle( 4, 0 , 1 );
    outtextxy( x + 60 , y + 70 + 50  , a[j].name);
    
    sprintf(cad, "%d", a[j].puntos );
    
    if( a[j].puntos < 100 && a[j].puntos > 0)
         outtextxy( x + 335 , y + 70 + 50 , cad);
    else
        if( a[j].puntos == 0 )
             outtextxy( x + 351 , y + 70 +50 , cad);
        else
             outtextxy( x + 320 , y + 70 +50 , cad);
   }
  
}

/*
 La funcion se encarga de guardar la estructura Records en un archivo de tipo binario para posteriormente leerla y 
 vislumbrar el puntaje almacenado en el archivo, recibe como parametro la estructura Records */
void guardaRecords( RECORDS rec )
{
  FILE *ptrArch;
  
  ptrArch = fopen( "records.dat", "ab" );
  
  if( ptrArch != NULL )
      fwrite( &rec, sizeof( RECORDS ), 1 , ptrArch );
  else
      printf(" Error no se pudo guardar eñ archivo" );
  
  fclose( ptrArch );

}
/*Esta funcion es para ordenar el puntaje de los jugadores, se opera en el momento de imprimir los records
  recibe como parametros la estructura y el nuemro de jugadores */
void ordenaBurbuja( RECORDS *a , int nJ )
{
  int i , pas, bandera = 1;
  RECORDS aux;

  for( pas = 1; pas < nJ  && bandera == 1  ; pas ++ )
  { 
     bandera  = 0; 
      
    for( i = nJ -1; i > 0 ; i-- )
         if( a[ i ].puntos > a[ i - 1 ].puntos )
         { 
           aux.puntos  = a[ i ].puntos;
           a[ i ].puntos = a[ i - 1 ].puntos;
           a[ i - 1 ].puntos = aux.puntos;
           
           strcpy( aux.name ,  a[ i ].name );
           strcpy( a[ i ].name ,  a[ i - 1].name );
           strcpy( a[ i - 1].name ,  aux.name );
           
           bandera = 1;
         } 
  }
}
void lee_texto(int x,int y, char cadena[])
{
 char tecla, var;     
 int contador=0;
 cadena[0]='\0';
 
 do{
      //Si no se presiona una tecla, se imprimira un guion definidamente
      do{  //párpadeo
			 setcolor(15);
			 outtextxy(x+textwidth(cadena),y,"_       ");
			 delay(50);
			 setcolor(0);
			 outtextxy(x+textwidth(cadena),y,"_       ");
			 delay(50);
			 setcolor(15);
      }while(!kbhit());
     
      tecla = getch();
     
      if(tecla==0)  // si es código extendido
         tecla=getch();
        if(tecla==8 && contador > 0)  // se presion¢ <backspace>
			{
				 setcolor(BLACK);
	 			 outtextxy(x,y,cadena);  // se borra en la pantalla
				 cadena[--contador]='\0';  //se borr¢ £ltimo caracter del arreglo
				 setcolor( WHITE );
				 outtextxy(x,y,cadena);
			}
		 else
			{  
              if(tecla!=13)
              {  
				 cadena[contador++]=tecla;
				 cadena[contador]='\0';
				 outtextxy(x,y,cadena);
              }
            }    
  }while(tecla!=13); 
}
int leerNombre(int x, int y,RECORDS *r )
{
  limpiaPantalla();
  setlinestyle( SOLID_LINE,0, 2);
  char cadena[50];
  int corX, corY, bandera = -1;
  int band = 2;
  
  rectangle( x, y ,x+540, y + 340 );  
  settextstyle(11,0,2);
  outtextxy( x+100, y + 50 , " I N T R O D U Z C A   S U   N O M B R E " );  
  rectangle( x+90, y + 100 ,x+440, y + 160 );  
  lee_texto( x+115 ,y + 120, cadena );  
  rectangle(x+60, y + 210 ,x+210, y + 270 );
  outtextxy(x+80, y + 230 , " I R  A L   J U E G O" );  
  rectangle(x+330, y + 210 ,x+480, y + 270 );
  outtextxy(x+355, y + 230 , " C A N C E L A R " );
  
  do
  {
    corX = mousex();
    corY = mousey();
    
    if( corX > x+60 && corX < x+210 && corY > y + 210 && corY < y + 270 )
    {   
        setcolor( YELLOW );
        setlinestyle( 0, 0, 6 );       
        rectangle( x+60, y + 210 ,x+210, y + 270 );
          
         if( ismouseclick( WM_LBUTTONDOWN) )
             band = 1;
     }else
       {
          setcolor( BLACK );
          setlinestyle( 0, 0, 6 );       
          rectangle(x+60, y + 210 ,x+210, y + 270);   
          
          setlinestyle( 0, 0, 2 );  
          setcolor( WHITE );       
          rectangle(x+60, y + 210 ,x+210, y + 270);
          
          if( corX >x+330&& corX <x+480&& corY > y + 210 && corY < y + 270 )
          {   
               setcolor( YELLOW );       
               setlinestyle( 0, 0, 6 ); 
               rectangle(x+330, y + 210 ,x+480, y + 270 );
          
              if( ismouseclick( WM_LBUTTONDOWN) )
                   band = 0;
          }else
              {  
                 setcolor( BLACK );
                 setlinestyle( 0, 0, 6 );  
                 rectangle(x+330, y + 210 ,x+480, y + 270);
                  
                 setcolor( WHITE );       
                 setlinestyle( 0, 0, 2 );
                 rectangle(x+330, y + 210 ,x+480, y + 270);
              }
           }
  
   clearmouseclick(WM_LBUTTONDOWN );
   delay(1);  
  
  }while (band==2); 
  
  if( band == 1 )
  {
     strcpy( r -> name , cadena ); 
     delay( 1000 );
  }
  setcolor(WHITE);
  setlinestyle( 0, 0, 0 ); 
  limpiaPantalla();
  return( band );
}
int insertarEnemigo(MALLA *posicion, PERSONAJE **enemigos, int direccion, int indice, int vidas, int n)
{
    int res=0,i;
    PERSONAJE *nuevo = NULL;
    
    for(i=0;i<n;i++)
    {         
         nuevo = crearEnemigo(posicion, direccion, indice, vidas);
         if(!nuevo) return 0;
         if(*enemigos == NULL)
           *enemigos = nuevo;
         else
         {  
           nuevo->sig = *enemigos;
           *enemigos=nuevo;
         }
    }
    if(n==i)
    res=1;
         return res;
}

PERSONAJE *crearEnemigo(MALLA *posicion, int direccion, int indice, int vidas)
{
         PERSONAJE *aux = NULL;
         
         aux = (PERSONAJE *)malloc(sizeof(PERSONAJE));
         if(!aux) return aux;
         
         aux->posicion = posicion;
         aux->indice = indice;
         //aux->direccion = direccion;
         aux->ataque=NULL;
         aux->vidas = vidas;
         aux->sig = NULL; 
         return aux;
}
int dispararAtaqueHeroe(PERSONAJE **heroe, int direccion, int tipo)
{
   //  (*heroe)->indice = direccion;
     switch(direccion)
     {
       case 0: if((*heroe)->posicion->up)
               {
                 (*heroe)->posicion->up->id = 1;
                 if(!insertarAtaque((*heroe)->posicion->up, &(*heroe)->ataque, tipo, direccion)) return 0;
               }
               break;
        
       case 1: if((*heroe)->posicion->down)
               {
                 (*heroe)->posicion->down->id = 1;
                 if(!insertarAtaque((*heroe)->posicion->down, &(*heroe)->ataque, tipo, direccion)) return 0;
               }
               break;
               
       case 2: if((*heroe)->posicion->left)
               {
                 (*heroe)->posicion->left->id = 1;
                 if(!insertarAtaque((*heroe)->posicion->left, &(*heroe)->ataque, tipo, direccion)) return 0;
               }
               break;
       
       case 3: if((*heroe)->posicion->right)
               {
                 (*heroe)->posicion->right->id = 1; 
                 if(!insertarAtaque((*heroe)->posicion->right, &(*heroe)->ataque, tipo, direccion)) return 0;
               }
               break;
     }
     
}

int insertarAtaque(MALLA *posicion, ATAQUE **ataque, int indice, int direccion)
{
    ATAQUE *nuevo; 
    
    nuevo = crearAtaque(posicion, indice, direccion);
    if(!nuevo) return 0;
    if(*ataque == NULL)
      *ataque = nuevo;
    else
    {
      nuevo->sig = *ataque;
      *ataque = nuevo;
    } 
    
    return 1;
}

ATAQUE *crearAtaque(MALLA *posicion, int indice, int direccion)
{
            ATAQUE *aux = NULL;
            
            aux = (ATAQUE *)malloc(sizeof(ATAQUE));
            if(!aux) return aux;
            aux->posicion=posicion;
            aux->indice=indice;
            aux->direccion=direccion;
            aux->sig=NULL;
            return aux;
}
void moverAtaqueA(PERSONAJE *h, PERSONAJE **b, int *puntos)
{
   ATAQUE *a;
   while(h)
   {
     a=h->ataque;
     while(a)
     {
             switch(a->direccion)
             {                     
                case 2:
                     if(a->posicion->left)
                     {
                        if(a->posicion->left->id==2)
                        {
                           elimIniAtaque(&(h)->ataque);
                           if((*b)->indice!=3)
                           {
                              (*b)->vidas--;
                              PlaySound(("link7.wav"),NULL,SND_ASYNC);
                           }
                           else
                              PlaySound(("defensa.wav"),NULL,SND_ASYNC);
                           /*if(!b->vidas)
                           elimAlien(&b);*/
                        }
                        else
                        if(a->posicion->left->id>2&&a->posicion->left->id<40&&h->posicion->id==2&&!a->posicion->right->id!=50)
                        {
                           elimIniAtaque(&(h)->ataque);
                           //for(;b&&b!=;
                           reduceVida(a->posicion->left->id,b);
                           *puntos+=100;
                           
                        }
                        else
                        {
                            a->posicion->id=0;
                            a->posicion=a->posicion->left;
                            a->posicion->id=1;
                        }
                     }
                     else
                     {
                         h->posicion->id=0;
                         elimIniAtaque(&(h)->ataque);
                     }
                     break;
                case 3:
                     if(a->posicion->right)
                     {
                        if(a->posicion->right->id==2)
                        {
                           elimIniAtaque(&(h)->ataque);
                           if((*b)->indice!=7)
                           {
                              (*b)->vidas--;
                              PlaySound(("link7.wav"),NULL,SND_ASYNC);
                           }
                           else
                              PlaySound(("defensa.wav"),NULL,SND_ASYNC);
                           
                        }
                        else
                        if(a->posicion->right->id>2&&a->posicion->left->id<40&&h->posicion->id==2&&!a->posicion->left->id!=50)
                        {
                           elimIniAtaque(&(h)->ataque);
                           reduceVida(a->posicion->right->id,b);
                           *puntos+=100;
                        }
                        else
                        {
                            a->posicion->id=0;
                            a->posicion=a->posicion->right;
                            a->posicion->id=1;
                        }
                     }
                     else
                     {
                         a->posicion->id=0;
                         elimIniAtaque(&(h)->ataque);
                     }
                     break;
             }
             a=a->sig;            
     }
   h=h->sig;
   }
}
void moverAtaqueN(PERSONAJE *h, PERSONAJE **b, int *puntos)
{
   ATAQUE *a;
   while(h)
   {
     a=h->ataque;
     while(a)
     {
             switch(a->direccion)
             {
                case 0:
                     if(a->posicion->up)
                        if(a->posicion->up->id>2)
                        {
                           elimIniAtaque(&(h)->ataque);
                           reduceVida(a->posicion->up->id,b);
                           *puntos+=100;
                        }
                        else
                        {
                            a->posicion->id=0;
                            a->posicion=a->posicion->up;
                            a->posicion->id=1;
                        }
                     else 
                     {
                         h->posicion->id=0;
                         elimIniAtaque(&(h)->ataque);
                     }                        
                     break; 
                case 1:
                     if(a->posicion->down)
                        if(a->posicion->down->id==2)
                        {
                           elimIniAtaque(&(h)->ataque);
                           if(!((*b)->indice==19||(*b)->indice==17))
                           {
                              (*b)->vidas--;
                              PlaySound(("link7.wav"),NULL,SND_ASYNC);
                           }
                           else
                              PlaySound(("defensa.wav"),NULL,SND_ASYNC);
                        }
                        else
                        {
                            a->posicion->id=0;
                            a->posicion=a->posicion->down;
                            a->posicion->id=1;
                        }
                     else 
                     {
                         h->posicion->id=0;
                         elimIniAtaque(&(h)->ataque);
                     }
                     break;  
             }
             a=a->sig;            
     }
   h=h->sig;
   }
}
int elimIniAtaque(ATAQUE **nodo)
{
    int res=0;
    ATAQUE *aux,*aux2;
    
    if(*nodo)
    {
             aux2=*nodo;
             while(aux2->sig)
             {
                             aux=aux2;
                             aux2=aux2->sig;
             }
             if(aux2==*nodo)
             {
                *nodo=NULL;
                aux2->posicion->id=0;
             }
             else
             {
                aux2->posicion->id=0;
                aux->sig=NULL;
             }
             free(aux2);
             res=1;
    }
    return(res);
} 
void AutomatizarAlien(PERSONAJE *alien, PERSONAJE *nave)
{
     int a,direccion,i=3;
     
     
      for(;alien;alien=alien->sig,i++)
      {
         a=rand()%150;
         if(a==10)
         {
            if(alien->indice==8)
               direccion=3;
            else
               if(alien->indice==10)
            direccion=2;
            dispararAtaqueHeroe(&alien,direccion,15);
            
         }
      //if(a==10)
         
         if(a==30)
         {
            if(alien->indice==8)
               direccion=4;
            else
               if(alien->indice==10)
            direccion=3;
            moverPosicionImagen(&(alien)->posicion,direccion,i);
         }
         
      alien->posicion->id=i;
      }
      
      for(;nave;nave=nave->sig,i++)
      {
         a=rand()%150;
         if(a==3)
           moverPosicionImagen(&(nave)->posicion,3,i);
         else
            if(a==7)
          moverPosicionImagen(&(nave)->posicion,4,i);
            
         
         
         if(a==10)
            dispararAtaqueHeroe(&nave,1,15);

            
        nave->posicion->id=i; 
        if(nave->posicion->left) 
           nave->posicion->left->id=i;   
         
      }
         
            
}
   /*
   alien1der.malla 8
   alien1derav.malla 9
   alien1izq.malla 10 
   alien1izqav.malla 11
   */  
int elimAlien(PERSONAJE **p, int x)
{
    PERSONAJE *aux,*aux2;
    int res=0;
    
    if(*p)
    if((*p)->posicion->id==x)
    {
             aux=*p;
             /*if(aux==*p)
             {
                liberaAtaque(&(aux)->ataque);
                *p=NULL;
             }
             else*/
             *p=aux->sig;
             liberaAtaque(&(aux)->ataque);
             aux->posicion->id=0;
             aux=NULL;
             free(aux);  
             res=1;
             //printf("\nPelicula Eliminada\n");
    }
    else
    {
        aux=*p;
        while(aux&&aux->posicion->id!=x)
        {
           aux2=aux;
           aux=aux->sig;
        }
        if(aux)
        {
           aux2->sig=aux->sig;
           liberaAtaque(&(aux)->ataque);
           aux->posicion->id=0;
           if(aux->posicion->left)
              aux->posicion->left->id=0;
           aux=NULL;
           free(aux);
           res=1;
           //printf("\nPelicula Eliminada\n");
        }
        else
           /*printf("\nNo se encuentra la pelicula\n")*/;  
    }                             
    return(res);
}    
void liberaAtaque(ATAQUE **A)
{
     if(*A)
     while(*A)
     {
        free(*A);
        *A=(*A)->sig;
     }
}
void reduceVida(int x,PERSONAJE **alien)
{
    int i;
    PERSONAJE *aux;
    
    
    for(aux=*alien;aux&&aux->posicion->id!=x;aux=aux->sig);
                                                    

    if(aux)
    {
      
        aux->vidas--;
        PlaySound(("wchoose.wav"),NULL,SND_ASYNC);
      if(aux->vidas==0)
         elimAlien(alien,x);
       
    }
}
void perdiste(RECORDS *rec, char *cad, int n, char *cad2)
{
     
     rectangle(getmaxx()/2-350, getmaxy()/2-170,getmaxx()-300,getmaxy()-150);
     settextstyle(8, 0, 6);
     setbkcolor(BLACK);
     
     outtextxy(getmaxx()/2-300, getmaxy()/2-160,cad2);
     
     settextstyle(8,0,4);
     sprintf(cad2,"Tiempo: %s",cad);
     outtextxy(getmaxx()/2-300, getmaxy()/2-110,cad2);
     
     sprintf(cad,"NIVEL %d",n);
     outtextxy(getmaxx()/2-300, getmaxy()/2-60,cad);
     
     sprintf(cad,"Puntos: %s",rec->name);
     outtextxy(getmaxx()/2-300, getmaxy()/2-10,cad);
      
     sprintf(cad,"Puntos: %d",rec->puntos);
     outtextxy(getmaxx()/2-300, getmaxy()/2+40,cad);
     
     setcolor(RED);
     outtextxy(350,400,"CLICK PARA CONTINUAR");  
     setcolor(WHITE);
     waitLMC();
     clearmouseclick(WM_LBUTTONDOWN);
}
void liberaPersonaje(PERSONAJE **P)
{
     if(*P)
     while(*P)
     {
        liberaAtaque(&(*P)->ataque);
        free(*P);
        *P=(*P)->sig;
     }
}



