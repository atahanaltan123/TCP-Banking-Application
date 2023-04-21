#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

using namespace std;



int client()
{
    //	SOKET YAPISI
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        return 1;
    }
    int port = 54000;
    string ipAddress = "127.0.0.1";

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);
    int connectRes = connect(sock, (sockaddr*)&hint, sizeof(hint));
    if (connectRes == -1)
    {
        return 1;
    }

    //	SUNUCUYA GÖNDERİLEN KİMLİK DOĞRULAMA KODLARI
    char buf[4096];
    string user;
	int i=0;
    do {

		if(i==0){
			cout << "Kullanıcı Adı: ";
			cin >> user;
		}

        //SUNUCU İŞLEMLERİ
        int sendRes = send(sock, user.c_str(), user.size() + 1, 0);
        
		if (sendRes == -1 )
        {
            cout << "Sunucuya iletilemedi.\r\n";
            continue;
        }

        //SUCUDAN BAĞLANTI GERİ DÖNÜŞÜ
        
		memset(buf, 0, 4096);
        string gelen;
        int bytesReceived = recv(sock, buf, 4096, 0);

        int choice;
        string choiceStr;
        int i=1;
        
        //VERİ ANALİZİ
        if (true){
            
			cout << "Şifre: ";
			cin >> user;
			send(sock, user.c_str(), user.size() + 1, 0);
            bytesReceived = recv(sock, buf, 4096, 0);
            //ŞİFRE KONTROLÜ
            if (string(buf,0, bytesReceived) == ("correct")){
                cout<< ""<< endl;
                cout << "Hoşgeldiniz" << endl;
                cout<< ""<< endl;
                do {

                cout << "1- Hesap bilgilerimi öğrenmek istiyorum" << endl;
                cout << "2-Hesabıma para yatırmak istiyorum." << endl;
                cout << "3-Para çekmek istiyorum." << endl;
                cout << "4-Başka bir hesaba para yatırmak istiyorum." << endl;
                cout<< ""<< endl;
                cout << "Yapmak istediğiniz işlemi seçin : ";
                cin>>choice;
                cout<< ""<< endl;
                if(choice<=0 || choice>=5) {
                    cout << "Lütfen belirtilen işlemlerden seçim yapın."<<endl;
                    cout<< ""<< endl;
                }

                }while(choice<=0 || choice>=5);

                
                string response="1";
                switch(choice) {
                    case 1:{
                        choiceStr=to_string(choice);
                        send(sock, choiceStr.c_str(), choiceStr.size() + 1, 0);
                        for(int i=0;i<5;i++){
                            if(i==0){
                                cout << "Adınız: ";
                            }
                            else if(i==1){
                                cout << "Soyadınız: ";
                            }
                            else if(i==2){
                                cout << "Kayıtlı olduğunuz banka: ";
                            }
                            else if(i==3){
                                cout << "Kullanıcı hesap numarası: ";
                            }else
                            {
                                cout << "Kullanıcı hesap bakiyesi: (tl)";
                                
                            }
                            bytesReceived = recv(sock, buf, 4096, 0);
                            cout << string(buf,0, bytesReceived) << endl;
                            send(sock, response.c_str(), response.size() + 1, 0);

                        }
                        cout<< " "<< endl;
                }
                    break;

                    case 2:{
                        int value;
                        choiceStr=to_string(choice);
                        send(sock, choiceStr.c_str(), choiceStr.size() + 1, 0);
                        bytesReceived = recv(sock, buf, 4096, 0);

                        cout << "Kullanıcı hesap bakiyesi: "<< (string(buf,0, bytesReceived)) << endl;
                        cout<< ""<< endl;
                        
                        cout << "Lütfen yatırmak istediğiniz miktarı giriniz: ";
                        cin >> value;
                        cout<< ""<< endl;
                        string valueStr;
                        valueStr= to_string(value);
                        send(sock, valueStr.c_str(), valueStr.size() + 1, 0);
                        bytesReceived = recv(sock, buf, 4096, 0);
                        cout << "İşlemden sonraki bakiyeniz:  " << (string(buf,0, bytesReceived)) <<endl;
                        cout<< ""<< endl;

                    }
                    break;

                    case 3:{
                        int value;
                        choiceStr=to_string(choice);
                        send(sock, choiceStr.c_str(), choiceStr.size() + 1, 0);
                        bytesReceived = recv(sock, buf, 4096, 0);
                        string oldValue = (string(buf,0, bytesReceived));
                        int toplamBakiye = stoi(oldValue);
                        do {
                            if(value > toplamBakiye){
                            cout << "Yetersiz bakiye!" << endl;
                            }
                        cout << "Kullanıcı hesap bakiyesi: "<< oldValue << endl;
                        cout<< ""<< endl;
                        cout << "Çekmek istediğiniz para miktarını girin: ";
                        cin >> value;
                        cout<< "" << endl;
                        }while(value > toplamBakiye);
                        string valueStr;
                        valueStr= to_string(value);

                        send(sock, valueStr.c_str(), valueStr.size() + 1, 0);
                        bytesReceived = recv(sock, buf, 4096, 0);
                        
                        
                        toplamBakiye = stoi((string(buf,0, bytesReceived)));
                        if(toplamBakiye != -1) {
                            cout << "İşlemden sonraki bakiyeniz: " << (string(buf,0, bytesReceived)) <<endl;
                        }
                        cout<< ""<< endl;
                    }
                    break;

                    case 4:
                    {
                        string tarName,tarSurname,noStr,valueStr;
                        int targetNo,value;
                        choiceStr=to_string(choice);
                        
                        send(sock, choiceStr.c_str(), choiceStr.size() + 1, 0);
                        bytesReceived = recv(sock, buf, 4096, 0);
                        
                        string oldValue = (string(buf,0, bytesReceived));
                        cout << "Farklı bankalarla yapılan işlemlere para kesintisi uygulanır."<< endl;
                        cout<< "" << endl;
                        cout << "Güncel bakiyeniz : "<< oldValue << endl;
                        cout << "Havale yapmak istediğiniz kullanıcının hesap numarasını girin. ";
                        cin >> targetNo;
                        noStr = to_string(targetNo);
                        
                        send(sock, noStr.c_str(), noStr.size() + 1, 0);
                        bytesReceived = recv(sock, buf, 4096, 0);
                        
                        cout << "Havale yapmak istediğiniz kullanıcının adı: ";
                        cin >> tarName;
                        
                        send(sock, tarName.c_str(), tarName.size() + 1, 0);
                        bytesReceived = recv(sock, buf, 4096, 0);
                        
                        cout << "Havale yapmak istediğiniz kullanıcının soyadı: ";
                        cin >> tarSurname;
                        
                        send(sock, tarSurname.c_str(), tarSurname.size() + 1, 0);
                        bytesReceived = recv(sock, buf, 4096, 0);
                        
                        cout << "Havale yapmak istediğiniz tutar: ";
                        cin >> value;
                        valueStr = to_string(value);
                        
                        send(sock, valueStr.c_str(), valueStr.size() + 1, 0);
                        bytesReceived = recv(sock, buf, 4096, 0);

                        string bakiyeLast = (string(buf,0, bytesReceived));
                        int bakiyeInt = stoi(bakiyeLast);
                        if(bakiyeInt==-1){
                            cout << "Hatalı hesap bilgileri"<<endl;
                            }
                        else if(bakiyeInt==-2)
                        {
                            cout << "Yetersiz bakiye"<<endl;
                        }
                        else{
                        
                             cout << "Güncel bakiyeniz: "<< bakiyeLast << endl;
                        }
                       

                    }
                    break;
                    
                    
                }

            }else{

                cout << "Hatalı giriş bilgileri" << endl;
            
                continue;

            }

            
            
        }else
        {   
            continue;
        }
        
    } while(true);

    //	Close the socket
    close(sock);

    return 0;
}
int main(){
	
	client();

}