#include <iostream>
#include <vector>
#include <iomanip>
#include <queue>
#include <cmath>
#include <bits/stdc++.h>
using namespace std;

#define NIL -1

// ATENÇÃO!
// Você é livre para incluir outras bibliotecas, implementar funções auxiliares, acrecentar definições, tipos e estruturas.

// Esta função deve retornar o valor obtido pela sua solução //
/* Parâmetros: 
    n, m e k são os números de terras, pontes e ingredientes, respecitivamente.
    pontes[m][2] é uma matriz m X 2 indicando as pontes existentes.
    probPontes[j] é a probabilidade da ponte j não cair, para cada ponte j de de 0 a m-1.
    mapaIngredientes[i] é o ingrediente na terra i, para toda terra i de 0 a n-1.
*/


class vertice
{
	public :
	int id;
	double prob;
	int coletados;
	friend bool operator<(const vertice & v1, const vertice &v2){
		return v1.prob < v2.prob;
	}
};


vertice criar_vertice(int id, int coletados, double prob){
	//cria e devolve um novo vertice
	vertice new_vertice;
	new_vertice.id = id;
	new_vertice.coletados = coletados;
	new_vertice.prob = prob;
	return new_vertice;
}

vector<vector<int> > devolver_adjacente_e_idx(int v, int num_pontes, vector<vector<int> > &pontes){
	vector<vector<int> > adjacentes;
	for (int i = 0; i < num_pontes; i++){
		if (pontes[i][0] == v){
			vector<int> encontrado(2);
			encontrado[0] = pontes[i][1];
			encontrado[1] = i;
			adjacentes.push_back(encontrado);
		} else if(pontes[i][1] == v){
			vector<int> encontrado(2);
			encontrado[0] = pontes[i][0];
			encontrado[1] = i;
			adjacentes.push_back(encontrado);
		}
	}
	return adjacentes;
}




double melhorRota(int n, int m, vector<vector<int> > &pontes, vector<double> &probPontes, int k, vector<int> &mapaIngredientes)
{
	double resultado = NIL;
	
	/*
	criar matriz onde o elemento matriz[i][j]
	representa a melhor probabilidade de sucesso 
	de chegar até o vértice i passando pelos elementos
	bits = 1 de j (j funciona como uma bitmask).	
	*/	
	vector<vector<double> > prob(n, vector<double>(pow(2,k), 0));

	priority_queue < vertice > encontrados;
	encontrados.push(criar_vertice(0, 0, 1)); //adiciona o vértice source na fila de prioridade
	/*
	o caminho iniciado no vértice source (id = 0)
	nao passou por nenhum ingrediente (coletado = 000000)
	e tem probabilidade 1 de chegar em si mesmo
	*/
	prob[0][0] = 1; //tem probabilidade 1 de chegar em si mesmo sem pegar nenhum ingrediente
	while (!encontrados.empty()){
		vertice atual = encontrados.top();
		encontrados.pop();
		if (atual.prob < prob[atual.id][atual.coletados]){
			continue;
		}

		vector<vector<int> > adjacentes = devolver_adjacente_e_idx(atual.id, m, pontes);
		for (int i = 0; i < static_cast<int>(adjacentes.size()); i++){
			vector<int> adj = adjacentes[i];
			//adj[0] = id do vértice adjacente
			//adj[1] = indice da ponte desse adjacente

			int coletados = atual.coletados;
			if (mapaIngredientes[adj[0]] != 0){
				coletados = coletados | (1 << (mapaIngredientes[adj[0]] - 1)); 
				//deslocamos o bit 1 num_ingrediente vezes para a esquerda
				/*
				e entao fazemos um or com coletados para salvar encontrados aquele ingrediente
				foi coletado
				*/ 
			}

			double prob_atual = atual.prob * probPontes[adj[1]];

			if (prob_atual > prob[adj[0]][coletados]){
				prob[adj[0]][coletados] = prob_atual;
				encontrados.push(criar_vertice(adj[0], coletados, prob_atual));
			}
		}
	}
	int coletou_todos = pow(2, k);
	resultado = prob[n-1][coletou_todos - 1];
	return resultado;
}

/////////////////////////////////////////////////////
// NÃO MOFIFICAR A PARTIR DAQUI
/* Função que lida com leitura e escrita de valores */
int main()
{
	int n, m, k;
	cin >> n >> m;

	vector<vector<int> > pontes(m, vector<int>(2, NIL));
	vector<int> mapaIngredientes(n, NIL);
	vector<double> probPontes(m, NIL);

	int u, v;
	double p;
	for (int i = 0; i < m; i++)
	{
		cin >> u >> v >> p;
		pontes[i][0] = u;
		pontes[i][1] = v;
		probPontes[i] = p;
	}

	cin >> k;

	int ing;
	for (int i = 0; i < n; i++)
	{
		cin >> ing;
		mapaIngredientes[i] = ing;
	}

	double resultado = NIL;
	resultado = melhorRota(n, m, pontes, probPontes, k, mapaIngredientes);

	cout << fixed << setprecision(5) << resultado << endl;

	return 0;
}