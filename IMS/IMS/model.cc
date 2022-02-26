#include <iostream>
#include <list>
#include <iterator> 
#include <simlib.h>
#include <getopt.h>
#include <string.h>

using namespace std;

#define POCET_VAGON_NAKLADKA 11;

enum { cem325r, cem425r, cem525r };

Store lokomotivy1("Lokomotivy1", 2);
Store lokomotivy2("Lokomotivy2", 2);

Store vagony("Vagony", 250);
Store nakladka("Nakladka", 11);

Facility obsluha("Obsluha");
Facility strojvedouci("Strojvedouci");

Queue cekani_na_stroj;
Queue cekani_na_ostatni;
Queue cekani_na_vagony;
Queue strojvedouci2_cekani;
Queue fronta_na_ostatni;
Queue fronta_na_cesta;

unsigned int pocet_vagonu = 27;

int cem_32_5r = 0;
int cem_42_5r = 0;
int cem_52_5r = 0;

class ProcessManager : public Process {
  void Behavior(){
    if(!strojvedouci2_cekani.Empty()){
      Entity *tmp = strojvedouci2_cekani.GetFirst();
      tmp->Activate(Time);
    }
    else{
      strojvedouci2_cekani.Insert(this);
      Passivate();
    }
  }
};

class Vagon : public Process {
  public:
  int typ_cem = -1;
  void Behavior() {
    Priority = 2;
    Enter(vagony, 1);
    Enter(nakladka , 1);      

    cekani_na_stroj.Insert(this);
    Passivate();
    
    Seize(obsluha, 1);
    Nalozit();
    Release(obsluha);

    cekani_na_ostatni.Insert(this);
    Passivate();

    while(!cekani_na_vagony.Empty()){
      Entity *tmp = cekani_na_vagony.GetFirst();
      tmp->Activate(Time);
    }
    
    
    Leave(nakladka, 1);

    fronta_na_ostatni.Insert(this);
    Passivate();

    fronta_na_cesta.Insert(this);
    Passivate();

    Pridej_typ_cem();

    Leave(vagony, 1);
  }

  void Nalozit(){
    double typ_cementu = Uniform(0, 100);
    
    if(typ_cementu <= 65){
      Wait(Uniform(18, 22));
      typ_cem = cem325r;
    }
    else if(typ_cementu <= 90){
      Wait(Uniform(23, 27));
      typ_cem = cem425r;
    }
    else{
      Wait(Uniform(28, 32));
      typ_cem = cem525r;
    }
  }

  void Pridej_typ_cem(){
    switch(typ_cem) {
      case cem325r:
        cem_32_5r++;
        break;
      case cem425r:
        cem_42_5r++;
        break;
      case cem525r:
        cem_52_5r++;
        break;
    }
  }
};

class Strojvedouci2 : public Process {
  public:
  Stat *_prevezene_vagony;
  Stat *_doba_jizdy;
  Stat *_cekal;

  Strojvedouci2(){
    _prevezene_vagony = new Stat;
    _doba_jizdy = new Stat;
    _cekal = new Stat;
  }

  void Behavior() {
    while (1) {
      int cas_pred = Time;
      if(!strojvedouci2_cekani.Empty()){
        Entity *tmp = strojvedouci2_cekani.GetFirst();
        tmp->Activate(Time);
      }
      else{
        strojvedouci2_cekani.Insert(this);
        Passivate();
      }

      cas_pred = Time - cas_pred;
      (*_cekal)(cas_pred);

      double odjezd = Time;
      Enter(lokomotivy2, 1);

      Wait(Uniform(18, 22)); // Kontrola lokomotivy
      Wait(Uniform(3, 7)); // Nastaveni vozu
      Wait(Uniform(23, 27)); // Zkouska brzd
      Wait(Uniform(23, 27)); // Kontrola vagonu
      double doba_jizdy = Normal(480, 5);
      Wait(doba_jizdy); // Prevoz
      (*_doba_jizdy)(doba_jizdy);
      Wait(Uniform(3, 7)); // Odpojeni vagonu
      Wait(Uniform(500, 580)); // Navrat vagonu

      int vagon_pocet = 0;

      while(!fronta_na_cesta.Empty()){
        Entity *tmp = fronta_na_cesta.GetFirst();
        tmp->Activate(Time);
        vagon_pocet++;
      }

      (*_prevezene_vagony)(vagon_pocet);

      Leave(lokomotivy2, 1);
    }
  }

  void Output(){
    cout << "strojvedouci2" << endl;
    cout << "cekal prumerne :" << _cekal->MeanValue()/60 << " hodin" << endl;
    cout << "cekal min :" << _cekal->Min()/60 << " hodin" << endl;
    cout << "cekal max :" << _cekal->Max()/60 << " hodin" << endl;
    cout << "cekal celkem :" << _cekal->Sum()/60 << " hodin" << endl;
    cout << "prevezl: " << _prevezene_vagony->Sum() << " vagonu, " << _prevezene_vagony->Sum() * 54
    << " tun cementu celkem" << endl;
    cout << "prumerna doba jizdy: " << _doba_jizdy->MeanValue()/60 << " hodin" << " ("
    << _doba_jizdy->MeanValue() << ")" << endl;

  }
};

class Strojvedouci : public Process {
  public:
  Stat *_prevezene_vagony;
  Stat *_cekani_na_koleji;
  Strojvedouci() {
    _prevezene_vagony = new Stat;
    _cekani_na_koleji = new Stat;
  }

  void Output(){
    cout << "strojvedouci1" << endl;
    cout << "prevezl: " << _prevezene_vagony->Sum() << " vagonu" << endl;
    cout << "cekani na koleji: " << _cekani_na_koleji->MeanValue()/60/24 << " dni" << endl;
    cout << "min: " << _cekani_na_koleji->Min()/60/24 << endl;
    cout << "max: " << _cekani_na_koleji->Max()/60/24 << endl;
    cout << endl;
  }

  void Behavior() {
    Priority = 1;
    
    while(1) {
      Enter(lokomotivy1, 1);
      Wait(Uniform(18, 22));

      //generuje 27 vagonu
      for(unsigned int i = 0; i < pocet_vagonu; i++){
        (new Vagon)->Activate(Time);
      }


      while(fronta_na_ostatni.Length() < pocet_vagonu) {
        Wait(Uniform(3, 7));
        Wait(Uniform(13, 17));

        while(!cekani_na_stroj.Empty()){
          Entity *tmp = cekani_na_stroj.GetFirst();
          tmp->Activate(Time);
        }

        Seize(obsluha, 0);
        
        Wait(Uniform(3, 7));
        Wait(Uniform(8, 12));

        while(!cekani_na_ostatni.Empty()){
          Entity *tmp = cekani_na_ostatni.GetFirst();
          tmp->Activate(Time);
          cekani_na_vagony.Insert(this);
          Passivate();
        }
        
        Release(obsluha);
      }

      (*_prevezene_vagony)(pocet_vagonu);
      
      Leave(lokomotivy1, 1);
      // Cekani na odstavne koleji
      double kolej_cekani = Uniform(20*60, 100*60);
      Wait(kolej_cekani);
      (*_cekani_na_koleji)(kolej_cekani);

      while(!fronta_na_ostatni.Empty()){
        Entity *tmp = fronta_na_ostatni.GetFirst();
        tmp->Activate(Time);
      }

      (new ProcessManager)->Activate(Time);
    }
  }
};


class Generator : public Event {
  public:
  Strojvedouci *strojvedouci;
  Strojvedouci2 *strojvedouci2;
  void Behavior() {
    strojvedouci = new Strojvedouci;
    strojvedouci->Activate(Time);
    strojvedouci2 = new Strojvedouci2;
    strojvedouci2->Activate(Time);
  }

  void Output(){
    strojvedouci->Output();
    strojvedouci2->Output();
    cout << "vagonu s CEM 32,5R: " << cem_32_5r << ", " << cem_32_5r * 54 << " tun" << endl;
    cout << "vagonu s CEM 42,5R: " << cem_42_5r << ", " << cem_42_5r * 54 << " tun" << endl;
    cout << "vagonu s CEM 52,5R: " << cem_52_5r << ", " << cem_52_5r * 54 << " tun" << endl;
  }
};

void print_help(){
  cout << "Napoveda:" << endl;
  cout << "--vagony=x - pocet vagonu x" << endl;
  cout << "--cas=y - delka simulace y (ve dnech)" << endl;
}


int main (int argc, char* argv[])
{
  int cas_konec;

  const struct option longopts[] =
  {
    {"vagony", required_argument, 0, 'v'},
    {"help", no_argument, 0, 'h'},
    {"cas", required_argument, 0, 'c'},
    {0,0,0,0},
  };

  int index;
  int iarg=0;
  bool cas_flag = false, vagony_flag = false;

  opterr=1; 

  while(iarg != -1)
  {
    iarg = getopt_long(argc, argv, "vhc", longopts, &index);

    switch (iarg)
    {
      case 'h':
        print_help();
        return 0;
      case 'v':
        if(strlen(optarg)){
          vagony_flag = true;
        }
        pocet_vagonu = atoi(optarg);
        break;

      case 'c':
        if(strlen(optarg)){
          cas_flag = true;
        }

        cas_konec = atoi(optarg) * 60 * 24;
        break;
    }
  }

  if(vagony_flag == false) {
    cout << "Zadejte pocet vagonu: ";
    cin >> pocet_vagonu;
  }

  if(cas_flag == false) {
    cout << "Zadejte delku simulace (dny): ";
    cin >> cas_konec;
    cas_konec = cas_konec * 60 * 24;
  }

  int start = 0;

  Init(start, cas_konec); // doba simulace
  Generator *generator = new Generator;
  generator->Activate(Time);
  Run(); // start simulace
  generator->Output();
  
  return 0;
}
