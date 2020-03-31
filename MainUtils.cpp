#include "MainUtils.hpp"

int MainUtils::isNumber(char* number){ //tsekarw an dinw ari8mo apo to pliktrologio
    int i;
    for (i=0;i<strlen(number);i++){
        if (number[i]<'0' || number[i]>'9') //koitaw an ka8e xaraktiras den einai anamesa stous xaraktires 0 kai 9
            return 0; // an den einai gurnaw 0
    }
    return 1; //alliws gurnaw 1
}   

inline bool MainUtils::file_exists (string& name) { //checkarei an to arxeio pou edwsa uparxei an uparxei gurnaei true
    if (FILE *file = fopen(name.c_str(), "r")) { //alliws false 
        fclose(file);
        return true;
    } else {
        return false;
    }   
} 

void MainUtils::getArgs(int argc,char **argv) {
    
    int p=0 ,h1=0 ,h2=0 ,b=0;
    cout << "You have entered " << argc << " arguments:" << "\n"; 
  
    if (argc != 9){
        cout << "La8os ari8mos orismatwn" << endl;
        exit(1);
    }

    for(int i=1; i < 9; i+=2){
        if (string(argv[i]) == "-p"){  
            params.patientRecordsFile = argv[i+1];
            if (file_exists(params.patientRecordsFile) == false){
               cout << "The file doesn't exist" << endl;
               exit(1);
            }
            p = 1;
        } 
        if (string(argv[i]) == "-h1"){ 
            if (isNumber(argv[i+1]) == 0){
                cout << "-h1 must be a number" <<endl;
                exit(1);
            }
            else{
                params.diseaseHashtableNumOfEntries = atoi(argv[i+1]);
                h1 = 1;
            }
        }  
        if (string(argv[i]) == "-h2"){ 
            if (isNumber(argv[i+1]) == 0){
                cout << "-h2 must be a number" <<endl;
                exit(1);
            }
            else{
                params.countryHashtableNumOfEntries = atoi(argv[i+1]);
                h2 = 1;
            }
        } 
        if (string(argv[i]) == "-b"){ 
            if (isNumber(argv[i+1]) == 0){
                cout << "-b must be a number" <<endl;
                exit(1);
            }
            else{
                params.bucketSize = atoi(argv[i+1]);
                b = 1;
            }
        }   
    }
    
    //tsekarw an do8ikan ola ta orismata
    if (p != 1 || h1 != 1 || h2 != 1 || b != 1){ //einai oi counter twn orismatwn opote kai an oloi exoun par8ei mia fora tote 8a prepei na einai oloi 1 alliws einai la8os
        cout << "Wrong arguments" << endl << "./diseaseMonitor -p patientRecordsFile –h1 diseaseHashtableNumOfEntries –h2 countryHashtableNumOfEntries –b bucketSize" <<endl;
        exit(1);
    }

}


void MainUtils::printArgs(void) { //kanoume print ta arguments pou dinontai apo ti grammi entolwn
    cout << "Patients record file " << params.patientRecordsFile << endl ; //perissotero to kanoume gia na epivevaiwsoume oti pernane swsta ta orismata
    cout << "h1 " << params.diseaseHashtableNumOfEntries << endl ;
    cout << "h2 " << params.countryHashtableNumOfEntries << endl ;
    cout << "b " << params.bucketSize << endl ;
}

void MainUtils::getCases(){

    std::ifstream infile(params.patientRecordsFile.c_str());

    string recordID,patientFirstName, patientLastName, diseaseID, country, entry , exit;

    while (infile >> recordID >> patientFirstName >> patientLastName >> diseaseID >> country >> entry >> exit)
    {
        // successfully extracted one line, data is in recordID, patientFirstName, patientLastName, diseaseID, country, entry, exit.
        if (caselist->AddNode(recordID,patientFirstName,patientLastName,diseaseID,country,entry,exit) == 2){
            cout << "H periptwsi tou as8eni uparxei idi sto arxeio, diakoptetai i diadikasia" << endl;
            break;
        }
        else if (caselist->AddNode(recordID,patientFirstName,patientLastName,diseaseID,country,entry,exit) == 1){
            cout << entry << " " << exit << endl;
            cout << "Akuri imerominia e3odou" << endl;
        }
        diseaseHashtable->insert_item(diseaseID,caselist->tail->patient->entryDate.date_size,caselist->tail);
        countryHashtable->insert_item(country,caselist->tail->patient->entryDate.date_size,caselist->tail);
    }    

}

void MainUtils::readCommands(){

    char buffer[1024]; //ston buffer 8a mpei i entoli
    int ch; //to xrisimopoiw gia na vrw to telos tis le3is pou einai -1

    cout<< "Menu:" << endl << "/globalDiseaseStats [date1 date2]" << endl << "/diseaseFrequency virusName date1 date2 [country]" << endl << "/topk-Diseases k country [date1 date2]" << endl << "/topk-Countries k disease [date1 date2]" << endl << "/insertPatientRecord recordID patientFirstName patientLastName diseaseID country entryDate [exitDate]" << endl << "/recordPatientExit recordID exitDate" << endl << "/numCurrentPatients [disease]" << endl << "/exit" << endl << "Starting:" << endl; 
    cout<< "/";
    ch = getchar();
    while ( ch != EOF ){ //oso de pataw enter i diadikasia epanalamvanetai
        switch(ch){
            case '\n':
                parseCommands(buffer); //dinei stin parseCommands tin entoli pou 8elei na ektelestei
                memset(buffer,0,1024); 
                cout<< "/";
                break;
            default:
                strncat(buffer, ((char*) &ch), 1); //pairnei ton epomeno xaraktira , an den ekana cast diladi to ((char*) &ch) tote 8a epairne ton tetarto 
                break;                             //epomeno xaraktika
        }
        ch = getchar();
    }
}

void MainUtils::parseCommands(char *buffer){

    char *token = strtok(buffer, " ");
    if (token == NULL) return; //gia tin periptwsi pou o xristis exei patisei mono enter
    
    if (!strcmp(token,"globalDiseaseStats"))
    {
        token = strtok(NULL," \t\n");

        int counter = 0;
        char Date1[80],Date2[80];
        string entryDate1,entryDate2;
    
        while (token != NULL){
            if (counter == 0)
                strcpy(Date1, token);
            else if (counter == 1)
                strcpy(Date2, token);
            counter += 1;
            token = strtok(NULL, " \t\n");
        }
        if (counter == 0)
        {
            //cout<<"Kaloume tin sunartisi globalDiseaseStats gia kanena antikeimeno"<<endl; 
            diseaseHashtable->globalDiseaseStats();
            return;
        }
        else if (counter == 2){
            entryDate1 = string(Date1); //den uparxei to country ara i timi pou apo8ikeuei i xwra einai to Date1
            entryDate2 = string(Date2); //kai to Date1 einai to Date2
            //cout<<"Kaloume tin sunartisi recordPatientExit me tis times" << entryDate1 << entryDate2 <<endl;
            Date d1(Date1),d2(Date2);
            diseaseHashtable->globalDiseaseStats(d1.date_size,d2.date_size);
        }
        else{
            cout<<"Error"<<endl;
            return;
        }
    }
    else if (!strcmp(token,"diseaseFrequency"))
    {
        token = strtok(NULL," \t\n");

        if (token ==NULL)
        {
            cout<<"Error"<<endl;
            return;
        }

        int counter = 0;
        char virus[80],xwra[80],Date1[80],Date2[80];
        string virusName,country,entryDate1,entryDate2;
        
        while (token != NULL){
            if (counter == 0)
                strcpy(virus, token);
            else if (counter == 1)
                strcpy(Date1, token);
            else if (counter == 2)
                strcpy(Date2, token);
            else if (counter == 3)
                strcpy(xwra, token);
            counter += 1;
            token = strtok(NULL, " \t\n");
        }
        if (counter == 3){
            virusName = string(virus);
            entryDate1 = string(Date1); //den uparxei to country ara i timi pou apo8ikeuei i xwra einai to Date1
            entryDate2 = string(Date2); //kai to Date1 einai to Date2
            //cout<<"Kaloume tin sunartisi recordPatientExit me tis times"<< virusName << entryDate1 << entryDate2 <<endl;
            Date d1(Date1),d2(Date2);
            diseaseHashtable->diseaseFrequency(virusName,d1.date_size,d2.date_size);
        }
        else if (counter == 4){
            virusName = string(virus);
            entryDate1 = string(Date1);
            entryDate2 = string(Date2);
            country = string(xwra);
            //cout<<"Kaloume tin sunartisi recordPatientExit me tis times"<< virusName << country  << entryDate1 << entryDate2 <<endl;
            Date d1(Date1),d2(Date2);
            diseaseHashtable->diseaseFrequency(virusName,d1.date_size,d2.date_size,country);
        }
        else{
            cout<<"Error"<<endl;
            return;
        }
    }
    else if (!strcmp(token,"topk-Diseases"))
    {
        token = strtok(NULL," \t\n");

        if (token ==NULL)
        {
            cout<<"Error"<<endl;
            return;
        }

        int k,counter = 0;
        char key[20],xwra[80],Date1[80],Date2[80];
        string country;
        
        while (token != NULL){
            if (counter == 0)
                strcpy(key, token);
            else if (counter == 1)
                strcpy(xwra, token);
            else if (counter == 2)
                strcpy(Date1, token);
            else if (counter == 3)
                strcpy(Date2, token);
            counter += 1;
            token = strtok(NULL, " \t\n");
        }
        if (counter == 2 || counter == 4){
            if (isNumber(key) == 0){
                cout<<"Error"<<endl;
                return;
            }
            k = atoi(key);
            country = string(xwra);
            if (counter == 2){ //kaloume ti sunartisi xwris imerominies kai vgazei ta sunolika apotelesmata
                countryHashtable->topkDiseases(k,country);
            }
            else{ //vgazei ta apotelesmata anamesa stis imerominies
                Date d1(Date1),d2(Date2);
                countryHashtable->topkDiseases(k,country,d1.date_size,d2.date_size);
            }
        }
        else{
            cout<<"Error"<<endl;
            return;
        }
    }
    else if (!strcmp(token,"topk-Countries"))
    {
        token = strtok(NULL," \t\n");

        if (token ==NULL)
        {
            cout<<"Error"<<endl;
            return;
        }

        int k,counter = 0;
        char key[20],as8eneia[80],Date1[80],Date2[80];
        string disease,entryDate1,entryDate2;
        
        while (token != NULL){
            if (counter == 0)
                strcpy(key, token);
            else if (counter == 1)
                strcpy(as8eneia, token);
            else if (counter == 2)
                strcpy(Date1, token);
            else if (counter == 3)
                strcpy(Date2, token);
            counter += 1;
            token = strtok(NULL, " \t\n");
        }
        if (counter == 2 || counter == 4){
            if (isNumber(key) == 0){
                cout<<"Error"<<endl;
                    return;
            }
            k = atoi(key);
            disease = string(as8eneia);
            if (counter == 2){
                //an dwsoume 2 arguments tote ekteloume ti topkcountries xwris date1 kai date2
                diseaseHashtable->topkCountries(k,disease);
            }
            else{
                Date d1(Date1),d2(Date2);
                diseaseHashtable->topkCountries(k,disease,d1.date_size,d2.date_size);
            }
        }
        else{
            cout<<"Error"<<endl;
            return;
        }
    }
    else if (!strcmp(token,"insertPatientRecord"))
    {
       token = strtok(NULL," \t\n");

        if (token ==NULL)
        {
            cout<<"Error"<<endl;
            return;
        }

        int counter = 0;
        char id[20],firstname[80],lastname[80],disease[80],xwra[80],Date1[80],Date2[80];
        string recordid,patientFirstName,patientLastName,diseaseID,country,entryDate,exitDate;
        
        while (token != NULL){
            if (counter == 0)
                strcpy(id, token);
            else if (counter == 1)
                strcpy(firstname, token);
            else if (counter == 2)
                strcpy(lastname, token);
            else if (counter == 3)
                strcpy(disease, token);
            else if (counter == 4)
                strcpy(xwra, token);
            else if (counter == 5)
                strcpy(Date1, token);
            else if (counter == 6)
                strcpy(Date2, token);
            counter += 1;
            token = strtok(NULL, " \t\n");
        }
        if (counter >= 6 && counter <= 7){
            if (isNumber(id) == 0){
                cout<<"Error"<<endl;
                return;
            }
            recordid = string(id);
            patientFirstName = string(firstname);
            patientLastName = string(lastname);
            diseaseID = string(disease);
            country = string(xwra);
            entryDate = string(Date1);
            if ( counter == 6 ) //an de dwsoume exitdate tote 8a valw apo default "-"
                exitDate = "-";
            else 
                exitDate = string(Date2); //alliws vazoume to exitdate pou dwsame apo to keyboard
            //eisagoume ta dedomena mesw tis addnode sti caselist kai sta hashtable mesw tis insert
            if (caselist->AddNode(recordid,patientFirstName,patientLastName,diseaseID,country,entryDate,exitDate) == 2){
                cout << "Error" << endl;
            }
            //an gurisei 2 tote to record uparxei idi
            else if (caselist->AddNode(recordid,patientFirstName,patientLastName,diseaseID,country,entryDate,exitDate) == 1){
                cout << entryDate << " " << exitDate << endl;
                cout << "Error" << endl;
            }
            //an gurisei 1 tote dwsame case me la8os imeromia e3odou
            diseaseHashtable->insert_item(diseaseID,caselist->tail->patient->entryDate.date_size,caselist->tail);
            countryHashtable->insert_item(country,caselist->tail->patient->entryDate.date_size,caselist->tail);
            cout<< "Record added" << endl;
        }
        else{
            cout<<"Error"<<endl;
            return;
        }
    }
    else if (!strcmp(token,"recordPatientExit"))
    {
        token = strtok(NULL," \t\n");

        if (token ==NULL)
        {
            cout<<"Error"<<endl;
            return;
        }

        int recordid,counter = 0;
        char id[20],date[80];
        string exit_date;
          
        while (token != NULL){
            if (counter == 0)
                strcpy(id, token);
            if (counter == 1)
                strcpy(date, token);
            counter += 1;
            token = strtok(NULL, " \t\n");
        }
        if (counter == 2){
            if (isNumber(id) == 0){
                cout<<"Error"<<endl;
                return;
            }
            recordid = atoi(id);
            exit_date = string(date);
            Date d(date);
            //cout<<"Kaloume tin sunartisi recordPatientExit me tis times"<< recordid << exit_date <<endl;
            if (caselist->recordPatientExit(recordid,d) == 0){
                cout<< "Record updated" << endl;
            }
            else if (caselist->recordPatientExit(recordid,d) == 1){
                cout<< "Error" << endl;
            }
            else{
                cout<< "Not found" << endl;
            }
        }
        else
            cout<< "Not found" << endl;
    }
    else if (!strcmp(token,"numCurrentPatients"))
    {
        token = strtok(NULL," \t\n");

        if (token ==NULL)
        {
            //cout<<"Kaloume tin sunartisi numCurrentPatients kai ousiastika tupwnei to diseaseHashtable"<<endl;
            diseaseHashtable->numCurrentPatients();
            return;
        }
    
        char as8eneia[80];
        string disease;

        strcpy(as8eneia, token);
        disease = string(as8eneia);
        //cout<<"Kaloume tin sunartisi numCurrentPatients kai tupwnei to diseaseHashtable alla mono gia to disease pou diale3ame"<<endl;
        diseaseHashtable->numCurrentPatients(disease);

    }
    else if (!strcmp(token,"exit"))
    {
        cout<<"Exiting"<<endl;
        delete caselist; //diagrafw ti lista kai ta hashtable kai vgainw apo to programma
        delete countryHashtable;
        delete diseaseHashtable;
        exit(0);
    }
    else{
        cout<<"Error\n"<<endl;
    }

}

MainUtils::MainUtils(int argc,char **argv){

    getArgs(argc, argv);
    printArgs();
    caselist = new CasesList(); //ftiaxnw ti lista me ta cases
    countryHashtable = new Hashtable(params.countryHashtableNumOfEntries,params.bucketSize); //O hashtable opou vazoume tis upo8eseis me vasi ti xwres
    diseaseHashtable = new Hashtable(params.diseaseHashtableNumOfEntries,params.bucketSize); //O hashtable opou vazoume tis upo8eseis me vasi tis as8eneies
    getCases(); //diavazoume to arxeio,pairnoume tis upo8eseis kai tis vazoume sti lista,sto dentro kai sta hashtable
    readCommands(); //diavazoume tis entoles apo to pliktrologio mexri na dw8ei exit

}


int main(int argc,char **argv){
    MainUtils mainutils(argc,argv);
}