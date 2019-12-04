#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <vector>

#include "File.h"
#include "Ngram.h"
#include "Prob.h"
#include "Vocab.h"

#define STRING_LEN_MAX 1024

const int order = 2;

struct Word{
    char w[6];
    
    Word(){};
    Word(const char *s){ strcpy(w, s);}
    void print() const{ printf("%s", w);}
    static int compare(const Word &w1, const Word &w2){
        return strcmp(w1.w, w2.w);
    }
};

struct String{
    std::vector<Word> str;
    
    String(){ str.clear();}
    String(const Word &a){ str.push_back(a);}
    String(const Word &a, const Word &b){ str.push_back(a); str.push_back(b);}
    String(const String &s, const Word &word){
        str.insert(str.end(), s.str.begin(), s.str.end());
        str.push_back(word);
    }
    String(const String &a, const String &b){
        str.insert(str.end(), a.str.begin(), a.str.end());
        str.insert(str.end(), b.str.begin(), b.str.end());
    }
    
    String(File &file, const bool add=false){
        char *buf;  buf = file.getline();
		if(buf == NULL){ str.clear();}
		else{
        	if(add) str.push_back(Word("<s>"));
			
			int index = 0, wordlen = 0;  Word tmp;
        	while(1){
				if(buf[index] == ' ' || buf[index] == '\0' || buf[index] == '\n'){
					if(wordlen > 0){
						tmp.w[wordlen] = '\0';
						str.push_back(tmp); wordlen = 0;
					}
                    if(buf[index] == '\0' || buf[index] == '\n') break;
				}else tmp.w[wordlen++] = buf[index];
				++index;
			}
			
	        if(add) str.push_back(Word("</s>"));
    	}
	}
    
    void print(const char *end="\n") const{
        for(int i=0;i<str.size();++i) str[i].print();
        printf("%s", end);
    }
};

namespace std{
	template<> struct less<Word>{
		bool operator() (const Word &l, const Word &r) const{
			return (Word::compare(l, r) < 0);
		}
	};
}

std::map<Word, std::vector<Word>> ZhuYin_Big5_map;

void getMap(const char *map_path, const int map_order){
	File mapfile(map_path, "r");
	while(1){
		String line(mapfile);
		if(line.str.empty()) break;
		vector<Word> vec(line.str.begin()+1, line.str.end());
		ZhuYin_Big5_map.insert(std::pair<Word,std::vector<Word>>(line.str[0], vec));
	}
}

VocabIndex myGetIndex(const Word &word, Vocab &voc){
    VocabIndex ret = voc.getIndex(word.w);
    return (ret != Vocab_None)? ret : voc.getIndex("<unk>");
}

float getLogProb(const Word &word, Vocab &voc, Ngram &lm){
    VocabIndex context[2] = {myGetIndex(word, voc), Vocab_None};
    return lm.wordProb(context[0], &context[1]);
}

float getLogProb(const Word &pre, const Word &post, Vocab &voc, Ngram &lm){
    VocabIndex context[3] = {myGetIndex(post.w, voc), myGetIndex(pre.w, voc), Vocab_None};
    return lm.wordProb(context[0], &context[1]);
}

String viterbi(const String &s, Vocab &voc, Ngram &lm){
    std::vector<std::pair<String,float>> vecs[2];  int len = s.str.size();
    float startProb = getLogProb(s.str[0], voc, lm); //logProb of <s>
    vecs[0].push_back(std::pair<String,float>(String(s.str[0]), startProb));
    
    for(int i=1;i<len;++i){
        int v = i&1;  int v_ = 1-v;  vecs[v].clear();
        std::vector<Word> candidates;
        
        auto it = ZhuYin_Big5_map.find(s.str[i]);
        if(it == ZhuYin_Big5_map.end()) candidates.push_back(s.str[i]);  //unknown word in map
        else candidates = it->second;
        
        for(auto &candidate : candidates){
            Word lastword = vecs[v_][0].first.str.back();
            float max = vecs[v_][0].second + getLogProb(lastword, candidate, voc, lm);
            int max_index = 0, prenum = vecs[v_].size();
            for(int j=1;j<prenum;++j){
                lastword = vecs[v_][j].first.str.back();
                float newLogProb = vecs[v_][j].second + getLogProb(lastword, candidate, voc, lm);
                if(newLogProb > max){ max = newLogProb; max_index = j;}
            }
            String maxstr(vecs[v_][max_index].first, candidate);
            vecs[v].push_back(std::pair<String,float>(maxstr, max));
        }
    }
    
    int v_ = (1-len&1), prenum = vecs[v_].size(), max_index = 0;
    float maxProb = vecs[v_][0].second;
    for(int j=1;j<prenum;++j){
        if(maxProb < vecs[v_][j].second){
            maxProb = vecs[v_][j].second;
            max_index = j;
        }
    }
    return vecs[v_][max_index].first;
}

int main(int argc, char **argv){
	if(argc != 5){
		printf("Format: %s [file_in] [mapping] [lm] [file_out]\n", argv[0]);
		exit(1);
	}

	//get lm
    Vocab voc;  Ngram lm(voc, order);
    File lmfile(argv[3], "r");  lm.read(lmfile);  lmfile.close();

    //get map
	getMap(argv[2], order);
    
    //parse file_in
    File infile(argv[1], "r"), outfile(argv[4], "w");
	for(int i=0;i<5;++i){
		String s(infile, true);
		s.print();
        
        String news = viterbi(s, voc, lm);
        news.print("\n\n");
        /*if(i == 4){
            printf("log Prob(");  s.str[1].print();  printf("|");
            s.str[0].print();  printf(") = %f\n", getLogProb(s.str[0].w, s.str[1].w, voc, lm));
        }*/
    }
	infile.close();  outfile.close();
	exit(0);
}
