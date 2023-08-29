#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <stdint.h>
#include <stdio.h>
#include "memorypool.h"
#include "linkedlist.h"
#include "map.h"
typedef struct graph_V_DataBox {
	int x;
	int y;
}graph_V_DataBox;

typedef struct graph_E_DataBox {
	int data;
}graph_E_DataBox;
//�ڵ�
typedef struct graph_V
{
	//���
	int key;
	//������Ϣ
	struct graph_E* out_list;

	struct graph_V* pre;
	struct graph_V* next;
	//������Ϣ
	struct graph_V_DataBox value;
	//�ɶ�̬�����޸�����
	struct graph_V* parent;
}graph_V;
//��
typedef struct graph_E
{
	//���
	int key;
	//������Ϣ
	struct graph_V* start;
	struct graph_V* end;

	struct graph_E* pre;
	struct graph_E* next;
	//������Ϣ
	struct graph_E_DataBox value;
}graph_E;



graph_V* CreatGraph_V(int key, graph_V_DataBox value);
graph_V* InsertGraph_V_Next(int key, graph_V_DataBox value, graph_V* target);
void graph_E_ValueAssignment(graph_E_DataBox value, graph_E_DataBox* target);
void graph_V_ValueAssignment(graph_V_DataBox value, graph_V_DataBox* target);
graph_E* Graph_V_Add_E(graph_V* start, graph_V* end, graph_E_DataBox value);
void GraphDSF(graph_V* start, int V_number);
void PrintGraph_V_Es(graph_V* node);
graph_V* FindGraph_V_DSF(graph_V* start, int target_key, int V_number);
graph_V* FindGraph_V(graph_V* start, int target_key);
#endif