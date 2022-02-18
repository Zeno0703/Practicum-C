#include <stdio.h>

#include "graph.h"
#include "test.h"

int main(void)
{
  /* 1. Run ta tests */
  fprintf(stdout, "--------------------------------------------------------\n");
  fprintf(stdout, "TA tests\n");
  fprintf(stdout, "--------------------------------------------------------\n");
  ta_test();

  /* 2. How to use the graph API */
  fprintf(stdout, "\n");
  fprintf(stdout, "--------------------------------------------------------\n");
  fprintf(stdout, "Example code\n");
  fprintf(stdout, "--------------------------------------------------------\n");

  /* 2.1 Create a directed graph that contains four vertices. */
  graph_t graph;

  if (graph_initialise(&graph, 4))
  {
    /* 2.2 Connect vertex 0 (tail) to vertex 1 (head) with weight 1 */
    if (! graph_connect(&graph, 0, 1, 1))
    {
      fprintf(stderr, "graph_connect() failed\n");
    }

    /* 2.3 Create more connections.
     *
     * NOTE: In general it is bad practice to ignore the return code of
     *       functions. We do it here to keep the example code compact.
     */
    (void) graph_connect(&graph, 1, 2, 2);
    (void) graph_connect(&graph, 2, 3, 3);
    (void) graph_connect(&graph, 3, 0, 0);

    /* 2.4 Print the graph */
    graph_print(&graph);

    /* 2.5 Create a dot file from the created graph.
     *
     * You can vizualize the graph by opening the generated dot file from a 
     * Linux terminal with the 'xdot' tool or you can use an online tool such as
     *    http://magjac.com/graphviz-visual-editor/
     *  or
     *    https://dreampuf.github.io/GraphvizOnline/
     *  to do so.
     */
    graph_to_dot(&graph, "test.dot");

    /* 2.6 Disconnect vertex 2 (tail) from vertex 3 (head) */
    graph_disconnect(&graph, 2, 3);

    /* 2.7 Print indegree and outdegree of vertex 1 */
    printf("Vertex 1 has an indegree of %d\n", graph_indegree(&graph, 1));
    printf("Vertex 1 has an outdegree of %d\n", graph_outdegree(&graph, 1));

    /* 2.8 Release the memory that was allocated to the graph */
    graph_release(&graph);

    /* 2.9 Load a graph from a file */
    graph_build_from_file(&graph, "graph1.txt");
    graph_print(&graph);
    graph_release(&graph);
  }
  else
  {
    fprintf(stderr, "graph_initialise() failed\n");
  }

  /* 3. Run student tests */
  fprintf(stdout, "\n");
  fprintf(stdout, "--------------------------------------------------------\n");
  fprintf(stdout, "Student tests\n");
  fprintf(stdout, "--------------------------------------------------------\n");

  student_test();

  return 0;
}
