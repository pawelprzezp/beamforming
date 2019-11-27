#include <stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>


#define PI 3.14159265
#define OKRES_PROBKOWANIA_s 0.000001 //10Hz
#define ILOSC_PROBEK 10000
#define PREDKOSC_DZWIEKU_m_s 340  //Predkosc dziwieku 340 m/s
#define LICZBA_KATOW 181  //Liczba kątów odpowiadająca ilosci iteracji


double mikrofon1[ILOSC_PROBEK]={0};
double mikrofon2[ILOSC_PROBEK]={0};
double mikrofon3[ILOSC_PROBEK]={0};
double mikrofon4[ILOSC_PROBEK]={0};
double opoznienia_mikrofon1[LICZBA_KATOW];
double opoznienia_mikrofon2[LICZBA_KATOW];
double opoznienia_mikrofon3[LICZBA_KATOW];
double opoznienia_mikrofon4[LICZBA_KATOW];


void print(double sig[], char* nazwa )//zapisuje do pliku sygnał o
{


  FILE *fp;
  char* rozwiniecie = ".dat";
  char Specyfikator[strlen(nazwa)+strlen(rozwiniecie)+1];

  snprintf( Specyfikator, sizeof( Specyfikator ), "%s%s", nazwa, rozwiniecie );
  fp = fopen (Specyfikator,"w");

  for(int i=0;i<ILOSC_PROBEK;i++)
  {
      fprintf(fp,"%d   %f\n",i,sig[i]);
  }
}

void print_max(double sig[])//zapisuje do pliku.dat maksima korelacji dla każdego z kątów
{
  FILE *fp;
  fp = fopen ("max.dat","w");

  for(int i=0;i<181;i++)
  {
      fprintf(fp,"%d   %f\n",i,sig[i]);
  }
}

void Symuluj(double mic[], int delay, int szum_delay) //funkcja tworząca sygnał do symulacji
{
    for(int i=0;i<delay;i++)
    {
        mic[i] = 0;
    }
    for(int i=delay;i<1000+delay;i++)
    {
        mic[i]=i-delay;
    }
    for(int i=1000+delay;i<2000+delay;i++)
    {
        mic[i]=2000+delay-i;
    }
    for(int i=2000+delay;i<5000;i++)
    {
        mic[i]=0;
    }
    for(int i=5000;i<5000+szum_delay;i++)
    {
      mic[i]=0;
    }
    for(int i=5000+szum_delay;i<5500+szum_delay;i++)
    {
        mic[i]=i-szum_delay-5000;
    }
    for(int i=5500+szum_delay;i<6000+szum_delay;i++)
    {
        mic[i]=6000+szum_delay-i;
    }
    for(int i=6000+szum_delay;i<ILOSC_PROBEK;i++)
    {
        mic[i]=0;
    }
}

double DegToRad(int kat) //konwersja stopni na radiany
{

  double przelicznik= PI/180;

  return kat*przelicznik;

}
int Count_Delay() //obliczanie opóżnień dla mikrofonów względem mic1
{



      for(int i=0, stopnie=-90; i<181 && stopnie<91 ;i++,stopnie++)
      {

          opoznienia_mikrofon2[i]=(0.02*sin(DegToRad(stopnie)))/PREDKOSC_DZWIEKU_m_s;
          opoznienia_mikrofon3[i]=(0.04*sin(DegToRad(stopnie)))/PREDKOSC_DZWIEKU_m_s;
          opoznienia_mikrofon4[i]=(0.06*sin(DegToRad(stopnie)))/PREDKOSC_DZWIEKU_m_s;

      }
      for(int i=0;i<181;i++)
      {
         //printf("%d\n",i );
         //printf("%lf\n",opoznienia_mikrofon2[i] );
      }
  return 0;
}

double Determinate_Singal( double Sig[ILOSC_PROBEK], double t, double tdelay,
                           double OkresProbkowania_s)
{
       if (t<tdelay) return 0;

       int Idx= floor((t-tdelay)/OkresProbkowania_s);
       double S=Sig[Idx];
       if (Idx>=ILOSC_PROBEK) return 0;


       return (((Sig[Idx+1]-Sig[Idx])/OkresProbkowania_s)
                 *((t-tdelay)-Idx*OkresProbkowania_s))+Sig[Idx];

}

double Beamforming(int argument_programu)
{

  double czas;
  double bufor[180]; //bufor przechowujący rozwiazania dla kazdej iteracji
  double sygnal1, sygnal2, sygnal3, sygnal4, suma_sygnalow;
  double maksimum_korelacji=0;
  int kat_sygnalu=0;

  for(int i=0;i<181;i++)
  {

  if (argument_programu==116)  maksimum_korelacji = -2147483648; //Zerowanie zmiennej do rysowania wykresu maksimum
  double opoznienie_mikrofon1_s=0;
  double opoznienie_mikrofon2_s=opoznienia_mikrofon2[i];
  double opoznienie_mikrofon3_s=opoznienia_mikrofon3[i];
  double opoznienie_mikrofon4_s=opoznienia_mikrofon4[i];


      for(int indeks=0;indeks<ILOSC_PROBEK;indeks++)
      {
          czas=indeks*OKRES_PROBKOWANIA_s;
          sygnal1=Determinate_Singal(mikrofon1,czas,opoznienie_mikrofon1_s,OKRES_PROBKOWANIA_s);
          sygnal2=Determinate_Singal(mikrofon2,czas,opoznienie_mikrofon2_s,OKRES_PROBKOWANIA_s);
          sygnal3=Determinate_Singal(mikrofon3,czas,opoznienie_mikrofon3_s,OKRES_PROBKOWANIA_s);
          sygnal4=Determinate_Singal(mikrofon4,czas,opoznienie_mikrofon4_s,OKRES_PROBKOWANIA_s);
          suma_sygnalow= sygnal1+sygnal2+sygnal3+sygnal4;

          if (maksimum_korelacji<suma_sygnalow)
          {
              maksimum_korelacji=suma_sygnalow;
              kat_sygnalu=i;
          }

      }
    bufor[i]=maksimum_korelacji;
//printf("%f\n",max );
//printf("%d\n",i );
  }
 printf("%f\n",maksimum_korelacji);
 printf("%d\n",kat_sygnalu);
 print_max(bufor);
   return kat_sygnalu;//print(mic1);

}



int main(int argc,char* argv[])
{
  Symuluj(mikrofon1,0,0);
  Symuluj(mikrofon2,20,-40);
  Symuluj(mikrofon3,40,-80);
  Symuluj(mikrofon4,60,-120);
  Count_Delay();

    if(argc>1)
       Beamforming(*argv[1]);
    else
       Beamforming(0);
  print(mikrofon1,"syg1");
  print(mikrofon2,"syg2");
  print(mikrofon3,"syg3");
  print(mikrofon4,"syg4");

   return 0;
}
