#include "AVL_Heap.hpp"

//epistrefei ta krousmata pou vre8ikan anamesa stis imerominies, an de tirountai oi proupo8eseis apla gurnaei 0 kai sunexizei tin anadromi
int AVL_tree::node_counter(AVL_node* node,size_t date_size1,size_t date_size2){
 
	if (node == NULL)
		return 0;
    else if (node->entry_date < date_size1)
        return 0 + node_counter(node->right,date_size1,date_size2);
    else if (node->entry_date > date_size2)
        return 0 + node_counter(node->left,date_size1,date_size2);
    else
	    return 1 + node_counter(node->left,date_size1,date_size2) + node_counter(node->right,date_size1,date_size2);

}


int AVL_tree::get_patient_frequency(size_t date_size1,size_t date_size2){

	return node_counter(root,date_size1,date_size2);

}

//to idio me parapanw apla twra koitaei kai ti xwra tous otan pros8etei ta cases
int AVL_tree::node_counter(AVL_node* node,size_t date_size1,size_t date_size2,string country){

    if (node == NULL)
        return 0;
    else if (node->entry_date < date_size1)
        return 0 + node_counter(node->right,date_size1,date_size2,country);
    else if (node->entry_date > date_size2)
        return 0 + node_counter(node->left,date_size1,date_size2,country);
    else
        if (node->patient_node->patient->country == country)
            return 1 + node_counter(node->left,date_size1,date_size2,country) + node_counter(node->right,date_size1,date_size2,country);
        else
            return 0 + node_counter(node->left,date_size1,date_size2,country) + node_counter(node->right,date_size1,date_size2,country);

}


int AVL_tree::get_patient_frequency(size_t date_size1,size_t date_size2,string country){

    return node_counter(root,date_size1,date_size2,country);

}

//4axnei tis energa cases me vasi to exit date an auto einai iso me 10000000 diladi "-" tote gurnaei 1 kai sunexizei tin anadromi
int AVL_tree::active_cases_counter(AVL_node* node){

    if (node == NULL)
        return 0;
    else if (node->patient_node->patient->exitDate.date_size == 10000000)
        return 1 + active_cases_counter(node->left) + active_cases_counter(node->right);
    else 
        return 0 + active_cases_counter(node->left) + active_cases_counter(node->right);

}


int AVL_tree::get_active_cases(){

    return active_cases_counter(root);

}



//kalei tin insert na valei case sto dentro
void AVL_tree::insert(size_t entry_date,CaseListNode *patient_node){

	root = insert(root,entry_date,patient_node);
	counter += 1;

}


AVL_node* AVL_tree::insert(AVL_node* node, size_t entry_date, CaseListNode *patient_node){

	/* 1. Perform the normal BST insertion */
    if (node == NULL)  
        return(new AVL_node(entry_date, patient_node));  
  
    if (entry_date <= node->entry_date)  
        node->left = insert(node->left, entry_date, patient_node);  
    else if (entry_date > node->entry_date)  
        node->right = insert(node->right, entry_date, patient_node);    
  
    /* 2. Update height of this ancestor node */
    node->height = 1 + max(height(node->left),  
                        height(node->right));  
  
    /* 3. Get the balance factor of this ancestor  
        node to check whether this node became  
        unbalanced */
    int balance = getBalance(node);  
  
    // If this node becomes unbalanced, then  
    // there are 4 cases  
  
    // Left Left Case  
    if (balance > 1 && entry_date < node->left->entry_date)  
        return rightRotate(node);  
  
    // Right Right Case  
    if (balance < -1 && entry_date > node->right->entry_date)  
        return leftRotate(node);  
  
    // Left Right Case  
    if (balance > 1 && entry_date > node->left->entry_date)  
    {  
        node->left = leftRotate(node->left);  
        return rightRotate(node);  
    }  
  
    // Right Left Case  
    if (balance < -1 && entry_date < node->right->entry_date)  
    {  
        node->right = rightRotate(node->right);  
        return leftRotate(node);  
    }  
  
    /* return the (unchanged) node pointer */
    return node; 

}


int AVL_tree::getBalance(AVL_node* node){

	return node == NULL ? 0 : height(node->left) - height(node->right) ;

}
    

AVL_node* AVL_tree::leftRotate(AVL_node* node){

	AVL_node *y = node->right;  
    AVL_node *x = y->left;  
  
    // Perform rotation  
    y->left = node;  
    node->right = x;  
  
    // Update heights  
    node->height = max(height(node->left),     
                    height(node->right)) + 1;  
    y->height = max(height(y->left),  
                    height(y->right)) + 1;  
  
    // Return new root  
    return y; 

}



AVL_node* AVL_tree::rightRotate(AVL_node* node){

	AVL_node *x = node->left;  
    AVL_node *y = x->right;  
  
    // Perform rotation  
    x->right = node;  
    node->left = y;  
  
    // Update heights  
    node->height = max(height(node->left), 
                    height(node->right)) + 1;  
    x->height = max(height(x->left), 
                    height(x->right)) + 1;  
  
    // Return new root  
    return x;  

}


//vriskei to megisto anamesa se 2 times kai tin epistrefei
int AVL_tree::max(int a, int b){

	return a > b ? a : b ;
    
}


//vriskei to u4os tou dentrou kai to epistrefei
int AVL_tree::height(AVL_node* node){

	return node == NULL ? 0 : node->height ;

}

//destructor
AVL_tree::~AVL_tree(){

	delete_recursive(root);

}

void AVL_tree::delete_recursive(AVL_node * root){

	if (root == NULL){
		return;
	}
	delete_recursive(root->left);
	delete_recursive(root->right);
	delete root;

}



Heap_element * MaxHeap::parent(Heap_element * root, int i){ //ta i einai o va8mos(epipedo) tou dentrou ston opoio 8a pros8esoume apo katw to kainourio komvo
    if(i == 0 && (root->left == NULL || root->right == NULL)) //i diadikasia epistrefei 4axnei anadromika to dentro mexri na vrei to prwto parent
        return root; //pou exei ena keno fulo kai epistrefei ton parent
    if(i < 0)
        return NULL;
    
    Heap_element * p1 = parent(root->left, i-1); 
    Heap_element * p2 = parent(root->right, i-1);
    if(p1) return p1;
    if(p2) return p2;
    return NULL;

}

void MaxHeap::swap(Heap_element * a, Heap_element * b){ //allazei to periexomeno meta3u twn antikeimenwn a kai b pou einai fula tou dentrou
    
    int temp_key;
    string temp_disease_country;   
    temp_key = a->key;
    temp_disease_country = a->disease_country;
    a->key = b->key;
    a->disease_country = b->disease_country;
    b->key = temp_key;  
    b->disease_country = temp_disease_country;

}

void MaxHeap::decreaseKey(Heap_element * element){
    Heap_element * temp = element;
    while((temp->left && temp->right) && ((temp->left->key > temp->key) || (temp->right->key > temp->key)
        || (temp->left->key == temp->key  && temp->left->disease_country < temp->disease_country) 
        || (temp->right->key == temp->key) && (temp->right->disease_country > temp->disease_country))){
        if(temp->right->key > temp->left->key){ // an to de3i paidi einai megalutero apo to aristero 
            swap(temp, temp->right); //tote kanoume swap to komvo pou vu8izoume me to megalutero paidi diladi to de3i sti periptwsi mas
            temp = temp->right; //kai kanoume parent to de3i paidi kai etsi o parent ginetai to de3i fulo kai to de3i fulo o parent
        }
        else { // an to aristero paidi einai megalutero apo to de3i 
            swap(temp, temp->left); //tote kanoume swap to komvo pou vu8izoume me to megalutero paidi diladi to aristero sti periptwsi mas
            temp = temp->left; //kai kanoume parent to aristero paidi kai etsi o parent ginetai to aristero fulo kai to aristero fulo o parent    
        }
    }
    // edw tha exei to poly ena aristero h ena dexi paidi
    if((temp->right) && ((temp->right->key > temp->key) || (temp->right->key == temp->key  && temp->right->disease_country < temp->disease_country))){
        swap(temp, temp->right);
    }
    else if((temp->left) && ((temp->left->key > temp->key) || (temp->left->key == temp->key  && temp->left->disease_country < temp->disease_country))){
        swap(temp, temp->left);
    }
}


void MaxHeap::increaseKey(Heap_element * leaf){ //sti periptwsi pou au3i8ei to kleidi tote allazoume 8esi anamesa sto fulo kai to parent
    while((leaf->parent && leaf->key > leaf->parent->key) || (leaf->parent && leaf->key == leaf->parent->key && leaf->disease_country < leaf->parent->disease_country)){ 
        swap(leaf, leaf->parent); //mexri o parent na einai megaluteros apo to paidi
        leaf = leaf->parent; //kanoume to parent fulo,kai to fulo parent kai sunexizoume
    }
}   

Heap_element * MaxHeap::nextPlaceInHeap(){
    int i = floor(log2(current_index)); // briskw to epipedo sto opoio tha topotheththei o neos komvos,to i sumvolizei to epipedo (level 0 is root)
    return parent(root, i-1);    
}

// an brei komvo me ayto to tel ton epistrefei alliws epistrefei NULL
Heap_element * MaxHeap::findNode(Heap_element * root,string disease_country){
    if(root == NULL){ // an to root == NULL tote to Heap einai adeio ara kai den exw kati na vrw
        return NULL;
    }
    else if(root->disease_country == disease_country){
        return root; // an einai auto pou 4axnw gurnaw to root (profanws den einai mono to root auto pou gurnaw alla apo ekei 3ekinaw)
    }
    Heap_element * t1 = findNode(root->left,disease_country); //kanw anadromi aristera gia na 4axw ousiastika oli tin aristeri pleura tou root
    if(t1) return t1; 
    Heap_element * t2 = findNode(root->right, disease_country); //kanw anadromi gia na 4a3w oli ti de3ia meria katw apo to root
    if(t2) return t2;
    return NULL;
}

// kanei enhmerwsh enos komvou me to neo key
bool MaxHeap::update(int key, string disease_country){
    Heap_element * temp = findNode(root, disease_country);
    // an uparxei komvos enhmerwse ton kai an xreiazetai allaxe thn thesh tou ston heap
    if(temp){
        temp->key += key;
        decreaseKey(temp);
        increaseKey(temp);
        return true;
    }
    // den uparxei komvos me auto to disease_country
    else
        return false;
}

void MaxHeap::insertKey(int key,string disease_country){
    // prwta koitame an tha kanoume update ton Heap
    if(update(key, disease_country))
        return;
    // den egine update ftiaxnoume kainourgio komvo
    Heap_element * temp = new Heap_element; //ftiaxnw to kainourio antikeimeno pou 8a einai kai to teleutaio ara 8a to kanw last node 
    temp->key = key;
    temp->disease_country = disease_country; 
    temp->left = NULL; 
    temp->right = NULL;
    temp->parent = NULL; //ara den exei paidia kai argotera 8a vrw kai ton parent tou
    last_node = temp;
    current_index++;
    // an to Heap einai adeio kanoume to root = temp pou einai to kainourio mas stoixeio
    if(isEmpty()){
        root = temp;
        return;
    }
    Heap_element * parent = nextPlaceInHeap(); //vriskei to epipedo pou einai o parent kai ton epistrefei
    temp->parent = parent;
    if(parent->left == NULL) 
        parent->left = temp;
    else
        parent->right = temp;
    increaseKey(temp); //afou to evala sti teleutaia dia8esimi 8esi ,kanw increaseKey gia na to anevasw sti swsti 8esi tou sto Heap 
    //cout << temp->key << " " << temp->disease_country << endl;
}

void MaxHeap::getMax(){
    int maxKey;
    string maxDisease_Country;
    if(isEmpty()){
        maxKey = 0;
        maxDisease_Country = "";
        cout << "The heap is empty " << endl;
    }
    maxKey = root->key;
    maxDisease_Country = root->disease_country;
    if(current_index == 1){ // yparxei mono h riza sto heap
        delete(root);
        root = NULL;
        last_node = NULL;
        current_index = 0;
        cout << maxDisease_Country << " " << maxKey << endl;
        return;
    }
    // h genikh periptwsh
    root->key = last_node->key; //kanw ti korufi isi me to teleutaio komvo
    root->disease_country = last_node->disease_country;
    Heap_element *temp = last_node; //dinw se ena temporary element to teleutaio node
    if(last_node == last_node->parent->right){ // an o teleuatios komvos einai dexi paidi 
        last_node->parent->right = NULL;
        last_node = last_node->parent->left; // tote o prohgoumenos pou eixe mpei einai to aristero paidi (h eukolh periptwsh)
    }
    // an einai aristero paidi tote prepei na brw ena dexi paidi sto idio epipedo kai an den uparxei tote to pio dexi paidi tou teleutaiou epipedou
    // dhladh tou prohgoumenou epipedou apo auto pou htan o last_node, dhladh htan to pio aristero paidi tou teleutaiou epipedou
    else{
        Heap_element *prov = last_node->parent;
        prov->left = NULL; // twra den exei aristero paidi (fusika oute kai dexio afou to dendro einai plhres)
        while(prov->parent != NULL && prov == prov->parent->left){ //anebainw oso pio dexia panw mporw (isws na ftasw kai sthn riza)
            prov = prov->parent; 
        }
        if(prov->parent == NULL){ // an exw ftasei sthn riza
            if(prov->left == NULL) // an den exei aristero paidi h riza einai o teleutaios komvos tou heap
                last_node = root;
            else {              // alliws phgainw olo dexia gia na brw ton proteleutaio pou eixe mpei
                while(prov->right != NULL)
                    prov = prov->right;
                last_node = prov;
            }
        }
        else {              // den eimai riza, uparxoun kai alloi komvoi aristera sto teleutaio epipedo, psaxnw ton pio dexio apo autous
            prov = prov->parent; // erxomai apo dexia kai anebainw epipedo
            prov = prov->left;    // katebainw ena epipedo apo aristera
            while(prov->right != NULL) // phgainw olo dexia wste na brw ton pio dexi komvo tou teleutaiou epipedou
                prov = prov->right;
            last_node = prov;
        }
    }
    current_index--;
    delete(temp);  
    decreaseKey(root);
    cout << maxDisease_Country << " " << maxKey << endl;
}



void MaxHeap::fillHeap_Diseases(AVL_node* node,size_t date_size1,size_t date_size2){

    if (node == NULL)
        return;
    else if (node->entry_date < date_size1) //an i imerominia einai mikroteri apo tin elaxisti imerominia proxwrame tin anadromi
        fillHeap_Diseases(node->right ,date_size1 ,date_size2);
    else if (node->entry_date > date_size2)
        fillHeap_Diseases(node->left ,date_size1 ,date_size2); //an i imerominia einai mikroteri apo tin megisti imerominia proxwrame tin anadromi
    else{
        fillHeap_Diseases(node->left ,date_size1 ,date_size2);
        insertKey(1,node->patient_node->patient->diseaseID); //alliws vazoume to kleidi sto heap
        fillHeap_Diseases(node->right ,date_size1 ,date_size2);
    }
    
}

void MaxHeap::fillHeap_Diseases(AVL_node* node){

    if (node == NULL)
        return;
    else{
        fillHeap_Diseases(node->left); //kainourio, sunexizoume kai koitazoume oli tin aristeri pleura katw apo to root
        insertKey(1,node->patient_node->patient->diseaseID); //eisagoume to key kai to country sto Heap,an uparxei idi apla 8a kanoume update alliws 8a dimiourgisoume
        fillHeap_Diseases(node->right); //eisaigoume oli ti de3ia pleura apo to root
    }

}



void MaxHeap::fillHeap_Countries(AVL_node* node,size_t date_size1,size_t date_size2){

    if (node == NULL)
        return;
    else if (node->entry_date < date_size1) //an i imerominia einai mikroteri apo tin elaxisti imerominia proxwrame tin anadromi
        fillHeap_Countries(node->right ,date_size1 ,date_size2);
    else if (node->entry_date > date_size2)
        fillHeap_Countries(node->left ,date_size1 ,date_size2); //an i imerominia einai mikroteri apo tin megisti imerominia proxwrame tin anadromi
    else{
        fillHeap_Countries(node->left ,date_size1 ,date_size2);
        insertKey(1,node->patient_node->patient->country); //alliws vazoume to kleidi sto heap
        fillHeap_Countries(node->right ,date_size1 ,date_size2);
    }
    
}

void MaxHeap::fillHeap_Countries(AVL_node* node){

    if (node == NULL)
        return;
    else{
        fillHeap_Countries(node->left); //kainourio, sunexizoume kai koitazoume oli tin aristeri pleura katw apo to root
        insertKey(1,node->patient_node->patient->country); //eisagoume to key kai to country sto Heap,an uparxei idi apla 8a kanoume update alliws 8a dimiourgisoume
        fillHeap_Countries(node->right); //eisaigoume oli ti de3ia pleura apo to root
    }

}

