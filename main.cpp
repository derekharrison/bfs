/*
 * main.cpp
 *
 *  Created on: Jul 9, 2021
 *      Author: d-w-h
 */

#include <iostream>
#include <stdio.h>
#include <vector>

#define QUEUE_SIZE 10000
const int inf = 3e+8;

enum ass_flags {
    ASSED = true,
    UNASSED = false
};

enum color_codes {
    WHITE = 0,
    GRAY = 1,
    BLACK = 2
};

typedef struct Node {
    color_codes color = WHITE;
    int d = inf;
    Node* pi = NULL;
    int parent_index = 0;
    int index = 0;
    std::vector<int> adj_nodes;
} node;

typedef struct Queue_array {
    ass_flags assed;
    int value;
    node elem;
} queue_array;

class Queue {
private:
    int tail;
    int head;
    int size;
    queue_array* array;

    void check_overflow();
    void check_underflow();
public:
    Queue();
    void enqueue(node);
    node dequeue();
    void print_queue();
    int get_head();
    int get_tail();
};

Queue::Queue() {
    this->tail = 0;
    this->head = 0;
    this->size = QUEUE_SIZE;

    this->array = new queue_array[QUEUE_SIZE];

    for(int n = 0; n < QUEUE_SIZE; ++n) {
        this->array[n] = {UNASSED, };
    }
}

void Queue::check_overflow() {
    if((this->tail == this->head) && (this->array[this->tail].assed == ASSED)) {
        throw "stack overflow";
    }
    else {};
}

void Queue::check_underflow() {
    if(this->tail == this->head && this->array[this->head].assed == UNASSED) {
        throw "stack underflow";
    }
    else {};
}

void Queue::enqueue(node element) {
    Queue::check_overflow();
    if(this->tail == this->size - 1) {
        this->array[this->tail].elem = element;
        this->array[this->tail].assed = ASSED;
        this->tail = 0;
    }
    else {
        this->array[this->tail].elem = element;
        this->array[this->tail].assed = ASSED;
        this->tail = this->tail + 1;
    }
}

node Queue::dequeue() {
    Queue::check_underflow();
    node element = this->array[this->head].elem;
    if(this->head == this->size - 1) {
        this->array[this->head].assed = UNASSED;
        this->head = 0;
    }
    else {
        this->array[this->head].assed = UNASSED;
        this->head = this->head + 1;
    }

    return element;
}

void Queue::print_queue() {
    for(int n = 0; n < this->size; ++n) {
        printf("%i: %i\tassed: %i\n", n, this->array[n].value, this->array[n].assed);
    }
}

int Queue::get_head() {
    return this->head;
}

int Queue::get_tail() {
    return this->tail;
}

void set_index_map(int size_graph, int* index_map, int* index_map_inverse, int s) {
    index_map[0] = index_map_inverse[0] = 0; //Point to zero for unused element

    int index_track = 1;
    for(int i = s; i <= size_graph; ++i) {
        index_map[i] = index_track;
        index_map_inverse[index_track] = i;
        index_track++;
    }
    for(int i = 1; i <= s - 1; ++i) {
        index_map[i] = index_track;
        index_map_inverse[index_track] = i;
        index_track++;
    }
}

std::vector<int> bfs(int n, int m, std::vector<std::vector<int>> edges, int s) {

    int num_vertices = n;
    int num_edges = m;
    int size_edge = 6; //Weight of an edge

    //Compute index map
    int* index_map = new int[num_vertices+1];
    int* index_map_inverse = new int[num_vertices+1];
    set_index_map(num_vertices, index_map, index_map_inverse, s);

    //Create edges
    std::vector<std::vector<int>> edges_reordered;
    for(int i = 0; i < m; ++i) {
        int start_index = edges[i][0];
        int end_index = edges[i][1];
        edges_reordered.push_back({index_map[start_index], index_map[end_index]});
    }

    //Initialize graph
    node* graph = new node[num_vertices + 1];
    for(int i = 0; i < num_vertices + 1; ++i) {
        graph[i].color = WHITE;
        graph[i].d = inf;
        graph[i].parent_index = 0;
        graph[i].index = i;
        graph[i].pi = NULL;
    }

    graph[1].color = GRAY;
    graph[1].d = 0;
    graph[1].pi = NULL;

    for(int i = 0; i < num_edges; ++i) {
        int start_index = edges_reordered[i][0];
        int end_index = edges_reordered[i][1];
        graph[start_index].adj_nodes.push_back(end_index);
        graph[end_index].adj_nodes.push_back(start_index);
    }

    //Perform bfs algorithm
    Queue Q;
    node node_elem = graph[1];
    try { Q.enqueue(node_elem); }
    catch(std::string s) { std::cout << s << std::endl; }

    int head = Q.get_head();
    int tail = Q.get_tail();
    int size_queue = tail - head;
    while(size_queue > 0) {

        try { node_elem = Q.dequeue(); }
        catch(std::string s) { std::cout << s << std::endl; }

        node* curr_node = &graph[node_elem.index];

        int num_adj_nodes = node_elem.adj_nodes.size();
        for(int i = 0; i < num_adj_nodes; ++i) {
            int adj_node_index = node_elem.adj_nodes[i];
            node* adj_node = &graph[adj_node_index];

            if(adj_node->color == WHITE) {
                adj_node->color = GRAY;
                adj_node->d = curr_node->d + 1;
                adj_node->pi = curr_node;

                try { Q.enqueue(*adj_node); }
                catch(std::string s) { std::cout << s << std::endl; }
            }
        }

        curr_node->color = BLACK;
        head = Q.get_head();
        tail = Q.get_tail();
        size_queue = tail - head;
    }

    //Compute result graph
    for(int i = 1; i < num_vertices + 1; ++i) {
        if(graph[i].d == inf) { graph[i].d = -1; }
        else { graph[i].d = size_edge * graph[i].d; }
    }

    //Reorder results
    std::vector<int> results;
    for(int i = 1; i < num_vertices + 1; ++i) {
        int it = 1;
        int index_inverse = index_map_inverse[graph[it].index];
        while(index_inverse != i) { it++; index_inverse = index_map_inverse[graph[it].index]; }
        int index_tr = index_map[i];
        if(index_inverse == i && i != s) { results.push_back(graph[index_tr].d); }
    }

    return results;
}

int main(int argc, char* argv[]) {

    std::vector<std::vector<int>> edges;
    int num_vertices = 4;
    int num_edges = 2;
    int s = 1; //Start vertex

    //Create edges
    edges.push_back({1, 2});
    edges.push_back({1, 3});

    //Perform breadth first search algorithm
    std::vector<int> results = bfs(num_vertices, num_edges, edges, s);

    //Print results
    int size_results = results.size();
    for(int i = 0; i < size_results; ++i) {
        printf("results[%i]: %i\n", i, results[i]);
    }

    return 0;
}

