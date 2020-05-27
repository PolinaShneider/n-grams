DATA="/Users/polina/GitHub/Big-Data-Programming/Data/"
OUTPUT="/Users/polina/GitHub/Big-Data-Programming/Output/"
NGRAM_SIZE=3

if test "$#" -ne 1; then
    echo "Useage {no of processors}"
else
    mpiexec -n $1 python3 parallel.py $DATA $OUTPUT $NGRAM_SIZE $NGRAM_SIZE
fi
