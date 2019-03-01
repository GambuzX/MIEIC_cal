/*
 * Change.cpp
 */

#include "Change.h"


/* FORMALIZACAO PROBLEMA
 *
 * ENTRADAS:
 * m: valor do troco a obter, em centimos
 * numCoints: numero de valores de moeda diferentes disponiveis
 * coinValues: lista de diferentes moedas disponiveis
 *
 * DADOS DE TRABALHO, NO FINAL DE CADA ITERACAO i (usando apenas i moedas):
 * minCoins: numero minimo de moedas
 * lastCoin: valor da ultima moeda *
 *
 * DADOS DE SAIDA:
 * retCoins: moedas utilizadas na solucao *
 *
 */


string calcChange(int m, int numCoins, int *coinValues)
{
	int minCoins[m+1] = {0};
	int lastCoin[m+1] = {0};

	for (int i = 0; i < numCoins; i++) {
		for (int k = 0; k <= m; k++) {
			if ((coinValues[i] <= k) && (minCoins[k - coinValues[i]] + 1 < minCoins[k] || minCoins[k] == 0)) {
				minCoins[k] = minCoins[k - coinValues[i]] + 1;
				lastCoin[k] = coinValues[i];
			}
		}
	}

	string ret_string = "";
	for (int total = m; total > 0; total -= lastCoin[total]) {
		ret_string += to_string(lastCoin[total]) + ";";

		if (lastCoin[total] == 0) {
			ret_string = "-";
			break;
		}
	}

    return ret_string;
}


