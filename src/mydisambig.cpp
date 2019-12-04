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
        if(add) str.push_back(Word("<s>"));
        
        Word tmp;  int res;
        while((res = sscanf(buf, "%s", tmp.w)) > 0){
            buf = &buf[res];
            str.push_back(tmp);
        }
        if(add) str.push_back(Word("</s>"));
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

int main(int argc, char **argv){
	int order = 2;
	if(argc == 6){
		order = atoi(argv[5]);
	}else if(argc != 5){
		printf("Format: %s [file_in] [mapping] [lm] [file_out]\n", argv[0]);
		printf("    or: %s [file_in] [mapping] [lm] [file_out] [order]\n", argv[0]);
		exit(1);
	}
    
    File textfile(argv[1], "r");
	
	for(int i=0;i<5;++i){
		String str(textfile, true);
		str.print();
    }
	
	textfile.close();

	exit(0);
}
