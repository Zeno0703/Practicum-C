#ifndef DIGRAPH_H
#define DIGRAPH_H

#include <stdbool.h>

/* Type representing an edge in a directed graph. */
typedef struct edge_s
{
  /* Points to the next edge when this edge is part of a linked list. */
  struct edge_s *next;

  unsigned tail;    /* The tail of this edge. */
  unsigned head;    /* The head of this edge. */
  unsigned weight;  /* The weight of this edge. */
} edge_t;

typedef struct adjacency_list_s
{
  edge_t *first; /* Pointer to the first element of the adjacency list */
} adjacency_list_t;

/* Type representing a graph */
typedef struct graph_s
{
  unsigned vertex_count; /* Number of vertices in this graph. */
  unsigned edge_count;   /* Number of edges in this graph. */

  /* Pointer to the first element of an array of adjacency lists. The array
   * is indexed by vertex number
   */
  adjacency_list_t *adjacency_lists;
} graph_t;

/* edge_to_string()
 *
 * Stores the string representation of the given edge 'edge' into the character
 * string (a memory region) pointed to by 'str' according to the format
 * specified below. The variable 'size' specifies the available size of the
 * given memory region specified in number of characters.
 *
 * Format: <tail> "->" <head> "("<weight>")"
 *
 * Additional constraints are:
 *   - always use two places for numbers
 *   - numbers must be right aligned
 *   - weights less then ten must have leading zeros
 * 
 * You must use snprintf to implement this. snprintf is one of the functions
 * that are provided by the C standard library. "man snprintf" will give you
 * access to its extensive documentation.
 * 
 * Example: An edge with tail 2, head 15, and weight 7 has the following
 *          string representation according to the format described above:
 *          " 2 -> 15 (07)"
 *          Note that the tail is preceded by a space and the weight by a zero.
 *
 * PRECONDITIONS: 
 *   - edge != NULL
 *   - str != NULL
 *   - for every i in 0..size, (str+i) points to valid memory
 *   - size > 0
 */
void edge_to_string(const edge_t *edge, char *str, unsigned size);

/* list_is_empty()
 * 
 * Returns true if the given list is empty, false otherwise.
 *
 * The time-complexity of the implementation must be constant time.
 *
 * PRECONDITIONS:
 *   - list != NULL
 */
bool list_is_empty(const adjacency_list_t *list);

/* list_size() 
 *
 * Returns the number of elements in the given list 
 * 
 * PRECONDITIONS:
 *   - list != NULL
 */
unsigned list_size(const adjacency_list_t *list);

/* list_prepend() 
 *
 * Inserts the given edge at the front of the given list 
 *
 * PRECONDITIONS:
 *   - list != NULL
 *   - edge != NULL
 */
void list_prepend(adjacency_list_t *list, edge_t *edge);

/* list_contains()
 * 
 * Returns true if the given adjacency list contains an edge with the given
 * tail and the given head. Returns false otherwise.
 *
 * PRECONDITIONS:
 *   - list != NULL
 */
bool list_contains(const adjacency_list_t *list, unsigned head, unsigned tail);

/* graph_initialise() 
 *
 * Initialises a graph with vertex_count number of vertices.
 *
 * This function needs to dynamically allocate the necessary memory for 
 * vertex_count adjacency_lists. The amount of memory to allocate depends on
 * the vertex_count parameter.
 *
 * Returns false when the dynamic allocation fails. Returns true otherwise.
 *
 * PRECONDITIONS: 
 *  - graph != NULL
 *
 * POSTCONDITIONS:
 *  - when the dynamic memory allocation succeeds
 *    - valid memory has been allocated for all the adjacency_lists
 *    - all the member variables are correctly initialised
 *
 * NOTE: Don't forget to initialise each adjacency list !
 */ 
bool graph_initialise(graph_t *graph, unsigned vertex_count);

/* graph_print()
 *
 * Prints the given graph to the standard output stream.
 *
 * First the number of vertices and the number of edges must be printed
 * according to the following format:
 *
 * "Graph with " <vertex_count> " vertices and " <edge_count> " edges:"
 *
 * then a newline,
 *
 * and then for every vertex, its identifier must be printed, in the following
 * way
 *
 * "vertex" <id>:
 *
 * followed by all the edges whose tail is the vertex. Every edge must be
 * printed on a separate line, preceded by two spaces, according to the
 * format that is specified in the documentation of the edge_to_string
 * function above. To avoid unnecessary code duplication, you must accomplish
 * this by calling edge_to_string.
 *
 * Example: A graph with two vertices and one edge with tail 1, head 0 and
 *          weight 0 must be printed as follows:
 *
 * Graph with 2 vertices and 1 edges:
 * vertex 0:
 * vertex 1:
 *    1 ->  0 (00)
 *
 * PRECONDITIONS: 
 *  - graph != NULL
 */
void graph_print(const graph_t *graph);

/* graph_release() 
 *
 * Releases the memory that was previously allocated by calls to 
 * graph_initialise and graph_connect on this graph. This function also 
 * updates the member fields of the given graph to represent an empty graph.
 *
 * PRECONDITIONS: 
 *  - graph != NULL
 *  - the memory used by the given graph is valid and was previously allocated
 *    via calls to graph_initialise and graph_connect
 *
 * POSTCONDITIONS
 *  - the memory previously occupied is made available again for allocation
 *  - vertex_count == 0
 *  - edge_count == 0
 *  - adjacency_lists == NULL
 */
void graph_release(graph_t *graph);

/* graph_connect()
 *
 * Allocates memory for a new edge and adds it to the given graph.
 *
 * This function initialises the newly allocated edge such that 1) the vertex
 * identified by the parameter 'tail' connects the vertex identified by the
 * parameter 'head', 2) tail points to head, and 3) the edge is weighted
 * with the given weight.
 *
 * The new edge must be put *in front* of the correct adjacency list.
 *
 * Returns false when the dynamic memory allocation fails or when the vertices
 * do not exist in the graph. Returns true otherwise.
 *
 * PRECONDITIONS:
 *  - graph != NULL
 *  - tail < vertex_count
 *  - head < vertex_count
 *  - graph is properly intialised
 *
 * POSTCONDITIONS:
 *  - when the dynamic memory allocation succeeds
 *    edge_count reflects the new total number of edges in the given graph
 */
bool
graph_connect(graph_t *graph, unsigned tail, unsigned head, unsigned weight);

/* graph_disconnect()
 *
 * Removes all edges with the given tail and the given head from the given
 * graph.
 *
 * The memory that was allocated by a previous call to graph_connect must
 * be released for every edge that is removed from the graph.
 *
 * PRECONDITIONS:
 *   graph != NULL
 *
 */
void graph_disconnect(graph_t *graph, unsigned tail, unsigned head);

/* graph_indegree()
 * 
 * Returns the indegree of the vertex with the given identifier in the given
 * graph. The indegree of a vertex in a directed graph is the number of 
 * incoming edges. Returns 0 if the given id does not represent a vertex
 * in the given graph.
 * 
 * PRECONDITIONS:
 *   graph != NULL
 */
unsigned graph_indegree(const graph_t *graph, unsigned id);

/* graph_outdegree() 
 *
 * Returns the outdegree of the vertex with the given identifier in the given
 * graph. The outdegree of a vertex in a directed graph is the number of
 * outgoing edges. Returns 0 if the given id does not represent a vertex
 * in the given graph.
 * 
 * PRECONDITIONS:
 *   graph != NULL
 */
unsigned graph_outdegree(const graph_t *graph, unsigned id);

/* graph_build_from_file()
 *
 * Initalises and populates the given graph based on the configuration
 * that is described in the file whose name is the string pointed to by 
 * pathname.
 *
 * PRECONDITIONS:
 *   graph != NULL
 *   pathname != NULL
 *
 * REMARKS:
 *  - This function exists for testing purposes as it provides a convenient way
 *    to create graphs. It might crash on bad input.
 *  - This function only works if the implementation of the following functions
 *     is correct:
 *      - graph_initialise
 *      - graph_connect
 *
 * NOTE: See the file graph1.txt for an example
 */
void graph_build_from_file(graph_t *graph, const char *pathname);

/* graph_to_dot()
 * 
 * Builds a dot representation of the given graph and saves it
 * to the file with the given name. 
 *
 * See "https://graphviz.org" and the manual page for dot for more information
 * on the dot format.
 *
 * You can vizualize the graph by opening the generated dot file from a 
 * Linux terminal with the 'xdot' tool or you can use an online tool such as
 *    http://magjac.com/graphviz-visual-editor/
 *  or
 *    https://dreampuf.github.io/GraphvizOnline/
 *  to do so.
 *
 * PRECONDITIONS:
 *   graph != NULL
 *   pathname != NULL
 */
void graph_to_dot(const graph_t *graph, const char *pathname);

#endif /* DIGRAPH_H */
