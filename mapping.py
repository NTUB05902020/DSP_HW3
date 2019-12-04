import os, sys

try:
    in_path, out_path = str(sys.argv[1]), str(sys.argv[2])
except IndexError:
    print('Format: {} [in_path] [out_path]'.format(sys.argv[0]))
    sys.exit(1)

zhuin_to_word = dict()

with open(in_path, 'r', encoding='Big5-HKSCS') as fp:
    for line in iter(fp):
        if line == '\n': continue
        word, zhuins = line.strip('\n').split(' ')
        zhuins = zhuins.split('/')
        zhuin_to_word[word] = word
        
        for zhuin in zhuins:
            zhuin_initial = zhuin[0]
            if zhuin_initial in zhuin_to_word:
                zhuin_to_word[zhuin_initial].append(word)
            else:
                zhuin_to_word[zhuin_initial] = [word]

with open(out_path, 'w', encoding='Big5-HKSCS') as fp:
    for zhuin,words in zhuin_to_word.items():
        fp.write(zhuin)
        for word in words: fp.write(' {}'.format(word))
        fp.write('\n')
