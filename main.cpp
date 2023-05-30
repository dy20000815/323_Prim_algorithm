#include <iostream>
#include <fstream>

using namespace std;

class uEdge{
public:
	int Ni, Nj, cost;
	uEdge* next;

	uEdge(int i, int j, int c, uEdge* n) {
		Ni = i;
		Nj = j;
		cost = c;
		next = n;
	}

	void printEdge(ofstream &writer) {
		if (next != NULL) {
			writer << "<" << Ni << ", " << Nj << ", " << cost << "," << next->Ni << ">";
		}else
		writer<< "<" << Ni << ", " << Nj << ", " << cost << ",null>" ;
	}
};

class PrimMST {
public:
	int numNodes, nodeInSetA;
	char* whichSet;
	uEdge* edgelistHead;
	uEdge* MSTlistHead;
	int totalMSTCost;

	PrimMST(int n, int source) {
		numNodes = n;
		nodeInSetA = source;
		whichSet = new char[numNodes + 1];
		for (int i = 0; i <= numNodes; i++) {
			whichSet[i] = 'B';
		}
		edgelistHead = new uEdge(0, 0, 0, NULL);
		MSTlistHead = new uEdge(0, 0, 0, NULL);
		totalMSTCost = 0;
	}

	void listInsert(uEdge* n) {
		uEdge* curr = edgelistHead;
		while (curr->next != NULL) {
			if (n->cost >= curr->next->cost) {
				curr = curr->next;
			}
			else {
				break;
			}
		}n->next = curr->next;
		curr->next = n;
	}

	uEdge* removeEdge() {
		uEdge* curr = edgelistHead;
		while (curr->next != NULL) {
			if (whichSet[curr->next->Ni] != whichSet[curr->next->Nj] && (whichSet[curr->next->Ni] == 'A' || whichSet[curr->next->Nj] == 'A')) {
				uEdge* r = curr->next;
				curr->next = curr->next->next;
				r->next = NULL;
				return r;
			}
			else curr = curr->next;
		}
		return NULL;
	}

	void addEdgeToMST(uEdge* e) {
		uEdge* curr = MSTlistHead;
		while (curr->next != NULL) {
			curr = curr->next;
		}curr->next = e;
	}

	void printSet(ofstream &output) {
		for (int i = 1; i < numNodes + 1; i++) {
			output << "edge " << i << " is in set " << whichSet[i] << endl;
		}
	}

	void printEdgeList(ofstream &output) {
		uEdge* curr = edgelistHead;
		output << "edgelistHead -> ";
		while (curr->next != NULL) {
			curr->printEdge(output);
			output << " -> ";
			curr = curr->next;
		}
		curr->printEdge(output);
		output << " -> "; 
		output << "end" << endl;
	}

	void printMSTList(ofstream& output) {
		uEdge* curr = MSTlistHead;
		output << "MSTlistHead->";
		while (curr->next != NULL) {
			curr->printEdge(output);
			output << "->";
			curr = curr->next;
		}curr->printEdge(output);
		output << " -> ";
		output << "end" << endl;
	}

	bool isEmpty() {
		for (int i = 1; i < numNodes + 1; i++) {
			if (whichSet[i] != 'A') {
				return false;
			}
		}return true;
	}

	void updateMST(uEdge* e) {
		addEdgeToMST(e);
		totalMSTCost += e->cost;
		if (whichSet[e->Ni] == 'A') {
			whichSet[e->Nj] = 'A';
		}
		else whichSet[e->Ni] = 'A';
	}
};

int main(int argc, char* argv[]) {
	ifstream input(argv[1]);
	int source = atoi(argv[2]);
	ofstream output(argv[3]);
	ofstream debug(argv[4]);
	int num;
	input >> num;
	PrimMST* mst =new PrimMST(num, source);
	mst->whichSet[0] = 'A';
	mst->whichSet[source] = 'A';
	mst->printSet(debug);
	int ni, nj, c;
	uEdge* newEdge;
	while (input.eof() == false) {
		input >> ni >> nj >> c;
		newEdge = new uEdge(ni,nj,c,NULL);
		mst->listInsert(newEdge);
		mst->printEdgeList(debug);
	}
	while (mst->isEmpty() == false) {
		uEdge* nEdge = mst->removeEdge();
		nEdge->printEdge(debug);
		debug << endl;
		mst->updateMST(nEdge);
		mst->printSet(debug);
		mst->printEdgeList(debug);
		mst->printMSTList(debug);
	}output << "*** Prim's MST of the input graph, G is: ***" << endl;
	output << num<<endl;
	mst->printMSTList(output);
	output << " *** MST total cost = " << mst->totalMSTCost << endl;
	input.close();
	output.close();
	debug.close();
	return 0;
}