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
    
    Word(const char *s){ strcat(w, s);}
    void print() const{ printf("%s", w);}
    bool operator < (const Word &other) const{
        return (strcmp(w, other.w) < 0)
    }
};

struct String{
    Vector<Word> str;
    
    String(){};
    String(const Word &a){ str(a.str);}
    String(const Word &a, const Word &b){
        str.insert(str.end(), a.str.begin(), a.str.end());
        str.insert(str.end(), b.str.begin(), b.str.end());
    }
    String (File &file, const bool add=false){
        char *buf;  buf = file.getline();  Word tmp;
        if(add) str.push_back(Word("<s>"));
        while(sscanf(buf, "%s", tmp.w) > 0) str.push_back(tmp);
        if(add) str.push_back(Word("</s>"));
    }
    
    void print(const char *end) const{
        for(int i=0;i<str.size();++i) printf("%s", str[i]);
        printf("%s", end);
    }
    bool operator < (const String &other) const{
        unsigned int minlen = (str.size() < other.str.size())? str.size() : other.str.size();
        for(int i=0;i<minlen;++i){
            if(str[i] < other.str[i]) return true;
            else if(str[i] > other.str[i]) return false;
        }
        return ((minlen == str.size()) & (minlen < other.str.size())) true :false;
    }
}
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
		MyString str(&textfile, true);
		str.print();
    }
	
	textfile.close();

	getMap(argv[2]);
	exit(0);
}
