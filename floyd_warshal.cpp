// Mohamad Al Mustapha
// Implementation of Floyd-Warshal Algorithm

#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <iterator>
using namespace std;


template <typename T>
struct vertex;

template <typename T>
struct edge
{
	int weight;
	vertex<T> *node;
};

template <typename T>
struct vertex
{
	T id;
	vector <edge<T>> links;
};

template <typename T>
class GRAPH
{
public:
	GRAPH(){};

	GRAPH& read_file(char* filepath)
	{
		ifstream file(filepath);
		if (!file)
		{
			cout << "Error opening file" << endl;
			return *this;
		}

		T from, to;
		int weight;

		//read from_node number first to make sure the file is not empty
		file >> from;
		while (!file.eof())
		{
			//Format: 1 2 10
			file >> to;
			file >> weight;
			add_node(from);
			add_node(to);
			add_edge(from, to, weight);
			file >> from;
		}

		file.close();
		return *this;
	}

	~GRAPH()
	{
		//free the allocated pointers in the hashmap
		map<T, vertex<T>* >::iterator itr = graph.begin();

		while (itr != graph.end())
		{
			delete itr->second;
			++itr;
		}
	}

	//A function to add a node to the graph 
	bool add_node(T id)
	{
		//add the node if it is not already present
		if (graph[id] == false)
		{
			vertex<T> *v;
			v = new vertex<T>;
			v->id = id;
			graph[id] = v;
			return true;
		}
		return false;
	}

	//A function to add an edge between two nodes of a certain weight
	bool add_edge(T from, T to, int weight)
	{
		vertex<T> *a = (graph.find(from))->second;
		if (a == nullptr)
			return false;
		vertex<T> *b = (graph.find(to))->second;
		if (b == nullptr)
			return false;

		edge<T> e;
		e.weight = weight;
		e.node = b;

		a->links.push_back(e);
		return true;
	}

	//a function to return the number of current nodes in a graph
	size_t size()
	{
		return graph.size();
	}

	//A function to print out all-shortest-paths using Floyd Warshal Algorithm
	void shortest_paths()
	{
		size_t n = size();

		//construct D(i,j) and via(i,j)
		int **D = new int *[n];
		int **via = new int *[n];

		for (size_t i = 0; i < n; ++i)
		{
			D[i] = new int[n];
			via[i] = new int[n];

			//initialize D(i,j) to +inf and via(i,j) to null
			for (size_t j = 0; j < n; ++j)
			{
				D[i][j] = INT_MAX;
				via[i][j] = NULL;
			}
			//distance from a node to itself is zero
			D[i][i] = 0;
		}

		//initialize D with the proper weights if the edges exist
		map<T, vertex<T> *>::iterator itr = graph.begin();
		while (itr != graph.end())
		{
			size_t num_adj = itr->second->links.size();
			for (size_t i = 0; i < num_adj; ++i)
			{
				D[itr->first][itr->second->links[i].node->id] = itr->second->links[i].weight;
			}
			++itr;
		}

		//apply dynamic programming
		for (size_t k = 0; k < n; ++k)
		{
			for (size_t i = 0; i < n; ++i)
			{
				for (size_t j = 0; j < n; ++j)
				{
					//to prevent buffer overflow, check if at least one of the numbers is inf
					int temp_weight = (D[i][k] == INT_MAX || D[k][j] == INT_MAX) ? INT_MAX : D[i][k] + D[k][j];
					if (D[i][j] > temp_weight)
					{
						D[i][j] = temp_weight;
						via[i][j] = k;
					}
				}
			}
		}

		//check for negative weight cycles
		for (size_t i = 0; i < n; ++i)
		{
			// a cycle exists if the distance from a node to itself becomes less than the initial zero value
			if (D[i][i]<0)
			{
				cout << "This graph has a negative-weight cycle at node " << i << endl;
				return;
			}
		}

		//print solution
		for (size_t i = 0; i < n; ++i)
		{
			for (size_t j = 0; j < n; ++j)
			{
				//don't print the path from a node to itself
				if (i != j)
				{
					cout << i << "->" << j << ":\n";
					if (D[i][j] == INT_MAX)
						cout << "No path exists.\n";
					else
					{
						cout << "Cost: " << D[i][j] << endl;
						cout << "Path: ";
						print_path(via, i, j);
						cout << j << endl;
					}
					cout << endl;
				}
			}
		}

		//free the allocated memory
		for (size_t i = 0; i < n; ++i)
		{
			delete[] D[i];
			delete[] via[i];
		}
		delete[] D;
		delete[] via;
	}

private:
	map<T, vertex<T> *> graph;

	void print_path(int**via, int i, int j)
	{
		if (via[i][j] == NULL)
		{
			cout << i << " ";
		}
		else
		{
			print_path(via, i, via[i][j]);
			print_path(via, via[i][j], j);
		}
	}
};


#endif