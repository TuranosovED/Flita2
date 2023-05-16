#include <stdio.h>
#include <stdlib.h>

typedef struct Edge
{
    int weight; //вес 
    int top;     //bottom -> top
    int bottom;
}Edge;

typedef struct Graph
{
    Edge* edges; //ссылка на массив ребер
    int countOfpoints; // число вершин
    int countOfEdges; //число ребер
    int directed; //направленный ли граф
}Graph;

typedef struct Array
{
    int *mas;
    int size;
}Array;

void PrintGraph(Graph *graph)
{
    printf("Count of edges: %d\nCount of points: %d\n",graph->countOfEdges,graph->countOfpoints);
    if(graph->directed)
        puts("Is directed: true");
    else 
        puts("Is directed: false");
    puts("List of edges:");
    for(int i =0;i<graph->countOfEdges;i++)
    {
        if(i/10 == 0)putchar(' ');
        if(graph->directed)
            printf("%d: %d -> %d\n",i,graph->edges[i].bottom,graph->edges[i].top);
        else
            printf("%d: %d -- %d\n",i,graph->edges[i].bottom,graph->edges[i].top);
    }
}

void clearBuffer()
{
    char c;
    while((c = getchar())!= '\n');
}

Graph *MakeGraph(char *fileName)
{
    int value1, value2;
    char Commads = '\0';
    FILE *f = fopen(fileName,"r");
    if(f == NULL)
    {
        puts("File not founded");
        return NULL;
    }

    Graph *graph = (Graph *)malloc(sizeof(Graph));

    for(graph->countOfpoints = 0;(fscanf(f,"%d",&value1)!=EOF);graph->countOfpoints++); //считаем кол-во вершин
    fclose(f);
    
    graph->countOfEdges = graph->countOfpoints / 2;
    graph->edges = (Edge*)malloc(sizeof(Edge) * graph->countOfEdges);

    fopen(fileName,"r");

    for(int i = 0;(fscanf(f,"%d%d",&value1,&value2)!=EOF);i++)
    {
        graph->edges[i].bottom = value1;
        graph->edges[i].top = value2;
    }

    fclose(f);

    while(Commads == '\0')
    {
        puts("Is graph directed? (Y/N)");
        scanf("%c",&Commads);
        
        switch(Commads)
        {
            case 'Y':
                graph->directed = 1;
                break;
            case 'y':
                graph->directed = 1;
                break;
            case 'N':
                graph->directed = 0;
                break;
            case 'n':
                graph->directed = 0;
                break;
            default:
                Commads = '\0';
                break;
        }
        clearBuffer();
    }

    return graph;
}

void MakeDotFile(Graph *graph)
{
    FILE *f = fopen("graph.dot","w");
    if(graph->directed)
        fprintf(f,"digraph mygraph {\n");
    else
        fprintf(f,"graph mygraph {\n");
    for(int i = graph->countOfEdges - 1;i>=0;i--)
    {
        if(graph->directed)
            fprintf(f,"%d -> %d\n",graph->edges[i].bottom,graph->edges[i].top);
        else
            fprintf(f,"%d -- %d\n",graph->edges[i].bottom,graph->edges[i].top);
    }
    fputc('}',f);
    fclose(f);
}

void MakeGraphPNG(Graph *graph)
{
    MakeDotFile(graph);
    system("move graph.dot %cd%\\graphviz\\bin & cd %cd%\\graphviz\\bin & dot -Tpng graph.dot -o graph.png & move graph.dot ..\\..\\ & move graph.png ..\\..\\");
}

void DeepFirstSeatch(Graph *graph,int point,Array *VisitedPoints)
{
    int trigger = 0;
    for(int i =0;i<graph->countOfEdges;i++)
    {
        trigger = 0;
        if(graph->edges[i].bottom == point)
        {
            for(int j = 0;j<(*VisitedPoints).size;j++)
            {
                if(graph->edges[i].top == (*VisitedPoints).mas[j])
                    trigger = 1;
            }
            if(trigger == 0)
            {
                for(int j = 0;j<(*VisitedPoints).size;j++)
                {
                    if((*VisitedPoints).mas[j] == point)break;
                    if((*VisitedPoints).mas[j] == -1)
                    {
                        (*VisitedPoints).mas[j] = point;
                        break;
                    }
                }
                DeepFirstSeatch(graph,graph->edges[i].top,VisitedPoints);
            }
        }
        else if(graph->edges[i].top == point)
        {
            for(int j = 0;j<(*VisitedPoints).size;j++)
            {
                if(graph->edges[i].bottom == (*VisitedPoints).mas[j])
                    trigger = 1;
            }
            if(trigger == 0)
            {
                for(int j = 0;j<(*VisitedPoints).size;j++)
                {
                    if((*VisitedPoints).mas[j] == point)break;
                    if((*VisitedPoints).mas[j] == -1)
                    {
                        (*VisitedPoints).mas[j] = point;
                        break;
                    }
                }
                DeepFirstSeatch(graph,graph->edges[i].bottom,VisitedPoints);
            }
        }
        else
        {
            if(trigger == 0)
            {
                for(int j = 0;j<(*VisitedPoints).size;j++)
                {
                    if((*VisitedPoints).mas[j] == point)break;
                    if((*VisitedPoints).mas[j] == -1)
                    {
                        (*VisitedPoints).mas[j] = point;
                        break;
                    }
                }
            }
        }
    }
    // for(int i =0;i<(*VisitedPoints).size;i++)
    //     printf("%d ",(*VisitedPoints).mas[i]);
    // putchar('\n');
}

int CountOfPersonalPoints(Graph *graph)
{
    int trigger;
    int *mas = (int *)malloc(sizeof(int) * graph->countOfpoints);
    for(int i =0;i<graph->countOfpoints;i++)
        mas[i] = -1;
    for(int i =0;i<graph->countOfEdges;i++)
    {
        trigger = 0;
        for(int j =0;j<graph->countOfpoints;j++)
        {
            if(graph->edges[i].bottom == mas[j])
                trigger = 1;
        }
        if(trigger == 0)
        {
            for(int j =0;j<graph->countOfpoints;j++)
            {
                if(mas[j] == -1)
                {
                    mas[j] = graph->edges[i].bottom;
                    break;
                }
            }
        }
        trigger = 0;
        for(int j =0;j<graph->countOfpoints;j++)
        {
            if(graph->edges[i].top == mas[j])
                trigger = 1;
        }
        if(trigger == 0)
        {
            for(int j =0;j<graph->countOfpoints;j++)
            {
                if(mas[j] == -1)
                {
                    mas[j] = graph->edges[i].top;
                    break;
                }
            }
        }
    }
    int count = 0;
    for(int i =0;mas[i] != -1;i++)
        count++;
    return count;
}

int CheakGraphConnectivity(Graph * graph)
{
    Array VisitedPoints;
    VisitedPoints.size = CountOfPersonalPoints(graph); 
    VisitedPoints.mas = (int *)malloc(sizeof(int) * VisitedPoints.size);
    for(int i =0;i<VisitedPoints.size;i++)
        VisitedPoints.mas[i] = -1;
    DeepFirstSeatch(graph,graph->edges[0].bottom,&VisitedPoints);
    putchar('\n');
    for(int i =0;i<VisitedPoints.size;i++)
        if(VisitedPoints.mas[i] == -1)
            return 0;
    return 1;
}

int main(int argc, char *argv[])
{
    Graph *graph = MakeGraph(argv[1]);
    PrintGraph(graph);
    MakeGraphPNG(graph);
    if(CheakGraphConnectivity(graph))
        puts("Graph is connected");
    else
        puts("Graph is not connected");
    return 0;
}