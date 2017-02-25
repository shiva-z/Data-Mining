//----------------------------------------------------------------------------------------
/* Assignment: #2 - CMPUT 690 Data Mining in Rich Data 
   Author : Shiva Zamani
   Instructor: Osmar R. Zaiane
   
*/
//----------------------------------------------------------------------------------------
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream> 
#include <unordered_map>
#include <iomanip>

using namespace std;

long num_transactions;
long threshold=0; // number of transactions to be read from the input file, if 0 then all transacitions are used
clock_t startTime;
//----------------------------------------------------------------------------------------
class TreeNode
{       
    public:
        long value;
	unordered_map<long,TreeNode*> children;
      	long counter=0;

        TreeNode(long value)
        {
            this->value = value;
        }
	
};
class Tree
{ 
    public:
	TreeNode * root;
	
        Tree(long value)
        {
            this->root = new TreeNode(value);
        }
	//insert a vector of items into the tree. each node represents one item
	void insert(vector<long> L){
		TreeNode * iter=this->root;
		vector<long>::iterator col;
		for (col = L.begin(); col != L.end(); col++) {
			auto t = iter->children.find(*col);
			if (t == iter->children.end()) {
				iter->children[*col]=new TreeNode(*col);
				iter=iter->children[*col];
			}
			else{
				iter=iter->children[*col];
			}
		}
	}
};
//----------------------------------------------------------------------------------------
class StrongRule
{       
    public:
        vector <long>  left,right;
	double confidence,support;
        StrongRule(vector <long> l,vector <long>  r)
        {
            this->left = l;
	    this->right =r;
        }
	
};
 //----------------------------------------------------------------------------------------
class freqItemSet
{       
    public:
        vector <long>  ItemSet;
	double support;
        freqItemSet(vector <long> I,double s)
        {
            this->ItemSet = I;
	    this->support =s;
        }
	
};
//----------------------------------------------------------------------------------------
// printVecOfVec: prints 1 vector per line
//----------------------------------------------------------------------------------------
void printVecOfVec(vector<vector<long> > V){
	vector< vector<long> >::iterator row;
	vector<long>::iterator col;
	for (row = V.begin(); row != V.end(); row++) {
	    for (col = row->begin(); col != row->end(); col++) {
		cout << *col <<" ";
	    }
	    cout << "\n";
	}

}
//----------------------------------------------------------------------------------------
// get_num_transactions: counts the number of transactions in the input file
//----------------------------------------------------------------------------------------

const long get_num_transactions(char * FileName){
	string line;
	ifstream fp;
	long num=0;
	
	fp.open(FileName);
	if (fp.is_open()){
		while(getline(fp,line)){
			num++;
		}
	fp.close();
	}
	else cout << "Unable to open file"; 
	const long num_trans=num;
	return num_trans;
}

//----------------------------------------------------------------------------------------
// read_Transactions: read transactions in the input file and store them in vector < vector <long > >
//----------------------------------------------------------------------------------------
vector< vector<long> >  read_Transactions(char * FileName){
	string item,line;
	char delim='\n';
	long nn=0;
	nn=get_num_transactions(FileName);
	if (threshold!=0 && threshold<nn){
		nn=threshold;
	}
	const long num_trans=nn;
	num_transactions=num_trans;
	vector< vector<long> > transactions(num_trans);
	ifstream fp;
	long itemL;
	int ind=0;	
	fp.open(FileName);
	
	if (fp.is_open()){
		while(getline(fp,line,delim)){
			if(threshold!=0 && ind==threshold){
			    break;
			}
			stringstream ss;
			ss.str(line);
			while(getline(ss,item,' ')){
				itemL=stol(item,nullptr,10);
				transactions[ind].push_back(itemL);		
			}
			ind++;
		}
	fp.close();
	}
	else cout << "Unable to open file"; 
	return transactions;
}
//----------------------------------------------------------------------------------------
// insert all items into an unorderedmap , the value for each item is the number of times item occured in transactions
//----------------------------------------------------------------------------------------
unordered_map<long,long> createL1map(vector< vector<long> > transactions){
	unordered_map<long,long> L1map;
	vector< vector<long> >::iterator row;
	vector<long>::iterator col;
	for (row = transactions.begin(); row != transactions.end(); row++) {
	    for (col = row->begin(); col != row->end(); col++) {
		auto t = L1map.find(*col);
		if (t == L1map.end()) {
			L1map[*col]=1;
		}
		else{
			L1map[*col]+=1;
		}
		
	    }
	}
	
	return L1map;
}
//----------------------------------------------------------------------------------------
// findL1: find frequent 1-itemset and return them as vector< freqItemSet* >
//----------------------------------------------------------------------------------------
vector< freqItemSet* > findL1(vector< vector<long> > transactions, unordered_map<long,long> L1map, double min_support){

	vector< freqItemSet* > L1;
		
	for(auto kv : L1map) {
	// >=min_support or >min_supports
	    if(((double)kv.second/num_transactions)>=min_support){
	            vector<long> e;
	       	    e.push_back(kv.first);
		    freqItemSet* fIS=new freqItemSet(e,(double)kv.second/num_transactions);
		    L1.push_back(fIS);
	    }

	}
	return L1;
}
//----------------------------------------------------------------------------------------
// generate_candidates : find nodes at depth k. generate all possible pairs of their children
//----------------------------------------------------------------------------------------
vector<vector<long>> generate_candidates(TreeNode *t,long k){
	
	vector<vector<long> > candidates;
	vector<vector<long> > Childcandidates;
	unordered_map<long,TreeNode*>::iterator kv;
	unordered_map<long,TreeNode*>::iterator kv1;
	if(k==0){	
		for(kv=t->children.begin(); kv != t->children.end(); kv++) {
		    for(kv1=kv; kv1 != t->children.end(); kv1++) {
		    	if(kv->first!=kv1->first){
				vector<long> e;
				if(kv->first<kv1->first){
					e.push_back(t->value);
					e.push_back(kv->first);
					e.push_back(kv1->first);
				}
				else{
					e.push_back(t->value);
					e.push_back(kv1->first);
					e.push_back(kv->first);				
				}	
				candidates.push_back(e);
			}
		    }
		}
		return candidates;
	}
	for(auto kv : t->children) {
	    Childcandidates=generate_candidates(kv.second,k-1);
	    long Childcandidates_size=Childcandidates.size();
	    for (long i=0;i<Childcandidates_size;i++) {
		Childcandidates[i].insert(Childcandidates[i].begin(),t->value);
		candidates.push_back(Childcandidates[i]);
		
	    }
	}
	return candidates;
}
//----------------------------------------------------------------------------------------
// findVecInTree: return true if vector exists in the tree, false otherwise
//----------------------------------------------------------------------------------------
bool findVecInTree(TreeNode *t,vector<long> V){
	
	TreeNode * iter=t;
	vector<long>::iterator col;
	for (col = V.begin(); col != V.end(); col++) {
		auto a = iter->children.find(*col);
		if (a == iter->children.end()) {
			return false;
		}
		iter=iter->children[*col];
	}
	return true;
}
//----------------------------------------------------------------------------------------
//  PruneCandidates: if (k-1)-subset of C does not exist in L(k-1), prune it
//----------------------------------------------------------------------------------------
vector< vector<long> > PruneCandidates(TreeNode *t,long k,vector<vector<long>> C){
	vector< vector<long> >::iterator row;
	vector< vector<long> > PrunedCandidates;
	if(k==2){
		return C;	
	}
	bool prune=false;
	for (row = C.begin(); row != C.end(); row++) {
	    prune=false;
	    for(int i=0;i<k-2;i++){
		vector<long> tmp(*row);
		tmp.erase(tmp.begin()+i);
	  	if(!findVecInTree(t,tmp)){
		//remove candidate
			prune=true;
			break;	
		}
	    }
            if(!prune){
	    	PrunedCandidates.push_back(*row);
	    }
	}
	return PrunedCandidates;
}
//----------------------------------------------------------------------------------------
//apriori_gen : as described in the paper
//----------------------------------------------------------------------------------------

vector< vector<long> > apriori_gen(vector< freqItemSet* > L,long k){
	vector< vector<long> > C;
	Tree *t=new Tree(0);
	//join
	vector< freqItemSet >::iterator row;
	vector<long>::iterator col;
	for (int i=0;i<L.size();i++) {
		t->insert(L[i]->ItemSet);
	}
	C=generate_candidates(t->root,k-2);
	//remove root
	for(int i=0;i<C.size();i++){
		C[i].erase(C[i].begin());
	}
	
	//prune
	vector< vector<long> > Candidates=PruneCandidates(t->root,k,C);
	return Candidates;
}
//----------------------------------------------------------------------------------------
//subset : as described in the paper
//----------------------------------------------------------------------------------------
void subset(TreeNode * t, vector<long> transaction){
	
	if(t->children.size()==0){
		t->counter=t->counter+1;
		return;				
	}
	for(long i=0;i<transaction.size();i++){
		auto a = t->children.find(transaction[i]);
		if (a != t->children.end()) {
			vector<long> tmp(transaction);
			for(int j=0;j<=i;j++){
				tmp.erase(tmp.begin());
			}
			subset(t->children[transaction[i]],tmp);
		}
		
	}	
	//cout <<"\n";
}
//----------------------------------------------------------------------------------------
vector< freqItemSet* > generateLargeItemSet(TreeNode * t, double min_support){
	// >= min_support or > min_support
	vector< freqItemSet* >  L;
	vector< freqItemSet* >  ChildL;
	if(t->children.size()==0 && (double)(t->counter)/num_transactions>=min_support){
		vector<long>  e;
		e.push_back(t->value);	
		freqItemSet* fis=new freqItemSet(e,(double)(t->counter)/num_transactions);
		L.push_back(fis);
		return L;
	}
	unordered_map<long,TreeNode*>::iterator kv;
	for(auto kv : t->children) {
		ChildL=generateLargeItemSet(kv.second,min_support);
		long ChildLargeItemSet_size=ChildL.size();
		for (long i=0;i< ChildLargeItemSet_size;i++) {
			ChildL[i]->ItemSet.insert(ChildL[i]->ItemSet.begin(),t->value);
			L.push_back(ChildL[i]);
		}
	}
	
	return L;
}
//----------------------------------------------------------------------------------------
vector< vector< long > > chooseLeftItems(int LeftSize, vector <long> L ){
	vector< vector< long > > LeftItems;
	vector< vector< long > > LI;
	if(LeftSize==1){
		for(int i=0;i<L.size();i++){
			vector< long > e;
			e.push_back(L[i]);
			LeftItems.push_back(e);
		}
		return LeftItems;
	}	
	for(int i=0;i<L.size()-1;i++){		
		vector< long > tmp(L);
		for(int j=0; j<=i; j++){
			tmp.erase(tmp.begin());
		}
		LI=chooseLeftItems(LeftSize-1, tmp);
		for(int h=0; h<LI.size();h++){
			LI[h].insert(LI[h].begin(),L[i]);
		}
		for(int h=0; h<LI.size();h++){
			LeftItems.push_back(LI[h]);
		}
	}


	return LeftItems;
}
//----------------------------------------------------------------------------------------

long findCounter(TreeNode * root, vector<long> L){
	
	TreeNode * iter=root;
	vector<long>::iterator col;
	for (col = L.begin(); col != L.end(); col++) {
		auto a = iter->children.find(*col);
		if (a == iter->children.end()) {
			return 0;
		}
		iter=iter->children[*col];
	}
	return iter->counter;

}
//----------------------------------------------------------------------------------------
void  printStrongRules(vector<StrongRule*> SR){
	vector<long> left,right;
	for(int i=0;i<SR.size();i++){
		left=SR[i]->left;
		right=SR[i]->right;
		int j;
		for(j=0;j<left.size()-1;j++){
			cout<<left[j]<<", ";
		}
		cout<<left[j]<<" ";
		cout << "-> ";
		for(j=0;j<right.size()-1;j++){
			cout<<right[j]<<", ";
		
		}
		cout<<right[j];
		printf(" (%.2f, %.2f)",SR[i]->support, SR[i]->confidence);
		cout << "\n";
	}


}
//----------------------------------------------------------------------------------------
vector<StrongRule*> generateStrongRules(unordered_map<long,long> L1map, vector< TreeNode* > candidateTreeVec, vector< freqItemSet* >  L, long itemsetSize, double min_confidence){
	vector<StrongRule *> SRules;
	vector< vector< long > > LeftItems;	
	long SupportLR,SupportL;
	TreeNode * root;
	for(int i=0; i<L.size(); i++){
		SupportLR=(L[i]->support)*num_transactions;
		for(long leftSize=1;leftSize<itemsetSize;leftSize++){
			
			LeftItems=chooseLeftItems(leftSize,L[i]->ItemSet);	
			for(int h=0;h<LeftItems.size();h++){
				if(leftSize==1){
					SupportL=L1map[LeftItems[h][0]];
				}
				else{
					root=candidateTreeVec[leftSize-2];
					SupportL=findCounter(root,LeftItems[h]);			
				}
				if((double)SupportLR/SupportL>=min_confidence){
					//create right itemset
					vector <long> right(L[i]->ItemSet);
					int Lind=0;
					for(int u=0;u<LeftItems[h].size();u++){
						//remove left items from right
						Lind=0;
						while(right[Lind]!=LeftItems[h][u]){
							Lind++;						
						}
						right.erase(right.begin()+Lind);
					}
					StrongRule *Sr=new StrongRule(LeftItems[h],right);
					Sr->confidence=(double)SupportLR/SupportL;
					Sr->support= (double)SupportLR/num_transactions;
					SRules.push_back(Sr);
				}
			}
		}
	}	
	
	return SRules;
}
//----------------------------------------------------------------------------------------
void display_StrongRules(vector< vector< StrongRule* > > StrongRules){
	for(int i=0;i<StrongRules.size();i++){
		printStrongRules(StrongRules[i]);
	}
}

//----------------------------------------------------------------------------------------
void display_freqItemSets(vector< vector< freqItemSet* > >  L){

	for(int i=0;i<L.size();i++){
		for(int j=0;j<L[i].size();j++){
			freqItemSet* f=L[i][j];
			int h;
			for(h=0;h<f->ItemSet.size()-1;h++){
				cout << (f->ItemSet)[h] <<", ";
			}
			cout << (f->ItemSet)[h] <<" "<< "(";
			printf("%.2f",f->support);
			cout << ")\n";

		}	
	}
}	

//----------------------------------------------------------------------------------------
void display_stats(vector< vector< freqItemSet* > >  L, vector< vector< StrongRule* > > StrongRules){
	for(int i=0;i<L.size();i++){
		cout <<"Number of frequent "<<i+1<<"_itemsets: "<<L[i].size()<<"\n";
	}
	long SRsize=0;
	for(int i=0;i<StrongRules.size();i++){
		SRsize+=StrongRules[i].size();
	}
	cout<<"Number of association rules: "<<SRsize<<"\n";
	cout <<"Execution time: " ;
	cout << double( clock() - startTime ) / (double)CLOCKS_PER_SEC<< " seconds." << endl;
}

//----------------------------------------------------------------------------------------
int main(int argc, char *argv[]){
	
	startTime = clock();

	char * FileName,* display_opt=(char*)"0";
	double min_support,min_confidence;
	FileName=argv[1];
	min_support=atof(argv[2]);
	min_confidence=atof(argv[3]);
	if(argc>4){
		display_opt=argv[4];
	}
	vector< vector<long> > transactions=read_Transactions(FileName);
	vector< vector< freqItemSet* > >  L;
	vector< vector< vector<long> > >  C;
	vector< TreeNode * > candidateTreeVec;
	vector< vector< StrongRule* > > StrongRules;
	unordered_map<long,long> L1map=createL1map(transactions);

	L.push_back(findL1(transactions,L1map,min_support));	
	vector< vector<long> > dummy;
	C.push_back(dummy);

	vector< vector<long> >::iterator row;
		
	for(int k=1;L.size()==k;k++){
		//apriori_gen
		vector< vector<long> > CC=apriori_gen(L[k-1],k+1);
		if(CC.size()==0){
			break;		
		}
		C.push_back(CC);
		Tree * candidateTree=new Tree(0);
		vector< vector<long> >::iterator row;
		for (row = C[k].begin(); row != C[k].end(); row++) {
			candidateTree->insert(*row);
		}
		
		candidateTreeVec.push_back(candidateTree->root);

		//subset
		for (row = transactions.begin(); row != transactions.end(); row++) {
		    subset(candidateTree->root,*row);
		    
		}

		//generateLargeItemSet
		vector< freqItemSet* > LL=generateLargeItemSet(candidateTree->root,min_support);
		for(int i=0;i<LL.size();i++){
			LL[i]->ItemSet.erase(LL[i]->ItemSet.begin());
		}
		if(LL.size()!=0){
			L.push_back(LL);
		}
	
	}
	
	if(display_opt[0]=='f'){
		display_freqItemSets(L);
	} 
	else if(display_opt[0]=='a' || display_opt[0]=='r' || display_opt[0]=='0'){
		//generate Strong rules
		long totalSRcnt=0;
		for(int i=1;i<L.size();i++){
			vector<StrongRule*> SR=generateStrongRules(L1map,candidateTreeVec, L[i], i+1, min_confidence);
			totalSRcnt+=SR.size();
			//printStrongRules(SR);
			StrongRules.push_back(SR);
		}
		//cout<<"\ntotal: "<<totalSRcnt;	
		if(display_opt[0]=='0'){
			display_stats(L,StrongRules);
		}
		else if(display_opt[0]=='r'){
			display_StrongRules(StrongRules);
		}
		else if(display_opt[0]=='a'){
			display_freqItemSets(L);
			display_StrongRules(StrongRules);
		}
		
	}
	
	


  	return 0;
}
