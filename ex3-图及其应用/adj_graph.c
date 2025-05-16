#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"
#include "pri_queue.h"

#define MAX_VERTEX_NUM   20
typedef const char* VertexType;

typedef struct EdgeInfo{
    double length;
    double time;
} InfoType;

typedef struct ArcNode{
    int             adjvex;
    struct ArcNode *nextarc;
    InfoType       info;
}ArcNode;

typedef struct VNode{
   VertexType       data;
   ArcNode         *firstarc; 
}VNode, AdjList[MAX_VERTEX_NUM];

typedef struct{
    AdjList         vertices;//顶点数组
    int             vexnum;
    int             arcnum;
}ALGraph;

typedef struct {
    int vertex; 
    double dist; 
} DijkstraNode;

int init_ALGraph(ALGraph *G)
{
    if (!G || G->vexnum > MAX_VERTEX_NUM) return -1;

    G->arcnum = 0;
    G->vexnum = 0;

    for (int i = 0; i < MAX_VERTEX_NUM; i++){
        G->vertices[i].firstarc = NULL;
    }

    return 0;
}

static int LocateVex(const ALGraph *G, VertexType u)
{
    if (!G) return -1; 

    int idx = -1;
    for (int i = 0; i < G->vexnum; i++){
        if (strcmp(G->vertices[i].data, u) == 0) idx = i;
    }

    return idx;
}
int AddVertex(ALGraph *G, VertexType data)
{
    if (!G || G->vexnum >= MAX_VERTEX_NUM) return -1;

    G->vertices[G->vexnum].data = data;
    G->vexnum++;    
    return 1;
}

int AddEdge(ALGraph *G, VertexType from, VertexType to, double length, double v)
{
    if (!G) return -1;

    int from_idx = LocateVex(G, from);
    int to_idx = LocateVex(G, to);
    
    ArcNode *new_arcnode = (ArcNode*)malloc(sizeof(ArcNode));
    if (!new_arcnode) return -1;

    new_arcnode->nextarc = G->vertices[from_idx].firstarc;
    G->vertices[from_idx].firstarc = new_arcnode;
    new_arcnode->adjvex = to_idx;
    new_arcnode->info.length = length;
    if (v != 0)
        new_arcnode->info.time = length/v;

    G->arcnum++;
    return 1;
}

int Ud_AddEdge(ALGraph *G, VertexType from, VertexType to, double length, double v)
{
    if (AddEdge(G, from, to, length, v) == -1) return -1;
    if (AddEdge(G, to, from, length, v) == -1) return -1;
    return 1;
}

int DestroyALGraph(ALGraph *G)
{
    if (!G) return -1;

    for (int i = 0; i < G->vexnum; i++){
        ArcNode *tmp = G->vertices[i].firstarc;
        while (tmp){
            ArcNode *next = tmp->nextarc;
            free(tmp);
            tmp = next;
        }
    }
    free(G);
    return 1;
}

int FirstAdjVex(const ALGraph *G, VertexType v)
{
    if (!G) return -1;

    int v_idx = LocateVex(G, v);
    if (G->vertices[v_idx].firstarc){
        return G->vertices[v_idx].firstarc->adjvex;
    }
    return -1;
}
int NextAdjVex(const ALGraph *G, VertexType v, VertexType w)
{
    if (!G) return -1;

    int v_idx = LocateVex(G, v);
    int w_idx = LocateVex(G, w);
    const ArcNode *tmp = G->vertices[v_idx].firstarc;

    while (tmp && tmp->adjvex != w_idx){
        tmp = tmp->nextarc;
    }

    if (tmp && tmp->nextarc){
        return tmp->nextarc->adjvex;
    }
    return -1;
}

static int visited[MAX_VERTEX_NUM];

void (*visitFunc) (VNode*);

static void DFS(ALGraph *G, int node_idx)
{
    if (node_idx < 0 || node_idx >= G->vexnum) return;

    visited[node_idx] = 1;

    visitFunc(&(G->vertices[node_idx]));

    ArcNode *tmp = G->vertices[node_idx].firstarc;
    while (tmp){
        if (!visited[tmp->adjvex]){
            //do something
            DFS(G, tmp->adjvex);
        }
        tmp = tmp->nextarc;
    }
    // for (int w = FirstAdjVex(G, node_idx); w >= 0; w = NextAdjVex(G, node_idx, w)){
    //     if (!visited[w]) DFS(G, w);
    // }
}

void DFSTraverse(ALGraph *G, void (*visit)(VNode*))
{
    if (!G) return;
   
    for (int i = 0; i < G->vexnum; i++){
        visited[i] = 0;
    }
    
    visitFunc = visit;

    for (int i = 0; i < G->vexnum; i++){
        if (!visited[i]) DFS(G, i);
    }
}

void BFSTraverse(ALGraph *G, void (*visit)(VNode *))
{
    if (!G) return;

    for (int i = 0; i < G->vexnum; i++){
        visited[i] = 0;
    }
    queue *Q = queue_new(G->vexnum);
    if (!Q) return;

    visitFunc = visit;
    
    for (int v = 0; v < G->vexnum; v++){
        if (visited[v]) continue;
    
        visited[v] = 1;
        visitFunc(&G->vertices[v]);
        queue_enqueue(Q, v);

        while (!queue_isEmpty(Q)){
            int v;
            queue_dequeue(Q, &v);
            for (ArcNode *tmp = G->vertices[v].firstarc; tmp; tmp = tmp->nextarc){
                if (visited[tmp->adjvex]) continue;
                visited[tmp->adjvex] = 1;
                visitFunc(&G->vertices[tmp->adjvex]);
                queue_enqueue(Q, tmp->adjvex);
            }//for
        }//while
    }//for
    queue_destroy(&Q);
}

Node* createDijkstraNode(int vertex, double dist)
{
    Node* node = (Node*)malloc(sizeof(Node));
    if (!node) return NULL;
    
    node->num = dist;     // 使用num字段存储距离或时间
    node->left = NULL;    // 这些字段在Dijkstra中没用，但需要初始化
    node->right = NULL;
    
    // 使用一个技巧：将顶点索引存储在指针中，转换为整数
    node->left = (Node*)(long)vertex;
    
    return node;
}

// 从Node中获取顶点索引
int getVertexFromNode(Node* node){
    return (int)(long)(node->left);
}

void Dijkstra(ALGraph *G, int source, double *dist, int *parent, int type)
{
    if (!G || source < 0 || source >= G->vexnum) return;
    
    for (int i = 0; i < G->vexnum; i++) {
        dist[i] = __DBL_MAX__;
        parent[i] = -1; 
        visited[i] = 0; 
    }
    dist[source] = 0;

    Heap *pq = NULL;
    pq = create_heap(pq);
    if (!pq) return;
    
    Node* sourceNode = createDijkstraNode(source, 0);
    push(pq, sourceNode);
    free(sourceNode); 
    
    while (!empty(pq)) {
        Node min = top(pq);
        pop(pq);
        
        int u = getVertexFromNode(&min);
        
        if (visited[u]) continue;
        visited[u] = 1;
        
        ArcNode *tmp = G->vertices[u].firstarc;
        while (tmp) {
            double weight = (type == 0) ? tmp->info.length : tmp->info.time;
            
            int v = tmp->adjvex;
            //松弛边
            if (!visited[v] && dist[v] >  dist[u] + weight) {
                dist[v] = dist[u] + weight;
                parent[v] = u;
                
                Node* newNode = createDijkstraNode(v, dist[v]);
                push(pq, newNode);
                free(newNode);
            }
            
            tmp = tmp->nextarc;
        }
    }
    
    free_heap(pq);
}

void printPath(ALGraph *G, int *parent, int target)
{
    if (parent[target] == -1) {
        printf("%s", G->vertices[target].data);
        return;
    }
    
    printPath(G, parent, parent[target]);
    printf(" -> %s", G->vertices[target].data);
}

void print(VNode* node)
{
    printf("%s ", node->data);
    fflush(stdout);
}


int main()
{
    ALGraph *G = malloc(sizeof(ALGraph));
    init_ALGraph(G);

    AddVertex(G, "v1");
    AddVertex(G, "v2");
    AddVertex(G, "v3");
    AddVertex(G, "v4");
    AddVertex(G, "v5");
    AddVertex(G, "v6");
    AddVertex(G, "v7");
    AddVertex(G, "v8");
    AddVertex(G, "v9");
    
    AddEdge(G, "v1", "v2", 12, 40);
    AddEdge(G, "v1", "v3", 8, 50);
    AddEdge(G, "v1", "v4", 6, 60);
    AddEdge(G, "v2", "v5", 5, 20);
    AddEdge(G, "v3", "v5", 8, 25);
    AddEdge(G, "v4", "v6", 7, 55);
    AddEdge(G, "v5", "v7", 5, 20);
    AddEdge(G, "v5", "v8", 6, 20);
    AddEdge(G, "v6", "v8", 10, 40);
    AddEdge(G, "v7", "v9", 6, 30);
    AddEdge(G, "v8", "v9", 8, 25);


    // printf("图的BFS遍历: ");
    // BFSTraverse(G, print);
    // printf("\n");
    
    // printf("图的DFS遍历: ");
    // DFSTraverse(G, print);
    // printf("\n\n");

    char s[10], t[10];
    while(1){
        printf("输入起点终点: ");
        scanf("%s %s", s, t);
        int source = LocateVex(G, s);  
        int target = LocateVex(G, t); 
        
        if (source == -1 || target == -1) {
            printf("节点不存在。\n\n");
            continue;
        }
        
        double *dist = (double*)malloc(sizeof(double) * G->vexnum);
        int *parent = (int*)malloc(sizeof(int) * G->vexnum);
        
        //距离
        Dijkstra(G, source, dist, parent, 0);
            printf("从顶点 %s 到顶点 %s 的最短距离为: %.2f\n", 
            G->vertices[source].data, G->vertices[target].data, dist[target]);
        printf("路径为: ");
        printPath(G, parent, target);
        printf("\n\n");
        
        //时间
        Dijkstra(G, source, dist, parent, 1);
        printf("从顶点 %s 到顶点 %s 的最短时间为: %.2f\n", 
            G->vertices[source].data, G->vertices[target].data, dist[target]);
        printf("路径为: ");
        printPath(G, parent, target);
        printf("\n");
        
        free(dist);
        free(parent);

        printf("/**********/\n");
    }
    
    DestroyALGraph(G);

    return 0;
}
