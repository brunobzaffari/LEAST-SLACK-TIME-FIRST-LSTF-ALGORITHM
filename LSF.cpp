#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class Job {
private:
    int TC, D, P, Count;
public:
    // Construtor
    Job(int t, int d, int p, int count) : TC(t), D(d), P(p), Count(count) {
        //cout << "Job created with T=" << TC << ", D=" << D << ", P=" << P << ", Count=" << Count << endl;
    }

    // Destrutor
    ~Job() {
        //cout << "Job destroyed" << endl;
    }

    int getTC(){ return TC; }
    int getD(){ return D; }
    int getP(){ return P; }
    int getCount() { return Count; }
    void setTC(int t) { TC = t; }
    void setD(int d) { D = d; }
    void setP(int p) { P = p; }
    void setCount(int count) { Count = count; }
};

void printjobs(vector<Job> jobs){
	cout << "-----------------------------------------------"<< endl;
	for(int i=0; i<jobs.size(); i++){//Debug
		cout << i << "- " << jobs[i].getTC() << " " << jobs[i].getD() << " " << jobs[i].getP() << " " << jobs[i].getCount() << endl;
	}
	cout << "-----------------------------------------------"<< endl;
}
void printlsf(vector<int> LSF){
	cout << "-----------------------------------------------"<< endl;
	for(int i=0; i<LSF.size(); i++){//Debug
		cout << i << "- " << LSF[i] << endl;
	}
	cout << "-----------------------------------------------"<< endl;
}
int main() { 
    vector<int> LSF;
    vector<Job> jobs;
    int T, N;
    string line;
    string str = "";//string de ordem de execucao
    int premp = 0;
    int troca = 0;
    int b;//indice do job que está sendo processado
    char c;//processo a ser posto na string
    int min_value = -1;// menor lsf
    int p;// indice do menor lsf
    char extra;// Resto 
    while(true){
        while (true) {
            if(str != ""){
                cout << str << endl;
                cout << premp << " " << troca << endl;
                cout << "" << endl;
                str = "";
                troca = 0;
                premp = 0;
                min_value = -1;
                LSF.clear(); 
        	jobs.clear(); 
                }
            
            getline(cin, line);  // Lê a linha inteira
            istringstream iss(line);
            //cout << line << endl;
            if (iss >> N >> T) {// Checa se só existem dois números na linha
                if (!(iss >> extra) && N > 0 && T > 0) {
                LSF.resize(N, 0); 
        	//jobs.reserve(N); 
        	jobs.resize(N, Job(0,0,0,0)); 
        	/*for(int i=0; i<N; i++){//Debug
		    cout << i << jobs[i].getTC() << " " << jobs[i].getD() << " " << jobs[i].getP() << endl;
		}*/
                    break;  // Sai do loop se exatamente dois números foram lidos e não há mais nada depois
                }
                else if (N == 0 && T == 0) {
                    return 0;
                }
            }
            
        }
        
        /*for (int i = 0; i < N; i++) {// Insere jobs
            int TC, D, P;
            if (getline(cin, line)) {
                istringstream jobStream(line);
                if (jobStream >> TC >> D >> P) {
                    jobs.emplace_back(Job(TC, D, P, TC));
                }
            }
            cout << TC << " " << D << " " << P << endl;
        }*/
        for (int i = 0; i < N; i++) {
            int TC, D, P;
            getline(cin, line);  // Lê a linha inteira
            istringstream iss(line);
            iss >> TC >> P >> D;
            jobs[i].setTC(TC);
            jobs[i].setD(D);
            jobs[i].setP(P);
            jobs[i].setCount(TC);
            //cout << TC << " " << D << " " << P << endl;
            //cout << i << jobs[i].getTC() << "* " << jobs[i].getD() << " " << jobs[i].getP() << endl;
            }
        
        //printjobs(jobs);//Debug
        
        for(int t=0; t<T; t++){//Calcula lsf
		for(int n=0; n<N; n++){//
			//cout<<"---------------------------------"<<endl;
			if(t%jobs[n].getP() == 0){
				jobs[n].setCount(jobs[n].getTC());
				//cout<< t<<"%"<<jobs[n].getD()<<"="<<t%jobs[n].getD()<<endl;
				}
			if(jobs[n].getCount() > 0) {
				LSF[n]= jobs[n].getD() -( t%jobs[n].getP()) - jobs[n].getCount();
				//cout<<jobs[n].getD()<<"-("<<t%jobs[n].getD()<<")-"<<jobs[n].getCount()<<"="<<LSF[n]<<endl;
				}
			else LSF[n]=0;
			//cout<<n<<"-"<<LSF[n]<<endl;
			//cout<<"---------------------------------"<<endl;
		}
		min_value = -1;
		for (int j = 0; j < N; j++) {// Assume que o primeiro elemento menor é o primeiro n nulo
			if (LSF[j] > 0) {
			min_value = LSF[j];
			p = j; 
			break;
			}
		} 
		for (int o= 0; o < N; o++) {
			if (LSF[o] < min_value && LSF[o] > 0) { 
			    min_value = LSF[o];  // Atualiza o menor valor encontrado
			    p = o;  // Atualiza a posição do menor valor encontrado
			    //cout<<"p"<<p<<endl;
			    //cout<<"o"<<o<<endl;
			}
		}
		//cout<<min_value<<endl;
		
		for(int n=0; n<N; n++){
			if(jobs[n].getCount() > 0 && LSF[n]==0){
				min_value = 1;
				p=n;
				break;
			}
		}
		
		if (min_value > 0){// Verifica se tem algum job que ainda n tem seu count zerado, o q indica ainda se tem processos para rodar
			//cout<<min_value<<endl;
			if (c != 'A' + p || c == '.') troca++;// ab or .n   
			if (c >='A' && c<= 'Z') b = c - 'A';
			if (c == 'A' + p && jobs[b].getCount() == jobs[b].getTC()) troca++;// troca de contexto de A|A mesmo processo mas periodos diferentes
			if(t>0){
				if (c != '.' && c != 'A' + p && jobs[b].getCount() > 0){
					premp++;
					//str += 'p';
					}
					
			}
			jobs[p].setCount(jobs[p].getCount()-1);
			//printjobs(jobs);
			c = 'A' + p;
			//cout<<c<<" - "<< t <<endl;
			str += c;
		}
		else{
			if (c != '.') {
			troca++;
			premp++;
			//str += 'p';
			}// n.
			c = '.';
			str += c;
		}
	}  
}
return 0;

}
