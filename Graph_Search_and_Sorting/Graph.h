/*
 * Graph.h
 */
#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <queue>
using namespace std;

template <class T> class Edge;
template <class T> class Graph;
template <class T> class Vertex;


/****************** Provided structures  ********************/

template <class T>
class Vertex {
	T info;                // contents
	vector<Edge<T> > adj;  // list of outgoing edges
	bool visited;          // auxiliary field used by dfs and bfs
	int indegree;          // auxiliary field used by topsort
	bool processing;       // auxiliary field used by isDAG

	void addEdge(Vertex<T> *dest, double w);
	bool removeEdgeTo(Vertex<T> *d);
public:
	Vertex(T in);
	friend class Graph<T>;
};

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
class Graph {
	vector<Vertex<T> *> vertexSet;    // vertex set

	void dfsVisit(Vertex<T> *v,  vector<T> & res) const;
	Vertex<T> *findVertex(const T &in) const;
	bool dfsIsDAG(Vertex<T> *v) const;
public:
	int getNumVertex() const;
	bool addVertex(const T &in);
	bool removeVertex(const T &in);
	bool addEdge(const T &sourc, const T &dest, double w);
	bool removeEdge(const T &sourc, const T &dest);
	vector<T> dfs() const;
	vector<T> bfs(const T &source) const;
	vector<T> topsort() const;
	int maxNewChildren(const T &source, T &inf) const;
	bool isDAG() const;
};

/****************** Provided constructors and functions ********************/

template <class T>
Vertex<T>::Vertex(T in): info(in) {}

template <class T>
Edge<T>::Edge(Vertex<T> *d, double w): dest(d), weight(w) {}


template <class T>
int Graph<T>::getNumVertex() const {
	return vertexSet.size();
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

/****************** 1a) addVertex ********************/

/*
 *  Adds a vertex with a given content/info (in) to a graph (this).
 *  Returns true if successful, and false if a vertex with that content already exists.
 */
template <class T>
bool Graph<T>::addVertex(const T &in) {
	if (findVertex(in) != NULL) return false;
	this->vertexSet.push_back(new Vertex<T>(in));
	return true;
}

/****************** 1b) addEdge ********************/

/*
 * Adds an edge to a graph (this), given the contents of the source (sourc) and
 * destination (dest) vertices and the edge weight (w).
 * Returns true if successful, and false if the source or destination vertex does not exist.
 */
template <class T>
bool Graph<T>::addEdge(const T &sourc, const T &dest, double w) {
	Vertex<T> * src = findVertex(sourc);
	Vertex<T> * des = findVertex(dest);
	if (src == NULL || des == NULL) return false;
	src->addEdge(des, w);
	return true;

}

/*
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 */
template <class T>
void Vertex<T>::addEdge(Vertex<T> *d, double w) {
	adj.push_back(Edge<T>(d, w));
}


/****************** 1c) removeEdge ********************/

/*
 * Removes an edge from a graph (this).
 * The edge is identified by the source (sourc) and destination (dest) contents.
 * Returns true if successful, and false if such edge does not exist.
 */
template <class T>
bool Graph<T>::removeEdge(const T &sourc, const T &dest) {
	Vertex<T> * src = findVertex(sourc);
	Vertex<T> * des = findVertex(dest);
	if (src == NULL || des == NULL) return false;
	return src->removeEdgeTo(des);
	return true;
}

/*
 * Auxiliary function to remove an outgoing edge (with a given destination (d))
 * from a vertex (this).
 * Returns true if successful, and false if such edge does not exist.
 */
template <class T>
bool Vertex<T>::removeEdgeTo(Vertex<T> *d) {
	for (size_t i = 0; i < adj.size(); i++) {
		if (adj.at(i).dest->info == d->info) {
			adj.erase(adj.begin()+i);
			return true;
		}
	}
	return false;
}


/****************** 1d) removeVertex ********************/

/*
 *  Removes a vertex with a given content (in) from a graph (this), and
 *  all outgoing and incoming edges.
 *  Returns true if successful, and false if such vertex does not exist.
 */
template <class T>
bool Graph<T>::removeVertex(const T &in) {
	Vertex<T> * v = findVertex(in);
	if (v == NULL) return false;
	for (size_t i = 0; i < vertexSet.size(); i++)
		vertexSet.at(i)->removeEdgeTo(v);
	for (auto it = vertexSet.begin(); it != vertexSet.end(); it++) {
		if ((*it)->info == in) {
			vertexSet.erase(it);
			break;
		}
	}
	return true;
}


/****************** 2a) dfs ********************/

/*
 * Performs a depth-first search (dfs) in a graph (this).
 * Returns a vector with the contents of the vertices by dfs order.
 * Follows the algorithm described in theoretical classes.
 */
template <class T>
vector<T> Graph<T>::dfs() const {
	vector<T> res;
	for (Vertex<T> * vertex : vertexSet) vertex->visited = false;
	for (Vertex<T> * vertex : vertexSet)
		if (!vertex->visited)
			dfsVisit(vertex, res);
	return res;
}

/*
 * Auxiliary function that visits a vertex (v) and its adjacent not yet visited, recursively.
 * Updates a parameter with the list of visited node contents.
 */
template <class T>
void Graph<T>::dfsVisit(Vertex<T> *v, vector<T> & res) const {
	v->visited = true;
	res.push_back(v->info);
	for (Edge<T> & edge : v->adj)
		if (!edge.dest->visited)
			dfsVisit(edge.dest, res);

}

/****************** 2b) bfs ********************/

/*
 * Performs a breadth-first search (bfs) in a graph (this), starting
 * from the vertex with the given source contents (source).
 * Returns a vector with the contents of the vertices by dfs order.
 * Follows the algorithm described in theoretical classes.
 */
template <class T>
vector<T> Graph<T>::bfs(const T & source) const {
	vector<T> res;
	queue<Vertex<T> *> to_visit;

	for (Vertex<T> * vertex : vertexSet) vertex->visited = false;

	to_visit.push(vertexSet.at(0));
	vertexSet.at(0)->visited = true;

	while(!to_visit.empty()) {
		Vertex<T> * next = to_visit.front();
		to_visit.pop();
		res.push_back(next->info);

		for(Edge<T> & edge : next->adj) {
			if (!edge.dest->visited) {
				to_visit.push(edge.dest);
				edge.dest->visited = true;
			}
		}

	}
	return res;
}

/****************** 2c) toposort ********************/

/*
 * Performs a topological sorting of the vertices of a graph (this).
 * Returns a vector with the contents of the vertices by topological order.
 * If the graph has cycles, returns an empty vector.
 * Follows the algorithm described in theoretical classes.
 */

template<class T>
vector<T> Graph<T>::topsort() const {
	// TODO (26 lines)
	vector<T> res;
	for (Vertex<T> * vertex : vertexSet) vertex->indegree = 0;
	for (Vertex<T> * vertex : vertexSet)
		for (Edge<T>& edge : vertex->adj)
			edge.dest->indegree++;

	queue<Vertex<T> *> C;
	for (Vertex<T> * vertex : vertexSet)
		if (vertex->indegree == 0)
			C.push(vertex);

	while(!C.empty()) {
		Vertex<T> * v = C.front();
		C.pop();
		res.push_back(v->info);
		for (Edge<T>& edge : v->adj) {
			edge.dest->indegree--;
			if (edge.dest->indegree == 0)
				C.push(edge.dest);
		}
	}

	return res.size() != vertexSet.size() ? vector<T>() : res;
}

/****************** 3a) maxNewChildren (HOME WORK)  ********************/

/*
 * Performs a breadth-first search in a graph (this), starting
 * from the vertex with the given source contents (source).
 * During the search, determines the vertex that has a maximum number
 * of new children (adjacent not previously visited), and returns the
 * contents of that vertex (inf) and the number of new children (return value).
 */

template <class T>
int Graph<T>::maxNewChildren(const T & source, T &inf) const {

	int max_childs = 0;
	vector<T> res;
	queue<Vertex<T> *> to_visit;

	for (Vertex<T> * vertex : vertexSet) vertex->visited = false;

	to_visit.push(vertexSet.at(0));
	vertexSet.at(0)->visited = true;

	while(!to_visit.empty()) {
		Vertex<T> * next = to_visit.front();
		to_visit.pop();
		res.push_back(next->info);

		int new_childs = 0;
		for(Edge<T> & edge : next->adj) {
			if (!edge.dest->visited) {
				to_visit.push(edge.dest);
				edge.dest->visited = true;
				new_childs++;
			}
		}
		if (new_childs > max_childs) {
			max_childs = new_childs;
			inf = next->info;
		}

	}
	return max_childs;
}

/****************** 3b) isDAG   (HOME WORK)  ********************/

/*
 * Performs a depth-first search in a graph (this), to determine if the graph
 * is acyclic (acyclic directed graph or DAG).
 * During the search, a cycle is found if an edge connects to a vertex
 * that is being processed in the the stack of recursive calls (see theoretical classes).
 * Returns true if the graph is acyclic, and false otherwise.
 */

template <class T>
bool Graph<T>::isDAG() const {
	for (Vertex<T>* vertex : vertexSet) {
		vertex->visited = false;
		vertex->processing = false;
	}
	for (Edge<T>& edge : vertexSet.at(0)->adj) {
		if (edge.dest->processing) return false;

		if (!edge.dest->visited) {
			edge.dest->processing = true;
			if (!dfsIsDAG(edge.dest)) return false;
		}
	}
	return true;
}

/**
 * Auxiliary function that visits a vertex (v) and its adjacent not yet visited, recursively.
 * Returns false (not acyclic) if an edge to a vertex in the stack is found.
 */
template <class T>
bool Graph<T>::dfsIsDAG(Vertex<T> *v) const {
	v->visited = true;

	for (Edge<T>& edge : v->adj) {
		if (edge.dest->processing) return false;
		if (!edge.dest->visited) {
			edge.dest->processing = true;
			if (!dfsIsDAG(edge.dest)) return false;
		}
	}

	return true;
}

#endif /* GRAPH_H_ */
