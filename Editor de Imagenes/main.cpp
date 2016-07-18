#include <winbgim.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define TRUE 1
#define FALSE 0

typedef struct boton
{
        int x, y, tamx, tamy, num, edo;
        char nom[20];
        struct boton *sig;
}BOTON;

typedef struct
{
        int existe;
        int color;
}ARREGLO;

void Programa();
BOTON *crearNodo(int x, int y, int tamx, int tamy, int num, char *nombre);
int insertarBoton(BOTON **lista, int x, int y, int tamx, int tamy, int num, char *nombre);
BOTON *crearListaMenu(int *ancho, int *alto);
void dibujarMenu(BOTON *lista, int num);
void liberarListaMenu(BOTON **lista);
void pideTam(int *dimx, int *dimy, int *color);
ARREGLO **crearArreglo(int dimx, int dimy);
void inicializarArreglo(ARREGLO **arr, int dimx, int dimy, int color);
void crearMalla(ARREGLO **arr, int dimx, int dimy, int xr, int yr, int color, int band); 
void liberarArreglo(ARREGLO ***arr, int dimx, int dimy);
int guardar(ARREGLO **arr, int dimx, int dimy, int xi1, int yi1, char *nombre, int como, int *nuevo);
int guardarImagen(ARREGLO **arr, int dimx, int dimy, int xi1, int yi1, char *nombre);
ARREGLO **abrirImagen(int *dimx, int *dimy, char *nombre);
void inText(int x, int y, char *cad);
void delay(int milisec);
void dibujarPaleta();
void vistaPrevia(ARREGLO **arr, int dimx, int dimy);
void cambiaColor(ARREGLO **,int, int);

int main()
{
    BOTON *l;
    initwindow(1220,730);
    setbkcolor(COLOR(54,92,248));//fondo de letra
    settextstyle(10,0,2);//tipo de letra
    setcolor(15);//color de letra
    outtextxy(500,300,"_By_Eduardo_Montes_");
    outtextxy(450,400,"Presiona una tecla para iniciar");
    getch();
    cleardevice();
    Programa();  
    closegraph();
}
BOTON *crearNodo(int x, int y, int tamx, int tamy, int num, char *nombre)
{
      BOTON *aux;
      
      aux = (struct boton *)malloc(sizeof(struct boton));
      if(aux != NULL)
      {
        aux->x = x;
        aux->y = y;
        aux->tamx = tamx;
        aux->tamy = tamy;
        aux->num = num;
        aux->edo = 0;
        strcpy(aux->nom, nombre);
        aux->sig = NULL;
      }
      else
        return NULL;
      return(aux);
}

int insertarBoton(BOTON **lista, int x, int y, int tamx, int tamy, int num, char *nombre)
{
    BOTON *nuevo, *aux;
    
   nuevo = crearNodo(x,y,tamx,tamy,num,nombre);
    if(nuevo != NULL)
    {
      if(*lista == NULL)
        *lista = nuevo;
      else
      {
        for(aux = *lista; aux->sig != NULL; aux = aux->sig);
        aux->sig = nuevo;
      }
      return(1);
    }
    else
      return(0);
}

BOTON *crearListaMenu(int *ancho, int *alto)
{
    BOTON *lista;
    int x=50, y=0, tamx, tamy;
    int ret = 0;
    
    lista = NULL;
    tamx = x + textwidth("Nuevo")+10;
    tamy = y + textheight("Nuevo")+5;
    
    ret = insertarBoton(&lista, x, y, tamx, tamy, 1, "Nuevo");
    if(ret)
    {
      x=tamx+5;
      tamx=x+textwidth("Abrir")+10;
      ret = insertarBoton(&lista, x, y, tamx, tamy, 2, "Abrir");
    }
    if(ret)
    {
      x = tamx+5;
      tamx = x + textwidth("Guardar")+10;
      ret = insertarBoton(&lista, x, y, tamx, tamy, 3, "Guardar");
    }
    if(ret)
    {
      x = tamx+5;
      tamx = x + textwidth("Guardar Como")+10;
      ret = insertarBoton(&lista, x, y, tamx, tamy, 4, "Guardar Como");
    }
    if(ret)
    {
      x = tamx+5;
      tamx = x + textwidth("Salir")+10;
      ret = insertarBoton(&lista, x, y, tamx, tamy, 5, "Salir");  
      *ancho = tamx; *alto = tamy;
    }
    if(ret)
      return(lista);
    else
      return NULL;
}

void liberarListaMenu(BOTON **lista)
{
     BOTON *aux;
     
     while(*lista != NULL)
     {
       aux = *lista;
       *lista = aux->sig;
       free(aux);
     }
     
}

void dibujarMenu(BOTON *lista, int num)
{
     BOTON *aux;
     
      settextstyle(10,0,4);
     for(aux = lista; aux != NULL; aux = aux->sig)
     {
       if(num != aux->num)
       {
         setfillstyle(1,RED);
         setlinestyle(DOTTED_LINE, 0, THICK_WIDTH);
         rectangle(aux->x, aux->y, aux->tamx, aux->tamy);
         floodfill(aux->x + 10, aux->y + 10, WHITE);
         setbkcolor(RED);
         outtextxy(aux->x + 6, aux->y + 4, aux->nom);
       }
       else
       { 
         setfillstyle(1,BLUE);
         setlinestyle(DOTTED_LINE, 0, THICK_WIDTH);
         rectangle(aux->x, aux->y, aux->tamx, aux->tamy);
         floodfill(aux->x + 10, aux->y + 10, WHITE);
         setbkcolor(BLUE);
         outtextxy(aux->x + 4, aux->y + 2, aux->nom);
       }
     }
    
}

void dibujarPaleta()
{
     int i, x=20, y=100;

     setlinestyle(3, 1, 1);
     rectangle(x-1,y-1, x+26,y+25*16);
     for(i = 0; i < 16; i++)
     {
       setfillstyle(1,i);
       bar(x, y, x + 25, y + 25);
       y += 25;
     }
}

void pideTam(int *dimx, int *dimy, int *color)
{
     int x=250, y=getmaxy()/2 - 100;
     char dim[50], basura[10];
     
     setcolor(BLUE);
     setlinestyle(1, 1, 10);
     bar(x, y, x + 400, y + 100);
     
     setcolor(WHITE);
     settextstyle(8, 0, 3);
     strcpy(dim, "  !Ingresar tamaño en x!  ");
     setlinestyle(1, 1, 3);
     line(x+20,y+textheight(dim)+15,x+15+textwidth(dim), y+textheight(dim)+15);
     outtextxy(x + 15, y + 10,dim);
     strcpy(dim,"   X : ");
     outtextxy(x+15,y+55,dim);
     inText(x+15+textwidth(dim), y+55, dim);
     *dimx = atoi(dim);
     
     strcpy(dim, "  !Ingresar tamaño en y!  ");
     outtextxy(x + 15, y + 10, dim);
     strcpy(dim,"   Y : ");
     outtextxy(x+15,y+55,dim);
     inText(x+15+textwidth(dim), y+55, dim);
     *dimy = atoi(dim);
     strcpy(dim, "  !Color de fondo!  ");
     outtextxy(x + 15, y + 10, dim);
     strcpy(dim,"   C : ");
     outtextxy(x+15,y+55,dim);
     inText(x+15+textwidth(dim), y+55, dim);
     *color = atoi(dim);
}

void inText(int x,int y, char cadena[])
{
     char tecla;     
     int cont=0;
 
     cadena[0]='\0';
 
     do{
       /* Esto es para simular un parpadeo en las coordenadas donde se realizará la captura del nombre */
       do{ 
         setcolor(15);
         outtextxy(x+textwidth(cadena),y,"_");
         delay(50);
         setcolor(0);
         outtextxy(x+textwidth(cadena),y,"_");
         delay(50);
         setcolor(15);
       }while(!kbhit());
     
       tecla = getch();
     
       if(tecla==0)  // si es código extendido
         tecla=getch();
       
       if(tecla==8 && cont > 0)  // se presiono retroseso
	   {
         setcolor(BLACK);
         outtextxy(x,y,cadena);  // se borra en la pantalla
         cadena[--cont]='\0';  //se borra el último caracter del arreglo
         setcolor( WHITE );
         outtextxy(x,y,cadena);
       }
       else
	   {  
         if(tecla!=13)
         {  
           cadena[cont++]=tecla;
           cadena[cont]='\0';
           outtextxy(x,y,cadena);
         }
        }
     }while(tecla!=13); 
}

ARREGLO **crearArreglo(int dimx, int dimy)
{
     ARREGLO **arr=NULL;
     int i;
     
     arr = (ARREGLO **)malloc(sizeof(ARREGLO *) * dimx);
     for(i = 0; i < dimx; i++)
       *(arr+i) = (ARREGLO *)malloc(sizeof(ARREGLO) * dimy);
     return arr;
}

void inicializarArreglo(ARREGLO **arr, int dimx, int dimy, int color)
{
     int i, j;
     
     for(i = 0; i < dimx; i++)
       for(j = 0; j < dimy; j++)
       {
         arr[i][j].color = color;
         if(color == BLACK)
           arr[i][j].existe = 0;
         else
           arr[i][j].existe = 1;
       }
}

void liberarArreglo(ARREGLO ***arr, int dimx, int dimy)
{
     int i;
     
     for(i = 0; i < dimx; i++)
       free(*(*arr+i));
     free(*arr);
}

void crearMalla(ARREGLO **arr, int dimx, int dimy, int xr, int yr, int color, int band)
{
     int i, j, x1, y1, x2, y2, tamPixX, tamPixY;
     
     tamPixX = (int )(800/dimx); tamPixY = (int )600/dimy;
     
     x1 = 50; y1 = 50; x2 = x1 + tamPixX; y2 = y1 + tamPixY;
     
     setlinestyle(0, 0, 1);
     for(i = 0; i < dimx; i++, x1 += tamPixX, x2 += tamPixX)
     {
       for(j = 0; j < dimy; j++, y1 += tamPixY, y2 += tamPixY)
       {         
         setcolor(WHITE);
         rectangle(x1, y1, x2, y2);
       
         if((xr > x1 && xr < x2 && yr > y1 && yr < y2) || band)
         {
           if(band)
             setfillstyle(1,arr[i][j].color);
           else
             setfillstyle(1, color);
           
           if(arr[i][j].color == WHITE)
           {
             if(band == 0)
               arr[i][j].color = color;
             setcolor(CYAN);
             rectangle(x1, y1, x2, y2);
             floodfill(x1+1, y1+1, CYAN);
           }
           else
           {
             if(band == 0)
               arr[i][j].color = color;
             floodfill(x1+1, y1+1, WHITE);
           }
         }
         if(arr[i][j].color == BLACK)
           arr[i][j].existe = 0;
         else
           arr[i][j].existe = 1;
       }
       y1 = 50;
       y2 = y1 + tamPixY;
       
     }
}


void vistaPrevia(ARREGLO **arr, int dimx, int dimy)
{
     int x=900,y=70,i, j;

     for(i = 0; i < dimx; i++)
       for(j = 0; j < dimy; j++)
         if(arr[i][j].existe == 1 )
           putpixel(x+i, y+j, arr[i][j].color);
         else
           putpixel(x+i, y+j, BLACK);
}
void delay(int milisec)
{ 
     time_t i,j;
     i=clock();
     do
     {
      j=clock();
     }while((j-i)< milisec); 
}
int seleccionarBoton(BOTON *lista, int xr, int yr)
{
    int ret = 0;
    
    if(lista)
    {
      if(xr > lista->x && xr < lista->tamx && yr > lista->y && yr < lista->tamy)
      {
        ret = lista->num;
        lista->edo = 1;
      }
      else
        ret = seleccionarBoton(lista->sig, xr, yr);
    }
    return ret;
}

int guardar(ARREGLO **arr, int dimx, int dimy, int xi1, int yi1, char *nombre, int como, int *nuevo)
{
     int x1=880, y1=500, x2=1180, y2=600;
     int x, y;
  
     setlinestyle(1, 1, 10);
     bar(x1, y1, x2, y2);
     
     if(como || *nuevo)
     {
       setcolor(WHITE);
       settextstyle(8, 0, 3);
       strcpy(nombre, "  !Ingresar Nombre!  ");
       setlinestyle(1, 1, 3);
       line(x1+5,y1+textheight(nombre)+15,x1+textwidth(nombre), y1+textheight(nombre)+15);
       outtextxy(x1+2, y1 + 10,nombre);
       strcpy(nombre,"   Nombre:");
       outtextxy(x1+2,y1+55,nombre);
       inText(x1+15+textwidth(nombre), y1+55, nombre);
       guardarImagen(arr,dimx,dimy, xi1, yi1, nombre);
       
       setviewport(880, 500, x2, y2, 1);
           clearviewport();
           setbkcolor(BLACK);
           setviewport(0, 0, getmaxx(), getmaxy(), 0);    
      *nuevo = 0; 
     }
     else
     {
       guardarImagen(arr,dimx,dimy, xi1, yi1, nombre);
       setviewport(880, 500, x2, y2, 1);
       clearviewport();
       setbkcolor(BLACK);
       setviewport(0, 0, getmaxx(), getmaxy(), 0);
     } 
}                  

int guardarImagen(ARREGLO **arr, int dimx, int dimy, int xi1, int yi1, char *nombre)//quitar archivo n por que no hace nada
{
     FILE *m, *n;
     void *img;
     long dimImg;
     int ret = 0, i, j;
     char malla[50], imagen[50];
     malla[0]='/0';
     imagen[0]='/0';
     
     dimImg = imagesize(xi1, yi1, xi1+dimx-1, yi1+dimy);
     img = malloc(dimImg);
     getimage(xi1, yi1, xi1+dimx-1, yi1+dimy-1,img);
     
     strcpy(imagen, nombre); 
     strcpy(malla, nombre);
    
     strcat(imagen,".img");
     strcat(malla,".malla");
     m = fopen(malla, "wb");
     if(m == NULL)
       printf("Error al crear el archivo");
     else
     {
       n = fopen(imagen, "wb");
       if( n == NULL)
         printf("Error al crear el archivo");
       else
       {
         fwrite(&dimx, sizeof(int), 1, m);
         fwrite(&dimy, sizeof(int), 1, m);
       
         for(i = 0; i < dimx; i++)
          for(j = 0; j < dimy; j++)
          {
            fwrite(&arr[i][j].color,sizeof(int),1,m);
            fwrite(&arr[i][j].existe,sizeof(int),1,m);
          }
         
         //fwrite(&dimImg, sizeof(long), 1, n);
         //fwrite(img, dimImg, 1, n);
         
         fclose(n);
         fclose(m);
         
         ret = 1;
       } 
     }
  return( ret );
}

ARREGLO **abrirImagen(int *dimx, int *dimy, char *nombre)
{
     FILE *f;
     ARREGLO **arr = NULL, **aux = NULL;
     int x1=880, y1=500, x2=1180, y2=600, i, j;
     char nom[50], nomAux[50];
      
      setcolor(WHITE);
      settextstyle(8, 0, 3);
      strcpy(nombre, "  !Ingresar Nombre!  ");
      setlinestyle(1, 1, 3);
      line(x1+5,y1+textheight("  !Ingresar Nombre!  ")+15,x1+textwidth("  !Ingresar Nombre!  "), y1+textheight("  !Ingresar Nombre!  ")+15);
      outtextxy(x1+2, y1 + 10,"  !Ingresar Nombre!  ");
      strcpy(nombre,"   Nombre:");
      outtextxy(x1+2,y1+55,"   Nombre:");       
      inText(x1+15+textwidth("   Nombre:"), y1+55, nombre);
      
      strcpy(nom,nombre);
      strcat(nom, ".malla");
      
      f = fopen(nom, "rb");
      if(f == NULL)
        printf("Error al abrir el archivo");
      else
      {
        fread(dimx,sizeof(int),1,f);
        fread(dimy,sizeof(int),1,f);
        arr = crearArreglo(*dimx, *dimy);
        //for(i = *dimx-1; i >=0; i--)//Activar esta linea para voltear una imagen al abrirla
        for(i = 0; i <*dimx; i++)
          for(j = 0; j < *dimy; j++)
          //for(j = *dimy-1; j>=0; j--)
          {
            fread(&arr[i][j].color,sizeof(int),1,f); 
            fread(&arr[i][j].existe,sizeof(int),1,f);
            if(arr[i][j].color != BLACK)
              arr[i][j].existe = 0;
            else
              arr[i][j].existe = 1;
          }
        fclose(f);
       
      }
      return arr;
}  
  
  
void Programa()
{
     BOTON *lista;
     ARREGLO **arr;
     int xr, yr, xi, yi, xf, yf, como, nuevo = 0, page, dimx, dimy, boton = 0, ancho, alto, color, band, b1, b2, a=0, bandera = 0;
     char nombre[50],tecla;
     arr = NULL;
     nombre[0] ='/0';
     band = 0;
     page = 0;
     b1 = 0;
     b2 = 0;
     color = -1;
     
     settextstyle(10,0,4);
     lista=crearListaMenu(&ancho,&alto);
     dibujarMenu(lista, 0);
     dibujarPaleta();
     
     
     do{ 
         xr = mousex();
         yr = mousey(); 
       if(a){
         if(page==0)
           page=1;
         else
           page =0;
       setactivepage(page);
       setbkcolor(BLACK);
       cleardevice();        
       dibujarMenu(lista, 0);
       dibujarPaleta(); 
       setbkcolor(BLACK);
       setvisualpage(page);
       a = 0 ;
       }
        
       if(b1){
         vistaPrevia(arr,dimx,dimy);
         crearMalla(arr,dimx,dimy,xr,yr,color,band);
         
        b1 = 0;
        band = 0;}
       if(ismouseclick(WM_LBUTTONDOWN))
       {
         
         boton = seleccionarBoton(lista, xr, yr);
         
         if(boton)
         {           
           switch(boton)
           {             
             case 1: dibujarMenu(lista, boton);// Dio clic en el boton Nuevo
                     delay(100);
                     dibujarMenu(lista, 0);
                     pideTam(&dimx,&dimy, &color);
                     if(arr = crearArreglo(dimx,dimy)){
                       inicializarArreglo(arr,dimx,dimy, color);
                       b1 = 1;
                       band = 1;
                       como = 1;
                       nuevo = 1;
                       a=1;}
                     break;
                     
             case 2: dibujarMenu(lista, boton);//  Abrir
                     delay(100);
                      dibujarMenu(lista, 0);
                      if(arr != NULL)
                        liberarArreglo(&arr,dimx,dimy);
                      arr = abrirImagen(&dimx, &dimy, nombre);
                      if(arr == NULL) break;
                     b1 = 1;
                     band = 1;
                     a = 1;
                     color = -1;
                     break;
                     
             case 3: dibujarMenu(lista, boton);// Guardar
                     delay(100);
                      dibujarMenu(lista, 0); 
                      como = 0;
                      guardar(arr,dimx,dimy, 900, 70, nombre, como, &nuevo);
                      
                     break;
                     
             case 4: dibujarMenu(lista, boton);// Guardar Como
                     delay(100);
                      dibujarMenu(lista, 0); 
                      como = 1;
                      guardar(arr,dimx,dimy, 900, 70, nombre, como, &nuevo);
                     break;
                     
             case 5: dibujarMenu(lista, boton);// salir
                     delay(100);
                      dibujarMenu(lista, 0);
                     liberarListaMenu(&lista);
                     if(b1)
                       liberarArreglo(&arr,dimx,dimy);
                    bandera = 1;
                     break;
           }
           boton = 0;
         }
         /*if(kbhit())
         {
            tecla=getchar();
            if(tecla=='C')
            cambiaColor(arr,dimx,dimy); 
         } */
         if(xr > 19 && xr < 46 && yr > 100 && yr < 500)
           color = getpixel(xr,yr);
           if((xr > 50 && xr < 850 && yr > 50 && yr < 650) && band == 0 && color != -1){
         crearMalla(arr,dimx,dimy,xr,yr,color,band);
         vistaPrevia(arr,dimx,dimy);}
       }
       
      clearmouseclick(WM_LBUTTONDOWN);
       
       }while(bandera == 0 && !kbhit());
}
void cambiaColor(ARREGLO **A,int dimx, int dimy)
{
     int i,j;
     for(i=0;i<dimx;i++)
        for(j=0;j<dimy;i++)
        if(A[i][j].existe)
        { 
           A[i][j].color++;
           if(A[i][j].color%17==0)
              A[i][j].color=1;
        }
}
        
     
     
     
     
     



