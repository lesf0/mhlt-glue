#include <stdio.h>
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <string.h>
using namespace std;

unsigned int bracketcount,i,gluecount,firstgluecnt,rrotb1,rrotb2;
bool flag,razorflag,butterflag;

string buf,glue,tmpglue,razor,butter;
vector<string> cur,tmp;
map<string, vector<string> > tot,razorm;
multimap<string, vector<string> > butterm;
char* infilename;
char* outfilename;

int main(int argc, char** argv){
	if(argc<2){
		cerr<<"useage: ./hlglue infilename [outfilename]\n";
		return 1;
	}else if(argc==2){
		infilename=argv[1];
		int size=strlen(infilename);
		outfilename=new char[size+2];
		strcpy(outfilename,infilename);
		outfilename[size]='_';
		outfilename[size+1]=0;
	}else{
		infilename=argv[1];
		outfilename=argv[2];
	}
	freopen(infilename,"r",stdin);
	freopen(outfilename,"w",stdout);
	while(!std::cin.eof()){
		getline(cin,buf);
		if(buf[0]=='{'){
			++bracketcount;
		}
		if(bracketcount==2){
			while(true){
				getline(cin,buf);
				if(buf[0]=='}'){
					break;
				}
				if(buf.find(" VOID ")!=-1){
					continue;
				}
				if(buf.find(" RAZOR ")!=-1){
					razor=buf.substr(buf.rfind("]"));
					razorflag=true;
					continue;
				}
				if(buf.find(" BUTTER ")!=-1){
					butter=buf.substr(buf.rfind("]"));
					butterflag=true;
					continue;
				}
				if(buf.find(" GLUE ")!=-1){
					tmpglue=buf.substr(buf.rfind("]"));
					if(!flag){
						glue=string(tmpglue);
					}else{
						if(glue!=tmpglue){
							cur.push_back(buf);
						}
					}
					flag=true;
				}else{
					cur.push_back(buf);
				}
			}
			if(razorflag){
				for(auto it=cur.begin();it!=cur.end();++it){
					rrotb1=it->find(")");
					rrotb2=it->find(")",rrotb1+1);
					*it=it->substr(rrotb1+2,rrotb2-rrotb1)+it->substr(0,rrotb1+1)+it->substr(rrotb2+1);
					//cerr<<*it<<'\n';
				}
				razorm[razor]=cur;
				auto it=butterm.begin();
				while((it=butterm.find(razor))!=butterm.end()){
					cout<<"{\n";
					for(auto d:it->second){
						cout<<d<<'\n';
					}
					for(auto d:cur){
						cout<<d<<'\n';
					}
					cout<<"}\n";
					butterm.erase(it);
				}
				goto labelrazor;
			}
			if(butterflag){
				if(razorm.count(butter)){
					cout<<"{\n";
					for(auto d:razorm[butter]){
						cout<<d<<'\n';
					}
					for(auto d:cur){
						cout<<d<<'\n';
					}
					cout<<"}\n";
				}else{
					butterm.insert(pair<string, vector<string> >(butter,cur));
				}
				goto labelrazor;
			}
			if(flag){
				label1:
				if(tot.count(glue)){
					for(auto d:tot[glue]){
						cur.push_back(d);
					}
					tot.erase(glue);
					gluecount=0;
					firstgluecnt=0;
					for(auto d:cur){
						if(d.find(" GLUE ")!=-1){
							tmpglue=d.substr(d.rfind("]"));
							if(!gluecount){
								glue=string(tmpglue);
								gluecount=1;
								firstgluecnt=1;
							}else{
								if(glue!=tmpglue){
									++gluecount;
								}else{
									++firstgluecnt;
								}
							}
						}
					}
					if(gluecount){
						tmp.clear();
						for(auto d:cur){
							if(d.find(" GLUE ")!=-1){
								tmpglue=d.substr(d.rfind("]"));
								if(glue!=tmpglue){
									tmp.push_back(d);
								}
							}else{
								tmp.push_back(d);
							}
						}
						cur=vector<string>(tmp);
						goto label1;
					}else{
						cout<<"{\n";
						for(i=0;i<cur.size();++i){
							cout<<cur[i]<<'\n';
						}
						cout<<"}\n";
					}
				}else{
					tot[glue]=cur;
				}
			}else{
				cout<<"{\n";
				for(i=0;i<cur.size();++i){
					cout<<cur[i]<<'\n';
				}
				cout<<"}\n";
			}
			labelrazor:
			cur.clear();
			flag=false;
			razorflag=false;
			butterflag=false;
		}else{
			cout<<buf<<'\n';
		}
		if(buf[0]=='}'){
			--bracketcount;
		}
		
	}
	if(tot.size()){
		cerr<<"corrupted\n"<<tot.size()<<" of unused glues\n";
		for(auto d:tot){
			cerr<<"#####\n"<<d.first<<"#####\n";
			for(auto f:d.second){
				cerr<<f<<'\n';
			}
		}
		return 2;
	}
}
