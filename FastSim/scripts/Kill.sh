JobMin=$1
JobMax=$2

if [ $# -le 1 ]
then
  echo "Usage: Kill.sh JobMin JobMax"
  exit
fi

i=$JobMin
while [ $i -le $2 ]; do
bkill $i
i=$[i+1]
done