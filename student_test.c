 #include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include "test.h"
#include "graph.h"

#define TEST(expr) test(expr, __FILE__, __LINE__, #expr)
#define TESTQ(expr) testq(expr, __FILE__, __LINE__)

static struct 
{
  int pass;
  int fail;
} stats;

/****************************************************************************/
static void test(bool pass, char *file, int line, char *expr)
{
  if (pass)
  {
    stats.pass++;
  }
  else
  {
    stats.fail++;
    fprintf(stderr, "%s:%3d: Fail '%s'\n", file, line, expr);
  }
}

/****************************************************************************/
static void testq(bool pass, char *file, int line)
{
  if (pass)
  {
    stats.pass++;
  }
  else
  {
    stats.fail++;
    fprintf(stderr, "%s:%d: Fail\n", file, line);
  }
}

/****************************************************************************/
static int redirect_stdout_to_buf(char buf[], size_t size)
{
  int state;

  fflush(stdout);
  state = dup(STDOUT_FILENO);         /* save stdout state */
  freopen("/dev/null", "a", stdout);  /* redirect stdout to /dev/null */
  setvbuf(stdout, buf, _IOFBF, size); /* set buf to stdout */

  return state;
}

/****************************************************************************/
static void restore_stdout(int state)
{
  freopen("/dev/null", "a", stdout);     /* redirect stdout to /dev/null */
  dup2(state, STDOUT_FILENO);            /* restore stdout state */
  setvbuf(stdout, NULL, _IONBF, BUFSIZ); /* disable buffer */
}

/****************************************************************************/
static void test_edge_to_string(void)
{
  char str[20];

  edge_t edge;
  edge.next   = NULL;
  edge.tail   = 10;
  edge.head   = 20;
  edge.weight = 30;

  edge_to_string(&edge, str, sizeof(str));
  TEST(strncmp(str, "10 -> 20 (30)", sizeof(str)) == 0);
  
  edge_t edge2;
  edge2.next   = NULL;
  edge2.tail   = 8;
  edge2.head   = 15;
  edge2.weight = 9;

  edge_to_string(&edge2, str, sizeof(str));
  TEST(strncmp(str, " 8 -> 15 (09)", sizeof(str)) == 0);

  edge_t edge3;
  edge3.next   = NULL;
  edge3.tail   = 1;
  edge3.head   = 8;
  edge3.weight = 8;

  edge_to_string(&edge3, str, sizeof(str));
  TEST(strncmp(str, " 1 ->  8 (08)", sizeof(str)) == 0);

  edge_t edge4;
  edge4.next   = NULL;
  edge4.tail   = 2;
  edge4.head   = 2;
  edge4.weight = 2;

  edge_to_string(&edge4, str, sizeof(str));
  TEST(strncmp(str, " 2 ->  2 (02)", sizeof(str)) == 0);

  edge_t edge5;
  edge5.next   = NULL;
  edge5.tail   = 22;
  edge5.head   = 22;
  edge5.weight = 22;

  edge_to_string(&edge5, str, sizeof(str));
  TEST(strncmp(str, "22 -> 22 (22)", sizeof(str)) == 0);

  edge_t edge6;
  edge6.next   = NULL;
  edge6.tail   = 1;
  edge6.head   = 0;
  edge6.weight = 0;

  edge_to_string(&edge6, str, sizeof(str));
  TEST(strncmp(str, " 1 ->  0 (00)", sizeof(str)) == 0);

  edge_t edge7;
  edge7.next   = NULL;
  edge7.tail   = 10;
  edge7.head   = 10;
  edge7.weight = 10;

  edge_to_string(&edge7, str, sizeof(str));
  TEST(strncmp(str, "10 -> 10 (10)", sizeof(str)) == 0);

  edge_t edge8;
  edge8.next   = NULL;
  edge8.tail   = 10;
  edge8.head   = 10;
  edge8.weight = 1;

  edge_to_string(&edge8, str, sizeof(str));
  TEST(strncmp(str, "10 -> 10 (01)", sizeof(str)) == 0);

  edge_t edge9;
  edge9.next   = NULL;
  edge9.tail   = 5;
  edge9.head   = 50;
  edge9.weight = 5;

  edge_to_string(&edge9, str, sizeof(str));
  TEST(strncmp(str, " 5 -> 50 (05)", sizeof(str)) == 0);

  edge_t edge10;
  edge10.next   = NULL;
  edge10.tail   = 1;
  edge10.head   = 1;
  edge10.weight = 1;

  edge_to_string(&edge10, str, sizeof(str));
  TEST(strncmp(str, " 1 ->  1 (01)", sizeof(str)) == 0);
}

/****************************************************************************/
static void test_list_is_empty(void)
{
  adjacency_list_t list;
  list.first = NULL;

  TEST(list_is_empty(&list));

  adjacency_list_t list2;
  list2.first = NULL;

  TEST(list_is_empty(&list2));

  adjacency_list_t list3;
  list3.first = list3.first -> next;

  TEST(list_is_empty(&list3));

}

/****************************************************************************/
static void test_list_size(void)
{
  edge_t edge;
  edge.next   = NULL;
  edge.tail   = 1;
  edge.head   = 2;
  edge.weight = 1;

  adjacency_list_t list;
  list.first = &edge;

  TEST(list_size(&list) == 1);

  
  edge_t edge2;
  edge2.next   = NULL;
  edge2.tail   = 3;
  edge2.head   = 2;
  edge2.weight = 3;

  adjacency_list_t list2;
  list2.first = &edge2;

  TEST(list_size(&list2) == 1);

}

/****************************************************************************/
static void test_list_prepend(void)
{
  edge_t edge1;
  edge1.next   = NULL;
  edge1.tail   = 0;
  edge1.head   = 0;
  edge1.weight = 0;

  edge_t edge2;
  edge2.next   = NULL;
  edge2.tail   = 0;
  edge2.head   = 0;
  edge2.weight = 0;

  adjacency_list_t list;
  list.first = &edge1;

  list_prepend(&list, &edge2);
  TEST(list.first == &edge2);
  TEST(list.first->next == &edge1);

  edge_t edge3;
  edge3.next   = NULL;
  edge3.tail   = 0;
  edge3.head   = 0;
  edge3.weight = 0;

  edge_t edge4;
  edge4.next   = NULL;
  edge4.tail   = 2;
  edge4.head   = 2;
  edge4.weight = 2;

  adjacency_list_t list2;
  list2.first = &edge3;

  list_prepend(&list2, &edge4);
  TEST(list2.first == &edge4);
  TEST(list2.first->next == &edge3);

  edge_t edge5;
  edge5.next   = NULL;
  edge5.tail   = 1;
  edge5.head   = 1;
  edge5.weight = 1;

  edge_t edge6;
  edge6.next   = NULL;
  edge6.tail   = 5;
  edge6.head   = 3;
  edge6.weight = 2;

  adjacency_list_t list3;
  list3.first = &edge5;

  list_prepend(&list3, &edge6);
  TEST(list3.first == &edge6);
  TEST(list3.first->next == &edge5);

  edge_t edge7;
  edge7.next   = NULL;
  edge7.tail   = 10;
  edge7.head   = 2;
  edge7.weight = 9;

  edge_t edge8;
  edge8.next   = NULL;
  edge8.tail   = 9;
  edge8.head   = 9;
  edge8.weight = 9;

  adjacency_list_t list4;
  list4.first = &edge7;

  list_prepend(&list4, &edge8);
  TEST(list4.first == &edge8);
  TEST(list4.first->next == &edge7);

  edge_t edge9;
  edge9.next   = NULL;
  edge9.tail   = 1;
  edge9.head   = 1;
  edge9.weight = 1;

  edge_t edge10;
  edge10.next   = NULL;
  edge10.tail   = 1;
  edge10.head   = 1;
  edge10.weight = 1;

  adjacency_list_t list5;
  list5.first = &edge9;

  list_prepend(&list5, &edge10);
  TEST(list5.first == &edge10);
  TEST(list5.first->next == &edge9);
}

/****************************************************************************/
static void test_list_contains(void)
{
  edge_t edge1;
  edge1.next   = NULL;
  edge1.tail   = 0;
  edge1.head   = 1;
  edge1.weight = 0;

  adjacency_list_t list;
  list.first = &edge1;

  TEST(list_contains(&list, 0, 1));

  edge_t edge2;
  edge2.next   = NULL;
  edge2.tail   = 2;
  edge2.head   = 2;
  edge2.weight = 0;

  adjacency_list_t list2;
  list2.first = &edge2;

  TEST(list_contains(&list2, 2, 2));

  edge_t edge3;
  edge3.next   = NULL;
  edge3.tail   = 10;
  edge3.head   = 10;
  edge3.weight = 0;

  adjacency_list_t list3;
  list3.first = &edge3;

  TEST(list_contains(&list3, 10, 10));

  edge_t edge4;
  edge4.next   = NULL;
  edge4.tail   = 8;
  edge4.head   = 8;
  edge4.weight = 0;

  adjacency_list_t list4;
  list4.first = &edge4;

  TEST(list_contains(&list4, 8, 8));

  edge_t edge5;
  edge5.next   = NULL;
  edge5.tail   = 22;
  edge5.head   = 22;
  edge5.weight = 0;

  adjacency_list_t list5;
  list5.first = &edge5;

  TEST(list_contains(&list5, 22, 22));
}

/****************************************************************************/
static void test_graph_initialise(void)
{
  graph_t graph;

  graph_initialise(&graph, 5);

  TEST(graph.vertex_count == 5);

  graph_t graph2;

  graph_initialise(&graph2, 10);

  TEST(graph2.vertex_count == 10);
  
  graph_t graph3;

  graph_initialise(&graph3, 15);

  TEST(graph3.vertex_count == 15);

  graph_t graph4;

  graph_initialise(&graph4, 1);

  TEST(graph4.vertex_count == 1);

  graph_t graph5;

  graph_initialise(&graph5, 0);

  TEST(graph5.vertex_count == 0);

  graph_t graph6;

  graph_initialise(&graph6, 2);

  TEST(graph6.vertex_count == 2);

  graph_t graph7;

  graph_initialise(&graph7, 22);

  TEST(graph7.vertex_count == 22);

  graph_t graph8;

  graph_initialise(&graph8, 0);

  TEST(graph8.vertex_count == 0);

  graph_t graph9;

  graph_initialise(&graph9, 33);

  TEST(graph9.vertex_count == 33);

  graph_t graph10;

  graph_initialise(&graph10, 100);

  TEST(graph10.vertex_count == 100);
}

/****************************************************************************/
static void test_graph_print(void)
{
  char buf[10*1024];
  int  save;

  edge_t edge;
  edge.next = NULL; 
  edge.tail = 0;
  edge.head = 1;
  edge.weight = 10;

  adjacency_list_t adjacency_lists[3];
  adjacency_lists[0].first = &edge;
  adjacency_lists[1].first = NULL;

  graph_t graph;
  graph.vertex_count = 2;
  graph.edge_count = 1;
  graph.adjacency_lists = adjacency_lists;

  save = redirect_stdout_to_buf(buf, sizeof(buf));
  graph_print(&graph);
  restore_stdout(save);
  TESTQ(strncmp(buf, 
                "Graph with 2 vertices and 1 edges:\n"
                "vertex 0:\n"
                "   0 ->  1 (10)\n"
                "vertex 1:\n",
               sizeof(buf)) == 0);

  edge_t edge2;
  edge2.next = NULL; 
  edge2.tail = 3;
  edge2.head = 2;
  edge2.weight = 8;

  edge_t edge3;
  edge3.next = NULL; 
  edge3.tail = 0;
  edge3.head = 1;
  edge3.weight = 10;

  adjacency_lists[0].first = &edge2;
  adjacency_lists[1].first = &edge3;
  adjacency_lists[2].first = NULL;

  graph_t graph2;
  graph2.vertex_count = 3;
  graph2.edge_count = 2;
  graph2.adjacency_lists = adjacency_lists;

  save = redirect_stdout_to_buf(buf, sizeof(buf));
  graph_print(&graph2);
  restore_stdout(save);
  TESTQ(strncmp(buf, 
                "Graph with 3 vertices and 2 edges:\n"
                "vertex 0:\n"
                "   3 ->  2 (08)\n"
                "vertex 1:\n"
                "   0 ->  1 (10)\n"
                "vertex 2:\n",
               sizeof(buf)) == 0);

}

/****************************************************************************/
static void test_graph_release(void)
{
  /* Add tests here */
}

/****************************************************************************/
static void test_graph_connect(void)
{
  bool success;

  adjacency_list_t adjacency_lists[2];
  adjacency_lists[0].first = NULL;
  adjacency_lists[1].first = NULL;

  graph_t graph;
  graph.vertex_count = 2;
  graph.edge_count = 0;
  graph.adjacency_lists = adjacency_lists;

  success = graph_connect(&graph, 0, 1, 1);
  TEST(success);
  if (success)
  {
    TEST(graph.adjacency_lists[0].first != NULL);
    if (graph.adjacency_lists[0].first != NULL)
    {
      TEST(graph.adjacency_lists[0].first->tail == 0);
      TEST(graph.adjacency_lists[0].first->head == 1);
    }
  }


  adjacency_lists[0].first = NULL;
  adjacency_lists[1].first = NULL;

  graph_t graph2;
  graph2.vertex_count = 2;
  graph2.edge_count = 0;
  graph2.adjacency_lists = adjacency_lists;

  success = graph_connect(&graph2, 0, 4, 1);
  TEST(success);
  if (success)
  {
    TEST(graph2.adjacency_lists[0].first != NULL);
    if (graph2.adjacency_lists[0].first != NULL)
    {
      TEST(graph2.adjacency_lists[0].first->tail == 0);
      TEST(graph2.adjacency_lists[0].first->head == 4);
    }
  }
}

/****************************************************************************/
static void test_graph_disconnect(void)
{
  /* Add tests here */
}

/****************************************************************************/
static void test_graph_outdegree(void)
{
  edge_t edge;
  edge.next = NULL; 
  edge.tail = 0;
  edge.head = 1;
  edge.weight = 1;

  adjacency_list_t adjacency_lists[2];
  adjacency_lists[0].first = &edge;
  adjacency_lists[1].first = NULL;

  graph_t graph;
  graph.vertex_count = 2;
  graph.edge_count = 1;
  graph.adjacency_lists = adjacency_lists;

  TEST(graph_outdegree(&graph, 0) == 1);


}

/****************************************************************************/
void student_test(void)
{
  memset(&stats, 0, sizeof(stats));

  test_edge_to_string();
  test_list_is_empty();
  test_list_size();
  test_list_prepend();
  test_list_contains();
  test_graph_initialise();
  test_graph_print();
  test_graph_release();
  test_graph_connect();
  test_graph_disconnect();
  test_graph_outdegree();

  fprintf(stdout, "%d tests passed\n", stats.pass);
  fprintf(stdout, "%d tests failed\n", stats.fail);
}