#include "Structs.hpp"


CaseListNode::CaseListNode(string & rid,string & pfn,string & pln,string & did,string & c,string & entry,string & exit){ //constructor 
	patient = new Case(rid, pfn, pln, did, c, entry, exit);
	next = NULL;
}


//return 1 la8os imerominia, return 2 idios kwdikos
int CasesList::AddNode(string & rid,string & pfn,string & pln,string & did,string & c,string & entry,string & exit){ //pros8iki antikeimenou sti lista

	CaseListNode *newNode = new CaseListNode( rid, pfn, pln, did, c, entry, exit);
	if (Wrong_Date(newNode->patient->entryDate,newNode->patient->exitDate)){
		delete newNode;
		return 1;
	}
	if (IsEmpty()){
		head = newNode;
		tail = newNode;
		counter = 1;
		return 0;
	}
	if (recordID_is_exist(newNode->patient->recordID)){
		delete newNode;
		return 2;
	}
	tail->next=newNode;
	tail = newNode;
	counter++;
	return 0;

}

int CasesList::recordPatientExit(int recordID,Date exitDate){ //ektelei ti recordPatientExit

	CaseListNode *node = head;
	while (node != NULL){
		if (node->patient->recordID == recordID){
			if (node->patient->exitDate.date_size == 10000000){
				node->patient->exitDate = exitDate;
				return 0; //an ola pane kala epistrefei 0
			}
			else 
				return 1; //an uparxei idi exitDate sto case pou dwsame epistrefei 1
		}
		else{
			node = node->next;
		}
	}
	return 2;//an de vre8ei to id pou dwsame,simainei oti den uparxei sti lista ara epistrefei 2

}
	
CasesList::~CasesList(){ //destructor
	CaseListNode *temp;
	while(head){
		temp = head;
		head = head->next;
		delete temp;
	}
}
	
bool CasesList::recordID_is_exist(int rid){ //vriskei an to recordid pou tou dwsame uparxei idi sti lista
	CaseListNode *temp = head;
	while (temp != NULL){
		if (rid == temp->patient->recordID)
			return true;
		temp = temp->next;
	}
	return false;
}
	
bool CasesList::Wrong_Date(Date entryDate,Date exitDate){
	return entryDate.date_size > exitDate.date_size; //epistrefei False an einai swsto
}


BucketMember::BucketMember(){ // contsructor

	avl_tree = new AVL_tree();

}

BucketMember::~BucketMember(){ //destructor
	delete avl_tree;
}


BucketList::~BucketList(){ //destructor 
	Bucket *bucket;
	for (int i=0;i < counter; i++){
		bucket = head;
		head = head->next;
		delete bucket;
	}
}

bool BucketList::isExist(string disease_country,int key_date,CaseListNode *patient_node){ //an i as8eneia i' i xwra uparxei idi sto bucket tote to pros8etei sti swsti 8esi kai 
	//epistrefei true alliws false
	Bucket *bucket = head;
	int i;
	while (bucket){
		for (i = 0;i < bucket->occupied;i++){
			if (bucket->bm[i].key_id == disease_country){
				bucket->bm[i].avl_tree->insert(key_date,patient_node);
				return true;
			}
		}
		bucket = bucket->next;
	}
	return false;
}

void BucketList::globalDiseaseStats(int date_size1 ,int date_size2){ //an dwsoume dates tote i efarmogi ektupwnei ola ta krousmata tis ka8e as8eneias
	//alliws ektupwnei ta krousmata pou sunevisan anamesa stis duo imerominies
	Bucket *bucket = head;
	int i;
	while (bucket){
		for (i = 0;i< bucket->occupied;i++){
			if (date_size1 == 0){
				cout << bucket->bm[i].key_id << " " << bucket->bm[i].avl_tree->counter << endl; //mpainww mesa sto dentro kai painrw to counter pou exei pou einai ta sunolika 
			}																					//krousmata tou iou kai parallila oloi komvoi tou dentrou
			else{
				cout << bucket->bm[i].key_id << " " << bucket->bm[i].avl_tree->get_patient_frequency(date_size1, date_size2) << endl;
			}																					//kalw ti get_patient_frequency pou vriskei ton akrivi ari8mo krousmatwn anamesa
		}																						//stis imerominies
		bucket = bucket->next;
	}

}

void BucketList::diseaseFrequency(string virus,int date_size1,int date_size2,string country){

	Bucket *bucket = head;
	int i;
	int counter = 0;
	while (bucket){
		for (i = 0;i< bucket->occupied;i++){
			if (bucket->bm[i].key_id == virus){
				if (country == ""){
					cout << bucket->bm[i].key_id << " " << bucket->bm[i].avl_tree->get_patient_frequency(date_size1, date_size2) << endl;
				}																//an de do8ei xwra apla kanei oti ekane i globalDiseaseStats
				else{
					cout << bucket->bm[i].key_id << " " << bucket->bm[i].avl_tree->get_patient_frequency(date_size1, date_size2, country) << endl;	
				}																//vriskei ta krousmata tou iou sti sugkekrimeni xwra 
				counter = 1;
			}
		}
		bucket = bucket->next;
	}
	if (counter == 0)
		cout << "Not found" << endl;
}

void BucketList::topkDiseases(int k ,string country ,int date_size1 ,int date_size2 ){
	Bucket *bucket = head;
	MaxHeap *heap = new MaxHeap();
	int i,j;
	int counter = 0;
	while (bucket){
		for (i = 0;i < bucket->occupied; i++){
			if (bucket->bm[i].key_id == country){
				if (date_size1 != 0){
					heap->fillHeap_Diseases(bucket->bm[i].avl_tree->root ,date_size1 ,date_size2); //gemizei to heap kai vgazei apotelesmata analoga me to k pou 8a dwsoume
					if (k > heap->current_index) //to heap ta vgazei me vasi kai to key = ari8mo alla an ta key einai isa tote vgazei me proteraiotita to disease
						k = heap->current_index; //i' to country , an to k einai megalutero twn stoixeiwn tote apla tupwnei ola ta stoixeia
					for (j = 0; j < k; j++){
						heap->getMax();
					}
				}
				else{
					heap->fillHeap_Diseases(bucket->bm[i].avl_tree->root );
					if (k > heap->current_index)
						k = heap->current_index;
					for (j = 0; j < k; j++){
						heap->getMax();
					}
				}
				counter = 1;
			}
		}
		bucket = bucket->next;
	}
	if (counter == 0)
		cout << "Not found" << endl;
}

void BucketList::topkCountries(int k ,string disease ,int date_size1 ,int date_size2 ){ //oti ekana gia ta toplDiseases
	Bucket *bucket = head;
	MaxHeap *heap = new MaxHeap();
	int i,j;
	int counter = 0;
	while (bucket){
		for (i = 0;i < bucket->occupied; i++){
			if (bucket->bm[i].key_id == disease){
				if (date_size1 != 0){
					heap->fillHeap_Countries(bucket->bm[i].avl_tree->root ,date_size1 ,date_size2);
					if (k > heap->current_index)
						k = heap->current_index;
					for (j = 0; j < k; j++){
						heap->getMax();
					}
				}
				else{
					heap->fillHeap_Countries(bucket->bm[i].avl_tree->root );
					if (k > heap->current_index)
						k = heap->current_index;
					for (j = 0; j < k; j++){
						heap->getMax();
					}
				}
				counter = 1;
			}
		}
		bucket = bucket->next;
	}
	if (counter == 0)
		cout << "Not found" << endl;
}

void BucketList::numCurrentPatients(string virus){ //ektelw ti numCurrentPatients

	Bucket *bucket = head;
	int i;
	int counter = 0;
	if (virus != ""){
		while (bucket){
			for (i = 0;i< bucket->occupied; i++){
				if (bucket->bm[i].key_id == virus){
					cout << bucket->bm[i].key_id << " " << bucket->bm[i].avl_tree->get_active_cases() <<endl; //vriskei ta cases sto avl tree pou den eixan exit date
					counter = 1;
				}
			}
			bucket = bucket->next;
		}
	}
	else {
		while (bucket){
			for (i = 0;i< bucket->occupied; i++){
				cout << bucket->bm[i].key_id << " " << bucket->bm[i].avl_tree->get_active_cases() << endl;
			}
			bucket = bucket->next;
		}
		counter = 1;
	}
	if (counter == 0)
		cout<< "Not found" << endl;
}


int BucketList::insert_item(string disease_country,int key_date,CaseListNode *patient_node){

	if (isEmpty()){ //an einai adeia tin arikopoiw kai vazw to prwto stoixeio
		Bucket *bucket = new Bucket(Bucket_member_size);
		bucket->bm[bucket->occupied].key_id = disease_country;
		bucket->bm[bucket->occupied].avl_tree->insert(key_date,patient_node);
		bucket->occupied += 1; // i prwti eleu8eri 8esi enos pinaka apo BucketMembers
		counter += 1; //posa buckets exei i lista
		head = bucket;
		tail = bucket;
		return 1;
	}
	if (isExist(disease_country,key_date,patient_node)){ //an to item uparxei bgainei apo to programma
		return 0;
	}
	if (tail->Bucket_member_size == tail->occupied){ //an exei gemisei to Bucket apla ftiaxnei kainourio kai to pros8etei se ekeino
		Bucket *bucket = new Bucket(Bucket_member_size);
		bucket->bm[bucket->occupied].key_id = disease_country;
		bucket->bm[bucket->occupied].avl_tree->insert(key_date,patient_node);
		bucket->occupied += 1;
		counter += 1;
		tail->next = bucket;
		tail = bucket;
		return 1;
	}
	tail->bm[tail->occupied].key_id = disease_country; //an de mpei sta alla simainei oti vrike bucket pou eixe xwro kai mpike sti prwti adeia 8esi
	tail->bm[tail->occupied].avl_tree->insert(key_date,patient_node);
	tail->occupied += 1;
	return 1;

}


int Hashtable::Hash_function(string str){ //universal hash function me string

	int h = 0;
	int a = 6;
	int p = 7919;
	int i = 0;
	while (str[i]){
		h = ( h*a+str[i] ) % p;
		i++;
	}
	return ( h%length );

}

Hashtable::Hashtable(int length,int Bucket_member_size){ //constructor
	this->length = length;
	items_counter = 0;
	hashtable = new BucketList*[length];
	for (int i = 0;i < length; i++){
		hashtable[i] = new BucketList(Bucket_member_size);
	}
}

void Hashtable::insert_item(string disease_country,int key_date,CaseListNode *patient_node){ //vazei ta item

	int position = Hash_function(disease_country);
	items_counter += hashtable[position]->insert_item(disease_country,key_date,patient_node);

}

void Hashtable::globalDiseaseStats(int date_size1,int date_size2 ){ //kalei ti globalDiseaseStats pou ekteleitai parapanw

	int i;
	for (i = 0; i < length; i++){
		hashtable[i]->globalDiseaseStats(date_size1,date_size2);
	}

}

void Hashtable::diseaseFrequency(string virus,int date_size1,int date_size2,string country){ //kalei ti diseaseFrequency pou ekteleitai parapanw

	int position = Hash_function(virus);
	hashtable[position]->diseaseFrequency(virus,date_size1,date_size2,country);

}

void Hashtable::topkDiseases(int k,string country,int date_size1,int date_size2){ //kalei ti topkDiseases pou ekteleitai parapanw
	int position = Hash_function(country);
	hashtable[position]->topkDiseases(k,country,date_size1,date_size2);
}

void Hashtable::topkCountries(int k,string disease,int date_size1,int date_size2){ //kalei ti topkCountries pou ekteleitai parapanw
	int position = Hash_function(disease);
	hashtable[position]->topkCountries(k,disease,date_size1,date_size2);
}

void Hashtable::numCurrentPatients(string virus){ //kalei ti numCurrentPatients pou ekteleitai parapanw

	if (virus != ""){
		int position = Hash_function(virus);
		hashtable[position]->numCurrentPatients(virus);
	}
	else {
		for (int i = 0;i < length; i++){
			hashtable[i]->numCurrentPatients(virus);
		}
	}

}

Hashtable::~Hashtable(){ //Destructor
	for (int i = 0;i < length; i++){
		delete hashtable[i];
	}
	delete [] hashtable;
}