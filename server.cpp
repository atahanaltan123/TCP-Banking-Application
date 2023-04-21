#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <fstream>
#include <algorithm>
#include <vector>
 
using namespace std;
struct Bank{   
    string name;
    int hesapNo;
    int totalAmount;
};
struct Kullanici{
    string kAdi;
    string name;
    string soy_isim;
    string sifre;
    Bank banka;
};
void addValuesToTxt();
Kullanici *dataBase()
{       
        static Kullanici client1;
        static Kullanici client2;
        static Kullanici client3;
        static Kullanici client4;
        static Kullanici client5;
        static Kullanici client6;
        static Kullanici arrayKullanici[6] = {client1,client2,client3,client4,client5,client6};
    return arrayKullanici;
}  
string* returnArray(Kullanici Kullanici)
{
    string* array = new string[5];
    array[0] = Kullanici.name;
    array[1] = Kullanici.soy_isim;
    array[2] = Kullanici.banka.name;
    array[3] = to_string(Kullanici.banka.hesapNo);
    array[4] = to_string(Kullanici.banka.totalAmount);

    return array;
}
 int kAdiCheck(string name) // kullanıcı adının veri tabanında olup olmadığını kontrol eden fonksiyon
{
    Kullanici * arrayKullanici;
    arrayKullanici = dataBase();
    int tmp =0;

    string isim;
    for(int i=0;i<6;i++){
        isim = (arrayKullanici+i)->kAdi;
        if((name) == (isim)){
            return 1;
        }
    }
    cout << "HATALI KULLANICI ADI" <<endl;
    return 0;
}
 int userPassCheck(string pass,string name) //verilen kullanıcı adı (name) ve şifre (pass) ile veritabanındaki kayıtlarla karşılaştırma yaparak, 
                                                                    //doğru bir eşleşme olup olmadığını kontrol eder.
{
    Kullanici *arrayKullanici = dataBase();
    for(int i = 0; i < 6; i++) {
        if(arrayKullanici[i].kAdi == name && arrayKullanici[i].sifre == pass) {
            cout << "KULLANICI ADI DOĞRU" << endl;
            cout << "GİRİŞ İŞLEMİ BAŞARILI" << endl;
            return 1;
        }
    }
    cout << "HATALI ŞİFRE" << endl;
    cout << "GİRİŞ İŞLEMİ BAŞARISIZ" << endl;
    return 0;
}
 string *showAccountInfo(string kAdi){ //dataBase() fonksiyonu ile veritabanına erişmek için kullanılan fonksiyon
                                                    //başarılı girilen kullanıcı adının veri tabanındaki kullanıcı adıyla eşleşen
                                                    //bilgilerini döndürür.
    Kullanici *p;
    p=dataBase();
    string isim,*array;
    for(int i=0;i<6;i++){
        isim = (p+i)->kAdi;
        if((kAdi) == (isim)){
            array = returnArray(*(p+i));           
            return array;           
        }
    }
    return 0;
}
 int deposit(string kAdi,int value){
    Kullanici *p;
    p=dataBase();
    string isim;
    for(int i=0;i<6;i++){
        isim = (p+i)->kAdi;
        if((kAdi) == (isim)){
            (p+i)->banka.totalAmount += value;
            return (p+i)->banka.totalAmount;
            
        }
    }
    return 0;
}
 int withdraw(string kAdi,int value){
     Kullanici *p;
    p=dataBase();
    string isim;
    for(int i=0;i<6;i++){
        isim = (p+i)->kAdi;
        if((kAdi) == (isim)){
            if((p+i)->banka.totalAmount>=value){
                (p+i)->banka.totalAmount -= value;           
                return (p+i)->banka.totalAmount;
            }
            else{
                return -1;
            }
            
        }
    }
    return 0;
}
 int transfer(string kAdi,int accountNo,string tarName,string tarsoy_isim, int value){
    Kullanici *p;
    p=dataBase();
    string isim,gidenIsim,gidenSoyisim;
    int userValue,bankaNo,gidenValue, islemUcreti = 0;
    int locationUser,locationTar=-1;

    transform(tarName.begin(), tarName.end(),tarName.begin(), ::toupper);
    transform(tarsoy_isim.begin(), tarsoy_isim.end(),tarsoy_isim.begin(), ::toupper);
    
    for(int i=0;i<6;i++){ //Kullanıcının toplam bakiyesi
        isim = (p+i)->kAdi;
        if((kAdi) == (isim)){
            locationUser = i;
            userValue = (p+i)->banka.totalAmount;
            break;
        }
    }
    for(int i=0;i<6;i++){
        bankaNo = (p+i)->banka.hesapNo;
        
        gidenIsim = (p+i)->name;
        
        gidenSoyisim = (p+i)->soy_isim;

        transform(gidenIsim.begin(), gidenIsim.end(),gidenIsim.begin(), ::toupper);
        transform(gidenSoyisim.begin(), gidenSoyisim.end(),gidenSoyisim.begin(), ::toupper);
        

        if((accountNo == bankaNo) && (gidenIsim==tarName) && (gidenSoyisim==tarsoy_isim)){
            locationTar = i;
            break;
        }
    }
    if(locationTar==-1){
        return -1;
    }
    if((p+locationUser)->banka.name!=(p+locationTar)->banka.name)
        {
        islemUcreti = 5;
        } 
    
    if(userValue >= (value + islemUcreti))
    {
        if (islemUcreti>0)
        {
            (p+locationUser)->banka.totalAmount = (p+locationUser)->banka.totalAmount - (value+islemUcreti);
            (p+locationTar)->banka.totalAmount = (p+locationTar)->banka.totalAmount + value;
            return (p+locationUser)->banka.totalAmount;
        }

        (p+locationUser)->banka.totalAmount = (p+locationUser)->banka.totalAmount - value;
        (p+locationTar)->banka.totalAmount = (p+locationTar)->banka.totalAmount + value;
        return (p+locationUser)->banka.totalAmount;

    }else
    {

        return -2;
    }
}
int socket()
{
    // Socket işlemleri
    int listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == -1)
    {
        cerr << "SOKET OLUŞTURULAMADI" << endl;
        return -1;
    }
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);
 
    bind(listening, (sockaddr*)&hint, sizeof(hint));
 
    // Listen işlemi
    listen(listening, SOMAXCONN);
    cout << "SUNUCU BAŞLADI" << endl;
    // Wait for a connection
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
 
    int clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
 
    char host[NI_MAXHOST];      // Client's remote name
    char service[NI_MAXSERV];   // Service (i.e. port) the client is connect on
 
    memset(host, 0, NI_MAXHOST); // same as memset(host, 0, NI_MAXHOST);
    memset(service, 0, NI_MAXSERV);
 
    if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
    {
        cout << host << " PORTA BAĞLANILDI " << service << endl;
    }
    else
    {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        cout << host << " PORTA BAĞLANILDI " << ntohs(client.sin_port) << endl;
    }
    close(listening);

    char buf[4096];
    string kAdi,userPass;
    
    while (true)
    {
        memset(buf, 0, 4096);
 
        // İstemciden veri bekleme
        

        int bytesReceivedName = recv(clientSocket, buf, 4096, 0);

        if (bytesReceivedName == -1)
        {
            addValuesToTxt();
            cerr << "Error in recv(). Quitting" << endl;
            break;
        }
 
        if (bytesReceivedName == 0)
        {   
            addValuesToTxt();
            cout << "İSTEMCİ ÇIKTI " << endl;
            break;
        }

        string error = "error";
        string correct = "correct";
        string name ;

        cout << "Trying to Login: " << string(buf, 0, bytesReceivedName) << endl;
        
        if(true)
        {   
            name = (string(buf, 0, bytesReceivedName));

            if(kAdiCheck(name)){
                cout << "Doğru kAdi" << endl;
            }

            send(clientSocket, correct.c_str(), correct.size() + 1, 0);
            int bytesReceivedPass = recv(clientSocket, buf, 4096, 0);

            if(userPassCheck((string(buf, 0, bytesReceivedPass)),name)){
                string choice;
                send(clientSocket, correct.c_str(), correct.size() + 1, 0);
                cout << "KULLANICI SEÇİMİ BEKLENİYOR"<<endl;
                cout << " " << endl;
                bytesReceivedPass = recv(clientSocket, buf, 4096, 0);

                if (bytesReceivedPass == -1)
                {
                    addValuesToTxt();
                    cerr << "Error in recv(). Quitting" << endl;
                    break;
                }
 
                if (bytesReceivedPass == 0)
                {   
                    addValuesToTxt();
                    cout << "Client disconnected " << endl;
                    break;
                }


                choice = (string(buf, 0, bytesReceivedPass));
                cout << choice <<endl;
                cout << " " << endl;
                int choiceInt= stoi(choice);
                string response = "1";
                string *array;
                array = showAccountInfo(name);
                switch (choiceInt){
                    case 1:{
                        cout << "HESAP BİLGİLERİ"<<endl;
                        cout << " " << endl;
                        for(int i=0;i<5;i++){       
                            send(clientSocket, (array[i]).c_str(), (array[i]).size() + 1, 0);  
                            bytesReceivedPass = recv(clientSocket, buf, 4096, 0);  
                        }
                    }
                    break;

                    case 2:{

                        cout << "PARA YATIRMA İŞLEMLERİ"<<endl;
                        cout << " " << endl;
                        
                        send(clientSocket, array[4].c_str(), array[4].size() + 1, 0);
                        bytesReceivedPass = recv(clientSocket, buf, 4096, 0);

                        if (bytesReceivedPass == -1)
                        {
                            addValuesToTxt();
                            cerr << "Error in recv(). Quitting" << endl;
                            break;
                        }
 
                        if (bytesReceivedPass == 0)
                        {   
                            addValuesToTxt();
                            cout << "Client disconnected " << endl;
                            break;
                        }

                        
                        int value = stoi(string(buf, 0, bytesReceivedPass));
                        int totalValue = deposit(name,value);
                        string valueStr = to_string(totalValue);
                        
                        send(clientSocket, valueStr.c_str(), valueStr.size() + 1, 0);

                    }
                    break;  

                    case 3:{
                        cout << "PARA ÇEKME İŞLEMLERİ"<<endl;
                        cout << " " << endl;
    
                        send(clientSocket, array[4].c_str(), array[4].size() + 1, 0);
                        bytesReceivedPass = recv(clientSocket, buf, 4096, 0);

                        if (bytesReceivedPass == -1)
                        {
                            addValuesToTxt();
                            cerr << "Error in recv(). Quitting" << endl;
                            break;
                        }
 
                        if (bytesReceivedPass == 0)
                        {   
                            addValuesToTxt();
                            cout << "Client disconnected " << endl;
                            break;
                        }
                        
                        int value = stoi(string(buf, 0, bytesReceivedPass));
                        int totalValue = withdraw(name,value); 
                        string valueStr = to_string(totalValue);
                        
                        send(clientSocket, valueStr.c_str(), valueStr.size() + 1, 0);

                    }
                    break;

                    case 4:
                    {
                        cout << "TRANSFER İŞLEMLERİ"<<endl;
                        cout << " " << endl;
                        send(clientSocket, array[4].c_str(), array[4].size() + 1, 0);
                        bytesReceivedPass = recv(clientSocket, buf, 4096, 0);

                        if (bytesReceivedPass == -1)
                        {
                            addValuesToTxt();
                            cerr << "Error in recv(). Quitting" << endl;
                            break;
                        }
 
                        if (bytesReceivedPass == 0)
                        {   
                            addValuesToTxt();
                            cout << "Client disconnected " << endl;
                            break;
                        }
                        
                        int targetNo = stoi(string(buf, 0, bytesReceivedPass));
                        
                        send(clientSocket, response.c_str(), response.size() + 1, 0);
                        bytesReceivedPass = recv(clientSocket, buf, 4096, 0);
                        
                        if (bytesReceivedPass == -1)
                        {
                            addValuesToTxt();
                            cerr << "Error in recv(). Quitting" << endl;
                            break;
                        }
 
                        if (bytesReceivedPass == 0)
                        {   
                            addValuesToTxt();
                            cout << "Client disconnected " << endl;
                            break;
                        }

                        string tarName = (string(buf, 0, bytesReceivedPass));
                        
                        send(clientSocket, response.c_str(), response.size() + 1, 0);
                        bytesReceivedPass = recv(clientSocket, buf, 4096, 0);
                        
                        if (bytesReceivedPass == -1)
                        {
                            addValuesToTxt();
                            cerr << "Error in recv(). Quitting" << endl;
                            break;
                        }
 
                        if (bytesReceivedPass == 0)
                        {   
                            addValuesToTxt();
                            cout << "Client disconnected " << endl;
                            break;
                        }

                        string tarsoy_isim = (string(buf, 0, bytesReceivedPass));
                        
                        send(clientSocket, response.c_str(), response.size() + 1, 0);
                        bytesReceivedPass = recv(clientSocket, buf, 4096, 0);
                        
                        if (bytesReceivedPass == -1)
                        {
                            addValuesToTxt();
                            cerr << "Error in recv(). Quitting" << endl;
                            break;
                        }
 
                        if (bytesReceivedPass == 0)
                        {   
                            addValuesToTxt();
                            cout << "Client disconnected " << endl;
                            break;
                        }

                        int value = stoi(string(buf, 0, bytesReceivedPass));
                        int snc = transfer(name,targetNo,tarName,tarsoy_isim,value);
                        string sncStr = to_string(snc);

                        send(clientSocket, sncStr.c_str(), sncStr.size() + 1, 0);
                    }    
                    break;         
                }
            }
            else{
                send(clientSocket, error.c_str(), error.size() + 1, 0);
            }
        }
        else{
            send(clientSocket, error.c_str(), error.size() + 1, 0);
        }       
    }
    close(clientSocket);
 
    return 0;
}
 void addValuesFromTxt(){

    Kullanici * arrayKullanici;
    arrayKullanici = dataBase();

    string kAdi,name,soy_isim,sifre,banka_ismi;
    int hesapNo,paraValue;
    
    fstream veri_cek("kullanıcı_bilgileri.txt");
    string satir = "";

    if ( veri_cek.is_open() ){
        
        for(int i=0;i<6;i++){

            veri_cek >> kAdi >> name>> soy_isim >> sifre >> banka_ismi >> hesapNo >> paraValue; 
            
            (arrayKullanici+i)->kAdi = kAdi;
            (arrayKullanici+i)->name = name;
            (arrayKullanici+i)->soy_isim = soy_isim;
            (arrayKullanici+i)->sifre = sifre;
            (arrayKullanici+i)->banka.name = banka_ismi;
            (arrayKullanici+i)->banka.hesapNo = hesapNo;
            (arrayKullanici+i)->banka.totalAmount = paraValue;
        }
        veri_cek.close();
    }
    
}
 void addValuesToTxt(){
    Kullanici * arrayKullanici;
    arrayKullanici = dataBase();

    string kAdi,name,soy_isim,sifre,banka_ismi;
    int hesapNo,paraValue;

    fstream veri_cek("kullanıcı_bilgileri.txt",ios::out | ios::ate);
    string satir = "";

    if ( veri_cek.is_open() ){       
        for(int i=0;i<6;i++){
            kAdi = (arrayKullanici+i)->kAdi ;
            name = (arrayKullanici+i)->name;
            soy_isim = (arrayKullanici+i)->soy_isim;
            sifre = (arrayKullanici+i)->sifre;
            banka_ismi = (arrayKullanici+i)->banka.name;
            hesapNo = (arrayKullanici+i)->banka.hesapNo;
            paraValue = (arrayKullanici+i)->banka.totalAmount;

            veri_cek << kAdi << " "<< name << " " << soy_isim << " "  << sifre << " "  << banka_ismi << " "  << hesapNo << " "  << paraValue << endl; 
        }
        veri_cek.close();
    }
}
int main(int argc, char* argv[]){
    addValuesFromTxt();
    socket();
    return 0;
}