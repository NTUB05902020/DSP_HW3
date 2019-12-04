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
    String(const Word &a){ str.clear(); str.push_back(a);}
    String(const String &a, const String &b){
        str.insert(str.end(), a.str.begin(), a.str.end());
        str.insert(str.end(), b.str.begin(), b.str.end());
    }

    String(File &file, const bool add=false){
        char *buf;  buf = file.getline();
		if(buf == NULL){ str.clear();}
		else{
        	if(add) str.push_back(Word("<s>"));
            printf("<s> ");
			
			int index = 0, wordlen = 0;  Word tmp;
        	while(1){
				if(buf[index] == ' ' || buf[index] == '\0'){
					if(wordlen > 0){
						tmp.w[wordlen] = '\0';
						str.push_back(tmp);
                        tmp.print(); printf(" ");
						wordlen = 0;
					}
                    if(buf[index] == '\0') break;
				}else tmp.w[wordlen++] = buf[index];
				++index;
			}
			
	        if(add) str.push_back(Word("</s>"));
            printf("</s>\n");
    	}
	}
    
    void print(const char *end="\n") const{
        for(int i=0;i<str.size();++i) str[i].print();
        printf("%s", end);
    }
    static int compare(const String &s1, const String &s2){
        unsigned int minlen = (s1.str.size() < s2.str.size())? s1.str.size() : s2.str.size();
        for(unsigned int i=0;i<minlen;++i){
            int res = Word::compare(s1.str[i], s2.str[i]);
            if(res != 0) return res;
        }
        if(minlen == s1.str.size())
            return (minlen == s2.str.size())? 0 : -1;
        else return 1;
    }
};

namespace std{
	template<> struct less<String>{
		bool operator() (const String &l, const String &r) const{
			return (String::compare(l, r) < 0);
		}
	};
	template<> struct less<Word>{
		bool operator() (const Word &l, const Word &r) const{
			return (Word::compare(l, r) < 0);
		}
	};
}

std::map<Word, std::vector<String>> ZhuYin_Big5_map;

void getMap(const char *map_path, const int map_order){
	File mapfile(map_path, "r");
	int cnt = 0;
	while(1){
		String line(mapfile);
		if(line.str.empty()) break;
		++cnt;
		vector<String> vec(line.str.begin()+1, line.str.end());
		ZhuYin_Big5_map.insert(std::pair<Word,std::vector<String>>(line.str[0], vec));
	}
}

float getLogProb(const Word &pre, const Word &post, Vocab &voc, Ngram &lm){
    VocabIndex context[3] = {voc.getIndex(post.w), voc.getIndex(pre.w), Vocab_None};
    if(context[0] == Vocab_None){ context[0] = voc.getIndex("<unk>");}
    if(context[1] == Vocab_None){ context[1] = voc.getIndex("<unk>");}
    return lm.wordProb(context[0], &context[1]);
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
		//s.print();
        
        if(i == 4){
            printf("log Prob(");  s.str[1].print();  printf("|");
            s.str[0].print();  printf(") = %f\n", getLogProb(s.str[0].w, s.str[1].w, voc, lm));    
        }
    }
	infile.close();  outfile.close();
	exit(0);
}
