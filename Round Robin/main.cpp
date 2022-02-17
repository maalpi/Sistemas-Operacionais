#include <unistd.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include <cstring>
#include <stdio.h>

using namespace std;

int inic = 0, fim = 6; // definindo o tamanho maximo e em qual posicao iniciara a lista

struct process{ // criando uma struct para armazenar os valores do arquivo
  string nome;
  int tempo,aux;
};typedef struct process processos;processos lista[6];


void imprimir(); // funcao para imprimir os elementos do arquivos
void processamento(); //funcao do round robin

int main(void)
{
  char palav[5];
  ifstream fin("arq.txt",ios::in); // pegando os dados
  if (!fin){
    return false;
  }
  
  while(fin >> palav)
  {
    char* word =strtok(palav, ";"); // usando o strtok()
    lista[inic].nome = word; // salvando os dados do processo na struct
    word = strtok(NULL, " ");
    lista[inic].tempo = atoi(word);
    inic++;
  }
  
  fin.close();
 
 //imprimir();
  processamento(); // iniciando round robin
 
 return 0;

}

void imprimir(){

    if(inic==0){
        printf("Erro, Fila vazia\n");
    }
    else{
        for(int i=0; i<inic; i++){
            cout << lista[i].nome <<" " << lista[i].tempo << endl;
  
        }
    }

}

void processamento() // round robin
{
    int quantum = 3; // valor do quantum
    int contfila = inic;  // contagem de processos que estão na fila
    int posic_inic = 0; // primeira posição da fila
    while(contfila != 0)
    {

      while(lista[posic_inic].tempo <= 0) // retira processos zerados
        { 
          posic_inic++;
          if(posic_inic>= fim) // Faz a rotação e volta a fila para posicao 0
              posic_inic=0;
        }

      cout << "---------------------------------" << endl;
      cout << "O processo " << lista[posic_inic].nome << " está executando." << endl;
      cout << "Ficou " << quantum << " segundos executando" << endl;
        

      lista[posic_inic].aux = lista[posic_inic].tempo; // atribuindo o tempo no aux para poder executar o tempo de maneira precisa antes de eliminar o processo da lista. 
      lista[posic_inic].tempo = lista[posic_inic].tempo-quantum; // Subtraindo o quantum

   

      if( lista[posic_inic].tempo<=0)
      {
        sleep(1*lista[posic_inic].aux); // tempo de execucao do processo
        cout << "Faltam "<<lista[posic_inic].aux <<" segundos para o fim da execução de "<<lista[posic_inic].nome<<endl;
        cout << lista[posic_inic].nome  << " terminou o processo." << endl;
        contfila--; // diminuindo o tamanho da lista(-1 elem.)
      }

      else
      {
        cout << "Tempo de execucao restante: " << lista[posic_inic].tempo << endl;
        sleep(1*quantum); // tempo de execucao do processo (quantum)
      }
        
      posic_inic++; // atualiza primeira posição
      if(posic_inic>fim)
        posic_inic=0; // faz a rotação
    
    }

  cout << endl << "Fim do programa." << endl;
}