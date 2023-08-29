#include "graph.h"

extern data_box point_list[30];
extern int point_list_len;

//链表模式

graph_V* CreatGraph_V(int key, graph_V_DataBox value)
{
	graph_V* node = (graph_V*)getMemory(sizeof(graph_V));
	if (node != NULL)
	{
		node->key = key;
		graph_V_ValueAssignment(value, &(node->value));
		return node;
	}
	else {
		return NULL;
	}
}
graph_V* InsertGraph_V_Next(int key, graph_V_DataBox value, graph_V* target)
{
	graph_V* node = (graph_V*)getMemory(sizeof(graph_V));
	if (target != NULL)
	{
		if (target->next != NULL)
		{
			node->next = target->next;
			node->pre = target;
			target->next = node;
		}
		else {
			target->next = node;
			node->pre = target;
		}
		node->key = key;
		graph_V_ValueAssignment(value, &(node->value));
		return node;
	}
	else {
		return NULL;
	}
}
void graph_E_ValueAssignment(graph_E_DataBox value, graph_E_DataBox* target)
{
	if (target != NULL) {
		target->data = value.data;
	}
}
void graph_V_ValueAssignment(graph_V_DataBox value, graph_V_DataBox* target)
{
	if (target != NULL) {
		target->x = value.x;
		target->y = value.y;
	}
}
graph_E* Graph_V_Add_E(graph_V* start, graph_V* end, graph_E_DataBox value)
{
	if (start != NULL && end != NULL && start!= end)
	{
		graph_E* line = (graph_E*)getMemory(sizeof(graph_E));
		graph_E* mid;
		if (line != NULL)
		{
			line->end = end;
			line->start = start;
			graph_E_ValueAssignment(value, &(line->value));

			if (start->out_list ==NULL) {
				start->out_list = line;
				line->pre = NULL;
				line->key = 1;
			}
			else {
				mid = start->out_list;
				while (mid->next != NULL) {
					mid = mid->next;
				}
				mid->next = line;
				line->pre = mid;
				line->key = mid->key+1;
			}
			return line;
		}
		else {
			return NULL;
		}
	}
	else {
		return NULL;
	}
}
//广度优先遍历
void GraphDSF(graph_V* start, int V_number)
{
	if (start == NULL || start->out_list==NULL)
	{
		return;
	}

	int* visited = (int*)getMemory(sizeof(int)* V_number);
	graph_V** p = (graph_V**)getMemory(sizeof(graph_V*) * V_number);

	if (visited == NULL || p == NULL) {
		return;
	}
	graph_V* use=NULL;
	graph_E* mid=NULL;

	int i = 0, j = 0,k=0,isVisited=0;


	p[j] = start;
	visited[j] = start->key;
	j++;

	while (i <= j && i <= V_number && j <= V_number)
	{
		use = p[i];
		//TODO
		printf("\n(%d)\n", use->key);

		//去向节点入队
		if (use->out_list != NULL) {
			mid = use->out_list;
			do {
				for (k = 0, isVisited=0; k <= j; k++) {
					if (visited[k] == mid->end->key)
					{
						isVisited = 1;
					}
				}
				if (isVisited==0) {
					p[j] = mid->end;
					visited[j] = mid->end->key;
					j++;
					printf("%d,", mid->end->key);
				}
				mid = mid->next;
			} while (mid != NULL);
		}
		i++;
		//printf("\n{i=%d,j=%d}\n",i,j);
	}
	freeMemory((char*)visited);
	freeMemory((char*)p);
}

//广度优先遍历
graph_V* FindGraph_V_DSF(graph_V* start,int target_key ,int V_number)
{
	if (start == NULL || start->out_list == NULL)
	{
		return;
	}

	int* visited = (int*)getMemory(sizeof(int) * V_number);
	graph_V** p = (graph_V**)getMemory(sizeof(graph_V*) * V_number);

	if (visited == NULL || p == NULL) {
		return;
	}
	graph_V* use = NULL;
	graph_E* mid = NULL;

	int i = 0, j = 0, k = 0, isVisited = 0;


	p[j] = start;
	visited[j] = start->key;
	j++;

	while (i <= j && i <= V_number && j <= V_number)
	{
		use = p[i];
		//TODO

		if (use->key == target_key)
		{
			graph_V* target = use;
			int x, y;
			point_list_len = 0;
			printf("%d,", use->key);
			while (use->parent != NULL) {
				x = use->parent->value.x - use->value.x;
				y = use->parent->value.y - use->value.y;
				if (x != 0 || y != 0)
				{
					point_list[point_list_len].x = x;
					point_list[point_list_len].y = y;
					point_list_len++;
				}
				use = use->parent;
				printf("%d,", use->key);
			}
			use = target;
			do {
				use->parent = NULL;
				use = use->next;
			} while (use != NULL);
			use = target;
			do {
				use->parent = NULL;
				use = use->pre;
			} while (use != NULL);

			freeMemory((char*)visited);
			freeMemory((char*)p);
			return target;
		}

		//去向节点入队
		if (use->out_list != NULL) {
			mid = use->out_list;
			do {
				for (k = 0, isVisited = 0; k <= j; k++) {
					if (visited[k] == mid->end->key)
					{
						isVisited = 1;
					}
				}
				if (isVisited == 0) {
					p[j] = mid->end;
					visited[j] = mid->end->key;
					//TODO
					mid->end->parent = use;

					j++;
				}
				mid = mid->next;
			} while (mid != NULL);
		}
		i++;
		//printf("\n{i=%d,j=%d}\n",i,j);
	}
	freeMemory((char*)visited);
	freeMemory((char*)p);
}

void PrintGraph_V_Es(graph_V* node)
{
	if (node != NULL && node->out_list != NULL)
	{
		graph_E* start = node->out_list;
		printf("\nGraph_V_Es(%d):\n",node->key);
		if (start != NULL)
		{
			do {
				printf("{%d,(%d->%d)}\n", start->key, start->start->key, start->end->key);
				start = start->next;
			} while (start != NULL);
		}
		else {
			printf("\nnot found\n");
		}
	}
	else {
		printf("\nnot found\n");
	}
}

graph_V* FindGraph_V(graph_V* start, int target_key)
{
	if (start != NULL)
	{
		graph_V* use;
		use = start;
		do {
			if (use->key == target_key) {
				return use;
			}
			use = use->next;
		} while (use != NULL);
		use = start;
		do {
			if (use->key == target_key) {
				return use;
			}
			use = use->pre;
		} while (use != NULL);
	}
	else {
		return NULL;
	}
}