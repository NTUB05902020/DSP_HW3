#!/bin/bash

extradir=exp
indir=test_data
outdir=$extradir/segmented
script=separator_big5.pl

rm -fr $extradir
mkdir $extradir
mkdir $outdir

perl $script corpus.txt > $outdir/corpus_seg.txt

for i in $(seq 1 10)
do
	perl $script $indir/$i.txt > $outdir/$i.txt
done
