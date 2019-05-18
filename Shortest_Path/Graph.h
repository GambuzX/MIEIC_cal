/*
 * Graph.h
 */
#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <queue>
#include <list>
#include <limits>
#include <cmath>
#include "MutablePriorityQueue.h"

using namespace std;

template <class T> class Edge;
template <class T> class Graph;
template <class T> class Vertex;

#define INF std::numeric_limits<double>::max()

/************************* Vertex  **************************/

template <class T>
class Vertex {
	T info;                // contents
	vector<Edge<T> > adj;  // outgoing edges
	bool visited;          // auxiliary field
	double dist = 0;
	Vertex<T> *path = NULL;
	int queueIndex = 0; 		// required by MutablePriorityQueue

	bool processing = false;
	void addEdge(Vertex<T> *dest, double w);

public:
	Vertex(T in);
	bool operator<(Vertex<T> & vertex) const; // // required by MutablePriorityQueue
	T getInfo() const;
	double getDist() const;
	Vertex *getPath() const;
	friend class Graph<T>;
	friend class MutablePriorityQueue<Vertex<T>>;
};


template <class T>
Vertex<T>::Vertex(T in): info(in) {}

/*
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 */
template <class T>
void Vertex<T>::addEdge(Vertex<T> *d, double w) {
	adj.push_back(Edge<T>(d, w));
}

template <class T>
bool Vertex<T>::operator<(Vertex<T> & vertex) const {
	return this->dist < vertex.dist;
}

template <class T>
T Vertex<T>::getInfo() const {
	return this->info;
}

template <class T>
double Vertex<T>::getDist() const {
	return this->dist;
}

template <class T>
Vertex<T> *Vertex<T>::getPath() const {
	return this->path;
}

/********************** Edge  ****************************/

template <class T>
class Edge {
	Vertex<T> * dest;      // destination vertex
	double weight;         // edge weight
public:
	Edge(Vertex<T> *d, double w);
	friend class Graph<T>;
	friend class Vertex<T>;
};

template <class T>
Edge<T>::Edge(Vertex<T> *d, double w): dest(d), weight(w) {}


/*************************** Graph  **************************/

template <class T>
class Graph {
	vector<Vertex<T> *> vertexSet;    // vertex set
	vector<vector<double>> minDist;
	vector<vector<Vertex<T>*>> next;

public:
	Vertex<T> *findVertex(const T &in) const;
	bool addVertex(const T &in);
	bool addEdge(const T &sourc, const T &dest, double w);
	int getNumVertex() const;
	vector<Vertex<T> *> getVertexSet() const;

	// Fp05 - single source
	void dijkstraShortestPath(const T &s);
	void dijkstraShortestPathOld(const T &s);
	void unweightedShortestPath(const T &s);
	void bellmanFordShortestPath(const T &s);
	vector<T> getPath(const T &origin, const T &dest) const;

	// Fp05 - all pairs
	void floydWarshallShortestPath();
	vector<T> getfloydWarshallPath(const T &origin, const T &dest) const;

};

template <class T>
int Graph<T>::getNumVertex() const {
	return vertexSet.size();
}

template <class T>
vector<Vertex<T> *> Graph<T>::getVertexSet() const {
	return vertexSet;
}

/*
 * Auxiliary function to find a vertex with a given content.
 */
template <class T>
Vertex<T> * Graph<T>::findVertex(const T &in) const {
	for (auto v : vertexSet)
		if (v->info == in)
			return v;
	return NULL;
}

/*
 *  Adds a vertex with a given content or info (in) to a graph (this).
 *  Returns true if successful, and false if a vertex with that content already exists.
 */
template <class T>
bool Graph<T>::addVertex(const T &in) {
	if ( findVertex(in) != NULL)
		return false;
	vertexSet.push_back(new Vertex<T>(in));
	return true;
}

/*
 * Adds an edge to a graph (this), given the contents of the source and
 * destination vertices and the edge weight (w).
 * Returns true if successful, and false if the source or destination vertex does not exist.
 */
template <class T>
bool Graph<T>::addEdge(const T &sourc, const T &dest, double w) {
	auto v1 = findVertex(sourc);
	auto v2 = findVertex(dest);
	if (v1 == NULL || v2 == NULL)
		return false;
	v1->addEdge(v2,w);
	return true;
}


/**************** Single Source Shortest Path algorithms ************/

template<class T>
void Graph<T>::dijkstraShortestPath(const T &origin) {
	const int infinite = 99999;
	for (Vertex<T> * vertex : vertexSet) {
		vertex->dist = infinite;
		vertex->path = NULL;
		vertex->visited = false;
		vertex->processing = false;
	}

	Vertex<T> * start = findVertex(origin);
	start->dist = 0;
	MutablePriorityQueue<Vertex<T>> minQueue;
	minQueue.insert(start);

	while(!minQueue.empty()) {

		Vertex<T> * curr = minQueue.extractMin();
		curr->visited = true;

		for (const Edge<T> & edge : curr->adj) {
			Vertex<T> * next = edge.dest;
			if (next->visited) continue;
			if (next->dist > curr->dist + edge.weight) {
				next->dist = curr->dist + edge.weight;
				next->path = curr;

				if (!next->processing) {
					minQueue.insert(next);
					next->processing = true;
				}
				else {
					minQueue.decreaseKey(next);
				}
			}
		}
	}
}

template<class T>
vector<T> Graph<T>::getPath(const T &origin, const T &dest) const{
	vector<T> res;

	Vertex<T> * start = findVertex(origin);
	Vertex<T> * end = findVertex(dest);

	while(end->info != start->info) {
		res.insert(res.begin(), end->info);
		end = end->path;
	}
	res.insert(res.begin(), end->info);

	return res;
}

template<class T>
void Graph<T>::unweightedShortestPath(const T &orig) {
	const int infinite = 99999;
	for (Vertex<T> * vertex : vertexSet) {
		vertex->dist = infinite;
		vertex->path = NULL;
	}

	Vertex<T> * start = findVertex(orig);
	start->dist = 0;
	queue<Vertex<T>*> vertexQ;
	vertexQ.push(start);

	while(!vertexQ.empty()) {

		Vertex<T> * curr = vertexQ.front();
		vertexQ.pop();

		for (const Edge<T> & edge : curr->adj) {
			Vertex<T> * next = edge.dest;
			if (next->dist == infinite) {
				next->dist = curr->dist + 1;
				next->path = curr;

				vertexQ.push(next);
			}
		}
	}

}

template<class T>
void Graph<T>::bellmanFordShortestPath(const T &orig) {
	const int infinite = 99999;
	for (Vertex<T> * vertex : vertexSet) {
		vertex->dist = infinite;
		vertex->path = NULL;
	}

	Vertex<T> * start = findVertex(orig);
	start->dist = 0;

	for (size_t i = 0; i < vertexSet.size()-1; i++) {

		for (Vertex<T> * vertex : vertexSet) {
			for (const Edge<T> & edge : vertex->adj) {
				Vertex<T> * next = edge.dest;
				if (next->dist > vertex->dist + edge.weight) {
					next->dist = vertex->dist + edge.weight;
					next->path = vertex;
				}
			}
		}

	}

	for (Vertex<T> * vertex : vertexSet) {
		for (const Edge<T> & edge : vertex->adj) {
			Vertex<T> * next = edge.dest;
			if (next->dist > vertex->dist + edge.weight) {
				cout << "Cycles of negative weight" << endl;
				return;
			}
		}
	}

}


/**************** All Pairs Shortest Path  ***************/

template<class T>
void Graph<T>::floydWarshallShortestPath() {
	const int infinity = 999999;
	int nVert = vertexSet.size();

	minDist.resize(nVert);
	for (auto & ele : minDist) ele.resize(nVert);

	next.resize(nVert);
	for (auto & ele : next) ele.resize(nVert);

	for (int i = 0; i < nVert; i++) {
		for (int j = 0; j < nVert; j++) {
			minDist.at(i).at(j) = infinity;
			next.at(i).at(j) = NULL;
		}
	}

	for (Vertex<T> * vertex : vertexSet) {
		for (const Edge<T> & edge : vertex->adj) {
			int i1=-1, i2=-1;
			for (int i = 0; i < nVert; i++)
				if (vertexSet.at(i)->info == vertex->info)
					i1 = i;
			for (int i = 0; i < nVert; i++)
				if (vertexSet.at(i)->info == edge.dest->info)
					i2 = i;

			minDist.at(i1).at(i2) = edge.weight;
			next.at(i1).at(i2) = edge.dest;
		}
	}

	for (int i = 0 ; i < nVert; i++) {
		minDist.at(i).at(i) = 0;
		next.at(i).at(i) = vertexSet.at(i);
	}

	for (int k = 0 ; k < nVert; k++) {
		for (int i = 0; i < nVert; i++) {
			for (int j = 0 ; j < nVert; j++) {
				if (minDist.at(i).at(j) > minDist.at(i).at(k) + minDist.at(k).at(j)) {
					minDist.at(i).at(j) =  minDist.at(i).at(k) + minDist.at(k).at(j);
					next.at(i).at(j) = next.at(i).at(k);
				}
			}
		}
	}

}

template<class T>
vector<T> Graph<T>::getfloydWarshallPath(const T &orig, const T &dest) const{
	vector<T> res;
	int nVert = vertexSet.size();

	Vertex<T> * v1, * v2;
	v1 = findVertex(orig);
	v2 = findVertex(dest);
	if (v1 == NULL || v2 == NULL) return res;

	int i1, i2;
	for (int i = 0; i < nVert; i++)
		if (vertexSet.at(i)->info == v1->info)
			i1 = i;
	for (int i = 0; i < nVert; i++)
		if (vertexSet.at(i)->info == v2->info)
			i2 = i;


	if (next.at(i1).at(i2) == NULL) return res;
	res.push_back(v1->info);
	while(v1->info != v2->info) {
		v1 = next.at(i1).at(i2);
		res.push_back(v1->info);
		for (int i = 0; i < nVert; i++)
			if (vertexSet.at(i)->info == v1->info)
				i1 = i;
	}
	return res;
}


#endif /* GRAPH_H_ */
