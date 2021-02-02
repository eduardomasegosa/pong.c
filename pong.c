#include <stdio.h>
#include <conio.h>
#include <conio.c>

#define TAM1 40
#define TAM2 20

typedef struct /*estructura COORD x y para utilizar en otras estructuras*/
{
   int x;
   int y;  
}Coord; 

typedef struct /*estructura BARRA*/
{
   Coord posicion;
   int cred; /*score de cada jugador*/
   int vida; /*vidas de cada jugado*/      
}Barra;

typedef struct /*estructura BOLA */
{
   Coord posicion;
   Coord angulo;    
}Bola;
typedef struct /*estructura PARTIDA para guardar y recuperar partida todo junto*/
{
   char nomPart[50];
   Barra barra1[3];
   Barra barra2[3];
   Bola bola;        
}Partida;

void margen(); 
/*imprime el margen del juego*/
int menu(); 
/*imprime menu y devuelve la opcion elegida*/
void inicil(Barra *barra1, Barra *barra2, Bola *bola); 
/*inicializa todas las estructuras a 0, barras y bola*/
int dibuja(Barra *barra1, Barra *barra2, Bola *bola); 
/*empieza el juego hasta que terminen las vidas o hasta k le des a "Q"*/
void dibujaBola(Barra **barra1, Barra **barra2, Bola **bola); 
/*dibuja la bola en su siguiente posicion, posicion (xy) + angulo (xy)*/
void dibujaBarra(Barra **barra1, Barra **barra2, Bola **bola); 
/*dibuja la barra en su posicion*/
void puntuacion(Barra *barra1, Barra *barra2); 
/*imprime la nueva puntuacion*/
int recuperarPart(Partida *partida, char cadena[]); 
/*busca la partida con un nombre en concreto y la guarda en partida*/
void pasarAEstructura(Barra *barra1, Barra *barra2, Bola *bola, Partida *partida);
/*pasa los valores de barras y bola a partida para guardar en fichero*/
void cojerDeEstructura(Barra *barra1, Barra *barra2, Bola *bola, Partida *partida);
/*coje los valores de partida, y se las pasa a barras y bola para seguir con una partida guardada*/
void guardarPartida(Partida *partida,char cadena[]); 
/*mete en el fichero "cadena" los datos de partida*/
void borrarPart(char cadena[]); 
/*borra el fichero "cadena"*/

main()
{
   Barra barra1[3], barra2[3]; /*cada posicion da un valor para el angulo, pero solo la primera tiene el valor de score y el de vida*/
   Bola bola;
   Partida partida;
   int opcion, devolucion;
   char cad[]="partGuard.txt"; /*nombre del fichero*/
   
   while((opcion=menu())!=5)
   {
      switch (opcion)
      {
         case 1: /*empieza un juego nuevo*/
              inicil(&barra1, &barra2, &bola);
              margen();
              puntuacion(barra1,barra2);
              while ((devolucion=dibuja(&barra1,&barra2, &bola))==1)
              {}
              clrscr();
              break; 
         
         case 2: /*Guardar,abrir archivo con "a"*/
              pasarAEstructura(&barra1, &barra2, &bola, &partida);
              guardarPartida(&partida,cad);
              break;
         case 3: /*recuperar partida*/
              if(recuperarPart(&partida,cad)) /*busca la partida guardada con el nombre k le indicamos, si la encuentra...*/
              {
                 clrscr();
                 cojerDeEstructura(&barra1, &barra2, &bola, &partida);/*carga los valores*/
                 margen();
                 
                 puntuacion(barra1,barra2);
                 while ((devolucion=dibuja(&barra1,&barra2, &bola))==1) /*y comienza la partida cargada*/
                 {}
                 clrscr();                            
              }
              else
              {
                 printf("Partida no encontrada\n");    
              }
              break;
         case 4:
              /*Borrar partidas guardadas abriendo archivo con "w"*/
              borrarPart(cad);
              break;
         default:
              printf("Letra equivocada\n");
              break;
      }
   }
}

int menu()
{
   int dev;
   clrscr();
   gotoxy(1,1);
   printf("----------------------------\n");
   printf("-          PONG!           -\n");
   printf("-   1  Nuevo juego         -\n");
   printf("-   2  Guardar partida     -\n");    
   printf("-   3  Recuperar partida   -\n");
   printf("-   4  Borrar partidas     -\n");
   printf("-   5  Salir               -\n");
   printf("----------------------------\n");
   printf("-      j1 D y S            -\n");
   printf("-      j2 P y L            -\n");
   printf("- Q en medio de la partida -\n");
   printf("- para guardarla despues   -\n");
   printf("----------------------------\n");
   printf("opcion --> ");
   scanf("%d",&dev);
   clrscr();
   return dev;
}
int dibuja(Barra *barra1, Barra *barra2, Bola *bola)
{
   int dev=1;
   char letra;
   
   while(!kbhit() && dev!=0) /*Mientras no apretes ninguna tecla...*/
   {  
      
/*REBOTES CON MARGEN*/
      if(bola->posicion.y == TAM2)
      {
         bola->angulo.y=-1;         /*si llega al margen inferior k rebote*/
      }
      if(bola->posicion.y == 2)
      {
         bola->angulo.y=1;          /*si llega al margen superior k rebote*/
      }
      
/*REBOTE CON LA 2º BARRA*/
      if((bola->posicion.y == barra2->posicion.y) && (bola->posicion.x == barra2->posicion.x))
      {
         bola->angulo.y=-1;
         bola->angulo.x=-1;          /*2º barra 1ºposicion*/
         barra2->cred=barra2->cred+100;
         puntuacion(barra1, barra2);
      }
      if((bola->posicion.y == (barra2+1)->posicion.y) && (bola->posicion.x == (barra2+1)->posicion.x))
      {
         bola->angulo.y=0;
         bola->angulo.x=-1;          /*2º barra 2ºposicion*/
         barra2->cred=barra2->cred+50;
         puntuacion(barra1, barra2);
      }
      if((bola->posicion.y == (barra2+2)->posicion.y) && (bola->posicion.x == (barra2+2)->posicion.x))
      {
         bola->angulo.y=1;
         bola->angulo.x=-1;          /*2º barra 3ºposicion*/
         barra2->cred=barra2->cred+100;
         puntuacion(barra1, barra2);
      }
      
/*REBOTE CON LA 1º BARRA*/
      if((bola->posicion.y == barra1->posicion.y) && (bola->posicion.x == barra1->posicion.x))
      {
         bola->angulo.y=-1;
         bola->angulo.x=1;          /*1º barra 1ºposicion*/
         barra1->cred=barra1->cred+100;
         puntuacion(barra1, barra2);
      }
      if((bola->posicion.y == (barra1+1)->posicion.y) && (bola->posicion.x == (barra1+1)->posicion.x))
      {
         bola->angulo.y=0;
         bola->angulo.x=1;          /*1º barra 2ºposicion*/
         barra1->cred=barra1->cred+50;
         puntuacion(barra1, barra2);
      }
      if((bola->posicion.y == (barra1+2)->posicion.y) && (bola->posicion.x == (barra1+2)->posicion.x))
      {
         bola->angulo.y=1;
         bola->angulo.x=1;          /*1º barra 3ºposicion*/
         barra1->cred=barra1->cred+100;
         puntuacion(barra1, barra2);
      }
      

/*MOVIMIENTO DE BARRA*/
      dibujaBarra(&barra1, &barra2, &bola);
   
/*MOVIMIENTO DE LA PELOTA*/
      dibujaBola(&barra1, &barra2, &bola);
      
/*jugador 2 pierde una vida*/
      if(bola->posicion.x == TAM1)
      {          
         gotoxy(bola->posicion.x, bola->posicion.y - bola->angulo.y);
         printf(" ");
         gotoxy(1,22);
         system("PAUSE");
         margen();
         bola->posicion.x=20;
         bola->posicion.y=10;
         bola->angulo.y=0;              
         barra1->vida=barra1->vida + 1;/*si llega  al final, k modifique las vidas*/
         barra2->vida=barra2->vida - 1;
         puntuacion(barra1, barra2);
      }
/*jugador 1 pierde una vida*/
      if(bola->posicion.x == 0)
      {
         gotoxy(2, bola->posicion.y - bola->angulo.y);
         printf(" ");
         gotoxy(1,22);
         system("PAUSE");
         margen();
         bola->posicion.x=20;
         bola->posicion.y=10;   
         bola->angulo.y=0;              
         barra1->vida=barra1->vida - 1;/*si llega al principio , k modifique las vidas*/
         barra2->vida=barra2->vida + 1;
         puntuacion(barra1, barra2);
      }
/*FIN DE LA PARTIDA porque se terminan las vidas de algun jugador*/
      if(barra1->vida==0 || barra2->vida==0)
      {
         dev=0;
         clrscr(); 
         gotoxy(20,10);
         printf("FIN DE PARTIDA\n");
      }
      sleep(80);
   }
   letra=getch();  /*guarda letra apretada*/
   
   switch(letra)
   {
      case 'q':
      case 'Q':/*SALIR y asi poder guardar*/
           dev=0;
           break;
      case 'w':
      case 'W':/*SUBIR 1º BARRA*/
           if((barra1->posicion.y>2))
           {
               gotoxy((barra1+2)->posicion.x,((barra1+2)->posicion.y));/*borra la posicion anterior*/
               printf(" ");
               barra1->posicion.y=barra1->posicion.y-1;
              (barra1+1)->posicion.y=(barra1+1)->posicion.y-1;/*imprime la barra en su nueva posicion*/
              (barra1+2)->posicion.y=(barra1+2)->posicion.y-1;
           }
           break;
      case 's':
      case 'S':/*BAJAR 1º BARRA*/
           if((barra1->posicion.y<TAM2-1))
           {
              gotoxy((barra1)->posicion.x,((barra1)->posicion.y));/*borra la posicion anterior*/
              printf(" ");
              barra1->posicion.y=barra1->posicion.y+1;
              (barra1+1)->posicion.y=(barra1+1)->posicion.y+1;/*imprime la barra en su nueva posicion*/
              (barra1+2)->posicion.y=(barra1+2)->posicion.y+1;
           }
           break;
      case 'p':
      case 'P':/*SUBIR 2º BARRA*/
           if((barra2->posicion.y>2))
           {
              gotoxy((barra2+2)->posicion.x,((barra2+2)->posicion.y));/*borra la posicion anterior*/
              printf(" ");
              barra2->posicion.y=barra2->posicion.y-1;
              (barra2+1)->posicion.y=(barra2+1)->posicion.y-1;/*imprime la barra en su nueva posicion*/
              (barra2+2)->posicion.y=(barra2+2)->posicion.y-1;
           }
           break;
      case 'l':
      case 'L':/*BAJAR 2º BARRA*/
           if((barra2->posicion.y<TAM2-1))
           {
              gotoxy((barra2)->posicion.x,((barra2)->posicion.y));/*borra la posicion anterior*/
              printf(" ");
              barra2->posicion.y=barra2->posicion.y+1;
              (barra2+1)->posicion.y=(barra2+1)->posicion.y+1;/*imprime la barra en su nueva posicion*/
              (barra2+2)->posicion.y=(barra2+2)->posicion.y+1;
           }
           break;
   }
   return dev;   
}

void inicil(Barra *barra1, Barra *barra2, Bola *bola)
{
   barra1->cred=0;/*score a 0*/
   barra2->cred=0;
   
   barra1->vida=5;/*vidas a 5*/
   barra2->vida=5;
   
   bola->posicion.x=20;/*posicion inicial de bola*/
   bola->posicion.y=10;
   
   bola->angulo.x=1;/*angulo inicial de bola*/
   bola->angulo.y=0;
   
   (barra1+0)->posicion.x=2;/*posicion inicial de barra1*/
   (barra1+0)->posicion.y=2;
   (barra1+1)->posicion.x=2;
   (barra1+1)->posicion.y=3;
   (barra1+2)->posicion.x=2;
   (barra1+2)->posicion.y=4;
   
   (barra2+0)->posicion.x=TAM1-1;/*posicion inicial de barra2*/
   (barra2+0)->posicion.y=2;
   (barra2+1)->posicion.x=TAM1-1;
   (barra2+1)->posicion.y=3;
   (barra2+2)->posicion.x=TAM1-1;
   (barra2+2)->posicion.y=4;
   
   (barra1+0)->cred=0;
   (barra2+0)->cred=0;
}

void dibujaBola(Barra **barra1, Barra **barra2, Bola **bola)
{
      gotoxy((*bola)->posicion.x,(*bola)->posicion.y);/*borra la posicion anterior...*/
      printf(" ");
      (*bola)->posicion.x=(*bola)->posicion.x+(*bola)->angulo.x; /*con el angulo actualiza su posicion*/
      (*bola)->posicion.y=(*bola)->posicion.y+(*bola)->angulo.y;
      gotoxy((*bola)->posicion.x,(*bola)->posicion.y);/*imprime la bola en su nueva posicion*/
      printf("%c",15);
}
void dibujaBarra(Barra **barra1, Barra **barra2, Bola **bola)
{
     
/*DIBUJA 1º BARRA*/
      gotoxy((*barra1)->posicion.x,(*barra1)->posicion.y);
      printf("%c",30);
      gotoxy((*barra1+1)->posicion.x,(*barra1+1)->posicion.y);
      printf("%c",17);
      gotoxy((*barra1+2)->posicion.x,(*barra1+2)->posicion.y);
      printf("%c",31);
      
/*DIBUJA 2º BARRA*/
      gotoxy((*barra2)->posicion.x,(*barra2)->posicion.y);
      printf("%c",30);
      gotoxy((*barra2+1)->posicion.x,(*barra2+1)->posicion.y);
      printf("%c",16);
      gotoxy((*barra2+2)->posicion.x,(*barra2+2)->posicion.y);
      printf("%c",31);
}

void pasarAEstructura(Barra *barra1, Barra *barra2, Bola *bola, Partida *partida)
{
   char cadena[50];
   printf("Introduzca nombre de la partida\n-->");
   scanf("\n");
   gets(cadena);
   strcpy(partida->nomPart,cadena);/*rellena el miembro , nombre de partida*/
   /*guarda los miembros de barras y bola en partida*/
   
   partida->barra1->cred = barra1->cred ;
   partida->barra1->vida = barra1->vida ;
   partida->barra2->cred = barra2->cred ;
   partida->barra2->vida = barra2->vida ;
   
   partida->bola.posicion.x = bola->posicion.x ;
   partida->bola.posicion.y = bola->posicion.y ;
   partida->bola.angulo.x = bola->angulo.x ;
   partida->bola.angulo.y = bola->angulo.y ;
   
   partida->barra1->posicion.x = barra1->posicion.x;
   partida->barra1->posicion.y = barra1->posicion.y;
   partida->barra1[1].posicion.x=barra1[1].posicion.x;
   partida->barra1[1].posicion.y=barra1[1].posicion.y;
   partida->barra1[2].posicion.x=barra1[2].posicion.x;
   partida->barra1[2].posicion.y=barra1[2].posicion.y;
   
   partida->barra2->posicion.x=barra2->posicion.x;
   partida->barra2->posicion.y=barra2->posicion.y;
   partida->barra2[1].posicion.x=barra2[1].posicion.x;
   partida->barra2[1].posicion.y=barra2[1].posicion.y;
   partida->barra2[2].posicion.x=barra2[2].posicion.x;
   partida->barra2[2].posicion.y=barra2[2].posicion.y;
}

void cojerDeEstructura(Barra *barra1, Barra *barra2, Bola *bola, Partida *partida)
{
/*coje los miembros de partida y los pone en barras y bola*/
   
   barra1->cred = partida->barra1->cred ;
   barra1->vida = partida->barra1->vida ;
   barra2->cred = partida->barra2->cred ;
   barra2->vida = partida->barra2->vida ;
   
   bola->posicion.x = partida->bola.posicion.x ;
   bola->posicion.y = partida->bola.posicion.y ;
   bola->angulo.x = partida->bola.angulo.x ;
   bola->angulo.y = partida->bola.angulo.y ;
   
   barra1->posicion.x=partida->barra1->posicion.x;
   barra1->posicion.y=partida->barra1->posicion.y;
   barra1[1].posicion.x=partida->barra1[1].posicion.x;
   barra1[1].posicion.y=partida->barra1[1].posicion.y;
   barra1[2].posicion.x=partida->barra1[2].posicion.x;
   barra1[2].posicion.y=partida->barra1[2].posicion.y;
   
   barra2->posicion.x=partida->barra2->posicion.x;
   barra2->posicion.y=partida->barra2->posicion.y;
   barra2[1].posicion.x=partida->barra2[1].posicion.x;
   barra2[1].posicion.y=partida->barra2[1].posicion.y;
   barra2[2].posicion.x=partida->barra2[2].posicion.x;
   barra2[2].posicion.y=partida->barra2[2].posicion.y;
   
}

int recuperarPart(Partida *partida, char cadena[])
{
   FILE *fich;
   int dev=0;
   int devFread=0,tam=0;
   char cadena2[50];
   fich=fopen(cadena,"r");
   
   printf("Introduzca nombre de la partida\n-->");
   scanf("\n");
   gets(cadena2); /*nombre de la partida que queremos encontrar*/

   while(fread(partida,sizeof(Partida),1,fich) && dev!=1)/*hasta que finalice el fichero o encuentre la partida*/
   {
     /*mirar a ver si esto esta bien...*/
     
     puts(partida->nomPart);
     puts(cadena2);
     if((strcmp(partida->nomPart,cadena2))==0)
     {
        dev=1;                                   
     }
   }
   fclose(fich);
   return dev;
}

void guardarPartida(Partida *partida,char cadena[])
{ 
     FILE *fich;
     fich=fopen(cadena,"a");/*guarda partida al final del fichero*/
     fwrite(partida, sizeof(Partida), 1, fich);   
     fclose(fich);
}

void borrarPart(char cadena[])
{
   FILE *fich;
   
   fich=fopen(cadena,"w");/*borra el fichero*/
   fclose(fich);
}

void puntuacion(Barra *barra1, Barra *barra2)
{
   gotoxy(1,22);
   printf("||--vidas--||--score--||");printf("<-----PUNTUACION\n");
   printf("J1    %d         %d     \n",barra1->vida,barra1->cred);  
   printf("J2    %d         %d     \n",barra2->vida,barra2->cred);  
}

void margen()
{
   int margen1, margen2;
   /*DIBUJA MARGEN*/
   gotoxy(1,1);
   for(margen1=0;margen1<=TAM1;margen1++)
      {
         for(margen2=0;margen2<=TAM2+1;margen2++)
         {
            if((margen2 == 1) || (margen2 == TAM2+1))
            {
               gotoxy(margen1,margen2);
               printf("%c", 176);
            }
            if((margen1 == 1) || (margen1 == TAM1))
            {
               gotoxy(margen1,margen2);
               printf("%c",176);
            }
         }       
      }     
}
