#!/bin/bash

extradir=exp
intxt=$extradir/segmented/corpus_seg.txt

cntdir=$extradir/counted
outcnt=$cntdir/corpus_cnt.cnt

lmdir=$extradir/lm
outlm=$lmdir/lm.lm

rm -fr $cntdir $lmdir
mkdir $cntdir $lmdir

ngram_order=2

ngram-count -text $intxt -write $outcnt -order $ngram_order
ngram-count -read $outcnt -lm $outlm -order $ngram_order -unk
